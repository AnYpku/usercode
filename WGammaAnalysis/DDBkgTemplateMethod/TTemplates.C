#include "TTemplates.h"
  //this class
#include "../Include/TPhotonCuts.h"
#include "../Include/TMathTools.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

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

  //_isClosureTest=CLOSURE_TEST_1;
  _isClosureTest=NO_CLOSURE_TEST;
  _cutWeight="weight";
  if (_isClosureTest) _cutWeight="1";

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
  if (!_isClosureTest) _fData = new TFile(_config.GetSelectedName(_selectionStage,_channel,_blind,_config.DATA));
  if (_isClosureTest) _fData = new TFile(_config.GetSelectedName(_selectionStage,_channel,_blind,_config.BKGMC,"Wjets_to_lnu"));
  _treeSign = (TTree *)_fSign->Get("selectedEvents");
  _treeData = (TTree *)_fData->Get("selectedEvents");
  for (unsigned int i=0; i<_INPUT->allInputs_.size(); i++){
    int sample = _INPUT->allInputs_[i].sample_;
    if (sample==_config.DATA || sample==_config.SIGMC)
      continue;
    TString sourceName =  _INPUT->allInputs_[i].sourceName_;
    //if (sourceName=="Wjets_to_lnu" || sourceName=="DYjets_to_ll" || sourceName=="ttbarjets") 
    //  continue;
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

  _nBinsLeft[0][_config.BARREL] = nBinsLeftBTot;
  _nBinsLeft[0][_config.ENDCAP] = nBinsLeftETot;
  _maxVarFit[0][_config.BARREL] = maxVarFitBTot;
  _maxVarFit[0][_config.ENDCAP] = maxVarFitETot;
  for (int i=1; i<_nKinBins+1; i++){
    _nBinsLeft[i][_config.BARREL] = nBinsLeftB[i-1];
    _nBinsLeft[i][_config.ENDCAP] = nBinsLeftE[i-1];
    _maxVarFit[i][_config.BARREL] = maxVarFitB[i-1];
    _maxVarFit[i][_config.ENDCAP] = maxVarFitE[i-1];
  }

}

TTemplates::~TTemplates()
{
}

void TTemplates::ComputeBackground(bool noPrint, bool noPlot)
{

  for (int ikin=0; ikin<=_nKinBins; ikin++){
    ComputeBackgroundOne(ikin,noPrint,noPlot);
  }
  if (!noPlot) PlotTemplates();
  PrintBkgYieldsAndChi2();
  SaveBkgYields();
}

void TTemplates::ComputeBackgroundOne(int ikin, bool noPrint, bool noPlot)
{
  SetThreeHists(ikin, _config.BARREL, noPrint);
  FitOne(ikin,_config.BARREL,noPrint,noPlot);
  SetThreeHists(ikin, _config.ENDCAP, noPrint);
  FitOne(ikin,_config.ENDCAP,noPrint,noPlot);
  if (!_isClosureTest) ComputeBkgYieldOneKinBin(ikin,noPrint);
}

void TTemplates::SetThreeHists(int kinBin, int etaBin, bool noPrint){
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"SetThreeHists for: "<<StrLabelKin(kinBin)<<", "<<StrLabelEta(etaBin)<<std::endl;
  }
  TString name=TString("h")+StrLabelKin(kinBin)+StrLabelEta(etaBin); 
  
  TCut cutSideband=SidebandCut();
  TCut cutEta = CutEtaBin(etaBin);
  TCut cutKin = CutKinBin(kinBin);
  TCut cut;

  int nFitBins=0;
  double min=0;
  double max=0;

  min=_minVarFit[etaBin];
  max=_maxVarFit[kinBin][etaBin];
