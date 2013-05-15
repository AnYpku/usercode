{
  //gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  //gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");
  gROOT->ProcessLine(".L ../Selection/TEventTree.C+");
  gROOT->ProcessLine(".L ../Include/TMuonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TPhotonCuts.C+");
  gROOT->ProcessLine(".L CheckParticleCuts.C+");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl; 


  TFile f("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_munu.root");
  f.cd("ggNtuplizer");
  TTree* tr = (TTree*)gDirectory->Get("EventTree");

  CheckCuts(tr);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
