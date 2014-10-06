{

  //mcDecayType when the mcPID is 24 (W)
  //mcDecayType==1 => Wjetjet   / Zjetjet (no events here)
  //mcDecayType==2:   W->enu    / Z->ee
  //mcDecayType==3:   W->munu   / Z->mumu
  //mcDecayType==4:   W->taunu  / Z->tautau

/*
  TString nameInput="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg.root";
  TString nameEle="job_summer12_Wg_enu.root";
  TString nameMuo="job_summer12_Wg_munu.root";
  TString nameTau="job_summer12_Wg_taunu.root";
*/

  TString nameInput="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Zg.root";
  TString nameEle="job_summer12_Zg_ee.root";
  TString nameMuo="job_summer12_Zg_mumu.root";
  TString nameTau="job_summer12_Zg_tautau.root";


  TString nameDir="ggNtuplizer";
  TString nameTree="EventTree"; 

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  


  SplitVDecayMC splitter(nameInput,nameEle,nameMuo,nameTau,nameDir,nameTree,0); 
  splitter.LoopOverInputTree();


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