//  if (!noPrint) std::cout<<"min="<<min<<", max="<<max<<std::endl;

  //define nBins, min and max more accurately
  double cutNominalVarFit = ValueCutNominalVarFit(etaBin);
  double unit=0;
  if (cutNominalVarFit>max || cutNominalVarFit<min){
    std::cout<<"varFit min="<<min<<"; varFit max = "<<max<<"; cut val = "<<cutNominalVarFit<<std::endl;
  }
  else{
    int nBLeft=0;
    nBLeft=_nBinsLeft[kinBin][etaBin];
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
    _nBinsLeft[kinBin][etaBin]++;
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

  if ((_varKin=="phoEt" || _varKin=="phoSCEt") && _kinBinLims[kinBin]>79)
    cutKin="phoEt>60";

  name+="_Sign"; 
  cut = (cutEta && cutKin)*_cutWeight;
  if (_isClosureTest==CLOSURE_TEST_1) 
      cut = (cutEta && cutKin && SidebandVarNominalCut())*_cutWeight; 
  if (!noPrint) {
    std::cout<<"cutEta="<<cutEta.GetTitle()<<std::endl;
    std::cout<<"cutKin="<<cutKin.GetTitle()<<std::endl;
    std::cout<<"cutWeight="<<_cutWeight.GetTitle()<<std::endl;
    std::cout<<"sign template cut=(cutEta && cutKin)*cutWeight="<<cut.GetTitle()<<std::endl;
  }

  _fOutForSave->cd();
  _hSign[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
  _hSign[kinBin][etaBin]->Sumw2();
  SetSignalTemplate(_hSign[kinBin][etaBin],cut);

  name.ReplaceAll("_Sign","_Bkgr");
  cut = (cutEta && cutKin && cutSideband)*_cutWeight;
  if (_isClosureTest==CLOSURE_TEST_1) 
      cut = (cutEta && cutKin && SidebandVarNominalCut())*_cutWeight;
  if (!noPrint) std::cout<<"bkgr template cut="<<cut.GetTitle()<<std::endl;

  _hBkgr[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
  _hBkgr[kinBin][etaBin]->Sumw2();
  name.ReplaceAll("_Bkgr","_Leak");
  _hLeak[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
  _hLeak[kinBin][etaBin]->Sumw2();
  TH1D* hLeakTemp = new TH1D("leakTemp","leakTemp",nFitBins,fitBinLims);
  hLeakTemp->Sumw2();
  SetBackgrTemplate(_hBkgr[kinBin][etaBin], _hLeak[kinBin][etaBin], hLeakTemp, cut, noPrint);
  delete hLeakTemp;

  if (!noPrint)     std::cout<<std::endl;
}

void TTemplates::SetSignalTemplate(TH1D* hSign, TCut cut)
{
  TString name=hSign->GetName();
  _treeSign->Draw(_varFit+TString(">>")+name,cut,"goff");
  if (_isClosureTest==CLOSURE_TEST_1 || _isClosureTest==CLOSURE_TEST_2){
    TPhotonCuts emptyPhoton;
    TCut cutTrue = emptyPhoton.RangeGenTruePhoton();
    _treeData->Draw(_varFit+TString(">>")+name,cut && cutTrue*_cutWeight,"goff");
  }
}

void TTemplates::SetBackgrTemplate(TH1D* hBkgr, TH1D* hLeak, TH1D* hLeakTemp, TCut cut, bool noPrint)
{

  TString nameBkgr=hBkgr->GetName();
  _treeData->Draw(_varFit+TString(">>")+nameBkgr,cut,"goff");
  TPhotonCuts emptyPhoton;
  if (_isClosureTest==CLOSURE_TEST_1){
    TCut cutFake = emptyPhoton.RangeGenFakePhoton();
    _treeData->Draw(_varFit+TString(">>")+nameBkgr,cut && cutFake*_cutWeight,"goff");
    return;
  }

  TString nameLeak=hLeak->GetName();
  _treeSign->Draw(_varFit+TString(">>")+nameLeak,cut,"goff");

  if (!noPrint){
      std::cout<<"(fake bkrg)+leak="<<hBkgr->GetSumOfWeights()<<", (sign leak)="<<hLeak->GetSumOfWeights()<<std::endl;
  }

  if (!noPrint) std::cout<<"_vecTreeBkg.size()="<<_vecTreeBkg.size()<<std::endl;
  if (!noPrint) std::cout<<"(true gamma bkg leaks):";

  TString nameLeakTemp=hLeakTemp->GetName();

  TCut cutTrueGamma = emptyPhoton.RangeGenFakePhoton();

  for (unsigned int is=0; is<_vecTreeBkg.size(); is++){
    if (_isClosureTest){
      if (_vecBkgNames[is]=="Wjets_to_lnu"){
        _treeData->Draw(_varFit+TString(">>")+nameLeak,cut && cutTrueGamma*_cutWeight,"goff");
        break;
      }
      else continue;
    }
    else{
      if (_vecBkgNames[is]=="Wjets_to_lnu" || _vecBkgNames[is]=="DYjets_to_ll" || _vecBkgNames[is]=="ttjets")
        continue;
      _vecTreeBkg[is]->Draw(_varFit+TString(">>")+nameLeakTemp,cut && cutTrueGamma*_cutWeight,"goff");
      hLeak->Add(hLeakTemp);
      if (!noPrint) std::cout<<" "<<hLeakTemp->GetSumOfWeights();
    }
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

  delete _hSign[kinBin][etaBin];

  delete _hBkgr[kinBin][etaBin]; 

  delete _hLeak[kinBin][etaBin];
}

TString TTemplates::StrLabelEta(int etaBin){
  if (etaBin==_config.BARREL) return "_Barrel_";
  else if (etaBin==_config.ENDCAP) return "_Endcap_";
  else if (etaBin==_config.COMMON) return "_EtaCommon_";
  return "_EtaUnknown_";
}

TString TTemplates::StrLabelKin(int kinBin){
  if (kinBin<0 || kinBin>_nKinBins) return TString("_")+_varKin+TString("Unknown_");
  if (kinBin==0) return TString("_")+_varKin+TString("Total_");
  TString str="_";
  str+=_varKin;
  str+=_kinBinLims[kinBin-1];
  str+="to";
  str+=_kinBinLims[kinBin];
  str+="_";
  return str;
}

TCut TTemplates::SidebandCut(){
  TPhotonCuts emptyPhoton;
  TCut cut="1";
  if (_varSideband=="phoSigmaIEtaIEta") 
    cut=emptyPhoton.SidebandSigmaIEtaIEta();
  else if (_varSideband=="phoSCRChIsoCorr"||_varSideband=="phoPFChIsoCorr")
    cut=emptyPhoton.SidebandPhoChIso(_varSideband);
  return cut;
}

TCut TTemplates::SidebandVarNominalCut(){
  TPhotonCuts emptyPhoton;
  TCut cut="1";
  if (_varSideband=="phoSigmaIEtaIEta") 
    cut=emptyPhoton.RangeSigmaIEtaIEta(_phoWP);
  else if (_varSideband=="phoSCRChIsoCorr")
    cut=emptyPhoton.RangePhoChIso("phoSCRChIsoCorr", _phoWP);
  else if (_varSideband=="phoPFChIsoCorr")
    cut=emptyPhoton.RangePhoChIso("phoPFChIsoCorr", _phoWP);
  return cut;
}

void TTemplates::FitOne(int kinBin, int etaBin, bool noPrint, bool noPlot)
{

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"FitOne: "<<StrLabelKin(kinBin)<<", "<<StrLabelEta(etaBin)<<std::endl;
    std::cout<<std::endl;
  }

  // RooFit variables
  double varMin=0;
  double varMax=0;
  int nBins=0;
  TH1D* hSig = 0;
  TH1D* hBkg = 0;


  hSig = _hSign[kinBin][etaBin];
  hBkg = _hBkgr[kinBin][etaBin];


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

  //_treeSign = (TTree *)_fSign->Get("selectedEvents");
  //_treeData = (TTree *)_fData->Get("selectedEvents");
  maxVarSideband = _treeData->GetMaximum(_varSideband);
  minVarSideband = _treeData->GetMinimum(_varSideband);
  maxPhoEta = _treeData->GetMaximum("phoSCEta");
  minPhoEta = _treeData->GetMinimum("phoSCEta");
  maxKin = _treeData->GetMaximum(_varKin);
  minKin = _treeData->GetMinimum(_varKin);
  maxWeight = _treeSign->GetMaximum("weight");
  minWeight = _treeSign->GetMinimum("weight");
  if (!noPrint){
    std::cout<<_varSideband<<" range: "<<minVarSideband<<" - "<<maxVarSideband<<std::endl;
    std::cout<<"phoSCEta range: "<<minPhoEta<<" - "<<maxPhoEta<<std::endl;
    std::cout<<_varKin<<" range: "<<minKin<<" - "<<maxKin<<std::endl;
    std::cout<<"weight range: "<<minWeight<<" - "<<maxWeight<<std::endl;
  }

  RooRealVar rooVarFit(_varFit,_labelVarFit, varMin, varMax);
  RooRealVar rooVarSideband(_varSideband,_varSideband,minVarSideband,maxVarSideband);
  RooRealVar rooPhoEta("phoSCEta","phoSCEta",minPhoEta,maxPhoEta);
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

  TCut sidebandCut = SidebandCut();
  TCut etaCut = CutEtaBin(etaBin);
  TCut kinCut = CutKinBin(kinBin);
  TCut sidebandVarNonimalCut = SidebandVarNominalCut();
  if (!noPrint){
    std::cout<<"etaCut="<<etaCut.GetTitle()<<std::endl;
    std::cout<<"kinCut="<<kinCut.GetTitle()<<std::endl;
    std::cout<<"sidebandVarNominalCut="<<sidebandVarNonimalCut.GetTitle()<<std::endl;
  }
  TCut cut = (kinCut && etaCut && sidebandVarNonimalCut)*_cutWeight;
	
  TString hName="hCloned";
  hName+=StrLabelKin(kinBin);
  hName+=StrLabelEta(etaBin);
  _hData[kinBin][etaBin] = (TH1D*)hBkg->Clone(hName);
  TString varDraw=_varFit;
  varDraw+=">>";
  varDraw+=hName;

  _treeData->Draw(varDraw,cut,"goff");
  RooDataHist dataHist("hist", "data set converted to hist", argList, _hData[kinBin][etaBin]);
  std::cout<<"data hist TH1D* hist: "<<std::endl;
  _hData[kinBin][etaBin]->Print();
//  RooDataSet dataSet("dataSet","dataSet",_treeData,argSetData,cut);

  //create total pdf
  int nMax = _treeData->GetEntries(cut);
  if (nMax<=0){
    if (!noPrint) std::cout<<"ATTENTION: No data events for bin "<<kinCut<<", "<<etaCut<<std::endl;
    _nBkgrFromFitVal[kinBin][etaBin]=0;
    _nBkgrFromFitErr[kinBin][etaBin]=0;
    _nSignFromFitVal[kinBin][etaBin]=0;
    _nSignFromFitErr[kinBin][etaBin]=0;
    _plotter[kinBin][etaBin] = new RooPlot(rooVarFit,varMin,varMax,nBins);
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
  _chi2ToNDF[kinBin][etaBin] = chi2.getVal()/(nBins-2);

  //get nbkg values from fit
  _nBkgrFromFitVal[kinBin][etaBin]=rooNBkg.getVal();
  _nBkgrFromFitErr[kinBin][etaBin]=rooNBkg.getError();
  _nSignFromFitVal[kinBin][etaBin]=rooNSig.getVal();
  _nSignFromFitErr[kinBin][etaBin]=rooNSig.getError();
  
 
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"nbkg="<<rooNBkg.getVal()<<"+-"<<rooNBkg.getError()<<", nsig="<<rooNSig.getVal()<<"+-"<<rooNSig.getError()<<", chi2="<<chi2.getVal()/(nBins-2)<<std::endl;
    std::cout<<std::endl;
  }


  //for plotting
  _plotter[kinBin][etaBin] = new RooPlot(rooVarFit,varMin,varMax,nBins); 
  dataHist.plotOn(_plotter[kinBin][etaBin]);
  fullPdf.plotOn(_plotter[kinBin][etaBin],Name("sum"),LineColor(kRed));
  fullPdf.plotOn(_plotter[kinBin][etaBin],Components("signalPdf"), Name("signal"),
  LineColor(kGreen));
  fullPdf.plotOn(_plotter[kinBin][etaBin], Components("backgroundPdf"),Name("background"),LineColor(kBlue));
  fullPdf.paramOn(_plotter[kinBin][etaBin]);
  


}

void TTemplates::ComputeBkgYieldOneKinBin(int ikin, bool noPrint)
{
  double nBkg[2];
  double nBkgErr[2];
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    if (_nBkgrFromFitVal[ikin][ieta]==0){
      nBkg[ieta]=_nBkgrFromFitVal[ikin][ieta];
      nBkgErr[ieta]=_nBkgrFromFitErr[ikin][ieta];
    }
    else ComputeBkgYieldOne(_hBkgr[ikin][ieta], _nBkgrFromFitVal[ikin][ieta], _nBkgrFromFitErr[ikin][ieta], nBkg[ieta], nBkgErr[ieta], ieta, ikin, noPrint);
  }
  _nBkgrYieldsVal[ikin]=nBkg[_config.BARREL]+nBkg[_config.ENDCAP];
  _nBkgrYieldsErr[ikin]=sqrt(nBkgErr[_config.BARREL]*nBkgErr[_config.BARREL]+nBkgErr[_config.ENDCAP]*nBkgErr[_config.ENDCAP]);    
}

void TTemplates::ComputeBkgYieldOne(TH1D* hBkgr, double nBkgrVal, double nBkgrErr, double& nBkgrYieldVal, double& nBkgrYieldErr, int ieta, int ikin, bool noPrint)
{

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"ComputeBkgYieldOne: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<std::endl;
  }

  int nHistBins = hBkgr->GetNbinsX();
  nBkgrYieldVal=0;
  double Scut = 0;
  double Stot = 0;
  int nCut = 0;
  
  int nBLeft=0;
  nBLeft=_nBinsLeft[ikin][ieta];
  for (int i=1; i<=nBLeft; i++){
    Scut+=hBkgr->GetBinContent(i);
    Stot+=hBkgr->GetBinContent(i);
  }
  if (!noPrint){
    std::cout<<"fit var cut value = "<<hBkgr->GetBinLowEdge(nBLeft)+hBkgr->GetBinWidth(nBLeft)<<std::endl;
  }
  for (int i=nBLeft+1; i<=nHistBins; i++){
    Stot+=hBkgr->GetBinContent(i);
  }

  if (Stot==0){
    nBkgrYieldVal=0;
    nBkgrYieldErr=0;
    return;
  }

  nBkgrYieldVal=nBkgrVal*Scut/Stot;
  //compute error
  double firstTerm = nBkgrErr*nBkgrErr*Scut*Scut/(Stot*Stot);
  double secondTerm = 0;
  for (int j=1; j<=nCut; j++){
    secondTerm += hBkgr->GetBinError(j)*hBkgr->GetBinError(j)* nBkgrVal*nBkgrVal*(Stot-Scut)*(Stot-Scut)/(Stot*Stot*Stot*Stot);
  }
  double thirdTerm = 0;
  for (int j=nCut+1; j<=nHistBins; j++){
    thirdTerm += hBkgr->GetBinError(j)*hBkgr->GetBinError(j)* nBkgrVal*nBkgrVal*Scut*Scut/(Stot*Stot*Stot*Stot);
  }  

  nBkgrYieldErr = sqrt(firstTerm+secondTerm+thirdTerm);
  if (!noPrint){
    std::cout<<"Error computation, "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<"1st term = "<<sqrt(firstTerm)<<", 2nd term = "<<sqrt(secondTerm)<<", 3rd term = "<<sqrt(thirdTerm)<<std::endl;
  }
}

void TTemplates::PrintBkgYieldsAndChi2()
{

  std::cout<<"Bkgr and sign values from fit:"<<std::endl;
  std::cout<<"  bin                            bkgr-B            sign-B           bkgr-E            sign-E"<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
//      std::cout<<std::setw(6)<<std::setprecision(0)<<_nBkgrFromFitVal[ikin][ieta]<<"+-";
//      std::cout<<std::setw(5)<<std::setprecision(0)<<_nBkgrFromFitErr[ikin][ieta]<<"; ";
//      std::cout<<std::setw(6)<<std::setprecision(0)<<_nSignFromFitVal[ikin][ieta]<<"+-";
//      std::cout<<std::setw(5)<<std::setprecision(0)<<_nSignFromFitErr[ikin][ieta]<<"; ";
      std::cout<<_nBkgrFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nBkgrFromFitErr[ikin][ieta]<<"; ";
      std::cout<<_nSignFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nSignFromFitErr[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  std::cout<<"Chi2/NDF for different bins: "<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": chi2/ndf=";
      std::cout<<_chi2ToNDF[ikin][ieta]<<",  nFitBinsLeft="<<_nBinsLeft[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;  
  }
  std::cout<<std::endl;

  std::cout<<"Fake bkgr yields:"<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    std::cout<<_nBkgrYieldsVal[ikin]<<"+-"<<_nBkgrYieldsErr[ikin]<<";  ";
    std::cout<<std::endl;
  }

  if (!_isClosureTest) return;


  TPhotonCuts emptyPhoton;
  TCut cutTrueGamma=emptyPhoton.RangeGenTruePhoton();
  TCut cutFakeGamma=emptyPhoton.RangeGenFakePhoton();
  std::cout<<std::endl;
  std::cout<<"--- Closure test only ---"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"Bkgr values from fit vs fake gamma Wjets MC truth:"<<std::endl;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    std::cout<<StrLabelEta(ieta)<<": "<<std::endl;
    TCut cutVarFit;
    TString strCutVarFit=_varFit;
    strCutVarFit+="<";
    strCutVarFit+=ValueCutNominalVarFit(ieta);
    cutVarFit=strCutVarFit;
    for (int ikin=0; ikin<1; ikin++){
    //for (int ikin=0; ikin<_nKinBins+1; ikin++){
      std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
      std::cout<<_nBkgrFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nBkgrFromFitErr[ikin][ieta]<<"; ";
      int y=_treeData->GetEntries( (CutKinBin(ikin) && CutEtaBin(ieta)&& cutFakeGamma && SidebandVarNominalCut())*_cutWeight);
      std::cout<<y<<"+-";
      std::cout<<sqrt(y)<<"; ";
      float yh=_hBkgr[ikin][ieta]->GetSumOfWeights();
      std::cout<<yh<<"+-";
      std::cout<<sqrt(yh)<<"; ";
      std::cout<<std::endl;
    }
  }
  std::cout<<"Sign values from fit vs true gamma Wjets MC truth vs _hSign->SumOfWeights():"<<std::endl;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    std::cout<<StrLabelEta(ieta)<<": "<<std::endl;
    TCut cutVarFit;
    TString strCutVarFit=_varFit;
    strCutVarFit+="<";
    strCutVarFit+=ValueCutNominalVarFit(ieta);
    cutVarFit=strCutVarFit;
    for (int ikin=0; ikin<1; ikin++){
    //for (int ikin=0; ikin<_nKinBins+1; ikin++){
      std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
      std::cout<<_nSignFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nSignFromFitErr[ikin][ieta]<<"; ";
      int y=_treeData->GetEntries((CutKinBin(ikin) && CutEtaBin(ieta) && cutTrueGamma && SidebandVarNominalCut())*_cutWeight);
      std::cout<<y<<"+-";
      std::cout<<sqrt(y)<<"; ";
      float yh=_hSign[ikin][ieta]->GetSumOfWeights();
      std::cout<<yh<<"+-";
      std::cout<<sqrt(yh)<<"; ";
      std::cout<<std::endl;
    }
  }
  std::cout<<std::endl;


}

