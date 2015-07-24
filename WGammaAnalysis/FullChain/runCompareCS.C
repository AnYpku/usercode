{
//  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L ../CrossSection/CompareCS.C+");
  CompareCS(TConfiguration::Z_GAMMA);
  CompareCS(TConfiguration::W_GAMMA);
}
