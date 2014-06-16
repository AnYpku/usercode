{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  Unfolding unf(TConfiguration::MUON);
  unf.PrepareMigrationMatrix();
  unf.TestDifferentMethods();
  unf.PlotAndStore();
}
