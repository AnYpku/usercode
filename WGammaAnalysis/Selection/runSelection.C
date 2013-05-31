{
  gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");
  gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  gROOT->ProcessLine(".L ../Configuration/TAllInputSamples.C+");

  gROOT->ProcessLine(".L ../Include/TEventTree.C+");
  gROOT->ProcessLine(".L ../Include/TMuonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TElectronCuts.C+");
  gROOT->ProcessLine(".L ../Include/TPhotonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TMetTools.C+");
  gROOT->ProcessLine(".L ../Include/TFullCuts.C+");
  gROOT->ProcessLine(".L TSelectedEventsTree.C+");
  gROOT->ProcessLine(".L ../Include/PhosphorCorrectorFunctor.cc+");

  gROOT->ProcessLine(".L WGammaSelection.C+");



  gSystem->SetAclicMode(TSystem::kOpt);
    //to run code in optimized mode; 
    //doesn't really make it much faster...

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

//WGammaSelection(int channel, int sampleMode, string configFile, bool isReleasedCutsMode, bool isDebugMode)
//WGammaSelection(int channel, string analyzedSampleNames, string configFile, bool isReleasedCutsMode, bool isDebugMode)

  //WGammaSelection selection( TInputSample::MUON, WGammaSelection::DATA, "../Configuration/config.txt");
  //selection.LoopOverInputFiles();

  //WGammaSelection selection( TInputSample::MUON, "DYjets_to_ll", "../Configuration/config.txt", 1);
  //selection.LoopOverInputFiles();

  WGammaSelection selection( TInputSample::MUON, "Wg_to_munu", "../Configuration/config.txt", 0, 1);
  selection.LoopOverInputFiles();

  //WGammaSelection selection( TInputSample::MUON, "Wg_to_munu Wjets_to_lnu DYjets_to_ll ttbarjet Zg Wg_to_taunu", "../Configuration/config.txt", 1, 0);
  //selection.LoopOverInputFiles();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  



}
