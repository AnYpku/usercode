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
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Pt");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Eta");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Phi");
//  fch.SetDefaultFullChainParameters(anPars,"lep1PhoDeltaR");
//  fch.SetDefaultFullChainParameters(anPars,"pfMET");
//  fch.SetDefaultFullChainParameters(anPars,"pfMETPhi");

  std::cout<<"varFit="<<anPars.strDDbkgVarFit<<", varSideband="<<anPars.strDDbkgVarSideband<<std::endl;
  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  anPars.noAdjustBinning=1;
  anPars.noPhoPFChIsoCut=1;

//  anPars.phoWP=TPhotonCuts::WP_TIGHT;
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;
  anPars.blind=TConfiguration::BLIND_PRESCALE;
  anPars.blind=TConfiguration::UNBLIND;
  anPars.vgamma=TConfiguration::Z_GAMMA;

  anPars.isDebugMode=0;
  anPars.doSystTemplateStat=0;
  anPars.sampleMode=Selection::SIGMC;
//  anPars.analyzedSamples="Zg_to_tautau";

  anPars.year=2012;
  anPars.noPreSelection=1;
  anPars.noExtraSelection=1;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=1;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
