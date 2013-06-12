{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
    //to run code in optimized mode; 
    //doesn't really make it much faster...

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

//WGammaSelection(int channel, int sampleMode, string configFile, bool isReleasedCutsMode, bool isPuReweight, bool isDebugMode)
//WGammaSelection(int channel, string analyzedSampleNames, string configFile, bool isReleasedCutsMode, bool isPuReweight, bool isDebugMode)

  //WGammaSelection selection( TInputSample::MUON, WGammaSelection::DATA, "../Configuration/config.txt");
  //selection.LoopOverInputFiles();

  //WGammaSelection selection( TInputSample::MUON, "DYjets_to_ll", "../Configuration/config.txt", 1);
  //selection.LoopOverInputFiles();

  WGammaSelection selection( TInputSample::MUON, "Wg_to_munu", "../Configuration/config.txt", 0, 1, 1);
  selection.LoopOverInputFiles();

  //WGammaSelection selection( TInputSample::MUON, "Wg_to_munu Wjets_to_lnu DYjets_to_ll ttbarjet Zg Wg_to_taunu", "../Configuration/config.txt", 1, 0);
  //selection.LoopOverInputFiles();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  



}
