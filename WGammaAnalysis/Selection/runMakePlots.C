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
  //PlotDataVsMC("lePhoDeltaR>0.7");
  string confFile = "../Configuration/config.txt";
  TSelectionPlots plotter;
  plotter.GetTrees(TInputSample::MUON, confFile);
  plotter.PrintWeightedNumberOfEvents("lePhoDeltaR>0.7");

/*
  string confFile = "../Configuration/config.txt";
  string plotVar="WMt";
  int nBins=20;
  float binLimits[nBins+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
  TSelectionPlots plotter;
  plotter.GetTrees(TInputSample::MUON, confFile, "DATA Wg_to_munu Wjets_to_lnu DYjets_to_ll Zg Wg_to_taunu multibosons topNotJets");
  plotter.SetHistograms(plotVar,nBins,binLimits,"lePhoDeltaR>0.7");
  plotter.DrawSpectrumDataVsMC("cWMt","WMt.png");
*/
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
