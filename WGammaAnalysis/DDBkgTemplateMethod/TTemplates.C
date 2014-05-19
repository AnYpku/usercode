#include "TTemplates.h"
  //this class
#include "../Include/TPhotonCuts.h"
#include "../Include/TMathTools.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"

#include <string>
#include <vector>

#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooExtendPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooChi2Var.h"
#include "RooAbsReal.h"
#include "TCanvas.h"
#include "TString.h"
#include "TText.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TVectorD.h"

using namespace RooFit ;

TTemplates::TTemplates(int channel)
{
  _channel=channel;
  _INPUT = new TAllInputSamples(_channel,"../Configuration/config.txt");
  _fOutForSave = new TFile(_config.GetDDTemplateBkgFileName(_channel),"recreate");
  _varFit="phoSigmaIEtaIEta";
  _varSideband="phoSCRChIsoCorr";
  _labelVarFit="#sigma_{i#etai#eta}";
//  _varFit="phoSCRChIsoCorr";
//  _varSideband="phoSigmaIEtaIEta";
//  _labelVarFit="I_ch";

  _fSign = new TFile(_config.GetSelectedName(_config.PRELIMINARY_FOR_TEMPLATE_METHOD,_channel,_config.SIGMC));
  _fData = new TFile(_config.GetSelectedName(_config.PRELIMINARY_FOR_TEMPLATE_METHOD,_channel,_config.DATA));
  _treeSign = (TTree *)_fSign->Get("selectedEvents");
  _treeData = (TTree *)_fData->Get("selectedEvents");
  for (unsigned int i=0; i<_INPUT->allInputs_.size(); i++){
    int sample = _INPUT->allInputs_[i].sample_;
    if (sample==_config.DATA || sample==_config.SIGMC)
      continue;
    TString sourceName =  _INPUT->allInputs_[i].sourceName_;
    if (sourceName=="Wjets_to_lnu" || sourceName=="DYjets_to_ll" || sourceName=="ttbarjets") 
      continue;
    std::cout<<"bkg source Name_="<<_INPUT->allInputs_[i].sourceName_<<std::endl;
    _vecFBkg.push_back(new TFile(_config.GetSelectedName(_config.PRELIMINARY_FOR_TEMPLATE_METHOD,_channel,_config.BKGMC,sourceName)));
    _vecTreeBkg.push_back((TTree*)_vecFBkg.back()->Get("selectedEvents"));

  }

  TPhotonCuts emptyPhoton;
  if (_varFit=="phoSigmaIEtaIEta"){
    _minVarFitB = emptyPhoton.GetPhoSigmaIEtaIEtaCutLeftB();
    _maxVarFitB = 0.022;
    _minVarFitE = emptyPhoton.GetPhoSigmaIEtaIEtaCutLeftE();
    _maxVarFitE = 0.067;
  }
  if (_varFit=="phoPFChIsoCorr"||_varFit=="phoSCRChIsoCorr"){
    _minVarFitB = 0;
    _maxVarFitB = 4;
    _minVarFitE = 0;
    _maxVarFitE = 4;
  }
  _phoPtBinLimits = new float[_config.GetNPhoPtBins()];
  _config.GetPhoPtBinsLimits(_phoPtBinLimits);
}

TTemplates::~TTemplates()
{
}

void TTemplates::ComputeBackground(bool noPrint, bool noPlot)
{
  _nBinsLeftTot[_config.BARREL] = 4;//25;//20;//total Pt
  _nBinsLeftTot[_config.ENDCAP] = 4;//25;//8;//total Pt
  _nBinsLeft[0][_config.BARREL] = 4;//25;//2;//15 to 20
  _nBinsLeft[0][_config.ENDCAP] = 4;//20;//5;//15 to 20
  _nBinsLeft[1][_config.BARREL] = 4;// 4;//10;//20 to 25
  _nBinsLeft[1][_config.ENDCAP] = 4;//16;//20;//20 to 25
/*
  _nBinsLeft[2][_config.BARREL] = 4;//25;//20;//25 to 30
  _nBinsLeft[2][_config.ENDCAP] = 4;//20;//5;//25 to 30
  _nBinsLeft[3][_config.BARREL] = 4;//20;//20;//30 to 35
  _nBinsLeft[3][_config.ENDCAP] = 4;// 8;//10;//30 to 35
  _nBinsLeft[4][_config.BARREL] = 4;//25;//16;//35 to 40; 
  _nBinsLeft[4][_config.ENDCAP] = 4;//25;//20;//35 to 40;
  _nBinsLeft[5][_config.BARREL] = 4;// 8;//5;//40 to 60
  _nBinsLeft[5][_config.ENDCAP] = 4;//20;//10;//40 to 60; 
  _nBinsLeft[6][_config.BARREL] = 4;//20;//16;//60 to 80; 
  _nBinsLeft[6][_config.ENDCAP] = 4;//20; //4;//60 to 80
  _nBinsLeft[7][_config.BARREL] = 4;//25;//10;//80 to 200
  _nBinsLeft[7][_config.ENDCAP] = 4;//16;// 8;//80 to 200
  _nBinsLeft[8][_config.BARREL] = 4;// 4;// 2;//200 to 600
  _nBinsLeft[8][_config.ENDCAP] = 4;//10;// 2;//200 to 600
*/
//  SelectOptimalNBinsLeft();
  int nPtBins = _config.GetNPhoPtBins();
  for (int ipt=-1; ipt<nPtBins; ipt++){
  //for (int ipt=0; ipt<2; ipt++){
    ComputeBackgroundOne(ipt,noPrint,noPlot);
  }
  PrintBkgYieldsAndChi2();
  SaveBkgYields();
}

