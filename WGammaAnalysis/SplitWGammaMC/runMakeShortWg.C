{
  TFile fIn("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg.root");
  fIn.cd("ggNtuplizer");
  TTree* trIn=(TTree*)gDirectory->Get("EventTree");
  trIn->Print();

  TFile f("WG_test.root","recreate");
  f.mkdir("ggNtuplizer");
  f.cd("ggNtuplizer");
  TTree* trOut = (TTree*)trIn->CopyTree("event<10000");
  trOut->Write();
}
