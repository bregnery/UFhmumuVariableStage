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
  //
