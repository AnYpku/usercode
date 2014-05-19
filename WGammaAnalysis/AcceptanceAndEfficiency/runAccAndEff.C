{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
    //to run code in optimized mode; 
    //doesn't really make it much faster...

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  CalcAccAndEff accAndEff( TConfiguration::MUON, TPhotonCuts::WP_MEDIUM, "SCR", "../Configuration/config.txt");
  //CalcAccAndEff accAndEff( TInputSample::MUON);
  accAndEff.LoopOverInputFiles();


  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  



}