void TTemplates::ComputeBackgroundOne(int ipt, bool noPrint, bool noPlot)
{
  PrepareTemplatesOnePtBin(ipt, noPrint);
  FitOne(ipt,_config.BARREL,noPrint,noPlot);
  FitOne(ipt,_config.ENDCAP,noPrint,noPlot);
  ComputeBkgYieldOnePtBin(ipt,noPrint);
}

void TTemplates::SelectOptimalNBinsLeft()
{
//  const int NBinOpts=10;
//  const int arrNBinsLeft[NBinOpts]={2,4,5,8,10,16,20,25,32,40};
  const int NBinOpts=7;
  const int arrNBinsLeft[NBinOpts]={4,5,8,10,16,20,25};
//  const int NBinOpts=4;
//  const int arrNBinsLeft[NBinOpts]={2,4,5,8};
  double arrNBinsLeftVal[NBinOpts];
  double arrNBinsLeftErr[NBinOpts];
  for (int i=0; i<NBinOpts; i++){
    arrNBinsLeftVal[i]=(double)arrNBinsLeft[i];
    arrNBinsLeftErr[i]=0.001;
  }
  double arrChi2ToNdfB[NBinOpts];
  double arrChi2ToNdfE[NBinOpts];
  double arrBkgYields[NBinOpts];
  double arrBkgYieldsErr[NBinOpts];

  int nPtBins = _config.GetNPhoPtBins();
  for (int ipt=-1; ipt<nPtBins; ipt++){
  //for (int ipt=-1; ipt<2; ipt++){

    double chi2BClosestToOne = 1000;
    double chi2EClosestToOne = 1000;
    int nBinsBestB = 2;
    int nBinsBestE = 2;

    for (int iBinOpt=0; iBinOpt<NBinOpts; iBinOpt++){
     for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){
        if (ipt==-1) _nBinsLeftTot[ieta]=arrNBinsLeft[iBinOpt];     
        if (ipt>=0) _nBinsLeft[ipt][ieta]=arrNBinsLeft[iBinOpt]; 
     }//end of loop over ieta
     ComputeBackgroundOne(ipt,1,1);     
     if (ipt==-1){
       arrChi2ToNdfB[iBinOpt] = _chi2ToNDFTot[_config.BARREL]; 
       arrChi2ToNdfE[iBinOpt] = _chi2ToNDFTot[_config.ENDCAP];
       arrBkgYields[iBinOpt] = _nBkgrYieldsValTot;
       arrBkgYieldsErr[iBinOpt] = _nBkgrYieldsErrTot;
       if (_chi2ToNDFTot[_config.BARREL]>0)
         if (fabs(_chi2ToNDFTot[_config.BARREL]-1)<fabs(chi2BClosestToOne-1)){
           chi2BClosestToOne=_chi2ToNDFTot[_config.BARREL];
           nBinsBestB = arrNBinsLeft[iBinOpt];
         }
       if (_chi2ToNDFTot[_config.ENDCAP]>0)
         if (fabs(_chi2ToNDFTot[_config.ENDCAP]-1)<fabs(chi2EClosestToOne-1)){
           chi2EClosestToOne=_chi2ToNDFTot[_config.ENDCAP];
           nBinsBestE = arrNBinsLeft[iBinOpt];
         }
     }
     if (ipt>=0){
       arrChi2ToNdfB[iBinOpt] = _chi2ToNDF[ipt][_config.BARREL]; 
       arrChi2ToNdfE[iBinOpt] = _chi2ToNDF[ipt][_config.ENDCAP];
       arrBkgYields[iBinOpt] = _nBkgrYieldsVal[ipt];
       arrBkgYieldsErr[iBinOpt] = _nBkgrYieldsErr[ipt];
       if (_chi2ToNDF[ipt][_config.BARREL]>0)
         if (fabs(_chi2ToNDF[ipt][_config.BARREL]-1)<fabs(chi2BClosestToOne-1)){
           chi2BClosestToOne=_chi2ToNDF[ipt][_config.BARREL];
           nBinsBestB = arrNBinsLeft[iBinOpt];
         }
       if (_chi2ToNDF[ipt][_config.ENDCAP]>0)
         if (fabs(_chi2ToNDF[ipt][_config.ENDCAP]-1)<fabs(chi2EClosestToOne-1)){
           chi2EClosestToOne=_chi2ToNDF[ipt][_config.ENDCAP];
           nBinsBestE = arrNBinsLeft[iBinOpt];
         }
     }
     DeleteThreeHists(ipt,_config.BARREL);
     DeleteThreeHists(ipt,_config.ENDCAP);

   }//end of loop over iBinOpt

   //set number of left fit bins 
   if (ipt==-1){
     _nBinsLeftTot[_config.BARREL] = nBinsBestB;
     _nBinsLeftTot[_config.ENDCAP] = nBinsBestE;
   }
   if (ipt>=0){
     _nBinsLeft[ipt][_config.BARREL] = nBinsBestB;
     _nBinsLeft[ipt][_config.ENDCAP] = nBinsBestE;
   }

   //plot the chi2/NDF and bkg yield of nFitBins
   TString canvChi2NameB("c");
   canvChi2NameB+=StrLabelPt(ipt);
   canvChi2NameB+="chi2toNdf_B";
   TCanvas* cChi2B = new TCanvas(canvChi2NameB, canvChi2NameB); 
   TGraph* grChi2B = new TGraph(NBinOpts,arrNBinsLeftVal,arrChi2ToNdfB);
   grChi2B->SetMarkerColor(4);
   grChi2B->SetMarkerStyle(20);
   grChi2B->SetTitle(canvChi2NameB);
   grChi2B->Draw("AP");
   canvChi2NameB+=".png";
   cChi2B->SaveAs(canvChi2NameB);

   TString canvChi2NameE("c");
   canvChi2NameE+=StrLabelPt(ipt);
   canvChi2NameE+="chi2toNdf_E";
   TCanvas* cChi2E = new TCanvas(canvChi2NameE, canvChi2NameE); 
   TGraph* grChi2E = new TGraph(NBinOpts,arrNBinsLeftVal,arrChi2ToNdfE);
   grChi2E->SetMarkerColor(kOrange+4);
   grChi2E->SetMarkerStyle(20);
   grChi2E->SetTitle(canvChi2NameE);
   grChi2E->Draw("AP");
   canvChi2NameE+=".png";
   cChi2E->SaveAs(canvChi2NameE);

 }//end of loop over ipt



}

