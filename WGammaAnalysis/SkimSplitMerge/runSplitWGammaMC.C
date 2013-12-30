{
  gROOT->ProcessLine(".L ../Include/TEventTree.C+");
  gROOT->ProcessLine(".L TInputOutputTree.C+");
  gROOT->ProcessLine(".L SplitWGammaMC.C+");

  //mcDecayType when the mcPID is 24 (W)
  //mcDecayType==1 => Wjetjet (no events here)
  //mcDecayType==2:   W->enu
  //mcDecayType==3:   W->munu
  //mcDecayType==4:   W->taunu

  TString nameWGammaInput="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg.root";
  TString nameWGammaEle="job_summer12_Wg_enu.root";
  TString nameWGammaMuo="job_summer12_Wg_munu.root";
  TString nameWGammaTau="job_summer12_Wg_taunu.root";
  TString nameDir="ggNtuplizer";
  TString nameTree="EventTree"; 

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  


  SplitWGammaMC splitter(nameWGammaInput,nameWGammaEle,nameWGammaMuo,nameWGammaTau,nameDir,nameTree); 
  splitter.LoopOverInputTree();


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
