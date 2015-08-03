  gSystem->cd("../");
  gROOT->ProcessLine(".L VariableAdder.C++");

  // This inputs the stage 1 ntuples on the UF hipergator 
  VariableAdder("/scratch/osg/bregnery/stage1/CMSSW_7_4_2/src/UserArea/UFDiMuonsAnalyzer/test/ggHmumu_13TeV_stage1.root","ggHmumu_13TeV_stageVar.root",false,false);
  VariableAdder("/scratch/osg/bregnery/stage1/CMSSW_7_4_2/src/UserArea/UFDiMuonsAnalyzer/test/VBFHmumu_13TeV_stage1.root","VBFHmumu_13TeV_stageVar.root",false,false);
  VariableAdder("/scratch/osg/bregnery/stage1/CMSSW_7_4_2/src/UserArea/UFDiMuonsAnalyzer/test/VHmumu_13TeV_stage1.root","VHmumu_13TeV_stageVar.root",false,false);
  VariableAdder("/scratch/osg/bregnery/stage1/CMSSW_7_4_2/src/UserArea/UFDiMuonsAnalyzer/test/ttHmumu_13TeV_Stage1.root","ttHmumu_13TeV_stageVar.root",false,false);
  VariableAdder("/scratch/osg/bregnery/stage1/CMSSW_7_4_2/src/UserArea/UFDiMuonsAnalyzer/test/stage_1_dy_jetsToLL_asympt50_ALL.root","DY_13TeV_stageVar.root",false,false);
  VariableAdder("/scratch/osg/bregnery/stage1/CMSSW_7_4_2/src/UserArea/UFDiMuonsAnalyzer/test/TT_13TeV_Stage1.root","TT_13TeV_stageVar.root",false,false);

  // This inputs the stage 1 ntuples on the UF Tier 2 cluster
  //VariableAdder("/raid/raid9/bregnery/13TeV/stage1/ggHmumu_13TeV_stage1.root","ggHmumu_13TeV_stageVar.root",false,false);
  //VariableAdder("/raid/raid9/bregnery/13TeV/stage1/VBFHmumu_13TeV_stage1.root","VBFHmumu_13TeV_stageVar.root",false,false);
  //VariableAdder("/raid/raid9/bregnery/13TeV/stage1/VHmumu_13TeV_stage1.root","VHmumu_13TeV_stageVar.root",false,false);
  //VariableAdder("/raid/raid9/bregnery/13TeV/stage1/ttHmumu_13TeV_Stage1.root","ttHmumu_13TeV_stageVar.root",false,false);
  //VariableAdder("/raid/raid9/bregnery/13TeV/stage1/stage_1_dy_jetsToLL_asympt50_ALL.root","DY_13TeV_stageVar.root",false,false);
  //VariableAdder("/raid/raid9/bregnery/13TeV/stage1/TT_13TeV_Stage1.root","TT_13TeV_stageVar.root",false,false);
