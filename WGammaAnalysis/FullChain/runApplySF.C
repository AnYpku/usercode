{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
//  gROOT->ProcessLine(".L ../ChangeVeryPreliminarySelected/ChangePUweight.C+");
  TConfiguration conf;
  ApplySF_Files(-1.00, conf.MUON, conf.W_GAMMA, "../Configuration/config.txt");
  ApplySF_Files(-1.00, conf.MUON, conf.Z_GAMMA, "../Configuration/config.txt");
  ApplySF_Files(-1.00, conf.ELECTRON, conf.Z_GAMMA, "../Configuration/config.txt");
}
