#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TH1F.h"
#include "TF1.h"

TH1F* hTrue;
TH1F* hFake;

double FuncTempl(Double_t* x, Double_t* par)
{
  //x[0] - I_ch
  //par[0] - nTrue, par[1] - nFake
  int nBins=hTrue->GetNbinsX();
  float sumTrue=0;
  float sumFake=0;
  int ibThisEvent=-1;
  for (int ib=1; ib<=nBins; ib++){
    sumTrue+=hTrue->GetBinContent(ib);
    sumFake+=hFake->GetBinContent(ib);
    if (x[0]>=hTrue->GetBinLowEdge(ib) && x[0]<hTrue->GetBinLowEdge(ib)+hTrue->GetBinWidth(ib))
      ibThisEvent=ib;
  }// end of loop over ib
  if (ibThisEvent==-1){
    if (x[0]<hTrue->GetBinLowEdge(1)) ibThisEvent=1;
    else ibThisEvent=nBins;
  }
  float val = par[0]*hTrue->GetBinContent(ibThisEvent)/sumTrue+par[1]*hFake->GetBinContent(ibThisEvent)/sumFake;
  return val;
}// end of FuncTempl

void FitOneBin()
{
  //data
  TFile fData("../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  TFile fSign("../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile fTemp("../WGammaOutput/MERGED/selected_WGamma_ForTemplates.root");

  //MC mixture
/*
  TFile fData("../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA_MCclosure.root");
  TFile fSign("../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile fTemp("../WGammaOutput/MERGED/selected_WGamma_ForTemplates_MCclosure.root");
*/
  TTree* trData=(TTree*)fData.Get("selectedEvents");
  TTree* trSign=(TTree*)fSign.Get("selectedEvents");
  TTree* trTemp=(TTree*)fTemp.Get("selectedEvents");

/*
  // begin 15-20
  TCut cutPtTrue="phoEt>15 && phoEt<20";
  TCut cutPtFake="phoEt>15 && phoEt<20";
  TCut cutPtData="phoEt>15 && phoEt<20";
//  int nbins=21; float low=-0.9; float up=20.1; // data
  int nbins=17; float low=-0.9; float up=16.1; // MC mixture
  TCut cutSihihSideband="phoSigmaIEtaIEta>0.014 && phoSigmaIEtaIEta<0.018";
  // end 15-20
*/

/*
  // begin 20-25
  TCut cutPtTrue="phoEt>20 && phoEt<25";
  TCut cutPtFake="phoEt>20 && phoEt<25";
  TCut cutPtData="phoEt>20 && phoEt<25";
//  int nbins=21; float low=-0.9; float up=20.1; // data
  int nbins=9; float low=-0.9; float up=17.1; // MC mixture
  TCut cutSihihSideband="phoSigmaIEtaIEta>0.014 && phoSigmaIEtaIEta<0.020";
  // end 20-25
*/

  // begin 45-55
  TCut cutPtTrue="phoEt>25";
  TCut cutPtFake="phoEt>55";
  TCut cutPtData="phoEt>45 && phoEt<55";
  int nbins=6; float low=-0.9; float up=17.1; // data
//  int nbins=8; float low=-0.9; float up=15.1;
  TCut cutSihihSideband="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.014";
  // end 45-55

/*
  // begin 95-120
  TCut cutPtTrue="phoEt>30";
  TCut cutPtFake="phoEt>55";
  TCut cutPtData="phoEt>95 && phoEt<120";
  int nbins=5; float low=-0.9; float up=14.1; // data
  TCut cutSihihSideband="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.012";
  // end 95-120
*/
/*
  // begin 120-500
  TCut cutPtTrue="phoEt>30";
  TCut cutPtFake="phoEt>55";
  TCut cutPtData="phoEt>120 && phoEt<500";
  int nbins=4; float low=-0.9; float up=15.1; // data
  TCut cutSihihSideband="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.012";
  // end 120-500
*/
  TCut cutB="phoSCEta>-1.4442 && phoSCEta<1.4442";

  TCut cutSihihNominal="phoSigmaIEtaIEta<0.011";




  TCut cutWeight="weight";

  TCut cutTrue = (cutPtTrue && cutB && cutSihihNominal)*cutWeight;
  TCut cutFake = (cutPtFake && cutB && cutSihihSideband)*cutWeight;

  TCut cutData = (cutPtData && cutB && cutSihihNominal)*cutWeight;

  TFile* fOut = new TFile("fOut.root","recreate"); 


  TH1F* hData = new TH1F("hData","hData",nbins,low,up);
  hData->Sumw2();
  hTrue = new TH1F("hTrue","hTrue",nbins,low,up);
  hTrue->Sumw2();
  hFake = new TH1F("hFake","hFake",nbins,low,up);
  hFake->Sumw2();
  TH1F* hLeak = new TH1F("hLeak","hLeak",nbins,low,up);
  hLeak->Sumw2();

  float contInitial = hData->GetSumOfWeights();

  trData->Draw("phoSCRChIsoCorr>>hData",cutData,"goff");
  trTemp->Draw("phoSCRChIsoCorr>>hFake",cutFake,"goff");
  trTemp->Draw("phoRandConeChIsoCorr>>hTrue",cutTrue,"goff");
  trSign->Draw("phoSCRChIsoCorr>>hLeak",cutFake,"goff");

  TF1* funcTempl = new TF1("FuncTempl",FuncTempl,low,up,2);
  funcTempl->SetParameters(0.5*contInitial,0.5*contInitial);
  funcTempl->SetParNames("nTrue","nFake");
  
  hData->Fit("FuncTempl","L");

}//end of FitOneBin()
