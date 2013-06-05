{
    //gROOT->ProcessLine(".L makePresentationPlots.C+");
  gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");
  gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  gROOT->ProcessLine(".L ../Configuration/TAllInputSamples.C+");
  gROOT->ProcessLine(".L TSelectionPlots.C+");
  gROOT->ProcessLine(".L auxSelectionPlots.C+");
  gStyle->SetCanvasColor(0);
    //makePresentationPlots();

  //PlotSigVsBkg();
  //PlotDataVsMC();


  string confFile = "../Configuration/config.txt";
  string plotVar="pfMETPhi";
  int nBins=12;
  float binLimits[nBins+1]={-3,-2.5,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,2.5,3};
  TSelectionPlots plotter;
  plotter.GetTrees(TInputSample::MUON, confFile, "DATA Wg_to_munu Wjets_to_lnu DYjets_to_ll Zg Wg_to_taunu multibosons topNotJets");
  plotter.SetHistograms(plotVar,nBins,binLimits,"lePhoDeltaR>0.7");
  plotter.DrawSpectrumDataVsMC("cpfMETPhi","pfMETPhi_dR07_cut.png");

/*
  string confFile = "../Configuration/config.txt";
  string plotVar="lePhoDeltaR";
  int nBins=16;
  float binLimits[nBins+1]={0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6};
  TSelectionPlots plotter;
  plotter.GetTrees(TInputSample::MUON, confFile, "DATA Wg_to_munu Wjets_to_lnu DYjets_to_ll Zg Wg_to_taunu multibosons topNotJets");
  plotter.SetHistograms(plotVar,nBins,binLimits,"1");
  plotter.DrawSpectrumDataVsMC("cdR","dR.png");
*/
  
}
