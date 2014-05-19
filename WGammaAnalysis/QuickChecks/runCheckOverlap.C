{
  TFile fFull("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wjets.root");
  fFull.cd("ggNtuplizer");
  TTree* tFull = (TTree*)gDirectory->Get("EventTree");

  TFile fSkim("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/MuPhoSkim/job_summer12_Wjets_MuPhoSkim.root");
  fSkim.cd("ggNtuplizer");
  TTree* tSkim = (TTree*)gDirectory->Get("EventTree");
  TH1F* hSkim = (TH1F*)gDirectory->Get("hskim");

}
