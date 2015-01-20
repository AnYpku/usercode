{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  TConfiguration::AnalysisParameters anPars;
  FullChain fch; 
  fch.SetDefaultFullChainParameters(anPars, "phoEt");

  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.blind=TConfiguration::UNBLIND;

  anPars.cutAdd="1";

  anPars.noPreSelection=1;
  anPars.noExtraSelection=1;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=1;

  anPars.vgamma=TConfiguration::W_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.templFits=TConfiguration::TEMPL_SIHIH;
  AuxTemplatesRandCone(anPars);
  AuxSubtractBackground(anPars);

  anPars.vgamma=TConfiguration::W_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.templFits=TConfiguration::TEMPL_CHISO;
  AuxTemplatesRandCone(anPars);
  AuxSubtractBackground(anPars);

  anPars.vgamma=TConfiguration::Z_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.templFits=TConfiguration::TEMPL_SIHIH;
//  AuxTemplatesRandCone(anPars);
//  AuxSubtractBackground(anPars);

  anPars.vgamma=TConfiguration::Z_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.templFits=TConfiguration::TEMPL_CHISO;
//  AuxTemplatesRandCone(anPars);
//  AuxSubtractBackground(anPars);
}
