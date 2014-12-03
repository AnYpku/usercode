
{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  TString fileToSkim;

  fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg_enu.root";
  SkimLeptonPhoton skimmer3(TConfiguration::ELECTRON, TConfiguration::W_GAMMA, TConfiguration::BKGMC, fileToSkim,0);
  skimmer3.LoopOverInputTree();

  fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg_taunu.root";
  SkimLeptonPhoton skimmer4(TConfiguration::ELECTRON, TConfiguration::W_GAMMA, TConfiguration::BKGMC, fileToSkim,0);
  skimmer4.LoopOverInputTree();



  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
