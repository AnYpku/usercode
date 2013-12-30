{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L EventTreeTest.C+");
  EventTreeTest();
}
