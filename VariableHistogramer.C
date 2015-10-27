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
#include "Math/Functor.h"
#include "Math/GSLMinimizer1D.h"

void VariableHistogramer (TString inputFileName,TString outputFileName, bool isData, bool isSignal, bool isDY)
{
  using namespace std;

  ///////////////////
  // Configuration

  // Weighting for Drell Yan
  float weight = 0.03;

  ///////////////////////////
  // Output Histograms
      
  setStyle();
          
  // Plots Mass of the Dimuons
  TH1F* dimuonMassHist = new TH1F("dimuonMassHist","",50,110,160);
  setHistTitles(dimuonMassHist,"M(#mu#mu) [GeV/c^{2}]","Events");
  dimuonMassHist->Sumw2();
                    
  // Plots invariant Dijet Mass
  TH1F* diJetMassHist = new TH1F("diJetMassHist","",50,0,200);
  setHistTitles(diJetMassHist,"M(2Jet) [GeV/c^{2}]","Events");
  diJetMassHist->SetStats(1);
  diJetMassHist->Sumw2();
  
  // Plots the diJet Eta
  TH1F* diJetEtaHist = new TH1F("diJetEtaHist","",50,-6,6);
  setHistTitles(diJetEtaHist,"#eta","Events");
  diJetEtaHist->SetStats(1);
  diJetEtaHist->Sumw2();

  // Plots the angle between V and H
  TH1F* VHphiHist = new TH1F("VHphiHist","",50,0,7);
  setHistTitles(VHphiHist,"#phi","Events");
  VHphiHist->SetStats(1);
  VHphiHist->Sumw2();

  // Plots the Phi star variable
  TH1F* phiStarHist = new TH1F("phiStarHist","",50,0,1);
  setHistTitles(phiStarHist,"#phi *","Events");
  phiStarHist->SetStats(1);
  phiStarHist->Sumw2();

  // Plot the Phi star cross check
  TH1F* phiStarCheckHist = new TH1F("phiStarCheckHist","",50,0,1);
  setHistTitles(phiStarCheckHist,"#phi *","Events");
  phiStarCheckHist->SetStats(1);
  phiStarHist->Sumw2();

  // Plots the Zeppenfeld1 Variable
  TH1F* Zeppenfeld1Hist = new TH1F("Zeppenfeld1Hist","",50,-6,6);
  setHistTitles(Zeppenfeld1Hist,"eta_{1} *","Events");
  Zeppenfeld1Hist->SetStats(1);
  Zeppenfeld1Hist->Sumw2();

  // Plots the Zeppenfeld2 Variable
  TH1F* Zeppenfeld2Hist = new TH1F("Zeppenfeld2Hist","",50,-6,6);
  setHistTitles(Zeppenfeld2Hist,"eta_{2} *","Events");
  Zeppenfeld2Hist->SetStats(1);
  Zeppenfeld2Hist->Sumw2();

  ////////////////////////////
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
  
  // These are the dimuon mass, pt, rapidity, and phi
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
  _genPartInfo genHpostFSR;
  if(!isData && tree->GetBranchStatus("genHpostFSR"))
     tree->SetBranchAddress("genHpostFSR", &genHpostFSR);
  
  _TrackInfo reco1GenPostFSR;
  if(!isData && tree->GetBranchStatus("genM1HpostFSR"))
     tree->SetBranchAddress("genM1HpostFSR", &reco1GenPostFSR);
  
  _TrackInfo reco2GenPostFSR;
  if(!isData && tree->GetBranchStatus("genM2HpostFSR"))
     tree->SetBranchAddress("genM2HpostFSR", &reco2GenPostFSR);

  // the jet collection
  _PFJetInfo rawJets;
  tree->SetBranchAddress("pfJets",&rawJets);

  // Variables from Variable Stage
  _diJetInfo diJet;
  float VHphi;
  float phiStar;
  float phiStarCheck;
  float Zeppenfeld1;
  float Zeppenfeld2;
  tree->SetBranchAddress("diJet",&diJet);
  tree->SetBranchAddress("VHphi",&VHphi);
  tree->SetBranchAddress("phiStar",&phiStar);
  tree->SetBranchAddress("phiStarCheck",&phiStarCheck);
  tree->SetBranchAddress("Zeppenfeld1",&Zeppenfeld1);
  tree->SetBranchAddress("Zeppenfeld2",&Zeppenfeld2);

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
   
    /////////////////////
    // Fill Histograms
    if(isDY){
       dimuonMassHist->Fill(recoCandMass,weight);
       diJetMassHist->Fill(diJet.mass,weight);
       VHphiHist->Fill(VHphi,weight);
       phiStarHist->Fill(phiStar,weight);
       phiStarCheckHist->Fill(phiStarCheck);
       Zeppenfeld1Hist->Fill(Zeppenfeld1,weight);
       Zeppenfeld2Hist->Fill(Zeppenfeld2,weight);
    }
    else{
       dimuonMassHist->Fill(recoCandMass);
       diJetMassHist->Fill(diJet.mass);
       VHphiHist->Fill(VHphi);
       phiStarHist->Fill(phiStar);
       phiStarCheckHist->Fill(phiStarCheck);
       Zeppenfeld1Hist->Fill(Zeppenfeld1);
       Zeppenfeld2Hist->Fill(Zeppenfeld2);
    }
  }

  TFile* outFile = new TFile(outputFileName,"RECREATE");
  outFile->cd();
  dimuonMassHist->Write();
  diJetMassHist->Write();
  VHphiHist->Write();
  phiStarHist->Write();
  phiStarCheckHist->Write();
  Zeppenfeld1Hist->Write();
  Zeppenfeld2Hist->Write();
  outFile->Close();
}
