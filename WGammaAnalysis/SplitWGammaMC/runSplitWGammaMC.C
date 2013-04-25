{
  gROOT->ProcessLine(".L TInputTree.C+");
  gROOT->ProcessLine(".L TOutputTree.C+");
  gROOT->ProcessLine(".L SplitWGammaMC.C+");

  //mcDecayType when the mcPID is 24 (W)
  //mcDecayType==1 => Wjetjet (no events here)
  //mcDecayType==2:   W->enu
  //mcDecayType==3:   W->munu
  //mcDecayType==4:   W->taunu

  TString nameWGammaSample[5];
  nameWGammaSample[0]="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg.root";
  //nameWGammaSample[0]="WG_test.root";
  //nameWGammaSample[1]="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/nothing.root"; (no events)
  //nameWGammaSample[2]="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_enu.root";
  //nameWGammaSample[3]="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_munu.root";
  //nameWGammaSample[4]="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_taunu.root";
  nameWGammaSample[1]="nothing.root";
  nameWGammaSample[2]="job_summer12_Wg_enu.root";
  nameWGammaSample[3]="job_summer12_Wg_munu.root";
  nameWGammaSample[4]="job_summer12_Wg_taunu.root";

  TString nameDir="ggNtuplizer";
  TString nameTree="EventTree"; 

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  


  SplitWGammaMC splitter(nameWGammaSample,nameDir,nameTree); 
  splitter.LoopOverInputTree();


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
  
}
