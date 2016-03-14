{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L QuickPrepareYields.C+");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  TConfiguration conf;

  // QuickPrepareYields_FSRandISR_Ich();
  // QuickPrepareYields_FSRandISR_TemplatesIch();
  // QuickPrepareYields_FSRandISR_TemplatesSihih();


  //  QuickPrepareYields_WMt(conf.MUON);
  //  QuickPrepareYields_WMt(conf.ELECTRON);
  //  QuickPrepareYields_FSRandISR();
    QuickPrepareYields_EtoGammaEnriched();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
