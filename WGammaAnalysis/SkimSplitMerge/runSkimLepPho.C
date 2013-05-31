{
  gROOT->ProcessLine(".L ../Include/TEventTree.C+");
  gROOT->ProcessLine(".L TInputOutputTree.C+");
  gROOT->ProcessLine(".L SkimLeptonPhoton.C+");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  SkimLeptonPhoton skimmer("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_WJetsToLNu2_skim.root", "ggNtuplizer", "EventTree", "./", SkimLeptonPhoton::MUON);
  skimmer.LoopOverInputTree();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
