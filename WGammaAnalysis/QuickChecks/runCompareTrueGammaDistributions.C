{
  gROOT->ProcessLine(".L CompareTrueGammaDistributions.C+"); 
  gStyle->SetCanvasColor(kWhite); 
  CompareSomePhotonEfficiencies();
  CompareTrueGammaDistributions();
}
