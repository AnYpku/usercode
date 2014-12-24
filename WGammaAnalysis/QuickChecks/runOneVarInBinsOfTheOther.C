{
  gStyle->SetCanvasColor(kWhite); 
  gROOT->ProcessLine(".L OneVarInBinsOfTheOther.C+");
//  TFile f("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile f("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root");
//  TFile f("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root");
//  TFile f("../WGammaOutput/MUON/FsrSelected/selected_ZGamma_UNblind_DATA.root");

  TTree* tr = (TTree*)f.Get("selectedEvents");
  
  TCut cutB="phoSCEta<1.442 && phoSCEta>-1.442";
  TCut cutE="(phoSCEta > 1.566 && phoSCEta < 2.5)||(phoSCEta <-1.566 && phoSCEta >-2.5)";
  TCut cutWeight="weight";

//phoSCRChIsoCorr in 7 pt bins, Endcap 
  TCut cutNoWeight=cutE;
  TString var1 = "phoSCRChIso04Corr";
  TString var2 = "phoEt";
  int nBinsVar2 = 5;
  float binLimsVar2[nBinsVar2+1]={15,20,25,30,35,40};
  int nBinsHist=21;
  float histMin = -0.9;
  float histMax = 20.1;

  OneVarInBinsOfTheOther("DYjets_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

/*
//phoSCRChIsoCorr in 9 pt bins, Barrel 

  TCut cutNoWeight="phoSCRChIsoCorr<4";
  cutNoWeight+=cutB;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoEt";
  int nBinsVar2 = 9;
  float binLimsVar2[nBinsVar2+1]={15,20,25,30,35,40,60,80,200,600};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 4.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/
/*
//phoSCRChIsoCorr in 7 pt bins, Barrel 

  TCut cutNoWeight="phoSCRChIsoCorr<4";
  cutNoWeight+=cutB;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoEt";
  int nBinsVar2 = 7;
  float binLimsVar2[nBinsVar2+1]={15,20,25,30,35,40,60,80};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 4.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/

/*
//phoSCRChIsoCorr in 2 eta bins: "Endcap" and "Barrel" 

  TCut cutNoWeight="phoSCRChIsoCorr<4";
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoEta";
  int nBinsVar2 = 2;
  float binLimsVar2[nBinsVar2+1]={-2.5,-1.45,0};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 4.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/


//phoSCRChIsoCorr in Endcap in 2 phoSigmaIEtaIEta bins: nominal and sideband
/*
  TCut cutNoWeight="phoSCRChIsoCorr<20";
  cutNoWeight+=cutE;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2 = 2;
  float binLimsVar2[nBinsVar2+1]={0.019,0.033,0.067};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 20.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/

//phoSCRChIsoCorr in Barrel in 2 phoSigmaIEtaIEta bins: nominal and sideband
/*
  TCut cutNoWeight="phoSCRChIsoCorr<20";
  cutNoWeight+=cutB;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2 = 2;
  float binLimsVar2[nBinsVar2+1]={0.005,0.011,0.022};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 20.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/

/*
  TCut cutNoWeight="phoEt>15 && phoEt<20";
  TString var1 = "phoSigmaIEtaIEta";
  TString var2 = "phoPFChIsoCorr";
  int nBinsVar2 = 4;
  float binLimsVar2[nBinsVar2+1]={0,1.5,2.6,15,100};
  int nBinsHist=16;
  float histMin = 0.005;
  float histMax = 0.021;

//  OneVarInBinsOfTheOther("Wjets_MC_MUON_Sihih_Barrel", "pho #sigmai#etai#eta",tr, var1, cutNoWeight && cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
OneVarInBinsOfTheOther("DYjets_MC_MUON_Sihih_Barrel", "pho #sigmai#etai#eta",tr, var1, cutNoWeight && cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  int nBinsVar2 = 4;
  float binLimsVar2[nBinsVar2+1]={0,1.2,2.3,15,100};
  int nBinsHist=12;
  float histMin = 0.019;
  float histMax = 0.067;

  //OneVarInBinsOfTheOther("Wjets_MC_MUON_Sihih_Endcap", "pho #sigmai#etai#eta", tr, var1, cutNoWeight && cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
 // OneVarInBinsOfTheOther("DYjets_MC_MUON_Sihih_Endcap", "pho #sigmai#etai#eta", tr, var1, cutNoWeight && cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/
/*
  TCut cutNoWeight="phoEt>15 && phoEt<20";
  TString var1 = "phoPFChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2 = 3;
  float binLimsVar2[nBinsVar2+1]={0.005,0.011,0.014,0.021};
  int nBinsHist=20;
  float histMin = 0-0.1;
  float histMax = 20-0.1;

  OneVarInBinsOfTheOther("Wjets_MC_MUON_PFChIso_Barrel", "pho PF I_ch",tr, var1, cutNoWeight && cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  int nBinsVar2 = 3;
  float binLimsVar2[nBinsVar2+1]={0.019,0.034,0.050,0.067};

  OneVarInBinsOfTheOther("Wjets_MC_MUON_PFChIso_Endcap", "pho PF I_ch",tr, var1, cutNoWeight && cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/
}
