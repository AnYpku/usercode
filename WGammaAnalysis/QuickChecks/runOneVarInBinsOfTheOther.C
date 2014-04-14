{
  gROOT->ProcessLine(".L OneVarInBinsOfTheOther.C+");
  TFile f("../WGammaOutput/MUON/VeryPreliminarySelected/selectedDATA.root");
  TTree* tr = (TTree*)f.Get("selectedEvents");
  
  TCut cutB="phoEta<1.442 && phoEta>-1.442";
  TCut cutE="(phoEta > 1.566 && phoEta < 2.5)||(phoEta <-1.566 && phoEta >-2.5)";
  TCut cutWeight="weight";

//phoSCRChIsoCorr in 7 pt bins, Endcap 
  TCut cutNoWeight="phoSCRChIsoCorr<4";
  cutNoWeight+=cutE;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoEt";
  int nBinsVar2 = 7;
  float binLimsVar2[nBinsVar2+1]={15,20,25,30,35,40,60,80};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 4.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

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

/*
//phoSCRChIsoCorr in Endcap in 2 phoSigmaIEtaIEta bins: nominal and sideband

  TCut cutNoWeight="phoSCRChIsoCorr<4";
  cutNoWeight+=cutE;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2 = 2;
  float binLimsVar2[nBinsVar2+1]={0.019,0.033,0.067};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 4.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/
/*
//phoSCRChIsoCorr in Barrel in 2 phoSigmaIEtaIEta bins: nominal and sideband

  TCut cutNoWeight="phoSCRChIsoCorr<4";
  cutNoWeight+=cutB;
  TString var1 = "phoSCRChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  int nBinsVar2 = 2;
  float binLimsVar2[nBinsVar2+1]={0.005,0.011,0.022};
  int nBinsHist=20;
  float histMin = -0.1;
  float histMax = 4.0;

  OneVarInBinsOfTheOther(tr, var1, cutNoWeight, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
*/

}
