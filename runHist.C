{
  gROOT->ProcessLine(".L VariableHistogramer.C++");
  
  ///////////////////////////////////////////////////////
  // This inputs the stage 1 ntuples on the UF hipergator 
  //VariableHistogramer("ggHmumu_13TeV_stageVar.root","ggHmumu_13TeV_Hist.root",false,true,false);
  //VariableHistogramer("VBFHmumu_13TeV_stageVar.root","VBFHmumu_13TeV_Hist.root",false,true,false);
  //VariableHistogramer("VHmumu_13TeV_stageVar.root","VHmumu_13TeV_Hist.root",false,true,false);
  //VariableHistogramer("ttHmumu_13TeV_stageVar.root","ttHmumu_13TeV_Hist.root",false,true,false);

  VariableHistogramer("DY_13TeV_stageVar.root","DY_13TeV_Hist.root",false,false,true);
  //VariableHistogramer("TT_13TeV_stageVar.root","TT_13TeV_Hist.root",false,false,false);
  
  ////////////////////////////////////////////////////////////
  // This inputs the stage 1 ntuples on the UF Tier 2 cluster
  //VariableHistogramer("ggHmumu_13TeV_stageVar.root","ggHmumu_13TeV_Hist.root",false,false);
  //VariableHistogramer("VBFHmumu_13TeV_stageVar.root","VBFHmumu_13TeV_Hist.root",false,false);
  //VariableHistogramer("VHmumu_13TeV_stageVar.root","VHmumu_13TeV_Hist.root",false,false);
  //VariableHistogramer("ttHmumu_13TeV_stageVar.root","ttHmumu_13TeV_Hist.root",false,false);
  //VariableHistogramer("DY_13TeV_stageVar.root","DY_13TeV_Hist.root",false,false);
  //VariableHistogramer("TT_13TeV_stageVar.root","TT_13TeV_Hist.root",false,false);

}


