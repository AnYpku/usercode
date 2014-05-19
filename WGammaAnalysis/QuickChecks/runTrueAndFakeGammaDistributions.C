{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L TrueAndFakeGammaDistributions.C+");
  RunComparisons();


}
