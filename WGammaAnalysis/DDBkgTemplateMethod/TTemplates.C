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

TTemplates::TTemplates(int channel, int blind, int phoWP, TString varFit, TString varSideband, TString varKin, int nKinBins, float* kinBinLims, int* nBinsLeftB, float* maxVarFitB, int* nBinsLeftE, float* maxVarFitE, int nBinsLeftBTot, float maxVarFitBTot, int nBinsLeftETot, float maxVarFitETot, bool isMetCutOptimization)
{
  _channel=channel;
  _blind=blind;
  //if (blind == _config.BLIND_PRESCALE) _blind=_config.UNBLIND;
  _blind=_config.UNBLIND;
  _phoWP=phoWP;
  _INPUT = new TAllInputSamples(_channel,"../Configuration/config.txt");
  _fOutForSave = new TFile(_config.GetDDTemplateBkgFileName(_channel),"recreate");
  _varFit=varFit;
  _varSideband=varSideband;
  if (_varFit=="phoSigmaIEtaIEta") _labelVarFit="#sigma_{i#etai#eta}";
  if (_varFit=="phoSCRChIsoCorr" || _varFit=="phoPFChIsoCorr") _labelVarFit="I_{ch}";

  _varKin=varKin;
  _nKinBins=nKinBins;
  _kinBinLims = new float[nKinBins+1];
  for (int i=0; i<=nKinBins; i++){
    _kinBinLims[i]=kinBinLims[i];
  }
  _isMetCutOptimization=isMetCutOptimization;
  if (_isMetCutOptimization) _selectionStage=_config.PRELIMINARY_FOR_MET_CUT;
  else _selectionStage=_config.PRELIMINARY_FOR_TEMPLATE_METHOD;

  _fSign = new TFile(_config.GetSelectedName(_selectionStage,_channel,_blind,_config.SIGMC));
  _fData = new TFile(_config.GetSelectedName(_selectionStage,_channel,_blind,_config.DATA));
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
    _vecFBkg.push_back(new TFile(_config.GetSelectedName(_selectionStage,_channel,_blind,_config.BKGMC,sourceName)));
    _vecTreeBkg.push_back((TTree*)_vecFBkg.back()->Get("selectedEvents"));
    _vecBkgNames.push_back(sourceName);
    _vecBkgColors.push_back(_INPUT->allInputs_[i].color_);

  }

  TPhotonCuts emptyPhoton;
  if (_varFit=="phoSigmaIEtaIEta"){
    _minVarFit[_config.BARREL] = emptyPhoton.GetPhoSigmaIEtaIEtaCutLeftB();
    _minVarFit[_config.ENDCAP] = emptyPhoton.GetPhoSigmaIEtaIEtaCutLeftE();
  }
  if (_varFit=="phoPFChIsoCorr"||_varFit=="phoSCRChIsoCorr"){
    _minVarFit[_config.BARREL] = 0;
    _minVarFit[_config.ENDCAP] = 0;
  }

  _nBinsLeftTot[_config.BARREL] = nBinsLeftBTot;
  _nBinsLeftTot[_config.ENDCAP] = nBinsLeftETot;
  _maxVarFitTot[_config.BARREL] = maxVarFitBTot;
  _maxVarFitTot[_config.ENDCAP] = maxVarFitETot;
  for (int i=0; i<_nKinBins; i++){
    _nBinsLeft[i][_config.BARREL] = nBinsLeftB[i];
    _nBinsLeft[i][_config.ENDCAP] = nBinsLeftE[i];
    _maxVarFit[i][_config.BARREL] = maxVarFitB[i];
    _maxVarFit[i][_config.ENDCAP] = maxVarFitE[i];
  }

}

TTemplates::~TTemplates()
{
}

void TTemplates::ComputeBackground(bool noPrint, bool noPlot)
{

  for (int ikin=-1; ikin<_nKinBins; ikin++){
    ComputeBackgroundOne(ikin,noPrint,noPlot);
  }
  if (!noPlot) PlotTemplates();
  PrintBkgYieldsAndChi2();
  SaveBkgYields();
}

void TTemplates::ComputeBackgroundOne(int ikin, bool noPrint, bool noPlot)
{
  PrepareTemplatesOneKinBin(ikin, noPrint);
  FitOne(ikin,_config.BARREL,noPrint,noPlot);
  FitOne(ikin,_config.ENDCAP,noPrint,noPlot);
  ComputeBkgYieldOneKinBin(ikin,noPrint);
}

