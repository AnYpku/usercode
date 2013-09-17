{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L DrawMigrationMatrix.C+");

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl; 


  TFile f("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_munu.root");
  f.cd("ggNtuplizer");
  TTree* tr = (TTree*)gDirectory->Get("EventTree");

  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("0.2f");
  DrawMigrationMatrix(tr);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}
