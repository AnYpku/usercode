{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl; 

  TPrepareYields yields(TConfiguration::MUON);
  yields.PrepareYields();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