void TTemplates::PrepareTemplatesOneKinBin(int ikin, bool noPrint)
{

  _fOutForSave->cd();

  if (!noPrint){
    if (ikin==-1){
      std::cout<<_varFit<<" B  range: "<<_minVarFit[_config.BARREL]<<" - "<<_maxVarFitTot[_config.BARREL]<<std::endl;
      std::cout<<_varFit<<" E  range: "<<_minVarFit[_config.ENDCAP]<<" - "<<_maxVarFitTot[_config.ENDCAP]<<std::endl;
    }
    else{
      std::cout<<_varFit<<" B  range: "<<_minVarFit[_config.BARREL]<<" - "<<_maxVarFit[ikin][_config.BARREL]<<std::endl;
      std::cout<<_varFit<<" E  range: "<<_minVarFit[_config.ENDCAP]<<" - "<<_maxVarFit[ikin][_config.ENDCAP]<<std::endl;
    }
  }

  TPhotonCuts emptyPhoton;
  TCut sidebandCut = SidebandCut(_varSideband);
  TCut barrelCut = emptyPhoton.RangeBarrel();
  TCut endcapCut = emptyPhoton.RangeEndcap();
  TCut weightCut = "weight";

  TString name;
  TCut cut;

  SetThreeHists(ikin, _config.BARREL, noPrint);
  SetThreeHists(ikin, _config.ENDCAP, noPrint);
  if (ikin==-1){
    _hBkgrTot[0]->Print();
    _hLeakTot[0]->Print();
    _hSignTot[0]->Print();
    std::cout<<"nFitBins="<<_hBkgrTot[0]->GetNbinsX()<<std::endl;
  }

}

