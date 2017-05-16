{
  //gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L CombineTheoryCS.C+");
  CombineTheoryCS(TConfiguration::Z_GAMMA);
  CombineTheoryCS(TConfiguration::W_GAMMA);
}
