{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
//  gROOT->ProcessLine(".L ../ChangeVeryPreliminarySelected/ChangePUweight.C+");
  TConfiguration conf;
  ApplySF_Files(0.00, conf.MUON, conf.W_GAMMA, "../Configuration/config.txt");
}
