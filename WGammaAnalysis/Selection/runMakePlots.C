{
    //gROOT->ProcessLine(".L makePresentationPlots.C+");
  gROOT->ProcessLine(".x ../Include/rootlogon.C+");

  gROOT->ProcessLine(".L auxSelectionPlots.C+");
  gStyle->SetCanvasColor(0);
    //makePresentationPlots();
  
  TSelectionPlots plotter;
  bool areTrees = plotter.GetTrees(TConfiguration::MUON, TConfiguration::W_GAMMA, TConfiguration::BLIND_PRESCALE, "../Configuration/config.txt");
  std::cout<<"areTrees="<<areTrees<<std::endl;
  int year = 2012;
  int vgamma = TConfiguration::W_GAMMA;
  int wp = TPhotonCuts::WP_MEDIUM;
  TString strIsoBase="PF";
  if (areTrees) plotter.SelectionEfficiencyInStages(year,vgamma,wp,strIsoBase);

//  PlotWithConsequetiveCuts();

  //PlotSigVsBkg();
  //PlotDataVsMC("lePhoDeltaR>0.7");
/*
  string confFile = "../Configuration/config.txt";
  TSelectionPlots plotter;
  plotter.GetTrees(TConfiguration::MUON, confFile);
  plotter.PrintWeightedNumberOfEvents("lePhoDeltaR>0.7");


  string confFile = "../Configuration/config.txt";
//  string plotVar="phoSCEt";
//  int nBins=9;
//  float binLimits[nBins+1]={15,20,25,30,35,40,60,80,200,600};
//  float binLimits[nBins+1]={15,20,30,40,60,100,1000};
  string plotVar="WMt";
  int nBins=20;
  float binLimits[nBins+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};

//  string plotVar="hasMoreLeptons";
//  int nBins=2;
//  float binLimits[nBins+1]={-0.5,0.5,1.5};
//  string plotVar="pfMETPhi";
//  int nBins=8;
//  float binLimits[nBins+1]={-3.1415,-2.355,-1.57,-0.785,0,0.785,1.57,2.355,3.1415};
//  string plotVar="phoElectronVeto";
//  int nBins=2;
//  float binLimits[nBins+1]={0,0.5,1.5};
//  TCut cutWeight="weight";

//  TCut cutTrg1="HLT_IsoMu24_v && trgMatchIsoMu24";
//  TCut cutTrg2="HLT_IsoMu24_eta2p1_ && trgMatchIsoMu24eta2p1";
//  TCut cutTrg=cutTrg2;

//  TCut cutLepton="!hasMoreLeptons && leptonId && leptonIsolation<0.12";

//  TCut cutWMt="WMt>50";

//  TPhotonCuts emptyPhoton;
//  TCut cutPhoton=emptyPhoton.RangePhoton(2, "SCR");
//  TMuonCuts emptyMuon;
//  TCut cutLepton=emptyMuon.RangeMuon();

//  TCut cut = (cutTrg && cutLepton && cutPhoton && cutWMt)*cutWeight;
  TCut cut = "weight";
  std::cout<<"cut="<<cut.GetTitle()<<std::endl;

  TSelectionPlots plotter;
  plotter.GetTrees(TConfiguration::MUON,confFile);
  plotter.SetHistograms(plotVar,nBins,binLimits,cut);
  plotter.DrawSpectrumDataVsMC("cWMt","WMt.png",1);
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
