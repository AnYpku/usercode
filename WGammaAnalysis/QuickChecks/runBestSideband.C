{

  gROOT->ProcessLine(".L BestSideband.C+");

  TFile fSign("../WGammaOutput/MUON/VeryPreliminarySelected/selectedSIGMC.root");
  TFile fBkg1("../WGammaOutput/MUON/VeryPreliminarySelected/selectedBKGMC_DYjets_to_ll.root");
  TFile fBkg2("../WGammaOutput/MUON/VeryPreliminarySelected/selectedBKGMC_ttbarjets.root");
  TFile fData("../WGammaOutput/MUON/VeryPreliminarySelected/selectedDATA.root");

  gStyle->SetCanvasColor(kWhite); 
  gStyle->SetPalette(1);
  TTree* trSign = (TTree*)fSign.Get("selectedEvents");
  TTree* trBkg1 = (TTree*)fBkg1.Get("selectedEvents");
  TTree* trBkg2 = (TTree*)fBkg2.Get("selectedEvents");
  TTree* trData = (TTree*)fData.Get("selectedEvents");

  TCut cutWeight("weight");
  TCut cutBarrel("phoEta>-1.442 && phoEta<1.442");
  TCut cutEndcap("(phoEta>-2.5 && phoEta<-1.566)||(phoEta<2.5 && phoEta>1.566)");
  TCut cutPhoSigmaIEtaIEta("phoSigmaIEtaIEta<1");
  TCut cutTruePhoton("phoGenPID==22");
  TCut cutPt("phoEt>15");
  int nPointsLeft=20;
  int nPointsRight=1;
  float sidebandLeftMin=-1;
  float sidebandLeftMax=10;
  float sidebandRightMin=0;
  float sidebandRightMax=100000;
  TFile fOut("fBestSideband.root","recreate");
  BestSidebandLeftAndRight(trData, trSign, trBkg1, trBkg2, "phoSCRChIsoCorr", cutPt && cutBarrel && cutPhoSigmaIEtaIEta, cutWeight, cutTruePhoton, nPointsLeft,  sidebandLeftMin, sidebandLeftMax, nPointsRight, sidebandRightMin, sidebandRightMax);

}
