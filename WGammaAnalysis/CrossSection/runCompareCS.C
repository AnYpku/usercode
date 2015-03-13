{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L CompareCS.C+");
  CompareCS(TConfiguration::Z_GAMMA);
    CompareCS(TConfiguration::W_GAMMA);
}