void TTemplates::PlotTemplates()
{

  for (int kinBin=0; kinBin<_nKinBins+1; kinBin++){
    for (int etaBin=_config.BARREL; etaBin<=_config.ENDCAP; etaBin++){
      PlotOneTemplate(kinBin, etaBin);
    }//end of loop over etaBin
  }//end of loop over kinBin

}


void TTemplates::PlotOneTemplate(int kinBin, int etaBin)
{

  TString strChi = "#chi^{2}/ndf=";
  float chiFloat=(100*_chi2ToNDF[kinBin][etaBin]);
  int chiInt=chiFloat;    
  strChi+=(chiInt/100);
  strChi+=".";
  strChi+=(chiInt%100);


  TString strRatio="hRatio";
  strRatio+=kinBin;
  strRatio+=etaBin;
  TString strSum="hSum";
  strSum+=kinBin;
  strSum+=etaBin;
  _hRatio[kinBin][etaBin]=(TH1D*)_hData[kinBin][etaBin]->Clone(strRatio);
  _hRatio[kinBin][etaBin]->Sumw2();
  _hSumm[kinBin][etaBin]=(TH1D*)_hData[kinBin][etaBin]->Clone(strSum);
  
  float areaHistSign=0;
  float areaHistBkgr=0;
  for (int ib=1; ib<=_hSign[kinBin][etaBin]->GetNbinsX(); ib++){
    areaHistSign+=_hSign[kinBin][etaBin]->GetBinContent(ib);
    areaHistBkgr+=_hBkgr[kinBin][etaBin]->GetBinContent(ib);
  }

  for (int ib=1; ib<=_hSign[kinBin][etaBin]->GetNbinsX(); ib++){

    float contS=0;
    float err1S=0;
    float err2S=0;
    float contB=0;
    float err1B=0;
    float err2B=0;

    if (areaHistSign==0);
    else{
      contS = _hSign[kinBin][etaBin]->GetBinContent(ib)*_nSignFromFitVal[kinBin][etaBin]/areaHistSign;
      err1S = (_hSign[kinBin][etaBin]->GetBinError(ib)*_nSignFromFitVal[kinBin][etaBin])/areaHistSign;
      err2S = (_hSign[kinBin][etaBin]->GetBinContent(ib)*_nSignFromFitErr[kinBin][etaBin])/areaHistSign;
    }
    if (areaHistBkgr==0);
    else {
      contB = _hBkgr[kinBin][etaBin]->GetBinContent(ib)*_nBkgrFromFitVal[kinBin][etaBin]/areaHistBkgr;
      err1B = (_hBkgr[kinBin][etaBin]->GetBinError(ib)*_nBkgrFromFitVal[kinBin][etaBin])/areaHistBkgr;
      err2B = (_hBkgr[kinBin][etaBin]->GetBinContent(ib)*_nBkgrFromFitErr[kinBin][etaBin])/areaHistBkgr;
    }
    _hSign[kinBin][etaBin]->SetBinContent(ib,contS);
    _hSign[kinBin][etaBin]->SetBinError(ib,sqrt(err1S*err1S+err2S*err2S));

    _hBkgr[kinBin][etaBin]->SetBinContent(ib,contB);
    _hBkgr[kinBin][etaBin]->SetBinError(ib,sqrt(err1B*err1B+err2B*err2B));

    float err = sqrt(err1S*err1S+err2S*err2S+err1B*err1B+err2B*err2B);
    _hSumm[kinBin][etaBin]->SetBinContent(ib,contS+contB);
    _hSumm[kinBin][etaBin]->SetBinError(ib,err);

  }  

  _hRatio[kinBin][etaBin]->Divide(_hSumm[kinBin][etaBin]);


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

    _plotter[kinBin][etaBin]->SetTitle("");
    _plotter[kinBin][etaBin]->Draw();

    //_hData[kinBin][etaBin]->SetStats(0);
    //_hData[kinBin][etaBin]->SetLineColor(1);
    //_hData[kinBin][etaBin]->SetLineWidth(2);
    //_hData[kinBin][etaBin]->Draw("EP"); 
    _hSumm[kinBin][etaBin]->SetLineColor(7);
    _hSumm[kinBin][etaBin]->SetLineWidth(2);
    _hSumm[kinBin][etaBin]->SetLineStyle(3);
    _hSumm[kinBin][etaBin]->Draw("HIST same");  
    //_hBkgr[kinBin][etaBin]->SetLineColor(4);
    //_hBkgr[kinBin][etaBin]->SetLineWidth(2);  
    //_hBkgr[kinBin][etaBin]->SetLineStyle(9);  
    //_hBkgr[kinBin][etaBin]->Draw("HIST same"); 
    //_hSign[kinBin][etaBin]->SetLineColor(30);
    //_hSign[kinBin][etaBin]->SetLineWidth(2);  
    //_hSign[kinBin][etaBin]->SetLineStyle(9);  
    //_hSign[kinBin][etaBin]->Draw("HIST same"); 
    //_hData[kinBin][etaBin]->SetLineColor(1);
    //_hData[kinBin][etaBin]->SetLineWidth(2);
    //_hData[kinBin][etaBin]->Draw("EP same"); 

    double valCut = ValueCutNominalVarFit(etaBin);
    TLine* cutLine = new TLine(valCut,0,valCut,_hSumm[kinBin][etaBin]->GetMaximum());
    cutLine->SetLineWidth(3);
    cutLine->Draw("same");
    pad2->cd();
    _hRatio[kinBin][etaBin]->SetLineWidth(2);
    _hRatio[kinBin][etaBin]->SetStats(0);
    _hRatio[kinBin][etaBin]->GetYaxis()->SetLabelSize(0.1);
    _hRatio[kinBin][etaBin]->GetXaxis()->SetLabelSize(0.1);
    _hRatio[kinBin][etaBin]->GetXaxis()->SetTitleOffset(1.0);
    _hRatio[kinBin][etaBin]->GetXaxis()->SetTitleSize(0.12);
    _hRatio[kinBin][etaBin]->SetTitle(TString("; ")+_labelVarFit+TString(" ;"));
    _hRatio[kinBin][etaBin]->Draw();
    int nBins = _hRatio[kinBin][etaBin]->GetNbinsX();
    TLine* line = new TLine(_hRatio[kinBin][etaBin]->GetBinLowEdge(1),1,_hRatio[kinBin][etaBin]->GetBinLowEdge(nBins)+_hRatio[kinBin][etaBin]->GetBinWidth(nBins),1);
    line->SetLineWidth(2);
    line->SetLineStyle(9);
    line->Draw("same");
   
    cName+=".png";
    cName.ReplaceAll("-1","total");
    c1->SaveAs(cName);

//  cName+="plotter";
//  TCanvas* c2 = new TCanvas(cName);
//  _plotter[kinBin][etaBin]->Draw();
}

