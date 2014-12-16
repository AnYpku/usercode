{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L DifferentTemplates.C+");
  DifferentTemplates();
}
