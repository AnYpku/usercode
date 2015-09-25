{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
//  gROOT->ProcessLine(".L ../ChangeVeryPreliminarySelected/ChangePUweight.C+");
  TConfiguration conf;
  ChangePUweight_Files(2.00, conf.MUON, conf.W_GAMMA, "../Configuration/config.txt");
}
