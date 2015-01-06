{
  gStyle->SetCanvasColor(kWhite); 
  gROOT->ProcessLine(".L OneVarInBinsOfTheOther.C+");
  TFile f_Zg("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root");
  TFile f_Zjets("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root");
  TFile f_Wg("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile f_Wjets("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root");

  TTree* tr_Zg = (TTree*)f_Zg.Get("selectedEvents");
  TTree* tr_Zjets = (TTree*)f_Zjets.Get("selectedEvents");
  TTree* tr_Wg = (TTree*)f_Wg.Get("selectedEvents");
  TTree* tr_Wjets = (TTree*)f_Wjets.Get("selectedEvents");
  
  TCut cutB="phoSCEta<1.4442 && phoSCEta>-1.4442";
  TCut cutE="(phoSCEta > 1.566 && phoSCEta < 2.5)||(phoSCEta <-1.566 && phoSCEta >-2.5)";
  TCut cutWeight="weight";
  TCut cutPtFirstBin="phoEt>15 && phoEt<20";

/*
  TString var1 = "phoSigmaIEtaIEta";
  TString var2 = "phoPFChIsoCorr";
  int nBinsVar2= 5;
  float binLimsVar2[nBinsVar2+1]={-0.1,1.5,2.6,5.0,15.0,100.0};

  int nBinsHist =24;
  float histMin = 0.019;
  float histMax = 0.067;


  OneVarInBinsOfTheOther("Zg_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  nBinsHist =17;
  histMin = 0.005;
  histMax = 0.022;

  OneVarInBinsOfTheOther("Zg_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/
/*
  ////////////////////////////////
  // begin
  // phoSigmaIEtaIEta in phoSCRChIso04Corr  bins
  TString var1 = "phoSigmaIEtaIEta";
  TString var2 = "phoSCRChIso04Corr";
  int nBinsVar2= 5;
  float binLimsVar2[nBinsVar2+1]={-0.1,1.5,2.6,5.0,15.0,100.0};

  int nBinsHist =24;
  float histMin = 0.019;
  float histMax = 0.067;


  OneVarInBinsOfTheOther("Zg_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  nBinsHist =17;
  histMin = 0.005;
  histMax = 0.022;

  OneVarInBinsOfTheOther("Zg_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  // phoSigmaIEtaIEta in phoSCRChIso04Corr  bins
  // end
  ////////////////////////////////
*/

/*
  TString var1 = "phoPFChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2B= 3;
  float binLimsVar2B[nBinsVar2B+1]={0.0051,0.0111,0.0141,0.0201};
  int nBinsVar2E= 3;
  float binLimsVar2E[nBinsVar2B+1]={0.0191,0.0331,0.0411,0.0611};
  int nBinsHist=21;
  float histMin = -0.9;
  float histMax = 20.1;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);

  OneVarInBinsOfTheOther("Zg_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
*/

/*
  ////////////////////////////////
  // begin
  // phoSCRChIso04Corr in phoSigmaIEtaIEta  bins
  TString var1 = "phoSCRChIso04Corr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2B= 3;
  float binLimsVar2B[nBinsVar2B+1]={0.0051,0.0111,0.0141,0.0201};
  int nBinsVar2E= 3;
  float binLimsVar2E[nBinsVar2B+1]={0.0191,0.0331,0.0411,0.0611};
  int nBinsHist=21;
  float histMin = -0.9;
  float histMax = 20.1;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);

  OneVarInBinsOfTheOther("Zg_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);

  // phoSCRChIso04Corr in phoSigmaIEtaIEta  bins
  // end
  ////////////////////////////////
*/


/*
  ////////////////////////////////
  // begin
  // phoSCRChIso04Corr and phoSigmaIEtaIEta in different phoEt bins

  TString var1 = "phoSCRChIso04Corr";
  TString var2 = "phoEt";
  int nBinsVar2 = 6;
  float binLimsVar2[nBinsVar2+1]={15,25,35,55,75,95,500};
  int nBinsHist=21;
  float histMin = -0.9;
  float histMax = 20.1;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  OneVarInBinsOfTheOther("Zg_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  var1 = "phoSigmaIEtaIEta";
  nBinsHist=24;
  histMin = 0.019;
  histMax = 0.067;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Zg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Zjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Wg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Wjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  nBinsHist=17;
  histMin = 0.005;
  histMax = 0.022;

  OneVarInBinsOfTheOther("Zg_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Zg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Zjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Wg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Wjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  // phoSCRChIso04Corr and phoSigmaIEtaIEta in different phoEt bins
  // end
  ////////////////////////////////
*/


/*
  // phoSCRChIso separately for barrel and endcap 
  var2 = "phoSCEta";
  int nBinsVar2_2 = 2;
  float binLimsVar2_2[nBinsVar2_2+1]={0,1.5,2.5};
  TCut cutPtFirstBin="phoEt>15 && phoEt<20";

  OneVarInBinsOfTheOther("Zg_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Zg, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Zjets, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Wg, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Wjets, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
*/

}
