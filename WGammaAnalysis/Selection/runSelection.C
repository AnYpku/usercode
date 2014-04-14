{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
    //to run code in optimized mode; 
    //doesn't really make it much faster...

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  //WGammaSelection(int channel, int sampleMode, string configFile, bool isNoPuReweight, bool isDebugMode)
  //WGammaSelection(int channel, string analyzedSampleNames, string configFile, bool isNoPuReweight, bool isDebugMode)

  //WGammaSelection selection( TInputSample::MUON, "DYjets_to_ll", "../Configuration/config.txt");
  //selection.LoopOverInputFiles();

  //WGammaSelection selection(TConfiguration::MUON, "Wjets_to_lnu");
  //selection.LoopOverInputFiles();
  WGammaSelection selection(TConfiguration::MUON,"DYjets_to_ll Zg Wg_to_taunu multibosons ttbarg ttbarjets");
  selection.LoopOverInputFiles();

  gROOT->ProcessLine(".L ExtraSelection.C+");
  ExtraSelection(TConfiguration::MUON,"phoSCRChIsoCorr");


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
