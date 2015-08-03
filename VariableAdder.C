//////////////////////////////
// The Hmumu Variable adder //
/////////////////////////////
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
#include "src/SmearingTool.h"
#include "src/SmearingTool2011.h"
#include "Math/Functor.h"
#include "Math/GSLMinimizer1D.h"

//Begin main program
void VariableAdder (TString inputFileName,TString outputFileName, bool isData, bool isSignaljjjjjjjjjjjjjjk)
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
  float recoCandMassRes, recoCandMassResCov;
  
  tree->SetBranchAddress("recoCandMass",       &recoCandMass);
  tree->SetBranchAddress("recoCandPt",         &recoCandPt);
  tree->SetBranchAddress("recoCandY",          &recoCandY);
  tree->SetBranchAddress("recoCandPhi",        &recoCandPhi);
  tree->SetBranchAddress("recoCandMassRes",    &recoCandMassRes);
  tree->SetBranchAddress("recoCandMassResCov", &recoCandMassResCov);

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
  
  float puJetFullDisc[10];
  float puJetSimpleDisc[10];
  float puJetCutDisc[10];
  
  tree->SetBranchAddress("puJetFullDisc",&puJetFullDisc);
  tree->SetBranchAddress("puJetSimpleDisc",&puJetSimpleDisc);
  tree->SetBranchAddress("puJetCutDisc",&puJetCutDisc);
  
  float puJetFullId[10];
  float puJetSimpleId[10];
  float puJetCutId[10];
  
  tree->SetBranchAddress("puJetFullId",&puJetFullId);
  tree->SetBranchAddress("puJetSimpleId",&puJetSimpleId);
  tree->SetBranchAddress("puJetCutId",&puJetCutId);
  
  int nPU=0;
  if (!isData)
    {
      tree->SetBranchAddress("nPU",&nPU);
    }
  _VertexInfo vertexInfo;
  tree->SetBranchAddress("vertexInfo",&vertexInfo);
  _EventInfo eventInfo;
  tree->SetBranchAddress("eventInfo",&eventInfo);

  // Be careful, the met has not been well validated
  _MetInfo met;
  tree->SetBranchAddress("met",&met);

  // I may add pile up reweighting here

  // I may add muon selection here

  /////////////////////////
  // Smearing
  SmearingTool *smearPT = new SmearingTool();

  ///////////////////////////////
  ///////////////////////////////
  ///////////////////////////////
  // Event Loop
    
  for(unsigned i=0; i<nEvents;i++)
  { 
      tree->GetEvent(i);
      if (i % reportEach == 0) cout << "Event: " << i << endl;
    
      // Reject events with invalid muons
      if (reco1.pt < 0. || reco2.pt < 0.)
          continue;
    
      /////////////////////////////////////////////////
      // Muon Resolution Smearing to match MuScleFit
        
      if(isSignal) // smear only signal because it has muons from higgs 
      {
          if(reco1GenPostFSR.pt<0.)
              cout << "Muon 1 Post FSR not valid!\n";
          if(reco2GenPostFSR.pt<0.)
              cout << "Muon 2 Post FSR not valid!\n";
          float ptReco1 = -1.;
          float ptReco2 = -1.;
          ptReco1 = smearPT -> PTsmear(reco1GenPostFSR.pt, reco1GenPostFSR.eta, reco1GenPostFSR.charge, reco1.pt, ISMEAR);
          ptReco2 = smearPT -> PTsmear(reco2GenPostFSR.pt, reco2GenPostFSR.eta, reco2GenPostFSR.charge, reco2.pt, ISMEAR);
          
	  LorentzVector reco1Vec;                                        
          TLorentzVector reco2Vec;
          reco1Vec.SetPtEtaPhiM(ptReco1,reco1.eta,reco1.phi,MASS_MUON);
          reco2Vec.SetPtEtaPhiM(ptReco2,reco2.eta,reco2.phi,MASS_MUON);
          LorentzVector diMuonVec = reco1Vec + reco2Vec;
          
          reco1.pt = ptReco1;
          reco2.pt = ptReco2;
          recoCandMass = diMuonVec.M();
          recoCandPt = diMuonVec.Pt();
          recoCandY = diMuonVec.Rapidity();
          recoCandPhi = diMuonVec.Phi();
          
          reco1Vec.SetPtEtaPhiM(ptReco1,reco1.eta,reco1.phi,MASS_MUON);
          reco2Vec.SetPtEtaPhiM(ptReco2,reco2.eta,reco2.phi,MASS_MUON);
          diMuonVec = reco1Vec + reco2Vec;
       }

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
      // Fill Histograms

     
  }
  TFile* outFile = new TFile(outputFileName,"RECREATE");
  outFile->cd();

}



