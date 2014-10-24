{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

//  TString fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg.root";
//  SkimLeptonPhoton skimmer(TConfiguration::MUON, TConfiguration::BKGMC, fileToSkim, "ggNtuplizer", "EventTree", "./");
  TString fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaData2012Jan22ReReco/job_2electron_2012a_Jan22rereco.root";
  SkimLeptonPhoton skimmer(TConfiguration::ELECTRON, TConfiguration::DATA, fileToSkim);

  skimmer.LoopOverInputTree();
  std::cout<<fileToSkim<<" is skimmed"<<std::endl;

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
