{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  gSystem->SetAclicMode(TSystem::kOpt);
    //to run code in optimized mode; 
    //doesn't really make it much faster...

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

//  SearchForOverlap search(TConfiguration::MUON,SearchForOverlap::CUT_PAR,0,"Zg DYjets_to_ll Wg Wjets_to_lnu");
  SearchForOverlap search(TConfiguration::MUON,SearchForOverlap::CUT_KIN_MLL_PAR,0,"Zg DYjets_to_ll");
  search.LoopOverInputFiles();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
