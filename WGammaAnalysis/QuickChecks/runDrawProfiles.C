{
 
  gROOT->ProcessLine(".L DrawProfiles.C+");
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(kWhite); 

  TFile fData("../WGammaOutput/MUON/VeryPreliminarySelected/selectedDATA.root");
  TFile fSign("../WGammaOutput/MUON/VeryPreliminarySelected/selectedSIGMC.root");
  TTree* trData = (TTree*)fData.Get("selectedEvents");
  TTree* trSign = (TTree*)fSign.Get("selectedEvents");

  TCut cut_phoEt("phoEt>15 && phoEt<600");
  TCut cut_phoEtStrict("phoEt>15 && phoEt<300");
  TCut cut_phoEtHigh("phoEt>80");
  TCut cut_barrel("phoEta>-1.442 && phoEta<1.442");
  TCut cut_endcap("(phoEta>-2.5 && phoEta<-1.566) || (phoEta>1.566 && phoEta<2.5)");
  TCut cut_phoEta=cut_barrel || cut_endcap;
  TCut cut_phoSCRChIsoCorr("phoSCRChIsoCorr<10 && phoSCRChIsoCorr>0");
  TCut cut_phoPFChIsoCorr("phoPFChIsoCorr<60");
  TCut cut_phoSigmaIEtaIEta("phoSigmaIEtaIEta>0.005 && phoSigmaIEtaIEta<0.015");
  TCut cut_weight("weight");

  trData->Draw("phoSigmaIEtaIEta:phoSCRChIsoCorr",cut_barrel && cut_phoSigmaIEtaIEta && cut_phoSCRChIsoCorr, "COLZ");

 // DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoSigmaIEtaIEta)*cut_weight,"phoSCRChIsoCorr","phoSigmaIEtaIEta");

//  DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoEt)*cut_weight,"phoSCRChIsoCorr","phoEt");

//  DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoEta)*cut_weight,"phoSCRChIsoCorr","phoEta");

//  DrawProfile(trData,trSign,(cut_phoPFChIsoCorr && cut_phoSigmaIEtaIEta)*cut_weight,"phoPFChIsoCorr","phoSigmaIEtaIEta");

//  DrawProfile(trData,trSign,(cut_phoPFChIsoCorr && cut_phoEt)*cut_weight,"phoPFChIsoCorr","phoEt");

//  DrawProfile(trData,trSign,(cut_phoPFChIsoCorr && cut_phoEta)*cut_weight,"phoPFChIsoCorr","phoEta");

//  DrawProfile(trData,trSign,(cut_phoSigmaIEtaIEta && cut_phoEta)*cut_weight,"phoSigmaIEtaIEta","phoEta");

//  DrawProfile(trData,trSign,(cut_phoSigmaIEtaIEta && cut_phoEtHigh)*cut_weight,"phoSigmaIEtaIEta","phoEt");
}
