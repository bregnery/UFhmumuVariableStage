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
void VariableAdder (TString inputFileName,TString outputFileName, bool isData, bool isSignal)
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
  
  TChain * tree = new TChain("tree");
  tree->Add(inputFileName);
  
  
  // These are the names of the muons (See src/DataFormats.h for definitions!)
  _MuonInfo reco1, reco2;
  
  tree->SetBranchAddress("reco1", &reco1);
  tree->SetBranchAddress("reco2", &reco2);
  
  // Create variables for dimuon mass, pt, rapidity, and phi
  float recoCandMass, recoCandPt, recoCandY, recoCandPhi;
  
  tree->SetBranchAddress("recoCandMass",       &recoCandMass);
  tree->SetBranchAddress("recoCandPt",         &recoCandPt);
  tree->SetBranchAddress("recoCandY",          &recoCandY);
  tree->SetBranchAddress("recoCandPhi",        &recoCandPhi);

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
  // these 'rawJets' already have Loose Jet ID applied, and JES corrections
  // and are cross-cleaned of tight muons
  // later, jets will have JER corrections, PUID, and basic cuts applied
  _PFJetInfo rawJets;
  tree->SetBranchAddress("pfJets",&rawJets);
  
  ///////////////////////////////
  ///////////////////////////////
  // Output file
  TFile* outFile = new TFile(outputFileName,"RECREATE");
  outFile->cd();
  TTree* outTree = new TTree("outTree","outTree");
  outTree = tree->CloneTree();
  float diJetMass;
  TBranch *diJetMassBranch = outTree->Branch("diJetMass", &diJetMass, "diJetMass/F");

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
    
      //
      // I may add Muon Resolution Smearing to match MuScleFit
        
       // I may add muon cuts

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
       // Create diJetMass Variable and Branch
       diJetMass = 0;
       if(jets.size() >= 2){
	  TLorentzVector diJet = jets[index1] + jets[index2];
          //cout << "DiJetMass: " << diJet.M() << endl;     
          diJetMass = diJet.M();
          //cout << "DiJetMass float: " << diJetMass << endl;
       }
       diJetMassBranch->Fill();
  }
  outTree->Write();
  outFile->Close();
}