void TTemplates::PrepareTemplatesOnePtBin(int ipt, bool noPrint)
{

  _fOutForSave->cd();
//  int nBins = _config.GetNPhoPtBins();

  if (!noPrint){
    std::cout<<_varFit<<" B  range: "<<_minVarFitB<<" - "<<_maxVarFitB<<std::endl;
    std::cout<<_varFit<<" E  range: "<<_minVarFitE<<" - "<<_maxVarFitE<<std::endl;
  }

  TPhotonCuts emptyPhoton;
  TCut sidebandCut = SidebandCut(_varSideband);
  TCut barrelCut = emptyPhoton.RangeBarrel();
  TCut endcapCut = emptyPhoton.RangeEndcap();
  TCut weightCut = "weight";

  TString name;
  TCut cut;

  SetThreeHists(ipt, _config.BARREL, noPrint);
  SetThreeHists(ipt, _config.ENDCAP, noPrint);
  if (ipt==-1){
    _hBkgrTot[0]->Print();
    _hLeakTot[0]->Print();
    _hSignTot[0]->Print();
    _hSignTot[0]->Draw();
    _hLeakTot[0]->Draw("same");
    _hBkgrTot[0]->Draw("same");
    std::cout<<"nFitBins="<<_hBkgrTot[0]->GetNbinsX()<<std::endl;
  }

//  std::cout<<varSideband<<" range: "<<minVarSideband<<" - "<<maxVarSideband<<std::endl;
//  std::cout<<"phoEta range: "<<minPhoEta<<" - "<<maxPhoEta<<std::endl;
//  std::cout<<"phoEt range: "<<minPhoEt<<" - "<<maxPhoEt<<std::endl;
//  std::cout<<"weight range: "<<minWeight<<" - "<<maxWeight<<std::endl;

}

