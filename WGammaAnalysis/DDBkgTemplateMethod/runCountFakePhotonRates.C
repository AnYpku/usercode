{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L CountFakePhotonRates.C");
  CountFakePhotonRates();
}
