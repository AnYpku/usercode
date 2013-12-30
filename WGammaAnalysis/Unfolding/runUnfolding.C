{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  Unfolding unf(TConfiguration::MUON);
  unf.PrepareMigrationMatrix();
  unf.PrepareUnfoldingMatrix();
  unf.ClosureTest();
  unf.Store();
}
