{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  FullChain fch;
  FullChain::FullChainParameters anPars;

  fch.SetDefaultFullChainParameters(anPars,"phoEt");
//  fch.SetDefaultFullChainParameters(anPars,"WMt");
//  fch.SetDefaultFullChainParameters(anPars,"phoSCEta");
//  fch.SetDefaultFullChainParameters(anPars,"phoPhi");
//  fch.SetDefaultFullChainParameters(anPars,"leptonPt");
//  fch.SetDefaultFullChainParameters(anPars,"leptonEta");
//  fch.SetDefaultFullChainParameters(anPars,"leptonPhi");
//  fch.SetDefaultFullChainParameters(anPars,"lePhoDeltaR");
//  fch.SetDefaultFullChainParameters(anPars,"pfMET");
//  fch.SetDefaultFullChainParameters(anPars,"pfMETPhi");

  std::cout<<"varFit="<<anPars.strDDbkgVarFit<<", varSideband="<<anPars.strDDbkgVarSideband<<std::endl;
  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  anPars.phoWP=TPhotonCuts::WP_TIGHT;
//  anPars.phoWP=TPhotonCuts::WP_MEDIUM;
  anPars.blind=TConfiguration::BLIND_PRESCALE;

  anPars.isDebugMode=0;
  anPars.sampleMode=WGammaSelection::SIGMC;

  anPars.noPreSelection=0;
  anPars.noExtraSelection=0;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=1;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
