{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  FullChain fch;
  FullChain::FullChainParameters anPars;

  fch.SetDefaultFullChainParameters(anPars,"phoEt");
//  fch.SetDefaultFullChainParameters(anPars,"phoEtNoPhosphor");
//  fch.SetDefaultFullChainParameters(anPars,"Mleplep");
//  fch.SetDefaultFullChainParameters(anPars,"WMt");
//  fch.SetDefaultFullChainParameters(anPars,"phoSCEta");
//  fch.SetDefaultFullChainParameters(anPars,"phoPhi");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Pt");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Eta");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Phi");
//  fch.SetDefaultFullChainParameters(anPars,"lep1PhoDeltaR");
//  fch.SetDefaultFullChainParameters(anPars,"pfMET");
//  fch.SetDefaultFullChainParameters(anPars,"pfMETPhi");

  std::cout<<"configfile="<<anPars.configfile<<std::endl;


  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;//WP_TIGHT, WP_MEDIUM, WP_LOOSE
  anPars.blind=TConfiguration::BLIND_PRESCALE;//BLIND_PRESCALE or UNBLIND
  anPars.vgamma=TConfiguration::W_GAMMA;//W_GAMMA or Z_GAMMA

  anPars.isDebugMode=0;
  anPars.doSystTemplateStat=0;
  anPars.sampleMode=Selection::DATA;//SIGMC, DATA, BKGMC, ALL, MC, NOBKG, NOTSPECIFIED;
//  anPars.analyzedSamples="WWg"; anPars.sampleMode=Selection::NOTSPECIFIED; 

  anPars.year=2012;
  anPars.noPreSelection=0;
  anPars.noExtraSelection=0;
  anPars.noDDBkgComputation=1;
  anPars.noPrepareYields=1;
  anPars.noCalcAccAndEff=1;
  anPars.noCalcCrossSection=1;

  anPars.noSystDDBkgSidebandVariation=1;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
