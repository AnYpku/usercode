{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  //initial selection
  WGammaSelection selection( TConfiguration::MUON, TConfiguration::DATA);
  selection.LoopOverInputFiles();
  //extra selection
  gROOT->ProcessLine(".L ../Selection/ExtraSelection.C+");
  ExtraSelection(TConfiguration::MUON);
  //compute fake-gamma background by data-driven template method
  TTemplates temp(TConfiguration::MUON);
  temp.ComputeBackground();
  //prepare yields
  TPrepareYields yields(TConfiguration::MUON);
  yields.PrepareYields();


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
