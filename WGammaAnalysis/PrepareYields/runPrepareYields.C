{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L QuickPrepareYields.C+");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  TConfiguration conf;

  // QuickPrepareYields_FSRandISR_Ich();


  // Run April, 26th, 2017:
  //    QuickPrepareYields_WMt(conf.MUON);
  //    QuickPrepareYields_WMt(conf.ELECTRON);
  //    QuickPrepareYields_EtoGammaEnriched(conf.ELECTRON);
  //    QuickPrepareYields_EtoGammaEnriched();
  //    QuickPrepareYields_FSRandISR();
   QuickPrepareYields_FSRandISR_TemplatesIch();
  // QuickPrepareYields_FSRandISR_TemplatesSihih();
 

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
