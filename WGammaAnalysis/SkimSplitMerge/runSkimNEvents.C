{
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  TString fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_DYToLL.root";
  long N=5000000;
  SkimNEvents skimmer(N, TConfiguration::MUON, TConfiguration::SIGMC, fileToSkim, "ggNtuplizer", "EventTree", "./",0);
  skimmer.LoopOverInputTree();
  std::cout<<fileToSkim<<" is \""<< N <<"events\" skimmed"<<std::endl;

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
