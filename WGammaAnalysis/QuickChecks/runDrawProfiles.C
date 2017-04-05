{
 
  gROOT->ProcessLine(".L DrawProfiles.C+");
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(kWhite); 

  TFile fData("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  TFile fSign("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TTree* trData = (TTree*)fData.Get("selectedEvents");
  TTree* trSign = (TTree*)fSign.Get("selectedEvents");

  TCut cut_phoEt("phoEt>15 && phoEt<40");
  TCut cut_phoEtStrict("phoEt>15 && phoEt<40");
  TCut cut_phoEtHigh("phoEt>80");
  TCut cut_barrel("phoSCEta>-1.442 && phoSCEta<1.442");
  TCut cut_endcap("(phoSCEta>-2.5 && phoSCEta<-1.566) || (phoSCEta>1.566 && phoSCEta<2.5)");
  TCut cut_phoEta=cut_barrel || cut_endcap;
  TCut cut_phoSCRChIsoCorr("phoSCRChIsoCorr<14 && phoSCRChIsoCorr>=0");
  TCut cut_phoPFChIsoCorr("phoPFChIsoCorr<60");
  TCut cut_phoSigmaIEtaIEta("phoSigmaIEtaIEta>0.005 && phoSigmaIEtaIEta<0.015");
  TCut cut_weight("weight");

//  trData->Draw("phoSigmaIEtaIEta:phoSCRChIsoCorr",cut_phoEt && cut_barrel && cut_phoSigmaIEtaIEta && cut_phoSCRChIsoCorr, "COLZ");

  DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoSigmaIEtaIEta && cut_barrel && cut_phoEt)*cut_weight,"phoSCRChIsoCorr","phoSigmaIEtaIEta");

  cut_phoSigmaIEtaIEta("phoSigmaIEtaIEta>0.019 && phoSigmaIEtaIEta<0.067");

  DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoSigmaIEtaIEta && cut_endcap && cut_phoEt)*cut_weight,"phoSCRChIsoCorr","phoSigmaIEtaIEta");

//  DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoEt)*cut_weight,"phoSCRChIsoCorr","phoEt");

//  DrawProfile(trData,trSign,(cut_phoSCRChIsoCorr && cut_phoEta)*cut_weight,"phoSCRChIsoCorr","phoEta");

//  DrawProfile(trData,trSign,(cut_phoPFChIsoCorr && cut_phoSigmaIEtaIEta)*cut_weight,"phoPFChIsoCorr","phoSigmaIEtaIEta");

//  DrawProfile(trData,trSign,(cut_phoPFChIsoCorr && cut_phoEt)*cut_weight,"phoPFChIsoCorr","phoEt");

//  DrawProfile(trData,trSign,(cut_phoPFChIsoCorr && cut_phoEta)*cut_weight,"phoPFChIsoCorr","phoEta");

//  DrawProfile(trData,trSign,(cut_phoSigmaIEtaIEta && cut_phoEta)*cut_weight,"phoSigmaIEtaIEta","phoEta");

//  DrawProfile(trData,trSign,(cut_phoSigmaIEtaIEta && cut_phoEtHigh)*cut_weight,"phoSigmaIEtaIEta","phoEt");
}
