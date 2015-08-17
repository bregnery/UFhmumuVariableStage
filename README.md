# UFhmumuVariableStage
This code calculates additional variables using the stage1 ntuples created by Justin's UFDiMuonsAnalyzer.

Current path: Data -> UFDiMuonsAnalyzer -> UFhmumuVariableStage

Instructions
============

Log on to one of the UF Tier 2 servers.  

You will need a working copy of ROOT to analyze Ntuples.  The easiest way to get one is to create a CMSSW release, move into it and setup your environment.

    cmsrel CMSSW_7_4_2
    cd CMSSW_7_4_2
    cmsenv

Now you should be able to type the root command and get a working prompt, if so you can continue to setup the anlaysis package.

First, clone this git repository:

    git clone https://github.com/bregnery/UFhmumuVariableStage.git
    git pull

Then recompile CMSSW:

    scram b
    
move into hmumuToyAnalysis.  VariableAdder.C is an script for calculating new variables in our Ntuples.  To run it, and load the needed libraries, execute:

    root -b -q -x run.C
    
