{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  Unfolding unf(TConfiguration::MUON, TConfiguration::W_GAMMA);
    unf.PrepareMigrationMatrix();
    unf.TestDifferentMethods();
    //unf.PlotAndStore();
}
