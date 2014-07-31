{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L CheckTruePhotonEfficiency.C++");
  int year=2011;
  int wp = TPhotonCuts::WP_MEDIUM;
  TString isoBase="PF";
  CheckTruePhotonEfficiency(year, wp, isoBase);
  
}