void TTemplates::SetThreeHists(int kinBin, int etaBin, bool noPrint){
  TString name=TString("h")+StrLabelKin(kinBin)+StrLabelEta(etaBin); 
  TCut cutWeight="weight";
  TCut cutSideband=SidebandCut(_varSideband);
  TCut cutEta = CutEtaBin(etaBin);
  TCut cutKin = CutKinBin(kinBin);
  TCut cut;

  int nFitBins=0;
  double min=0;
  double max=0;

  min=_minVarFit[etaBin];
  if (kinBin==-1) max=_maxVarFitTot[etaBin];
  else max=_maxVarFit[kinBin][etaBin];
//  if (!noPrint) std::cout<<"min="<<min<<", max="<<max<<std::endl;

  //define nBins, min and max more accurately
  double cutNominalVarFit = ValueCutNominalVarFit(_varFit, etaBin);
  double unit=0;
  if (cutNominalVarFit>max || cutNominalVarFit<min){
    std::cout<<"varFit min="<<min<<"; varFit max = "<<max<<"; cut val = "<<cutNominalVarFit<<std::endl;
  }
  else{
    int nBLeft=0;
    if (kinBin>=0) {nBLeft=_nBinsLeft[kinBin][etaBin];}
    if (kinBin==-1) {nBLeft=_nBinsLeftTot[etaBin];}
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
    if (kinBin==-1) _nBinsLeftTot[etaBin]++;
    if (kinBin>=0) _nBinsLeft[kinBin][etaBin]++;
  }

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"bin lims:";
  }
  for (int i=0; i<=nFitBins; i++){
    fitBinLims[i]=min+i*unit;
    if (!noPrint) std::cout<<fitBinLims[i]<<", ";
  }
  std::cout<<std::endl;

  if (!noPrint){
    std::cout<<"SetThreeHists:"<<StrLabelKin(kinBin)<<StrLabelEta(etaBin)<<std::endl;
  }

  if ((_varKin=="phoEt" || _varKin=="phoSCEt") && _kinBinLims[kinBin]>79)
    cutKin="phoEt>60";

  name+="_Sign"; 
  cut = (cutEta && cutKin)*cutWeight;
  if (!noPrint) std::cout<<"sign template cut="<<cut.GetTitle()<<std::endl;

  _fOutForSave->cd();
  if (kinBin==-1){
    _hSignTot[etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    SetSignalTemplate(_hSignTot[etaBin],cut);
  }
  else{
    _hSign[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    SetSignalTemplate(_hSign[kinBin][etaBin],cut);
  }

  name.ReplaceAll("_Sign","_Bkgr");
  cut = (cutEta && cutKin && cutSideband)*cutWeight;
  if (!noPrint) std::cout<<"bkgr template cut="<<cut.GetTitle()<<std::endl;
  if (kinBin==-1){
    _hBkgrTot[etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    name.ReplaceAll("_Bkgr","_Leak");
    _hLeakTot[etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    TH1D* hLeakTemp = new TH1D("leakTempTot","leakTempTot",nFitBins,fitBinLims);
    SetBackgrTemplate(_hBkgrTot[etaBin], _hLeakTot[etaBin], hLeakTemp, cut, noPrint);
    delete hLeakTemp;

  }
  else{
    _hBkgr[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    name.ReplaceAll("_Bkgr","_Leak");
    _hLeak[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
    TH1D* hLeakTemp = new TH1D("leakTemp","leakTemp",nFitBins,fitBinLims);
    SetBackgrTemplate(_hBkgr[kinBin][etaBin], _hLeak[kinBin][etaBin], hLeakTemp, cut, noPrint);
    delete hLeakTemp;
  }
  if (!noPrint)     std::cout<<std::endl;
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

void TTemplates::DeleteThreeHists(int kinBin, int etaBin){

  if (kinBin==-1)
    delete _hSignTot[etaBin];
  else
    delete _hSign[kinBin][etaBin];

  if (kinBin==-1)
    delete _hBkgrTot[etaBin];
  else
    delete _hBkgr[kinBin][etaBin]; 

  if (kinBin==-1)
    delete _hLeakTot[etaBin];
  else
    delete _hLeak[kinBin][etaBin];
}

TString TTemplates::StrLabelEta(int etaBin){
  if (etaBin==_config.BARREL) return "_Barrel_";
  else if (etaBin==_config.ENDCAP) return "_Endcap_";
  else if (etaBin==_config.COMMON) return "_EtaCommon_";
  return "_EtaUnknown_";
}

TString TTemplates::StrLabelKin(int kinBin){
  if (kinBin==-1) return TString("_")+_varKin+TString("Total_");
  if (kinBin>=_nKinBins) return TString("_")+_varKin+TString("Unknown_");
  TString str="_";
  str+=_varKin;
  str+=_kinBinLims[kinBin];
  str+="to";
  str+=_kinBinLims[kinBin+1];
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

void TTemplates::FitOne(int kinBin, int etaBin, bool noPrint, bool noPlot)
{
  // RooFit variables
  double varMin=0;
  double varMax=0;
  int nBins=0;
  TH1D* hSig = 0;
  TH1D* hBkg = 0;

  if (kinBin==-1){
    hSig = _hSignTot[etaBin];
    hBkg = _hBkgrTot[etaBin];
  }
  else{
    hSig = _hSign[kinBin][etaBin];
    hBkg = _hBkgr[kinBin][etaBin];
  }

  nBins = hBkg->GetNbinsX();
  varMin = hBkg->GetBinLowEdge(1);
  varMax = hBkg->GetBinLowEdge(nBins)+hBkg->GetBinWidth(nBins);

  double minVarSideband = 0;
  double maxVarSideband = 100000;
  double minPhoEta = -2.6;
  double maxPhoEta = 2.6;
  double minKin = -100000;
  double maxKin =  100000;
  double minWeight = 0;
  double maxWeight = 1;

  _treeSign = (TTree *)_fSign->Get("selectedEvents");
  _treeData = (TTree *)_fData->Get("selectedEvents");
  maxVarSideband = _treeData->GetMaximum(_varSideband);
  minVarSideband = _treeData->GetMinimum(_varSideband);
  maxPhoEta = _treeData->GetMaximum("phoEta");
  minPhoEta = _treeData->GetMinimum("phoEta");
  maxKin = _treeData->GetMaximum(_varKin);
  minKin = _treeData->GetMinimum(_varKin);
  maxWeight = _treeSign->GetMaximum("weight");
  minWeight = _treeSign->GetMinimum("weight");
  if (!noPrint){
    std::cout<<_varSideband<<" range: "<<minVarSideband<<" - "<<maxVarSideband<<std::endl;
    std::cout<<"phoEta range: "<<minPhoEta<<" - "<<maxPhoEta<<std::endl;
    std::cout<<_varKin<<" range: "<<minKin<<" - "<<maxKin<<std::endl;
    std::cout<<"weight range: "<<minWeight<<" - "<<maxWeight<<std::endl;
  }

  RooRealVar rooVarFit(_varFit,_labelVarFit, varMin, varMax);
  RooRealVar rooVarSideband(_varSideband,_varSideband,minVarSideband,maxVarSideband);
  RooRealVar rooPhoEta("phoEta","phoEta",minPhoEta,maxPhoEta);
  RooRealVar rooKin(_varKin,_varKin,minKin,maxKin);
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
  RooArgSet argSetData(rooVarFit,rooVarSideband,rooPhoEta,rooKin,rooWeight);

  _fData = new TFile(_config.GetSelectedName(_selectionStage,_channel,_blind,_config.DATA));

  TTree* treeData = (TTree *)_fData->Get("selectedEvents");
  TCut sidebandCut = SidebandCut(_varSideband);
  TCut etaCut = CutEtaBin(etaBin);
  TCut kinCut = CutKinBin(kinBin);
  TCut weightCut = "weight";
  TCut cut = (kinCut && etaCut)*weightCut;
	
  TH1D* hist = (TH1D*)hBkg->Clone("hCloned");
  TString varDraw=_varFit;
  varDraw+=">>hCloned";

  treeData->Draw(varDraw,cut,"goff");
  RooDataHist dataHist("hist", "data set converted to hist", argList, hist);
//  RooDataSet dataSet("dataSet","dataSet",treeData,argSetData,cut);

  //create total pdf
  int nMax = treeData->GetEntries(cut);
  if (nMax<=0){
    if (!noPrint) std::cout<<"ATTENTION: No data events for bin "<<kinCut<<", "<<etaCut<<std::endl;
    if (kinBin==-1) {
      _nBkgrValTot[etaBin]=0;
      _nBkgrErrTot[etaBin]=0;
    }
    else{
      _nBkgrVal[kinBin][etaBin]=0;
      _nBkgrErr[kinBin][etaBin]=0;
    }
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
  if (kinBin==-1) _chi2ToNDFTot[etaBin] = chi2.getVal()/(nBins-2);
  else _chi2ToNDF[kinBin][etaBin] = chi2.getVal()/(nBins-2);

  //get nbkg values from fit
  if (kinBin==-1) {
    _nBkgrValTot[etaBin]=rooNBkg.getVal();
    _nBkgrErrTot[etaBin]=rooNBkg.getError();
  }
  else{
    _nBkgrVal[kinBin][etaBin]=rooNBkg.getVal();
    _nBkgrErr[kinBin][etaBin]=rooNBkg.getError();
  }
 
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"nbkg="<<rooNBkg.getVal()<<"+-"<<rooNBkg.getError()<<", nsig="<<rooNSig.getVal()<<"+-"<<rooNSig.getError()<<", chi2="<<chi2.getVal()/(nBins-2)<<std::endl;
    std::cout<<std::endl;
  }


  //for plotting
  if (kinBin==-1) {
    _plotterTot[etaBin] = new RooPlot(rooVarFit,varMin,varMax,nBins); 
    fullPdf.plotOn(_plotterTot[etaBin],Name("sum"),LineColor(kRed));
    fullPdf.plotOn(_plotterTot[etaBin],Components("signalPdf"), Name("signal"),
    LineColor(kGreen));
    fullPdf.plotOn(_plotterTot[etaBin], Components("backgroundPdf"),Name("background"),LineColor(kBlue));
    fullPdf.paramOn(_plotterTot[etaBin]);
  }
  else{
    _plotter[kinBin][etaBin] = new RooPlot(rooVarFit,varMin,varMax,nBins); 
    dataHist.plotOn(_plotter[kinBin][etaBin]);
    fullPdf.plotOn(_plotter[kinBin][etaBin],Name("sum"),LineColor(kRed));
    fullPdf.plotOn(_plotter[kinBin][etaBin],Components("signalPdf"), Name("signal"),
    LineColor(kGreen));
    fullPdf.plotOn(_plotter[kinBin][etaBin], Components("backgroundPdf"),Name("background"),LineColor(kBlue));
    fullPdf.paramOn(_plotter[kinBin][etaBin]);
  }

  TString plotTitle = "#chi^{2}/ndf=";
  float chiFloat=(100*chi2.getVal()/(nBins-2));
  int chiInt=chiFloat;    
  plotTitle+=(chiInt/100);
  plotTitle+=".";
  plotTitle+=(chiInt%100);
  if (kinBin==-1)
    _plotterTot[etaBin]->SetTitle(plotTitle);
  else 
    _plotter[kinBin][etaBin]->SetTitle(plotTitle);

  TString strRatio="hRatio";
  strRatio+=kinBin;
  strRatio+=etaBin;
  TString strSum="hSum";
  strSum+=kinBin;
  strSum+=etaBin;
  if (kinBin==-1){
    _hRatioTot[etaBin]=(TH1D*)hist->Clone(strRatio);
    _hRatioTot[etaBin]->Sumw2();
    _hSumForRatioTot[etaBin]=(TH1D*)hist->Clone(strSum);
  }
  else{
    _hRatio[kinBin][etaBin]=(TH1D*)hist->Clone(strRatio);
    _hRatio[kinBin][etaBin]->Sumw2();
    _hSumForRatio[kinBin][etaBin]=(TH1D*)hist->Clone(strSum);
  }


  for (int ib=1; ib<=hSig->GetNbinsX(); ib++){

    float cont = (hSig->GetBinContent(ib)*rooNSig.getVal())/hSig->GetSumOfWeights()+(hBkg->GetBinContent(ib)*rooNBkg.getVal())/hBkg->GetSumOfWeights();
    float err1 = (hSig->GetBinError(ib)*rooNSig.getVal())/hSig->GetSumOfWeights();
    float err2 = (hBkg->GetBinError(ib)*rooNBkg.getVal())/hBkg->GetSumOfWeights();
    float err3 = (hSig->GetBinContent(ib)*rooNSig.getError())/hSig->GetSumOfWeights();
    float err4 = (hBkg->GetBinContent(ib)*rooNBkg.getError())/hBkg->GetSumOfWeights();
    float err = sqrt(err1*err1+err2*err2+err3*err3+err4*err4);
    if (kinBin==-1) _hSumForRatioTot[etaBin]->SetBinContent(ib,cont);
    else _hSumForRatio[kinBin][etaBin]->SetBinContent(ib,cont);
    if (kinBin==-1) _hSumForRatioTot[etaBin]->SetBinError(ib,err);
    else _hSumForRatio[kinBin][etaBin]->SetBinError(ib,err);

  }  

  if (kinBin==-1) _hRatioTot[etaBin]->Divide(_hSumForRatioTot[etaBin]);
  else _hRatio[kinBin][etaBin]->Divide(_hSumForRatio[kinBin][etaBin]);

}

void TTemplates::ComputeBkgYieldOneKinBin(int ikin, bool noPrint)
{
  double nBkg[2];
  double nBkgErr[2];
  if (ikin==-1){
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      if (_nBkgrValTot[ieta]==0){
        nBkg[ieta]=_nBkgrValTot[ieta];
        nBkgErr[ieta]=_nBkgrErrTot[ieta];
      }
      else ComputeBkgYieldOne(_hBkgrTot[ieta], _nBkgrValTot[ieta], _nBkgrErrTot[ieta], nBkg[ieta], nBkgErr[ieta], ieta, -1, noPrint);
    }
    _nBkgrYieldsValTot=nBkg[_config.BARREL]+nBkg[_config.ENDCAP];
    _nBkgrYieldsErrTot=sqrt(nBkgErr[_config.BARREL]*nBkgErr[_config.BARREL]+nBkgErr[_config.ENDCAP]*nBkgErr[_config.ENDCAP]);
  }//end of if (ikin==-1)
  if (ikin>=0){
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++)
      if (_nBkgrVal[ikin][ieta]==0){
        nBkg[ieta]=_nBkgrVal[ikin][ieta];
        nBkgErr[ieta]=_nBkgrErr[ikin][ieta];
      }
      else ComputeBkgYieldOne(_hBkgr[ikin][ieta], _nBkgrVal[ikin][ieta], _nBkgrErr[ikin][ieta], nBkg[ieta], nBkgErr[ieta], ieta, -1, noPrint);
    _nBkgrYieldsVal[ikin]=nBkg[_config.BARREL]+nBkg[_config.ENDCAP];
    _nBkgrYieldsErr[ikin]=sqrt(nBkgErr[_config.BARREL]*nBkgErr[_config.BARREL]+nBkgErr[_config.ENDCAP]*nBkgErr[_config.ENDCAP]);    
  }//end of if (ikin>=0)
}

void TTemplates::ComputeBkgYieldOne(TH1D* hBkgr, double nBkgrVal, double nBkgrErr, double& nBkgrYieldVal, double& nBkgrYieldErr, int ieta, int ikin, bool noPrint)
{
  int nHistBins = hBkgr->GetNbinsX();
  nBkgrYieldVal=0;
  double Scut = 0;
  double Stot = 0;
  int nCut = 0;

  double frac;
  double fracErr;
  FractionOfSidebandVarInRange(ikin,ieta,frac,fracErr);
  
  int nBLeft=0;
  if (ikin==-1) nBLeft=_nBinsLeftTot[ieta];
  if (ikin>=0)  nBLeft=_nBinsLeft[ikin][ieta];
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

  for (int ikin=-1; ikin<_nKinBins; ikin++){
    std::cout<<StrLabelKin(ikin)<<": ";
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": ";
      if (ikin==-1) 
        std::cout<<_chi2ToNDFTot[ieta]<<",  nLeft="<<_nBinsLeftTot[ieta]<<"; ";
      else 
        std::cout<<_chi2ToNDF[ikin][ieta]<<",  nLeft="<<_nBinsLeft[ikin][ieta]<<"; ";
    }
    std::cout<<" yield = ";
    if (ikin==-1) std::cout<<_nBkgrYieldsValTot<<"+-"<<_nBkgrYieldsErrTot;
    if (ikin>=0)  std::cout<<_nBkgrYieldsVal[ikin]<<"+-"<<_nBkgrYieldsErr[ikin]<<";  ";
    std::cout<<std::endl;
  }
}

void TTemplates::PlotTemplates()
{
  PlotOneTemplate(-1, _config.BARREL, _plotterTot[_config.BARREL], _hRatioTot[_config.BARREL], _hSumForRatioTot[_config.BARREL]);
  PlotOneTemplate(-1, _config.ENDCAP, _plotterTot[_config.ENDCAP],  _hRatioTot[_config.ENDCAP], _hSumForRatioTot[_config.ENDCAP]);
  for (int kinBin=0; kinBin<_nKinBins; kinBin++){
    PlotOneTemplate(kinBin, _config.BARREL, _plotter[kinBin][_config.BARREL],  _hRatio[kinBin][_config.BARREL], _hSumForRatio[kinBin][_config.BARREL]);
    PlotOneTemplate(kinBin, _config.ENDCAP, _plotter[kinBin][_config.ENDCAP],  _hRatio[kinBin][_config.ENDCAP], _hSumForRatio[kinBin][_config.ENDCAP]);
  }
}


void TTemplates::PlotOneTemplate(int kinBin, int etaBin, RooPlot* plotter, TH1D* hRatio, TH1D* hRatioSum)
{
    TString cName="c";
    cName+=StrLabelKin(kinBin);
    cName+=StrLabelEta(etaBin);
    TCanvas* c1 = new TCanvas(cName,cName, 800, 600);

    c1->Divide(1,2);
    TPad* pad1 = (TPad*)c1->GetPad(1);
    TPad* pad2 = (TPad*)c1->GetPad(2);
    pad1->SetPad(0,0.3,1.0,1.0);
    pad2->SetPad(0,0,  1.0,0.28);
    pad1->SetLeftMargin(0.18);
    pad1->SetTopMargin(0.08);
    pad1->SetRightMargin(0.07);
    pad1->SetBottomMargin(0.01); // All X axis labels and titles are thus cut off
    pad2->SetLeftMargin(0.18);
    pad2->SetTopMargin(0.01);
    pad2->SetRightMargin(0.07);
    pad2->SetBottomMargin(0.45);
    pad1->cd();
    plotter->Draw();
    double valCut = ValueCutNominalVarFit(_varFit, etaBin);
    TLine* cutLine = new TLine(valCut,0,valCut,hRatioSum->GetMaximum());
    cutLine->SetLineWidth(3);
    cutLine->Draw("same");
    pad2->cd();
    hRatio->SetLineWidth(2);
    hRatio->SetStats(0);
    hRatio->GetYaxis()->SetLabelSize(0.1);
    hRatio->GetXaxis()->SetLabelSize(0.1);
    hRatio->GetXaxis()->SetTitleOffset(1.0);
    hRatio->GetXaxis()->SetTitleSize(0.12);
    hRatio->SetTitle(TString("; ")+_labelVarFit+TString(" ;"));
    hRatio->Draw();
    int nBins = hRatio->GetNbinsX();
    TLine* line = new TLine(hRatio->GetBinLowEdge(1),1,hRatio->GetBinLowEdge(nBins)+hRatio->GetBinWidth(nBins),1);
    line->SetLineWidth(2);
    line->SetLineStyle(9);
    line->Draw("same");
   
    cName+=".png";
    cName.ReplaceAll("-1","total");
    c1->SaveAs(cName);
}

void TTemplates::SaveBkgYields()
{
  _fOutForSave->cd();

  TString strTot=_config.GetYieldsDDTemplateBkgName(_config.TOTAL);
  TH1F hTotBkgYield(strTot,strTot,1,_kinBinLims[0],_kinBinLims[_nKinBins]);
  TString str1D=_config.GetYieldsDDTemplateBkgName(_config.ONEDI);
  TH1F h1DBkgYield(str1D,str1D,_nKinBins,_kinBinLims);

  hTotBkgYield.SetBinContent(1,_nBkgrYieldsValTot);
  hTotBkgYield.SetBinError(1,_nBkgrYieldsErrTot);
  for (int i=1; i<_nKinBins+1; i++){  
    h1DBkgYield.SetBinContent(i,_nBkgrYieldsVal[i-1]);
    h1DBkgYield.SetBinError(i,_nBkgrYieldsErr[i-1]);
  }

  hTotBkgYield.Write(strTot); 
  h1DBkgYield.Write(_config.GetYieldsDDTemplateBkgName(_config.ONEDI)); 
}

TCut TTemplates::CutKinBin(int kinBin){

  if (kinBin<-1 || kinBin>=_nKinBins){
    TCut cut("0");
    return cut;
  }
  float min;
  float max;
  if (kinBin==-1){
    min=_kinBinLims[0];
    max=_kinBinLims[_nKinBins];
  }
  else{
    min=_kinBinLims[kinBin];
    max=_kinBinLims[kinBin+1];
  }

  TString kinBinCutStr;
  kinBinCutStr=_varKin;
  kinBinCutStr+=">";
  kinBinCutStr+=min;
  kinBinCutStr+=" && ";
  kinBinCutStr+=_varKin;
  kinBinCutStr+="<=";
  kinBinCutStr+=max;
  TCut cut(kinBinCutStr);
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
    if (etaBin==_config.BARREL) return phoSigmaIEtaIEtaBarrelCut_[_phoWP];
    else if (etaBin==_config.ENDCAP) return phoSigmaIEtaIEtaEndcapCut_[_phoWP];
    else return 1;
  }
  if (varFit=="phoPFChIsoCorr"||varFit=="phoSCRChIsoCorr"){
    if (etaBin==_config.BARREL) return phoChIsoBarrelCut_[_phoWP];
    else if (etaBin==_config.ENDCAP) return phoChIsoEndcapCut_[_phoWP];
    else return 1;
  }
  return 100000;
}

void TTemplates::FractionOfSidebandVarInRange(int ikin, int ieta, double& frac, double& fracErr){
  TCut cutEta = CutEtaBin(ieta);
  TCut cutKin = CutKinBin(ikin);
  TCut rangeCut = "1";
  TCut cutWeight = "weight";
  TPhotonCuts emptyPhoton;

  if (_varSideband=="phoSCRChIsoCorr"||_varSideband=="phoPFChIsoCorr") 
    rangeCut=emptyPhoton.RangePhoChIso(_varSideband,_phoWP);
  if (_varSideband=="phoSigmaIEtaIEta")
    rangeCut=emptyPhoton.RangeSigmaIEtaIEta(_phoWP);

  double min=_treeData->GetMinimum(_varFit)-0.1;
  double max=_treeData->GetMaximum(_varFit)+0.1;
  int nFitBins=1;
  double fitBinLims[nFitBins+1];
  fitBinLims[0]=min;
  fitBinLims[1]=max;

  double Npass=0;
  TString namePass="hBkgrPass";
//  namePass+=ieta;
//  namePass+=ikin;
  TCut cutPass=(cutEta && cutKin && rangeCut)*cutWeight;
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
//  nameFail+=ikin;
  TCut cutFail=(cutEta && cutKin && !rangeCut)*cutWeight;
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

