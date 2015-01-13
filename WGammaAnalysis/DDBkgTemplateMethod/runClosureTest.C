{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  TConfiguration::AnalysisParameters anPars;
  FullChain fch; 
  fch.SetDefaultFullChainParameters(anPars, "phoEt");

  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.vgamma=TConfiguration::W_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.blind=TConfiguration::UNBLIND;
  anPars.templFits=TConfiguration::TEMPL_SIHIH;

  anPars.cutAdd="1";

  anPars.noPreSelection=1;
  anPars.noExtraSelection=1;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=1;

//  AuxTemplatesRandCone(anPars);
//  AuxSubtractBackground(anPars);

  anPars.templFits=TConfiguration::TEMPL_CHISO;

  AuxTemplatesRandCone(anPars);
  AuxSubtractBackground(anPars);
}
