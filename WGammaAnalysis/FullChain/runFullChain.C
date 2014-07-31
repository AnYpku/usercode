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
//  fch.SetDefaultFullChainParameters(anPars,"phoEta");
//  fch.SetDefaultFullChainParameters(anPars,"phoPhi");
//  fch.SetDefaultFullChainParameters(anPars,"leptonPt");
//  fch.SetDefaultFullChainParameters(anPars,"leptonEta");
//  fch.SetDefaultFullChainParameters(anPars,"leptonPhi");
//  fch.SetDefaultFullChainParameters(anPars,"lePhoDeltaR");
//  fch.SetDefaultFullChainParameters(anPars,"pfMET");
//  fch.SetDefaultFullChainParameters(anPars,"pfMETPhi");

  std::cout<<"varFit="<<anPars.strDDbkgVarFit<<", varSideband="<<anPars.strDDbkgVarSideband<<std::endl;
  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  anPars.noAdjustBinning=1;
  anPars.noPhoPFChIsoCut=1;

//  anPars.phoWP=TPhotonCuts::WP_TIGHT;
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;
  anPars.blind=TConfiguration::BLIND_PRESCALE;

  anPars.isDebugMode=0;
  anPars.sampleMode=WGammaSelection::ALL;

  anPars.year=2012;
  anPars.noPreSelection=1;
  anPars.noExtraSelection=1;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=1;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=1;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