void TTemplates::SaveBkgYields()
{
  _fOutForSave->cd();

  TString strTot=_config.GetYieldsDDTemplateBkgName(_config.TOTAL);
  TH1F hTotBkgYield(strTot,strTot,1,_kinBinLims[0],_kinBinLims[_nKinBins]);
  TString str1D=_config.GetYieldsDDTemplateBkgName(_config.ONEDI);
  TH1F h1DBkgYield(str1D,str1D,_nKinBins,_kinBinLims);

  hTotBkgYield.SetBinContent(1,_nBkgrYieldsVal[0]);
  hTotBkgYield.SetBinError(1,_nBkgrYieldsErr[0]);
  for (int i=1; i<_nKinBins+1; i++){  
    h1DBkgYield.SetBinContent(i,_nBkgrYieldsVal[i]);
    h1DBkgYield.SetBinError(i,_nBkgrYieldsErr[i]);
  }

  hTotBkgYield.Write(strTot); 
  h1DBkgYield.Write(_config.GetYieldsDDTemplateBkgName(_config.ONEDI)); 
}

TCut TTemplates::CutKinBin(int kinBin){

  if (kinBin<0 || kinBin>_nKinBins){
    TCut cut("0");
    return cut;
  }
  float min;
  float max;
  if (kinBin==0){
    min=_kinBinLims[0];
    max=_kinBinLims[_nKinBins];
  }
  else{
    min=_kinBinLims[kinBin-1];
    max=_kinBinLims[kinBin];
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

double TTemplates::ValueCutNominalVarFit(int etaBin){
  TPhotonCuts emptyPhoton;
  if (_varFit=="phoSigmaIEtaIEta"){
    if (etaBin==_config.BARREL) return phoSigmaIEtaIEtaBarrelCut_[_phoWP];
    else if (etaBin==_config.ENDCAP) return phoSigmaIEtaIEtaEndcapCut_[_phoWP];
    else return 1;
  }
  if (_varFit=="phoPFChIsoCorr"||_varFit=="phoSCRChIsoCorr"){
    if (etaBin==_config.BARREL) return phoChIsoBarrelCut_[_phoWP];
    else if (etaBin==_config.ENDCAP) return phoChIsoEndcapCut_[_phoWP];
    else return 1;
  }
  return 100000;
}
