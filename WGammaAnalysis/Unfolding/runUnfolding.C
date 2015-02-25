{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  Unfolding unf(TConfiguration::MUON, TConfiguration::Z_GAMMA);
    unf.PrepareMigrationMatrix();
    unf.TestDifferentMethods();
    unf.PlotAndStore();
}
