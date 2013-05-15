{
  //gROOT->ProcessLine(".L makePresentationPlots.C+");
  gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");
  gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  gROOT->ProcessLine(".L ../Configuration/TAllInputSamples.C+");
  gROOT->ProcessLine(".L TSelectionPlots.C+");
  gStyle->SetCanvasColor(0);
  //makePresentationPlots();

  int nBins = 20;
  float binLimits[nBins+1] = {0,10,20,30,40,50,60,80,90,100,120,130,140,150,160,170,180,190,200,210,220};
  TSelectionPlots plotter;
  string confFile = "../Configuration/config.txt";
  string plotVar = "pfMET";
  plotter.GetTrees(TInputSample::MUON, confFile);
  plotter.SetHistograms(plotVar, nBins, binLimits);
  plotter.ScaleHistogramsToData();
  plotter.DrawSpectrumDataVsMC();

  plotter.DrawSpectrumSigVsBkg();

}
