
{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  TString fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wjets.root";
  SkimLeptonPhoton skimmer1(TConfiguration::BOTH_CHANNELS, TConfiguration::V_GAMMA, TConfiguration::BKGMC, fileToSkim,0);
  skimmer1.LoopOverInputTree();
  std::cout<<fileToSkim<<" is skimmed"<<std::endl;

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  time.Start("time");

  fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_DYToLL.root";
  SkimLeptonPhoton skimmer2(TConfiguration::BOTH_CHANNELS, TConfiguration::V_GAMMA, TConfiguration::BKGMC, fileToSkim,0);
//  TString fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaData2012Jan22ReReco/job_muon_2012a_Jan22rereco.root";
//  SkimLeptonPhoton skimmer(TConfiguration::MUON, TConfiguration::W_GAMMA, TConfiguration::DATA, fileToSkim);
  skimmer2.LoopOverInputTree();
  std::cout<<fileToSkim<<" is skimmed"<<std::endl;


//  TString fileToSkim="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaData2012Jan22ReReco/job_muon_2012a_Jan22rereco.root";
//  SkimLeptonPhoton skimmer1(TConfiguration::MUON, TConfiguration::W_GAMMA, TConfiguration::DATA, fileToSkim,0);
//  skimmer1.LoopOverInputTree();
//  std::cout<<fileToSkim<<" is skimmed"<<std::endl;


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
