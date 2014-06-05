{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L SanityCheckForSelection.C+");

  SanityCheckForSelection();
}
