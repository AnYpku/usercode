{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  FullChain fch;
  FullChain::FullChainParameters anPars;
//  fch.SetDefaultFullChainParameters(anPars,"WMt");
  fch.SetDefaultFullChainParameters(anPars,"phoEt");
  anPars.phoWP=TPhotonCuts::WP_TIGHT;
  anPars.blind=TConfiguration::BLIND_PRESCALE;

  anPars.isDebugMode=0;
  anPars.sampleMode=WGammaSelection::SIGMC;

  anPars.noPreSelection=1;
  anPars.noExtraSelection=1;
  anPars.noDDBkgComputation=1;
  anPars.noPrepareYields=1;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=0;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
