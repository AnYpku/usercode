{
  gROOT->ProcessLine(".L makePresentationPlots.C+");
  gStyle->SetCanvasColor(0);
  makePresentationPlots();
}
