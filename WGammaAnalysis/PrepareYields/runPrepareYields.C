{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  fch.SetDefaultFullChainParameters(anPars,"phoEt");
//  fch.SetDefaultFullChainParameters(anPars,"phoEtNoPhosphor");
//  fch.SetDefaultFullChainParameters(anPars,"Mleplep");
//  fch.SetDefaultFullChainParameters(anPars,"Mpholeplep");
//  fch.SetDefaultFullChainParameters(anPars,"Mpholep1");
//  fch.SetDefaultFullChainParameters(anPars,"Mpholep2");
//  fch.SetDefaultFullChainParameters(anPars,"WMt");
//  fch.SetDefaultFullChainParameters(anPars,"phoSCEta");
//  fch.SetDefaultFullChainParameters(anPars,"phoPhi");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Pt");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Eta");
//  fch.SetDefaultFullChainParameters(anPars,"lepton1Phi");
//  fch.SetDefaultFullChainParameters(anPars,"lep1PhoDeltaR");
//  fch.SetDefaultFullChainParameters(anPars,"pfMET");
//  fch.SetDefaultFullChainParameters(anPars,"pfMETPhi");g

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon; 
  anPars.phoWP=photon.WP_MEDIUM;//WP_TIGHT, WP_MEDIUM, WP_LOOSE
  anPars.isDebugMode=0;
  anPars.cutAdd="1";//"phoEt<40";
//  anPars.cutAdd=photon.RangeSigmaIEtaIEta(2012,photon.WP_MEDIUM,conf.COMMON);

  anPars.blind[conf.MUON][conf.W_GAMMA]=conf.BLIND_COMBINED;
  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;
  anPars.blind[conf.BOTH_CHANNELS][conf.Z_GAMMA]=conf.UNBLIND;
  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.BLIND_COMBINED;
  anPars.blind[conf.ELECTRON][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  //void AuxPrepareYields(TConfiguration::AnalysisParameters &anPars, bool isMCclosure, int selStage=config.FULLY)

  int selStage=conf.FSR_EXCLUDED;
  std::cout<<"runPrepareYields: selStage="<<conf.StrSelectionStage(selStage)<<std::endl;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd="(phoSCEta>-1.4442 && phoSCEta<1.4442) && phoSigmaIEtaIEta<0.011 || ( phoSCEta*phoSCEta<2.5*2.5 && phoSCEta*phoSCEta>1.5*1.5 && phoSigmaIEtaIEta<0.033)";
  AuxPrepareYields(anPars,0,selStage);
  anPars.cutAdd="1";

/*
  int selStage=conf.FULLY;
  std::cout<<"runPrepareYields: selStage="<<conf.StrSelectionStage(selStage)<<std::endl;

  fch.SetDefaultFullChainParameters(anPars,"Mpholeplep");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  AuxPrepareYields(anPars,0,selStage); 

  fch.SetDefaultFullChainParameters(anPars,"Mleplep");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  AuxPrepareYields(anPars,0,selStage); 

  fch.SetDefaultFullChainParameters(anPars,"lep1PhoDeltaR");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  AuxPrepareYields(anPars,0,selStage);

  fch.SetDefaultFullChainParameters(anPars,"lep2PhoDeltaR");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  AuxPrepareYields(anPars,0,selStage);
*/

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}