void TTemplates::SetThreeHists(int ptBin, int etaBin, bool noPrint){
  TString name=TString("h")+StrLabelPt(ptBin)+StrLabelEta(etaBin); 
  TCut cutWeight="weight";
  TCut cutSideband=SidebandCut(_varSideband);
  TCut cutEta = CutEtaBin(etaBin);
  TCut cutPt = CutPtBin(ptBin);
  TString cutStrPt="phoEt>";
  cutStrPt+=_config.GetPhoPtMin();
  cutPt=cutStrPt;
//  if (ptBin>0)
//    if (_phoPtBinLimits[ptBin]>80)
//      cutPt = "phoEt>80";

  TCut cut;

  int nFitBins=0;
  double min=0;
  double max=0;

//  int nEntData = 0.7*_treeData->GetEntries(cutEta && cutPt);
//  nFitBins = nEntBkg/750;
//  if (nFitBins<nBinsMin) nFitBins=nBinsMin;
//  if (nFitBins>nBinsMax) nFitBins=nBinsMax;
  if (etaBin==_config.BARREL)
  {min=_minVarFitB; max=_maxVarFitB;}
  else if (etaBin==_config.ENDCAP) 
  {min=_minVarFitE; max=_maxVarFitE;}
  else if (etaBin==_config.COMMON) 
  {min=_minVarFitB; max=_maxVarFitE;}
//  if (!noPrint) std::cout<<"min="<<min<<", max="<<max<<std::endl;

  //define nBins, min and max more accurately
  double cutNominalVarFit = ValueCutNominalVarFit(_varFit, etaBin);
  double unit=0;
  if (cutNominalVarFit>max || cutNominalVarFit<min){
    std::cout<<"varFit min="<<min<<"; varFit max = "<<max<<"; cut val = "<<cutNominalVarFit<<std::endl;
  }
  else{
    int nBLeft=0;
    if (ptBin>=0) {nBLeft=_nBinsLeft[ptBin][etaBin];}
    if (ptBin==-1) {nBLeft=_nBinsLeftTot[etaBin];}
    unit = (cutNominalVarFit-min)/nBLeft;
//    if (!noPrint) std::cout<<"min="<<min<<", cut="<<cutNominalVarFit<<", unit="<<unit<<", nBinsLeft="<<nBLeft<<std::endl;
    nFitBins = (max-min)/unit;
//    if (!noPrint) std::cout<<"min="<<min<<", max="<<max<<", unit="<<unit<<", nFitBins="<<nFitBins<<std::endl;
    max = min+unit*nFitBins;
//    if (!noPrint) std::cout<<"min="<<min<<", max="<<max<<", unit="<<unit<<", nFitBins="<<nFitBins<<std::endl;
  }

  if (_varFit=="phoSCRChIsoCorr"||_varFit=="phoPFChIsoCorr")
    nFitBins=nFitBins+1;

  double fitBinLims[nFitBins+1];

  if (_varFit=="phoSCRChIsoCorr"||_varFit=="phoPFChIsoCorr"){
    min = 0.000000001 - unit;
//    fitBinLims[0]=-0.000000001;
//    fitBinLims[1]=0.000000001;
//    for (int i=2; i<=nFitBins; i++){
//      fitBinLims[i]=min+(i-1)*unit;
//    }
    if (!noPrint){
      std::cout<<"fit bins:";
      for (int i=0; i<=nFitBins; i++)
        std::cout<<fitBinLims[i]<<" ";
    }
    std::cout<<std::endl;
    if (ptBin==-1) _nBinsLeftTot[etaBin]++;
    if (ptBin>=0) _nBinsLeft[ptBin][etaBin]++;
  }
//  else{
//    for (int i=0; i<=nFitBins; i++){
//      fitBinLims[i]=min+i*unit;
//    }



  if (!noPrint)
    std::cout<<"bin lims:";
  for (int i=0; i<=nFitBins; i++){
    fitBinLims[i]=min+i*unit;
    std::cout<<fitBinLims[i]<<", ";
  }
  std::cout<<std::endl;

  if (!noPrint){
    std::cout<<"SetThreeHists:"<<StrLabelPt(ptBin)<<StrLabelEta(etaBin)<<std::endl;
  }

  name+="_Sign";
  cut = (cutEta && cutPt)*cutWeight; 
  if (ptBin==-1){
    _hSignTot[etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    SetSignalTemplate(_hSignTot[etaBin],cut);
  }
  else{
    _hSign[ptBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    SetSignalTemplate(_hSign[ptBin][etaBin],cut);
  }

  name.ReplaceAll("_Sign","_Bkgr");
  cut = (cutEta && cutPt && cutSideband)*cutWeight;
  if (ptBin==-1){
    _hBkgrTot[etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    name.ReplaceAll("_Bkgr","_Leak");
    _hLeakTot[etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    TH1D* hLeakTemp = new TH1D("leakTempTot","leakTempTot",nFitBins,fitBinLims);
    SetBackgrTemplate(_hBkgrTot[etaBin], _hLeakTot[etaBin], hLeakTemp, cut, noPrint);
  }
  else{
    _hBkgr[ptBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    name.ReplaceAll("_Bkgr","_Leak");
    _hLeak[ptBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    TH1D* hLeakTemp = new TH1D("leakTemp","leakTemp",nFitBins,fitBinLims);
    SetBackgrTemplate(_hBkgr[ptBin][etaBin], _hLeak[ptBin][etaBin], hLeakTemp, cut, noPrint);
    delete hLeakTemp;
  }

}

void TTemplates::SetSignalTemplate(TH1D* hSign, TCut cut)
{
  TString name=hSign->GetName();
  _treeSign->Draw(_varFit+TString(">>")+name,cut,"goff");
}

void TTemplates::SetBackgrTemplate(TH1D* hBkgr, TH1D* hLeak, TH1D* hLeakTemp, TCut cut, bool noPrint)
{

  TString nameBkgr=hBkgr->GetName();
  _treeData->Draw(_varFit+TString(">>")+nameBkgr,cut,"goff");
  TString nameLeak=hLeak->GetName();
  _treeSign->Draw(_varFit+TString(">>")+nameLeak,cut,"goff");

  if (!noPrint){
      std::cout<<"(fake bkrg)+leak="<<hBkgr->GetSumOfWeights()<<", (sign leak)="<<hLeak->GetSumOfWeights()<<std::endl;
  }

  if (!noPrint) std::cout<<"_vecTreeBkg.size()="<<_vecTreeBkg.size()<<std::endl;
  if (!noPrint) std::cout<<"(true gamma bkg leaks):";

  TString nameLeakTemp=hLeakTemp->GetName();

  TCut cutTrueGamma = "(phoGenPID==22 && (phoGenMomPID>=-24 && phoGenMomPID<=24))*weight";

  for (unsigned int is=0; is<_vecTreeBkg.size(); is++){
    
    _vecTreeBkg[is]->Draw(_varFit+TString(">>")+nameLeakTemp,cut,"goff");
    hLeak->Add(hLeakTemp);
    if (!noPrint) std::cout<<" "<<hLeakTemp->GetSumOfWeights();
  }
  if (!noPrint) std::cout<<std::endl;
  hBkgr->Add(hLeak,-1);
  if (!noPrint) {
    std::cout<<"leak = (sign leak)+(true gamma bkg leaks) = ";
    std::cout<<hLeak->GetSumOfWeights()<<std::endl;
    std::cout<<"fake bkgr = ";
    std::cout<<hBkgr->GetSumOfWeights()<<std::endl;
  }

}

void TTemplates::DeleteThreeHists(int ptBin, int etaBin){

  if (ptBin==-1)
    delete _hSignTot[etaBin];
  else
    delete _hSign[ptBin][etaBin];

  if (ptBin==-1)
    delete _hBkgrTot[etaBin];
  else
    delete _hBkgr[ptBin][etaBin]; 

  if (ptBin==-1)
    delete _hLeakTot[etaBin];
  else
    delete _hLeak[ptBin][etaBin];
}

TString TTemplates::StrLabelEta(int etaBin){
  if (etaBin==_config.BARREL) return "_Barrel_";
  else if (etaBin==_config.ENDCAP) return "_Endcap_";
  else if (etaBin==_config.COMMON) return "_EtaCommon_";
  return "_EtaUnknown_";
}

TString TTemplates::StrLabelPt(int ptBin){
  if (ptBin==-1) return "_PtTotal_";
  if (ptBin>=(int)_config.GetNPhoPtBins()) return "_PtUnknown_";
  TString str="_Pt";
  str+=_phoPtBinLimits[ptBin];
  str+="to";
  str+=_phoPtBinLimits[ptBin+1];
  str+="_";
  return str;
}

TCut TTemplates::SidebandCut(TString varSideband){
  TPhotonCuts emptyPhoton;
  TCut cut="1";
  if (varSideband=="phoSigmaIEtaIEta") 
    cut=emptyPhoton.SidebandSigmaIEtaIEta();
  else if (varSideband=="phoSCRChIsoCorr"||varSideband=="phoPFChIsoCorr")
    cut=emptyPhoton.SidebandPhoChIso(varSideband);
  return cut;
}

void TTemplates::FitOne(int ptBin, int etaBin, bool noPrint, bool noPlot)
{


  // RooFit variables
  double varMin=0;
  double varMax=0;
  int nBins=0;
  TH1D* hSig = 0;
  TH1D* hBkg = 0;

  if (ptBin==-1){
    hSig = _hSignTot[etaBin];
    hBkg = _hBkgrTot[etaBin];
  }
  else{
    hSig = _hSign[ptBin][etaBin];
    hBkg = _hBkgr[ptBin][etaBin];
  }

  nBins = hBkg->GetNbinsX();
  varMin = hBkg->GetBinLowEdge(1);
  varMax = hBkg->GetBinLowEdge(nBins)+hBkg->GetBinWidth(nBins);

  double minVarSideband = 0;
  double maxVarSideband = 100000;
  double minPhoEta = -2.6;
  double maxPhoEta = 2.6;
  double minPhoEt = 0;
  double maxPhoEt = 1000;
  double minWeight = 0;
  double maxWeight = 1;

  _treeSign = (TTree *)_fSign->Get("selectedEvents");
  _treeData = (TTree *)_fData->Get("selectedEvents");
  maxVarSideband = _treeData->GetMaximum(_varSideband);
  minVarSideband = _treeData->GetMinimum(_varSideband);
  maxPhoEta = _treeData->GetMaximum("phoEta");
  minPhoEta = _treeData->GetMinimum("phoEta");
  maxPhoEt = _treeData->GetMaximum("phoEt");
  minPhoEt = _treeData->GetMinimum("phoEt");
  maxWeight = _treeSign->GetMaximum("weight");
  minWeight = _treeSign->GetMinimum("weight");
  if (!noPrint){
    std::cout<<_varSideband<<" range: "<<minVarSideband<<" - "<<maxVarSideband<<std::endl;
    std::cout<<"phoEta range: "<<minPhoEta<<" - "<<maxPhoEta<<std::endl;
    std::cout<<"phoEt range: "<<minPhoEt<<" - "<<maxPhoEt<<std::endl;
    std::cout<<"weight range: "<<minWeight<<" - "<<maxWeight<<std::endl;
  }

  RooRealVar rooVarFit(_varFit,_labelVarFit, varMin, varMax);
  RooRealVar rooVarSideband(_varSideband,_varSideband,minVarSideband,maxVarSideband);
  RooRealVar rooPhoEta("phoEta","phoEta",minPhoEta,maxPhoEta);
  RooRealVar rooPhoEt("phoEt","phoEt",minPhoEt,maxPhoEt);
  RooRealVar rooWeight("weight","weight",minWeight,maxWeight);

  RooArgList argList;
  argList.add(rooVarFit);
  RooArgSet argSet;
  argSet.add(rooVarFit);
  //create bkg and sig PDFs
  RooDataHist signalDataHist("signalDataHist","signal RooDataHist", argList, hSig);
  RooHistPdf signalPdf("signalPdf",_varFit+TString(" of signal"), argSet, signalDataHist);
  RooDataHist backgroundDataHist("backgroundDataHist", "background RooDataHist", argList, hBkg);
  RooHistPdf backgroundPdf("backgroundPdf",_varFit+TString(" of background"), argSet, backgroundDataHist);

  //load data
  RooArgSet argSetData(rooVarFit,rooVarSideband,rooPhoEta,rooPhoEt,rooWeight);

  _fData = new TFile(_config.GetSelectedName(_config.PRELIMINARY_FOR_TEMPLATE_METHOD,_channel,_config.DATA));

  TTree* treeData = (TTree *)_fData->Get("selectedEvents");
  TCut sidebandCut = SidebandCut(_varSideband);
  TCut etaCut = CutEtaBin(etaBin);
  TCut ptCut = CutPtBin(ptBin);
  TCut weightCut = "weight";
  TCut cut = ptCut && etaCut;
	
  TH1D* hist = (TH1D*)hBkg->Clone("hCloned");
  TString varDraw=_varFit;
  varDraw+=">>hCloned";
  treeData->Draw(varDraw,cut,"goff");
  RooDataHist dataHist("hist", "data set converted to hist", argList, hist);
//  RooDataSet dataSet("dataSet","dataSet",treeData,argSetData,cut);

  //create total pdf
  int nMax = treeData->GetEntries(cut);
  if (nMax<=0){
    if (!noPrint) std::cout<<"ATTENTION: No data events for bin "<<ptCut<<", "<<etaCut<<std::endl;
    return;
  }
  int nSigStart = nMax/4;
  int nBkgStart = 3*nMax/4;
  RooRealVar rooNSig("nsig","nsignal",nSigStart,0,nMax);
  RooExtendPdf esigPdf("esig","extended signal",signalPdf,rooNSig);
  RooRealVar rooNBkg("nbkg","n bkg  ",nBkgStart,0,nMax);
  RooExtendPdf ebkgPdf("ebkg","extended bkg",backgroundPdf,rooNBkg);
  RooAddPdf fullPdf("fitModel","fit model",RooArgList(esigPdf,ebkgPdf));
  //fit
  fullPdf.fitTo(dataHist);


  //calc chi squared
  RooChi2Var chi2("chi2","chi2",fullPdf,dataHist);
  if (ptBin==-1) _chi2ToNDFTot[etaBin] = chi2.getVal()/(nBins-2);
  else _chi2ToNDF[ptBin][etaBin] = chi2.getVal()/(nBins-2);

  //get nbkg values from fit
  if (ptBin==-1) {
    _nBkgrValTot[etaBin]=rooNBkg.getVal();
    _nBkgrErrTot[etaBin]=rooNBkg.getError();
  }
  else{
    _nBkgrVal[ptBin][etaBin]=rooNBkg.getVal();
    _nBkgrErr[ptBin][etaBin]=rooNBkg.getError();
  }

  //plot results
  if (!noPlot){
    TString cName="c";
    cName+=StrLabelPt(ptBin);
    cName+=StrLabelEta(etaBin);
    TCanvas* c1 = new TCanvas(cName,cName, 800, 600);
    RooPlot* plotter = new RooPlot(rooVarFit,varMin,varMax,nBins); 
    dataHist.plotOn(plotter);
    fullPdf.plotOn(plotter,Name("sum"),LineColor(kRed));
    fullPdf.plotOn(plotter,Components("signalPdf"), Name("signal"),
    LineColor(kGreen));
    fullPdf.plotOn(plotter, Components("backgroundPdf"),Name("background"),LineColor(kBlue));
    fullPdf.paramOn(plotter);

    TString plotTitle = "chi2/NDF=";
    plotTitle+=chi2.getVal()/(nBins-2);
    plotter->SetTitle(plotTitle);

    plotter->Draw();

    double valCut = ValueCutNominalVarFit(_varFit, etaBin);
    TLine* cutLine = new TLine(valCut,0,valCut,hist->GetMaximum());
    cutLine->SetLineWidth(3);
    cutLine->Draw("same");

    cName+=".png";
    cName.ReplaceAll("-1","total");
    c1->SaveAs(cName);
  }

//  TString plotName(_config.GetTemplatePicNameBase(ptBin,etaBin));
//  plotName+=ptBin;
//  plotName+="_";
//  plotName+=etaBin;
//  plotName+=".png";
//  fOutForSave_->cd();
//  c1->Write(_config.GetTemplatePicNameBase(ptBin,etaBin));
//  delete plotter;

}

void TTemplates::ComputeBkgYieldOnePtBin(int ipt, bool noPrint)
{
  double nBkg[2];
  double nBkgErr[2];
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    if (ipt==-1){
      ComputeBkgYieldOne(_hBkgrTot[ieta], _nBkgrValTot[ieta], _nBkgrErrTot[ieta], nBkg[ieta], nBkgErr[ieta], ieta, -1, noPrint);
    _nBkgrYieldsValTot=nBkg[0]+nBkg[1];
    _nBkgrYieldsErrTot=sqrt(nBkgErr[0]*nBkgErr[0]+nBkgErr[1]*nBkgErr[1]);
    }
    if (ipt>=0){
      ComputeBkgYieldOne(_hBkgr[ipt][ieta], _nBkgrVal[ipt][ieta], _nBkgrErr[ipt][ieta], nBkg[ieta], nBkgErr[ieta], ieta, -1, noPrint);
    _nBkgrYieldsVal[ipt]=nBkg[0]+nBkg[1];
    _nBkgrYieldsErr[ipt]=sqrt(nBkgErr[0]*nBkgErr[0]+nBkgErr[1]*nBkgErr[1]);
    }
  }//end of ieta loop
}

void TTemplates::ComputeBkgYieldOne(TH1D* hBkgr, double nBkgrVal, double nBkgrErr, double& nBkgrYieldVal, double& nBkgrYieldErr, int ieta, int ipt, bool noPrint)
{
  int nHistBins = hBkgr->GetNbinsX();
  nBkgrYieldVal=0;
  double Scut = 0;
  double Stot = 0;
  int nCut = 0;

  double frac;
  double fracErr;
  FractionOfSidebandVarInRange(ipt,ieta,frac,fracErr);
  
  int nBLeft=0;
  if (ipt==-1) nBLeft=_nBinsLeftTot[ieta];
  if (ipt>=0)  nBLeft=_nBinsLeft[ipt][ieta];
  for (int i=1; i<=nBLeft; i++){
    Scut+=hBkgr->GetBinContent(i);
    Stot+=hBkgr->GetBinContent(i);
  }
  for (int i=nBLeft+1; i<=nHistBins; i++){
    Stot+=hBkgr->GetBinContent(i);
  }
  nBkgrYieldVal=frac*nBkgrVal*Scut/Stot;
  //compute error
  double firstTerm = frac*frac*nBkgrErr*nBkgrErr*Scut*Scut/(Stot*Stot);
  double secondTerm = 0;
  for (int j=1; j<=nCut; j++){
    secondTerm += frac*frac*hBkgr->GetBinError(j)*hBkgr->GetBinError(j)* nBkgrVal*nBkgrVal*(Stot-Scut)*(Stot-Scut)/(Stot*Stot*Stot*Stot);
  }
  double thirdTerm = 0;
  for (int j=nCut+1; j<=nHistBins; j++){
    thirdTerm += frac*frac*hBkgr->GetBinError(j)*hBkgr->GetBinError(j)* nBkgrVal*nBkgrVal*Scut*Scut/(Stot*Stot*Stot*Stot);
  }  
  double forthTerm = fracErr*fracErr*nBkgrVal*nBkgrVal*Scut*Scut/(Stot*Stot);
  nBkgrYieldErr = sqrt(firstTerm+secondTerm+thirdTerm+forthTerm);
  if (!noPrint){
    std::cout<<"Error computation, "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<"1st term = "<<sqrt(firstTerm)<<", 2nd term = "<<sqrt(secondTerm)<<", 3rd term = "<<sqrt(thirdTerm)<<", 4th term = "<<sqrt(forthTerm)<<std::endl;
  }
}

void TTemplates::PrintBkgYieldsAndChi2()
{
  std::cout<<"Chi2/NDF for different bins"<<std::endl;

  for (int ipt=-1; ipt<_config.GetNPhoPtBins(); ipt++){
    std::cout<<StrLabelPt(ipt)<<": ";
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": ";
      if (ipt==-1) 
        std::cout<<_chi2ToNDFTot[ieta]<<",  nLeft="<<_nBinsLeftTot[ieta]<<"; ";
      else 
        std::cout<<_chi2ToNDF[ipt][ieta]<<",  nLeft="<<_nBinsLeft[ipt][ieta]<<"; ";
    }
    std::cout<<" yield = ";
    if (ipt==-1) std::cout<<_nBkgrYieldsValTot<<"+-"<<_nBkgrYieldsErrTot;
    if (ipt>=0)  std::cout<<_nBkgrYieldsVal[ipt]<<"+-"<<_nBkgrYieldsErr[ipt]<<";  ";
    std::cout<<std::endl;
  }
}

void TTemplates::SaveBkgYields()
{
  _fOutForSave->cd();
  int nPtBins = _config.GetNPhoPtBins();

  TString strTot=_config.GetYieldsDDTemplateBkgName(_config.TOTAL);
  TH1F hTotBkgYield(strTot,strTot,1,_config.GetPhoPtMin(),_config.GetPhoPtMax());
  TString str1D=_config.GetYieldsDDTemplateBkgName(_config.ONEDI);
  TH1F h1DBkgYield(str1D,str1D,nPtBins,_phoPtBinLimits);

  hTotBkgYield.SetBinContent(1,_nBkgrYieldsValTot);
  hTotBkgYield.SetBinError(1,_nBkgrYieldsErrTot);
  for (int i=1; i<nPtBins+1; i++){  
    h1DBkgYield.SetBinContent(i,_nBkgrYieldsVal[i-1]);
    h1DBkgYield.SetBinError(i,_nBkgrYieldsErr[i-1]);
  }

  hTotBkgYield.Write(strTot); 
  h1DBkgYield.Write(_config.GetYieldsDDTemplateBkgName(_config.ONEDI)); 
}

TCut TTemplates::CutPtBin(int ptBin){

  if (ptBin==-1){
    TCut cut("1");
    return cut;
  }
  if (ptBin<0 || ptBin>=(int)_config.GetNPhoPtBins()){
    TCut cut("0");
    return cut;
  }
  TString ptBinCutStr;
  ptBinCutStr="phoEt>";
  ptBinCutStr+=_phoPtBinLimits[ptBin];
  ptBinCutStr+=" && phoEt<=";
  ptBinCutStr+=_phoPtBinLimits[ptBin+1];
  TCut cut(ptBinCutStr);
  return cut;
}

TCut TTemplates::CutEtaBin(int etaBin){
  TPhotonCuts emptyPhoton;
  TCut cut;
  if (etaBin==_config.BARREL) 
    cut=emptyPhoton.RangeBarrel();
  else if (etaBin==_config.ENDCAP) 
    cut=emptyPhoton.RangeEndcap();
  else if (etaBin==_config.COMMON) 
    cut=(emptyPhoton.RangeEndcap()&&emptyPhoton.RangeBarrel());
  else
    cut="0";
  return cut;
}

double TTemplates::ValueCutNominalVarFit(TString varFit, int etaBin){
  TPhotonCuts emptyPhoton;
  if (varFit=="phoSigmaIEtaIEta"){
    if (etaBin==_config.BARREL) return phoSigmaIEtaIEtaBarrelCut_[emptyPhoton.GetWP()];
    else if (etaBin==_config.ENDCAP) return phoSigmaIEtaIEtaEndcapCut_[emptyPhoton.GetWP()];
    else return 1;
  }
  if (varFit=="phoPFChIsoCorr"||varFit=="phoSCRChIsoCorr"){
    if (etaBin==_config.BARREL) return phoChIsoBarrelCut_[emptyPhoton.GetWP()];
    else if (etaBin==_config.ENDCAP) return phoChIsoEndcapCut_[emptyPhoton.GetWP()];
    else return 1;
  }
  return 100000;
}

void TTemplates::FractionOfSidebandVarInRange(int ipt, int ieta, double& frac, double& fracErr){
  TCut cutEta = CutEtaBin(ieta);
  TCut cutPt = CutPtBin(ipt);
  TCut rangeCut = "1";
  TCut cutWeight = "weight";
  TPhotonCuts emptyPhoton;

  if (_varSideband=="phoSCRChIsoCorr"||_varSideband=="phoPFChIsoCorr") 
    rangeCut=emptyPhoton.RangePhoChIso(_varSideband,emptyPhoton.GetWP());
  if (_varSideband=="phoSigmaIEtaIEta")
    rangeCut=emptyPhoton.RangeSigmaIEtaIEta(emptyPhoton.GetWP());

  double min=_treeData->GetMinimum(_varFit)-0.1;
  double max=_treeData->GetMaximum(_varFit)+0.1;
  int nFitBins=1;
  double fitBinLims[nFitBins+1];
  fitBinLims[0]=min;
  fitBinLims[1]=max;

  double Npass=0;
  TString namePass="hBkgrPass";
//  namePass+=ieta;
//  namePass+=ipt;
  TCut cutPass=(cutEta && cutPt && rangeCut)*cutWeight;
  TH1D* hPassBkgr=new TH1D(namePass,namePass,nFitBins,fitBinLims);
  TH1D* hPassLeak=new TH1D(namePass+TString("L"),namePass,nFitBins,fitBinLims);
  TH1D* hPassLeakTemp=new TH1D(namePass+TString("LT"),namePass,nFitBins,fitBinLims);
  SetBackgrTemplate(hPassBkgr,hPassLeak,hPassLeakTemp,cutPass,1);
  Npass=hPassBkgr->GetSumOfWeights();
  delete hPassBkgr;
  delete hPassLeak;
  delete hPassLeakTemp;

  double Nfail=0;
  TString nameFail="hBkgrFail";
//  nameFail+=ieta;
//  nameFail+=ipt;
  TCut cutFail=(cutEta && cutPt && !rangeCut)*cutWeight;
  TH1D* hFailBkgr=new TH1D(nameFail,nameFail,nFitBins,fitBinLims);
  TH1D* hFailLeak=new TH1D(nameFail+TString("L"),nameFail,nFitBins,fitBinLims);
  TH1D* hFailLeakTemp=new TH1D(nameFail+TString("LT"),nameFail,nFitBins,fitBinLims);
  SetBackgrTemplate(hFailBkgr,hFailLeak,hFailLeakTemp,cutFail,1);
  Nfail=hFailBkgr->GetSumOfWeights();
  delete hFailBkgr;
  delete hFailLeak;
  delete hFailLeakTemp;


  if ((Nfail+Npass)==0){ frac=0; fracErr=0;}
  else {
    frac = Npass/(Nfail+Npass);
    TMathTools math;
    fracErr = math.ErrOfTwoIndependent("x1/(x1+x2)",Npass,Nfail,sqrt(Npass),sqrt(Nfail));
  }

}

