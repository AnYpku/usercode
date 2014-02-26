{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  Unfolding unf(TConfiguration::MUON);
  bool itsOk1=1;
  bool itsOk2=1;
  if (!unf.PrepareConstants()) {itsOk1=0; itOk2=0;}
  if (itsOk1 && itsOk2) unf.TestDifferentMethods();
}
