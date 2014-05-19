{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
    //to run code in optimized mode; 
    //doesn't really make it much faster...

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

//  WGammaSelection sel(TConfiguration::MUON,WGammaSelection::SIGMC);
//  sel.LoopOverInputFiles();
//  WGammaSelection sel(TConfiguration::MUON);
//  sel.LoopOverInputFiles();
  
  TPhotonCuts emptyPhoton;
  int wp = emptyPhoton.GetWP();
  ExtraSelection(TConfiguration::MUON,wp,"SCR");

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
