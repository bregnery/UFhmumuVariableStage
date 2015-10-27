//////////////////////////////
// The Hmumu Variable adder //
//////////////////////////////
// The purpose of this program is to calculate
// additional variables needed for the Hmumu 
// analysis. This program uses the stage 1 
// ntuples from Justin's UFDiMuonsAnalyzer to
// calculate these variables and then adds the
// new variables and variables from to stage 1
// ntuples to a new .root file

#include <TSystem.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TParticle.h>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TF1.h>
#include <TRandom3.h>
#include <algorithm>
#include <vector>
#include "src/DataFormats.h"
#include "src/helpers.h"
#include "src/LumiReweightingStandAlone.h"
#include "Math/Functor.h"
#include "Math/GSLMinimizer1D.h"

//Begin main program
void VariableAdder (TString inputFileName,TString outputFileName, bool isData, bool isSignal, bool isCrab)
{
  using namespace std;

  ///////////////////////////
  Double_t MASS_MUON = 0.105658367;    //GeV/c2

  //////////////////////////
  // Tree Branches
  cout << "Analyzing filename: "<< inputFileName.Data() << endl;
  if (isData)
    cout << "isData\n";
  if (isSignal)
    cout << "isSignal\n";

  
  // If the root file came from Andrew's crab script
  // one additional step is needed
  TChain * tree = nullptr;
  if (isCrab){
    tree = new TChain("dimuons/tree");
    tree -> Add(inputFileName);
  }
  else {
    tree = new TChain("tree");
    tree -> Add(inputFileName);
  }

  std::cout << "Entries in the tree: " << tree -> GetEntries() << std::endl;
  
  // These are the names of the muons (See src/DataFormats.h for definitions!)
  _MuonInfo reco1, reco2;

  std::cout << "======== Setting Branch Address ========" << std::endl;
  tree->SetBranchAddress("reco1", &reco1);
  tree->SetBranchAddress("reco2", &reco2);
  
  // Create variables for dimuon mass, pt, rapidity, and phi
  float recoCandMass, recoCandPt, recoCandY, recoCandPhi;
  
  tree->SetBranchAddress("recoCandMass",       &recoCandMass);
  tree->SetBranchAddress("recoCandPt",         &recoCandPt);
  tree->SetBranchAddress("recoCandY",          &recoCandY);
  tree->SetBranchAddress("recoCandPhi",        &recoCandPhi);

  std::cout << "======== Branch Addresses Set! ========" << std::endl;

  std::cout << "======== Checking truth info! ========" << std::endl;

  // MC truth info
  float trueMass=-99999.0;
  if(!isData && tree->GetBranchStatus("trueMass"))
      tree->SetBranchAddress("trueMass", &trueMass);
 
  // Higgs Boson MC truth info (after FSR)
  _genPartInfo genWpreFSR;
  if(!isData && tree->GetBranchStatus("genWpreFSR"))
      tree->SetBranchAddress("genWpreFSR", &genWpreFSR);
  
  _TrackInfo reco1GenPostFSR;
  if(!isData && tree->GetBranchStatus("genM1HpostFSR"))
      tree->SetBranchAddress("genM1HpostFSR", &reco1GenPostFSR);
  
  _TrackInfo reco2GenPostFSR;
  if(!isData && tree->GetBranchStatus("genM2HpostFSR"))
      tree->SetBranchAddress("genM2HpostFSR", &reco2GenPostFSR);
  
  // the jet collection
  _PFJetInfo rawJets;
  tree->SetBranchAddress("pfJets",&rawJets);

  std::cout << "======= Truth Info and Branch Addresses Set! ========" << std::endl;
  
  ///////////////////////////////
  ///////////////////////////////
  // Output file
  std::cout << "======== Making Output File! :) ========" << std::endl;

  TFile* outFile = new TFile(outputFileName,"RECREATE");
  std::cout << "======== Created Output File object ========" << std::endl;
  outFile->cd();
  TTree* outTree = new TTree("outTree","RECREATE");
  std::cout << "======== Created outTree object ========" << std::endl;
  outTree = tree->CloneTree();
  std::cout << "======== Made Output File! ========" << std::endl;
  
  // Add a branch for diJet TLorentzVector
  _diJetInfo diJet;
  TBranch *diJetBranch = outTree->Branch("diJet", &diJet, "mass/F:phi/F:eta/F:pt/F:Et/F");

  // Add a branch for the transverse angle
  // between the dimuon pair and the dijet pair
  float VHphi;
  TBranch *VHphiBranch = outTree->Branch("VHphi", &VHphi, "VHphi/F");

  // Add a branch for the phiStar variable
  float phiStar;
  TBranch *phiStarBranch = outTree->Branch("phiStar", &phiStar, "phiStar/F");

  // Add a branch for cross checking phiStar
  float phiStarCheck;
  TBranch *phiStarCheckBranch = outTree->Branch("phiStarCheck", &phiStarCheck, "phiStarCheck/F");

  // Add branchs for the Zeppenfeld variables
  float Zeppenfeld1 = 0;
  TBranch *Zeppenfeld1Branch = outTree->Branch("Zeppenfeld1", &Zeppenfeld1, "Zeppenfeld1/F");
  float Zeppenfeld2 = 0;
  TBranch *Zeppenfeld2Branch = outTree->Branch("Zeppenfeld2", &Zeppenfeld2, "Zeppenfeld2/F");  

  // number of events
  unsigned nEvents = tree->GetEntries();
  unsigned reportEach = 100000;
  if (nEvents/100000 > reportEach)
	reportEach = nEvents/100000;

  ///////////////////////////////
  // Event Loop
    
  for(unsigned i=0; i<nEvents;i++)
  { 
      tree->GetEvent(i);
      if (i % reportEach == 0) cout << "Event: " << i << endl;
    
      // Reject events with invalid muons
      if (reco1.pt < 0. || reco2.pt < 0.)
          continue;
    
       // Order muons by pt
       if (reco1.pt < reco2.pt)
       {
            _MuonInfo tmpMuon = reco1;
           reco1 = reco2;
           reco1 = tmpMuon;
       }
       
       // Jet Part
       std::vector<TLorentzVector> jets;
       std::vector<TLorentzVector> genJets;

       for(unsigned iJet=0; (iJet < unsigned(rawJets.nJets) && iJet < 10);iJet++)
       {
        TLorentzVector tmpJetVec;
        tmpJetVec.SetPtEtaPhiM(rawJets.pt[iJet],rawJets.eta[iJet],rawJets.phi[iJet],rawJets.mass[iJet]);
        jets.push_back(tmpJetVec);
        TLorentzVector tmpGenJetVec;
        tmpGenJetVec.SetPtEtaPhiM(rawJets.genPt[iJet],rawJets.genEta[iJet],rawJets.genPhi[iJet],rawJets.genMass[iJet]);
        genJets.push_back(tmpGenJetVec);
       }

       ///////////////
       // Calculate new variables

       //cout << "number of jets: " << jets.size() << endl;

       int njetsel = 0;
       int index1 = 0;
       int index2 = 0;
       // Find the two highest Pt jets
       for (unsigned iJet=0; iJet < jets.size(); iJet++){
	  njetsel++;
	  if (njetsel == 1){
		index1 = iJet;
	  } 
	  else if (njetsel == 2){
		index2 = iJet;
	  }  
       }
       // calculate and fill the diJetMass and VHphi
       VHphi = 0;
       diJet.mass = 0;
       diJet.phi = 0;
       diJet.eta = 0;
       diJet.pt = 0;
       diJet.Et = 0;
       if(jets.size() >= 2){
	  TLorentzVector diJetIn = jets[index1] + jets[index2];
          //cout << "DiJetMass: " << diJet.M() << endl;
          diJet.mass = diJetIn.M();
          diJet.phi = diJetIn.Phi();
          diJet.eta = diJetIn.Eta();
          diJet.pt = diJetIn.Pt();
          diJet.Et = diJetIn.Et();
          VHphi = TMath::Abs(diJet.phi - recoCandPhi);
          if(VHphi > TMath::Pi()){
             VHphi = 2*TMath::Pi() - VHphi;
          }     
       }
       diJetBranch->Fill();
       VHphiBranch->Fill();

       // calculate and fill the Zeppenfeld Variable
       if(jets.size() >= 2){
          double meanEta = (jets[index1].Eta()+jets[index2].Eta())/2;
          Zeppenfeld1 = jets[index1].Eta()-meanEta;
          Zeppenfeld2 = jets[index2].Eta()-meanEta;
       }
       Zeppenfeld1Branch->Fill();
       Zeppenfeld2Branch->Fill();

       // calculate and fill the phiStar variable
       phiStar = 0;
       float muonDelPhi = TMath::Abs(reco1.phi - reco2.phi);
       if(muonDelPhi > TMath::Pi()){
	  muonDelPhi = 2*TMath::Pi() - muonDelPhi;
       }
       //cout << "muonDelPhi: " << muonDelPhi << endl;
       float phiACOP = TMath::Pi() - muonDelPhi;
       //cout << "phiACOP: " << phiACOP << endl;
       float thetaStarEta = TMath::ACos(TMath::TanH((reco1.eta - reco2.eta)/2));
       //cout << "thetaStarEta: " << thetaStarEta << endl;
       phiStar = TMath::Tan(phiACOP/2)*TMath::Sin(thetaStarEta);
       //cout << "phiStar: " << phiStar << endl;
       phiStarBranch->Fill();
       
       // phi star cross check
       phiStarCheck = 0;
       float tanDelPhi = TMath::Tan(phiACOP/2);
       float delEta = TMath::Abs((reco1.eta - reco2.eta)/2);
       float tanhDelEta = TMath::TanH(delEta);
       phiStarCheck = tanDelPhi*(TMath::Sqrt(1 - (tanhDelEta*tanhDelEta)));
       phiStarCheckBranch->Fill();
  }
  outTree->Write();
  outFile->Close();
}



