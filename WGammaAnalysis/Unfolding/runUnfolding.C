{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  Unfolding unf(TConfiguration::ELECTRON, TConfiguration::W_GAMMA);
    unf.PrepareMigrationMatrix();
    unf.TestDifferentMethods();
    //unf.PlotAndStore();
}
