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
  anPars.sampleMode=Selection::DATA;//SIGMC, DATA, BKGMC, ALL, MC, NOBKG, NOTSPECIFIED;
//  anPars.sampleMode=Selection::NOTSPECIFIED; anPars.analyzedSamples="ttbarjets ttbarg";
  anPars.cutAdd="1";//"phoEt<40";
//  anPars.cutAdd=photon.RangeSigmaIEtaIEta(2012,photon.WP_MEDIUM,conf.COMMON);

  anPars.blind[conf.MUON][conf.W_GAMMA]=conf.BLIND_COMBINED;
  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;
  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.BLIND_COMBINED;
  anPars.blind[conf.ELECTRON][conf.Z_GAMMA]=conf.UNBLIND;

  // [conf.MUON][conf.W_GAMMA]
  anPars.noPreSelection[conf.MUON][conf.W_GAMMA]=1;
  anPars.noExtraSelection[conf.MUON][conf.W_GAMMA]=1;
  anPars.noDDBkgComputation[conf.MUON][conf.W_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noDDBkgComputation[conf.MUON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noPrepareYields[conf.MUON][conf.W_GAMMA]=1;
  anPars.noSubtractBackground[conf.MUON][conf.W_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSubtractBackground[conf.MUON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noSubtractBackground[conf.MUON][conf.W_GAMMA][conf.TEMPL_OVERLAY]=1;
  anPars.noCalcAccAndEff[conf.MUON][conf.W_GAMMA]=1;
  anPars.noCalcCrossSection[conf.MUON][conf.W_GAMMA]=1;
  anPars.noSystDDBkgSidebandVariation[conf.MUON][conf.W_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSystDDBkgSidebandVariation[conf.MUON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noCalcAccAndEff[conf.MUON][conf.W_GAMMA]=1;
  anPars.noCalcCrossSection[conf.MUON][conf.W_GAMMA]=1;

  // [conf.MUON][conf.Z_GAMMA]
  anPars.noPreSelection[conf.MUON][conf.Z_GAMMA]=1;
  anPars.noExtraSelection[conf.MUON][conf.Z_GAMMA]=1;
  anPars.noDDBkgComputation[conf.MUON][conf.Z_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noDDBkgComputation[conf.MUON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noPrepareYields[conf.MUON][conf.Z_GAMMA]=1;
  anPars.noSubtractBackground[conf.MUON][conf.Z_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSubtractBackground[conf.MUON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noSubtractBackground[conf.MUON][conf.Z_GAMMA][conf.TEMPL_OVERLAY]=1;
  anPars.noCalcAccAndEff[conf.MUON][conf.Z_GAMMA]=1;
  anPars.noCalcCrossSection[conf.MUON][conf.Z_GAMMA]=1;
  anPars.noSystDDBkgSidebandVariation[conf.MUON][conf.Z_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSystDDBkgSidebandVariation[conf.MUON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noCalcAccAndEff[conf.MUON][conf.Z_GAMMA]=1;
  anPars.noCalcCrossSection[conf.MUON][conf.Z_GAMMA]=1;

  // [conf.ELECTRON][conf.W_GAMMA]
  anPars.noPreSelection[conf.ELECTRON][conf.W_GAMMA]=1;
  anPars.noExtraSelection[conf.ELECTRON][conf.W_GAMMA]=1;
  anPars.noDDBkgComputation[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noDDBkgComputation[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noPrepareYields[conf.ELECTRON][conf.W_GAMMA]=0;
  anPars.noSubtractBackground[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSubtractBackground[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noSubtractBackground[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_OVERLAY]=1;
  anPars.noCalcAccAndEff[conf.ELECTRON][conf.W_GAMMA]=1;
  anPars.noCalcCrossSection[conf.ELECTRON][conf.W_GAMMA]=1;
  anPars.noSystDDBkgSidebandVariation[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSystDDBkgSidebandVariation[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noCalcAccAndEff[conf.ELECTRON][conf.W_GAMMA]=1;
  anPars.noCalcCrossSection[conf.ELECTRON][conf.W_GAMMA]=1;

  // [conf.ELECTRON][conf.Z_GAMMA]
  anPars.noPreSelection[conf.ELECTRON][conf.Z_GAMMA]=1;
  anPars.noExtraSelection[conf.ELECTRON][conf.Z_GAMMA]=1;
  anPars.noDDBkgComputation[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noDDBkgComputation[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noPrepareYields[conf.ELECTRON][conf.Z_GAMMA]=1;
  anPars.noSubtractBackground[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSubtractBackground[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noSubtractBackground[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_OVERLAY]=1;
  anPars.noCalcAccAndEff[conf.ELECTRON][conf.Z_GAMMA]=1;
  anPars.noCalcCrossSection[conf.ELECTRON][conf.Z_GAMMA]=1;
  anPars.noSystDDBkgSidebandVariation[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_CHISO]=1;
  anPars.noSystDDBkgSidebandVariation[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  anPars.noCalcAccAndEff[conf.ELECTRON][conf.Z_GAMMA]=1;
  anPars.noCalcCrossSection[conf.ELECTRON][conf.Z_GAMMA]=1;


  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
