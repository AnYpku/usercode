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
#include "TLatex.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TVectorD.h"

using namespace RooFit ;

TTemplates::TTemplates(int year, int channel, int blind, int phoWP, TString varFit, TString varSideband, TString varKin, int nKinBins, float* kinBinLims, bool noAdjustBinning, bool noPhoPFChIsoCut, bool isMetCutOptimization)
{

 // _isClosureTest=CLOSURE_TEST_3;
  _isClosureTest=NO_CLOSURE_TEST;
  _cutWeight="weight";
  if (_isClosureTest) _cutWeight="1";

  _year=year;
  _channel=channel;
  _blind=blind;
  _iter=0;
  //if (blind == _config.BLIND_PRESCALE) _blind=_config.UNBLIND;
  _blind=_config.UNBLIND;
  _phoWP=phoWP;
  _INPUT = new TAllInputSamples(_channel,"../Configuration/config.txt");
  _fOutForSave = new TFile(_config.GetDDTemplateFileName(_channel),"recreate");
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

  _noAdjustBinning=noAdjustBinning;
  _noPhoPFChIsoCut=noPhoPFChIsoCut;

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

  if (_varFit=="phoSigmaIEtaIEta"){
    _minVarFit[_config.BARREL] = _photon.GetPhoSigmaIEtaIEtaCutLeftB();
    _minVarFit[_config.ENDCAP] = _photon.GetPhoSigmaIEtaIEtaCutLeftE();
  }
  if (_varFit=="phoSCRChIsoCorr"){
    _minVarFit[_config.BARREL] = 0;
    _minVarFit[_config.ENDCAP] = 0;
  }

  for (int i=0; i<_nKinBins+1; i++){
    _nBinsLeft[i][_config.BARREL] = _nBinsLeftVariants[0];
    _nBinsLeft[i][_config.ENDCAP] = _nBinsLeftVariants[0];
    if (_varFit=="phoSigmaIEtaIEta"){
      _maxVarFit[i][_config.BARREL] = 0.022;
      _maxVarFit[i][_config.ENDCAP] = 0.067;
    }
    if (_varFit=="phoSCRChIsoCorr"){
      _maxVarFit[i][_config.BARREL] = _nPhoSCRChIsoRightLimits[0];
      _maxVarFit[i][_config.ENDCAP] = _nPhoSCRChIsoRightLimits[0];
    }
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
  PrintYieldsAndChi2();
  SaveYields();

}

void TTemplates::ComputeBackgroundOne(int ikin, bool noPrint, bool noPlot)
{
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<"||||========== ComputeBackgroundOne - "<<StrLabelKin(ikin)<<" starts"<<std::endl;
  }
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    SetThreeHists(ikin, ieta, noPrint);
    std::cout<<std::endl;
    std::cout<<"Histograms (hTrue, hFake, hLeak):"<<std::endl;
    PrintOneHistogramBinByBin(_hTrue, ikin, ieta);
    PrintOneHistogramBinByBin(_hFake, ikin, ieta);
    PrintOneHistogramBinByBin(_hLeak, ikin, ieta);
    std::cout<<std::endl;
    FitOne(ikin,ieta,noPrint,noPlot);  
    if (_noAdjustBinning) continue;
    if (_chi2ToNDF[ikin][ieta]<2 && _chi2ToNDF[ikin][ieta]>0.5);
    else AdjustFitBinning(ikin,ieta,noPrint,noPlot);
  }//end of loop over ieta
  ComputeYieldOneKinBin(ikin,noPrint);
  if (!noPrint){
    std::cout<<"||||========== ComputeBackgroundOne - "<<StrLabelKin(ikin)<<" ends"<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<std::endl;
  }
}

void TTemplates::AdjustFitBinning(int ikin, int ieta, bool noPrint, bool noPlot)
{
  if (! noPrint) std::cout<<"AdjustFitBinning starts:"<<std::endl;
  float chiBest=2000;//the best value of chi2/ndf
  int iLeftBest=0;//index of number of left bins giving the best fit
  int iLimitBest=0;//index of the right limit giving the best fit
  int nOfnRightLimits=1;//if _varFit="phoSigmaIEtaIEta" - right limit is strictly known
  bool fitDone=0;//flag determining whether parameters giving good chi2/ndf were found
  if (_varFit=="phoSCRChIsoCorr") nOfnRightLimits=_nOfnRightLimits;
  int iter=0;
  for (int iLimit=0; iLimit<nOfnRightLimits; iLimit++){
    //loop over different number of left bins
    if (fitDone) break;
    if (_varFit=="phoSCRChIsoCorr")
      _maxVarFit[ikin][ieta]=_nPhoSCRChIsoRightLimits[iLimit];
    for (int iLeft=0; iLeft<_nOfnBinsLeft; iLeft++){
      //loop over different right limits
      if (fitDone) break;
      _nBinsLeft[ikin][ieta]=_nBinsLeftVariants[iLeft];
      SetThreeHists(ikin, ieta, noPrint);
      FitOne(ikin,ieta,noPrint,noPlot);
      if (!noPlot){
        std::cout<<"in the loops over iLeft/iLimit"<<std::endl;
        std::cout<<"chi2/NDF="<<_chi2ToNDF[ikin][ieta]<<std::endl;
      }
      if (_chi2ToNDF[ikin][ieta]<2 && _chi2ToNDF[ikin][ieta]>0.5){
        fitDone=1;
        //if parameters giving good enough chi2/ndf were found - 
        //break this and outer loops
      }
      else{
        DeleteThreeHists(ikin,ieta);
        if (IsNewChi2ToNDFBetter(_chi2ToNDF[ikin][ieta], chiBest)){
          chiBest=_chi2ToNDF[ikin][ieta];
          iLeftBest=iLeft;
          iLimitBest=iLimit;
          //if no "good enough" parameters were found -
          //trying to find the best among bad parameters 
          //(which give best chi2/ndf)
        }
      }
      //delete histograms before creating them again
    }//end of loop over inbLimit
  }//end of loop over inbLeft
  if (fitDone) return;
  //if parameters giving good enough chi2/ndf found - continue
  //if not - do fit with the best among bad 
  _nBinsLeft[ikin][ieta]=_nBinsLeftVariants[iLeftBest];
  if (_varFit=="phoSCRChIsoCorr")
    _maxVarFit[ikin][ieta]=_nPhoSCRChIsoRightLimits[iLimitBest]; 
  if (!noPrint){
    std::cout<<"Adjust binning was done, the selected parameters are:"<<std::endl;
    std::cout<<"chiBest="<<chiBest<<", iLeftBins="<<iLeftBest<<", iLimitBets="<<iLimitBest<<std::endl;
    std::cout<<"_nBinsLeft="<<_nBinsLeft[ikin][ieta]<<", _maxVarFit="<<_maxVarFit[ikin][ieta]<<std::endl;
  }
  SetThreeHists(ikin, ieta, noPrint);
  FitOne(ikin,ieta,noPrint,noPlot);  
  if (! noPrint) std::cout<<"AdjustFitBinning ends:"<<std::endl;
}

bool TTemplates::IsNewChi2ToNDFBetter(float newChi2, float oldChi2)
{
  if (newChi2<=0 || oldChi2==1) return 0;
  if (newChi2==1) return 1;
  if (newChi2>1 && oldChi2>1){
    if (newChi2<oldChi2) return 1;
  }
  if (newChi2<1 && oldChi2<1){
    if (newChi2>oldChi2) return 1;
  }
  if (newChi2>1 && oldChi2<1){
    if (newChi2<1./oldChi2) return 1;
  }
  if (newChi2<1 && oldChi2>1){
    if (newChi2>1./oldChi2) return 1;
  }
  return 0;
}

void TTemplates::SetThreeHists(int kinBin, int etaBin, bool noPrint){
  _iter++;
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"SetThreeHists for: "<<StrLabelKin(kinBin)<<", "<<StrLabelEta(etaBin)<<", iteration="<<_iter<<std::endl;
  }
  TString name=TString("h")+StrLabelKin(kinBin)+StrLabelEta(etaBin); 
  name+=_iter;
  
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

  //_kinBinLims[kinBin] is upper limit
  //_kinBinLims[kinBin-1] is lower limit
  if ((_varKin=="phoEt" || _varKin=="phoSCEt") && (_kinBinLims[kinBin]>79 && _kinBinLims[kinBin-1]>59))
    cutKin="phoEt>60";

  name+="_True"; 
  cut = cutEta && cutKin;
  if (_isClosureTest==CLOSURE_TEST_1) 
      cut = cutEta && cutKin && SidebandVarNominalCut(); 
  if (!noPrint) {
    std::cout<<"cutEta="<<cutEta.GetTitle()<<std::endl;
    std::cout<<"cutKin="<<cutKin.GetTitle()<<std::endl;
    std::cout<<"cutWeight="<<_cutWeight.GetTitle()<<std::endl;
    std::cout<<"sign template cut=(cutEta && cutKin)*cutWeight="<<cut.GetTitle()<<std::endl;
  }

  _fOutForSave->cd();
  _hTrue[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
  _hTrue[kinBin][etaBin]->Sumw2();
  SetTrueTemplate(_hTrue[kinBin][etaBin],cut,noPrint);

  name.ReplaceAll("_True","_Fake");
  cut = cutEta && cutKin && cutSideband;
  if (_isClosureTest==CLOSURE_TEST_1) 
      cut = cutEta && cutKin && SidebandVarNominalCut();
  if (!noPrint) std::cout<<"Fake template cut="<<cut.GetTitle()<<std::endl;

  _hFake[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
  _hFake[kinBin][etaBin]->Sumw2();
  name.ReplaceAll("_Fake","_Leak");
  _hLeak[kinBin][etaBin] = new TH1D(name,name,nFitBins,fitBinLims);
  _hLeak[kinBin][etaBin]->Sumw2();
  TH1D* hLeakTemp = new TH1D("leakTemp","leakTemp",nFitBins,fitBinLims);
  hLeakTemp->Sumw2();
  SetFakeTemplate(_hFake[kinBin][etaBin], _hLeak[kinBin][etaBin], hLeakTemp, cut, noPrint);
  delete hLeakTemp;

  if (!noPrint)     std::cout<<std::endl;
}

void TTemplates::SetTrueTemplate(TH1D* hTrue, TCut cut, bool noPrint)
{
  TString name=hTrue->GetName();
  _treeSign->Draw(_varFit+TString(">>")+name,cut*_cutWeight,"goff");
  if (_isClosureTest==CLOSURE_TEST_1 || _isClosureTest==CLOSURE_TEST_2){
    TCut cutTrue = _photon.RangeGenTruePhoton();
    _treeData->Draw(_varFit+TString(">>")+name,(cut && cutTrue)*_cutWeight,"goff");
  }

  TH1F* histTemp = new TH1F("hist","hist",1,_treeData->GetMinimum("weight"),_treeData->GetMaximum("weight"));
  histTemp->Sumw2();
  if (_isClosureTest==CLOSURE_TEST_1 || _isClosureTest==CLOSURE_TEST_2) 
    _treeData->Draw("weight>>hist",cut,"goff");
  else
    _treeSign->Draw("weight>>hist",cut,"goff");
  std::cout<<std::endl;
  std::cout<<std::fixed;
  std::cout<<"True Template, mean weight =";
  std::cout<<std::setprecision(2)<<histTemp->GetMean()<<"+-";
  std::cout<<std::setprecision(2)<<histTemp->GetRMS()<<"; ";
  std::cout<<std::endl;
  std::cout<<std::endl;
  delete histTemp;

}

void TTemplates::SetFakeTemplate(TH1D* hFake, TH1D* hLeak, TH1D* hLeakTemp, TCut cut, bool noPrint)
{

  TString nameFake=hFake->GetName();
  _treeData->Draw(_varFit+TString(">>")+nameFake,cut*_cutWeight,"goff");
  if (!noPrint)
    std::cout<<"cut for hFake: "<<(cut*_cutWeight).GetTitle()<<std::endl;
  TCut cutFake = _photon.RangeGenFakePhoton();
  if (_isClosureTest==CLOSURE_TEST_1){
    _treeData->Draw(_varFit+TString(">>")+nameFake,(cut && cutFake)*_cutWeight,"goff");
  }

  TH1F* histTemp = new TH1F("hist","hist",100,_treeData->GetMinimum("weight")-1,_treeData->GetMaximum("weight")+1);
  histTemp->Sumw2();
  if (_isClosureTest==CLOSURE_TEST_1) 
    _treeData->Draw("weight>>hist",(cut && cutFake)*_cutWeight,"goff");
  else
    _treeData->Draw("weight>>hist",cut*_cutWeight,"goff");
  std::cout<<std::endl;
  std::cout<<"Backgr Template, mean weight =";
  std::cout<<std::fixed;
  std::cout<<std::setprecision(2)<<histTemp->GetMean()<<"+-";
  std::cout<<std::setprecision(2)<<histTemp->GetRMS()<<"; ";
  std::cout<<std::endl;
  std::cout<<std::endl;
  delete histTemp;  

  if (_isClosureTest==CLOSURE_TEST_1){
    return;
  }


  TString nameLeak=hLeak->GetName();
  _treeSign->Draw(_varFit+TString(">>")+nameLeak,cut*_cutWeight,"goff");

  TCut cutTrueGamma = _photon.RangeGenTruePhoton();
  if (_isClosureTest) {
    _treeData->Draw(_varFit+TString(">>")+nameLeak,(cut && cutTrueGamma)*_cutWeight,"goff");
    if (!noPrint)
      std::cout<<"cut for closure test for hLeak: "<<((cut && cutTrueGamma)*_cutWeight).GetTitle()<<std::endl;
  }

  if (!noPrint){
  std::cout<<std::setprecision(2);
  std::cout<<std::setw(6);
  std::cout<<std::fixed;
      std::cout<<"(fake bkgr)+leak="<<hFake->GetSumOfWeights()<<"="<<hFake->GetEntries()<<"*weight; (sign leak)="<<hLeak->GetSumOfWeights()<<"="<<hLeak->GetEntries()<<"*weight; "<<std::endl;
  }

  if (!noPrint) std::cout<<"_vecTreeBkg.size()="<<_vecTreeBkg.size()<<std::endl;
  if (!noPrint) std::cout<<"(true gamma bkg leaks):";

  TString nameLeakTemp=hLeakTemp->GetName();

//  TCut cutFakeGamma = _photon.RangeGenFakePhoton();

  for (unsigned int is=0; is<_vecTreeBkg.size(); is++){
    if (_isClosureTest) continue;
    if (_vecBkgNames[is]=="Wjets_to_lnu" || _vecBkgNames[is]=="DYjets_to_ll" || _vecBkgNames[is]=="ttbarjets")
      continue;
    _vecTreeBkg[is]->Draw(_varFit+TString(">>")+nameLeakTemp,(cut && cutTrueGamma)*_cutWeight,"goff");
    hLeak->Add(hLeakTemp);
    if (!noPrint) std::cout<<" "<<_vecBkgNames[is]<<": "<<hLeakTemp->GetSumOfWeights();
    //compute GetSum of Weights by hand
    float histArea=0;
    for (int ib=1; ib<=hLeakTemp->GetNbinsX(); ib++){
      histArea+=hLeakTemp->GetBinContent(ib);
    }
    if (!noPrint) std::cout<<"="<<histArea<<"=";
    if (!noPrint) std::cout<<hLeakTemp->GetEntries()<<"*weight; ";
  }
  if (!noPrint) std::cout<<std::endl;
  hFake->Add(hLeak,-1);
  if (!noPrint) {
    std::cout<<"leak = (sign leak)+(true gamma bkg leaks) = ";
    std::cout<<hLeak->GetSumOfWeights()<<std::endl;
    std::cout<<"fake Fake = ";
    std::cout<<hFake->GetSumOfWeights()<<std::endl;
  }

}

void TTemplates::DeleteThreeHists(int kinBin, int etaBin){

  delete _hTrue[kinBin][etaBin];

  delete _hFake[kinBin][etaBin]; 

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
  TCut cut="1";
  if (_varSideband=="phoSigmaIEtaIEta") 
    cut=_photon.SidebandSigmaIEtaIEta();
  else if (_varSideband=="phoSCRChIsoCorr")
    cut=_photon.SidebandPhoSCRChIsoCorr();
  return cut;
}

TCut TTemplates::SidebandVarNominalCut(){
  TCut cut="1";
  if (_varSideband=="phoSigmaIEtaIEta") 
    cut=_photon.RangeSigmaIEtaIEta(_year,_phoWP);
  else if (_varSideband=="phoSCRChIsoCorr")
    cut=_photon.RangePhoSCRChIsoCorr();
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
//  TH1D* hTrue = 0;
//  TH1D* hFake = 0;



  nBins = _hFake[kinBin][etaBin]->GetNbinsX();
  varMin = _hFake[kinBin][etaBin]->GetBinLowEdge(1);
  varMax = _hFake[kinBin][etaBin]->GetBinLowEdge(nBins)+_hFake[kinBin][etaBin]->GetBinWidth(nBins);

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


  TCut sidebandCut = SidebandCut();
  TCut etaCut = CutEtaBin(etaBin);
  TCut kinCut = CutKinBin(kinBin);
  TCut sidebandVarNonimalCut = SidebandVarNominalCut();
  if (!noPrint){
    std::cout<<"etaCut="<<etaCut.GetTitle()<<std::endl;
    std::cout<<"kinCut="<<kinCut.GetTitle()<<std::endl;
    std::cout<<"sidebandVarNominalCut="<<sidebandVarNonimalCut.GetTitle()<<std::endl;
  }
  TCut cut = kinCut && etaCut && sidebandVarNonimalCut;
	
  TString hName="hDataToFit";
  hName+=StrLabelKin(kinBin);
  hName+=StrLabelEta(etaBin);
  hName+=_iter;
  _hData[kinBin][etaBin] = (TH1D*)_hFake[kinBin][etaBin]->Clone(hName);
  _hData[kinBin][etaBin]->SetTitle(hName);
  TString varDraw=_varFit;
  varDraw+=">>";
  varDraw+=hName;

  _treeData->Draw(varDraw,cut*_cutWeight,"goff");

  TH1F* histTemp = new TH1F("hist","hist",1,_treeData->GetMinimum("weight"),_treeData->GetMaximum("weight"));
  histTemp->Sumw2();
  _treeData->Draw("weight>>hist",cut,"goff");
  std::cout<<std::endl;
  std::cout<<std::fixed;
  std::cout<<"Data Template, mean weight =";
  std::cout<<std::setprecision(2)<<histTemp->GetMean()<<"+-";
  std::cout<<std::setprecision(2)<<histTemp->GetRMS()<<"; ";
  std::cout<<std::endl;
  std::cout<<std::endl;
  delete histTemp; 

  if (!noPrint){
    std::cout<<"data hist TH1D* hist: "<<std::endl;
    _hData[kinBin][etaBin]->Print();
    std::cout<<"true template hist TH1D* hist: "<<std::endl;
    _hTrue[kinBin][etaBin]->Print();
    std::cout<<"fake template hist TH1D* hist: "<<std::endl;
    _hFake[kinBin][etaBin]->Print();
    std::cout<<"leak (true to sideband) hist TH1D* hist: "<<std::endl;
    _hLeak[kinBin][etaBin]->Print();
  }
  //RooDataSet dataSet("dataSet","dataSet",_treeData,argSetData,cut);

  //create total pdf
  int nMax = _treeData->GetEntries(cut);
  int nTrueStart = 0.5*nMax;
  int nFakeStart = 0.5*nMax;

  RooRealVar rooVarFit(_varFit,_labelVarFit, varMin, varMax);
  if (nMax<=0){
    if (!noPrint) std::cout<<"ATTENTION: No data events for bin "<<kinCut<<", "<<etaCut<<std::endl;
    _nFakeFromFitVal[kinBin][etaBin]=0;
    _nFakeFromFitErr[kinBin][etaBin]=0;
    _nTrueFromFitVal[kinBin][etaBin]=0;
    _nTrueFromFitErr[kinBin][etaBin]=0;
    _plotter[kinBin][etaBin] = new RooPlot(rooVarFit,varMin,varMax,nBins);
    return;
  }
  RooRealVar rooVarSideband(_varSideband,_varSideband,minVarSideband,maxVarSideband);
  RooRealVar rooPhoEta("phoEta","phoEta",minPhoEta,maxPhoEta);
  RooRealVar rooKin(_varKin,_varKin,minKin,maxKin);
  RooRealVar rooWeight("weight","weight",minWeight,maxWeight);
  RooArgList argList;
  argList.add(rooVarFit);
  RooArgSet argSet;
  argSet.add(rooVarFit);
  std::cout<<"Historgams (hTrue, hFake, hLeak):"<<std::endl;
  PrintOneHistogramBinByBin(_hTrue, kinBin, etaBin);
  PrintOneHistogramBinByBin(_hFake, kinBin, etaBin);
  PrintOneHistogramBinByBin(_hLeak, kinBin, etaBin);
  RooDataHist dataHist("hist", "data set converted to hist", argList, _hData[kinBin][etaBin]);
  //create Fake and True PDFs
  RooDataHist trueDataHist("trueDataHist","true RooDataHist", argList, _hTrue[kinBin][etaBin]);
  RooHistPdf truePdf("truePdf",_varFit+TString(" of true"), argSet, trueDataHist);
  RooDataHist fakeDataHist("fakeDataHist", "fake RooDataHist", argList, _hFake[kinBin][etaBin]);
  RooHistPdf fakePdf("fakePdf",_varFit+TString(" of fake"), argSet, fakeDataHist);

  //load data
  RooArgSet argSetData(rooVarFit,rooVarSideband,rooPhoEta,rooKin,rooWeight);
  RooRealVar rooNTrue("nTrue","n True",nTrueStart,0,nMax);
  RooExtendPdf eTruePdf("eTrue","extended True",truePdf,rooNTrue);
  RooRealVar rooNFake("nFake","n Fake",nFakeStart,0,nMax);
  RooExtendPdf eFakePdf("eFake","extended Fake",fakePdf,rooNFake);
  RooAddPdf fullPdf("fitModel","fit model",RooArgList(eTruePdf,eFakePdf));
  //fit
  fullPdf.fitTo(dataHist,SumW2Error(kFALSE),Extended(kTRUE));

  //calc chi squared
  RooChi2Var chi2("chi2","chi2",fullPdf,dataHist);
  _chi2ToNDF[kinBin][etaBin] = chi2.getVal()/(nBins-2);

  //get nFake values from fit
  _nFakeFromFitVal[kinBin][etaBin]=rooNFake.getVal();
  _nFakeFromFitErr[kinBin][etaBin]=rooNFake.getError();
  _nTrueFromFitVal[kinBin][etaBin]=rooNTrue.getVal();
  _nTrueFromFitErr[kinBin][etaBin]=rooNTrue.getError();
  
 
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"nFake="<<rooNFake.getVal()<<"+-"<<rooNFake.getError()<<", nTrue="<<rooNTrue.getVal()<<"+-"<<rooNTrue.getError()<<", chi2="<<chi2.getVal()/(nBins-2)<<std::endl;
    std::cout<<std::endl;
  }


  //for plotting
  _plotter[kinBin][etaBin] = new RooPlot(rooVarFit,varMin,varMax,nBins); 
  dataHist.plotOn(_plotter[kinBin][etaBin]);
  fullPdf.plotOn(_plotter[kinBin][etaBin],Name("sum"),LineColor(kRed));
  fullPdf.plotOn(_plotter[kinBin][etaBin],Components("truePdf"), Name("true"),
  LineColor(kGreen),LineStyle(9));
  fullPdf.plotOn(_plotter[kinBin][etaBin], Components("fakePdf"),Name("fake"),LineColor(kBlue),LineStyle(9));
  fullPdf.paramOn(_plotter[kinBin][etaBin]);

}

void TTemplates::ComputeYieldOneKinBin(int ikin, bool noPrint)
{

  if (!noPrint)
    std::cout<<"Compute fake background yield: "<<std::endl;

  bool isTrue=0;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    if (_nFakeFromFitVal[ikin][ieta]==0){
      _nFakeYieldsVal[ikin][ieta]=_nFakeFromFitVal[ikin][ieta];
      _nFakeYieldsErr[ikin][ieta]=_nFakeFromFitErr[ikin][ieta];
    }
    else ComputeYieldOne(_hFake[ikin][ieta], _nFakeFromFitVal[ikin][ieta], _nFakeFromFitErr[ikin][ieta], _nFakeYieldsVal[ikin][ieta], _nFakeYieldsErr[ikin][ieta], ieta, ikin, isTrue, noPrint);
  }
  _nFakeYieldsVal[ikin][_config.COMMON]=_nFakeYieldsVal[ikin][_config.BARREL]+_nFakeYieldsVal[ikin][_config.ENDCAP];
  _nFakeYieldsErr[ikin][_config.COMMON]=sqrt(_nFakeYieldsErr[ikin][_config.BARREL]*_nFakeYieldsErr[ikin][_config.BARREL]+_nFakeYieldsErr[ikin][_config.ENDCAP]*_nFakeYieldsErr[ikin][_config.ENDCAP]); 

  if (!noPrint)
    std::cout<<"Compute signal + true bkg yield: "<<std::endl;

  isTrue=1;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    if (_nTrueFromFitVal[ikin][ieta]==0){
      _nTrueYieldsVal[ikin][ieta]=_nTrueFromFitVal[ikin][ieta];
      _nTrueYieldsErr[ikin][ieta]=_nTrueFromFitErr[ikin][ieta];
    }
    else ComputeYieldOne(_hTrue[ikin][ieta], _nTrueFromFitVal[ikin][ieta], _nTrueFromFitErr[ikin][ieta], _nTrueYieldsVal[ikin][ieta], _nTrueYieldsErr[ikin][ieta], ieta, ikin, isTrue, noPrint);
  }
  _nTrueYieldsVal[ikin][_config.COMMON]=_nTrueYieldsVal[ikin][_config.BARREL]+_nTrueYieldsVal[ikin][_config.ENDCAP];
  _nTrueYieldsErr[ikin][_config.COMMON]=sqrt(_nTrueYieldsErr[ikin][_config.BARREL]*_nTrueYieldsErr[ikin][_config.BARREL]+_nTrueYieldsErr[ikin][_config.ENDCAP]*_nTrueYieldsErr[ikin][_config.ENDCAP]); 

}

void TTemplates::ComputeYieldOne(TH1D* hTemplate, double nValFromFit, double nErrFromFit, double& nYieldVal, double& nYieldErr, int ieta, int ikin, bool isTrue, bool noPrint)
{

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"ComputeYieldOne: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<std::endl;
  }

  int nHistBins = hTemplate->GetNbinsX();
  nYieldVal=0;
  double Scut = 0;
  double Stot = 0;
  int nCut = 0;
  
  int nBLeft=0;
  nBLeft=_nBinsLeft[ikin][ieta];
  for (int i=1; i<=nBLeft; i++){
    Scut+=hTemplate->GetBinContent(i);
    Stot+=hTemplate->GetBinContent(i);
  }
  if (!noPrint){
    std::cout<<"nBLeft = "<<nBLeft<<std::endl;
    std::cout<<"fit var cut value = "<<hTemplate->GetBinLowEdge(nBLeft)+hTemplate->GetBinWidth(nBLeft)<<std::endl;
  }
  for (int i=nBLeft+1; i<=nHistBins; i++){
    Stot+=hTemplate->GetBinContent(i);
  }

  if (Stot==0){
    nYieldVal=0;
    nYieldErr=0;
    return;
  }

  float eff;

  if (_varFit=="phoSCRChIsoCorr" && !_noPhoPFChIsoCut) eff=EffPhoChIsoCorr(ikin, ieta, isTrue);
  else eff=1;
  if (!noPrint) std::cout<<"EffPhoChIsoCorr="<<eff<<std::endl;

  nYieldVal=eff*nValFromFit*Scut/Stot;
  //compute error
  double firstTerm = eff*eff*nErrFromFit*nErrFromFit*Scut*Scut/(Stot*Stot);
  double secondTerm = 0;
  for (int j=1; j<=nBLeft; j++){
    secondTerm += eff*eff*hTemplate->GetBinError(j)*hTemplate->GetBinError(j)* nValFromFit*nValFromFit*(Stot-Scut)*(Stot-Scut)/(Stot*Stot*Stot*Stot);
  }
  double thirdTerm = 0;
  for (int j=nBLeft+1; j<=nHistBins; j++){
    thirdTerm += eff*eff*hTemplate->GetBinError(j)*hTemplate->GetBinError(j)* nValFromFit*nValFromFit*Scut*Scut/(Stot*Stot*Stot*Stot);
  }  

  nYieldErr = sqrt(firstTerm+secondTerm+thirdTerm);
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"Compute Yield One: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<"nBLeft="<<nBLeft<<", "<<" cut Value = "<<hTemplate->GetBinLowEdge(nBLeft)+hTemplate->GetBinWidth(nBLeft)<<std::endl;
    std::cout<<"nVal(from fit)="<<nValFromFit<<", Scut="<<Scut<<", Stot="<<Stot<<std::endl;
    std::cout<<"Error computation: "<<std::endl;
    std::cout<<"1st term = "<<sqrt(firstTerm)<<", 2nd term = "<<sqrt(secondTerm)<<", 3rd term = "<<sqrt(thirdTerm)<<", err = "<<nYieldErr<<std::endl;
    std::cout<<std::endl;
  }

}

float TTemplates::EffPhoChIsoCorr(int ikin, int ieta, bool isTrue)
{
  TCut cutEta = CutEtaBin(ieta);
  TCut cutKin = CutKinBin(ieta);
  TCut cutPhoSigmaIEtaIEta = _photon.RangeSigmaIEtaIEta(_year,_phoWP);
  TCut cutPhoSCRChIso = _photon.RangePhoSCRChIsoCorr();
  TCut cutPhoChIso = _photon.RangeOneIsolation(_year,_phoWP,_photon.ISO_CHorTRK);
  TCut cutDen = cutEta && cutKin && cutPhoSigmaIEtaIEta && cutPhoSCRChIso;
  TCut cutNom = cutDen && cutPhoChIso;
  if (isTrue){
    float den = _treeSign->GetEntries(cutDen);
    float nom = _treeSign->GetEntries(cutNom);
    return nom/den; 
  }
  else{
    float den = _treeData->GetEntries(cutDen);
    float nom = _treeData->GetEntries(cutNom);
    for (int is=0; is<_vecTreeBkg.size(); is++){
      if (_vecBkgNames[is]=="Wjets_to_lnu" || _vecBkgNames[is]=="DYjets_to_ll" || _vecBkgNames[is]=="ttbarjets")
        continue;
      den-=_vecTreeBkg[is]->GetEntries(cutDen && _photon.RangeGenTruePhoton());
      nom-=_vecTreeBkg[is]->GetEntries(cutDen && _photon.RangeGenTruePhoton());
    }
    return nom/den; 
  }
  return 1;
}

void TTemplates::PrintYieldsAndChi2()
{

  std::cout<<"Fake and True values from fit:"<<std::endl;
  std::cout<<"  bin                            Fake-B            True-B           Fake-E            True-E"<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
     std::cout<<std::setw(6)<<std::setprecision(0)<<std::fixed;
//      std::cout<<std::setw(6)<<std::setprecision(0)<<_nFakeFromFitVal[ikin][ieta]<<"+-";
//      std::cout<<std::setw(5)<<std::setprecision(0)<<_nFakeFromFitErr[ikin][ieta]<<"; ";
//      std::cout<<std::setw(6)<<std::setprecision(0)<<_nTrueFromFitVal[ikin][ieta]<<"+-";
//      std::cout<<std::setw(5)<<std::setprecision(0)<<_nTrueFromFitErr[ikin][ieta]<<"; ";
      std::cout<<_nFakeFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nFakeFromFitErr[ikin][ieta]<<"; ";
      std::cout<<_nTrueFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nTrueFromFitErr[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  std::cout<<"Histograms:"<<std::endl;
  PrintHistogramsBinByBin(_hTrue);
  PrintHistogramsBinByBin(_hFake);
  PrintHistogramsBinByBin(_hLeak);
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

  std::cout<<"Fake yields:"<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin);
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": "<<_nFakeYieldsVal[ikin][ieta]<<"+-"<<_nFakeYieldsErr[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"True yields:"<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin);
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": "<<_nTrueYieldsVal[ikin][ieta]<<"+-"<<_nTrueYieldsErr[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;
  }

  if (!_isClosureTest) return;


  TCut cutTrueGamma=_photon.RangeGenTruePhoton();
  TCut cutFakeGamma=_photon.RangeGenFakePhoton();
  std::cout<<std::endl;
  std::cout<<"--- Closure test only ---"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"Fake values from fit vs fake gamma Wjets MC truth vs _hFake->SumOfWeights()::"<<std::endl;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    std::cout<<StrLabelEta(ieta)<<": "<<std::endl;
    TCut cutVarFit;
    TString strCutVarFit=_varFit;
    strCutVarFit+="<";
    strCutVarFit+=ValueCutNominalVarFit(ieta);
    cutVarFit=strCutVarFit;
    //for (int ikin=0; ikin<1; ikin++){
    for (int ikin=0; ikin<_nKinBins+1; ikin++){
      std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
      std::cout<<_nFakeFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nFakeFromFitErr[ikin][ieta]<<"; ";

      TH1F* histTemp = new TH1F("hist","hist",1,_treeData->GetMinimum("event"),_treeData->GetMaximum("event"));
      histTemp->Sumw2();
      _treeData->Draw("event>>hist",(CutKinBin(ikin) && CutEtaBin(ieta)&& cutFakeGamma && SidebandVarNominalCut())*_cutWeight,"goff");
      std::cout<<histTemp->GetBinContent(1)<<"+-";
      std::cout<<histTemp->GetBinError(1)<<"; ";
      delete histTemp;

      float yh=_hFake[ikin][ieta]->GetSumOfWeights();
      std::cout<<yh<<"+-";
      std::cout<<sqrt(yh)<<"; ";
      std::cout<<std::endl;
    }
  }
  std::cout<<"True values from fit vs true gamma Wjets MC truth vs _hTrue->SumOfWeights():"<<std::endl;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    std::cout<<StrLabelEta(ieta)<<": "<<std::endl;
    TCut cutVarFit;
    TString strCutVarFit=_varFit;
    strCutVarFit+="<";
    strCutVarFit+=ValueCutNominalVarFit(ieta);
    cutVarFit=strCutVarFit;
    //for (int ikin=0; ikin<1; ikin++){
    for (int ikin=0; ikin<_nKinBins+1; ikin++){
      std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
      std::cout<<_nTrueFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nTrueFromFitErr[ikin][ieta]<<"; ";
      int y=_treeData->GetEntries((CutKinBin(ikin) && CutEtaBin(ieta) && cutTrueGamma && SidebandVarNominalCut())*_cutWeight);
      std::cout<<y<<"+-";
      std::cout<<sqrt(y)<<"; ";
      float yh=_hTrue[ikin][ieta]->GetSumOfWeights();
      std::cout<<yh<<"+-";
      std::cout<<sqrt(yh)<<"; ";
      std::cout<<std::endl;
    }
  }
  std::cout<<std::endl;


}

void TTemplates::PrintHistogramsBinByBin(TH1D* hist[_nBinsMax][3]){
  std::cout<<"TH1D* hists "<<hist[0][0]->GetTitle()<<": "<<std::endl;
  std::cout<<std::setprecision(0)<<std::fixed;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    for (int ikin=0; ikin<_nKinBins+1; ikin++)
      PrintOneHistogramBinByBin(hist, ikin, ieta);
  }
}

void TTemplates::PrintOneHistogramBinByBin(TH1D* hist[_nBinsMax][3], int ikin, int ieta){
  std::cout<<StrLabelEta(ieta)<<StrLabelKin(ikin)<<": ";
  float sum=0;
  std::cout<<std::setprecision(0)<<std::fixed;
  for (int ifit=1; ifit<=hist[ikin][ieta]->GetNbinsX(); ifit++){
    std::cout<<std::setw(6)<<hist[ikin][ieta]->GetBinContent(ifit)<<"; ";
    sum+=hist[ikin][ieta]->GetBinContent(ifit);
  }
  std::cout<<"sum="<<sum<<std::endl;
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

  TString strBin = StrLabelKin(kinBin)+StrLabelEta(etaBin);
  TString strChi = "#chi^{2}/ndf=";
  float chiFloat=(100*_chi2ToNDF[kinBin][etaBin]);
  int chiInt=chiFloat;    
  strChi+=(chiInt/100);
  strChi+=".";
  strChi+=(chiInt%100);
  TLatex* textChi2; 
  textChi2 =new TLatex(0.75,0.65,strChi);
  textChi2->SetNDC();

  TText* textBin; 
  textBin =new TText(0.55,0.75,strBin);
  textBin->SetNDC();


  TString strRatio="hRatio";
  strRatio+=kinBin;
  strRatio+=etaBin;
  TString strSum="hSum";
  strSum+=kinBin;
  strSum+=etaBin;
  _hRatio[kinBin][etaBin]=(TH1D*)_hData[kinBin][etaBin]->Clone(strRatio);
  _hRatio[kinBin][etaBin]->Sumw2();
  _hSumm[kinBin][etaBin]=(TH1D*)_hData[kinBin][etaBin]->Clone(strSum);
  
  float areaHistTrue=0;
  float areaHistFake=0;
  for (int ib=1; ib<=_hTrue[kinBin][etaBin]->GetNbinsX(); ib++){
    areaHistTrue+=_hTrue[kinBin][etaBin]->GetBinContent(ib);
    areaHistFake+=_hFake[kinBin][etaBin]->GetBinContent(ib);
  }

  for (int ib=1; ib<=_hTrue[kinBin][etaBin]->GetNbinsX(); ib++){

    float contS=0;
    float err1S=0;
    float err2S=0;
    float contB=0;
    float err1B=0;
    float err2B=0;

    if (areaHistTrue==0);
    else{
      contS = _hTrue[kinBin][etaBin]->GetBinContent(ib)*_nTrueFromFitVal[kinBin][etaBin]/areaHistTrue;
      err1S = (_hTrue[kinBin][etaBin]->GetBinError(ib)*_nTrueFromFitVal[kinBin][etaBin])/areaHistTrue;
      err2S = (_hTrue[kinBin][etaBin]->GetBinContent(ib)*_nTrueFromFitErr[kinBin][etaBin])/areaHistTrue;
    }
    if (areaHistFake==0);
    else {
      contB = _hFake[kinBin][etaBin]->GetBinContent(ib)*_nFakeFromFitVal[kinBin][etaBin]/areaHistFake;
      err1B = (_hFake[kinBin][etaBin]->GetBinError(ib)*_nFakeFromFitVal[kinBin][etaBin])/areaHistFake;
      err2B = (_hFake[kinBin][etaBin]->GetBinContent(ib)*_nFakeFromFitErr[kinBin][etaBin])/areaHistFake;
    }
//    _hTrue[kinBin][etaBin]->SetBinContent(ib,contS);
//    _hTrue[kinBin][etaBin]->SetBinError(ib,sqrt(err1S*err1S+err2S*err2S));

//    _hFake[kinBin][etaBin]->SetBinContent(ib,contB);
//    _hFake[kinBin][etaBin]->SetBinError(ib,sqrt(err1B*err1B+err2B*err2B));

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

    _hSumm[kinBin][etaBin]->SetLineColor(7);
    _hSumm[kinBin][etaBin]->SetLineWidth(2);
    _hSumm[kinBin][etaBin]->SetLineStyle(3);
    _hSumm[kinBin][etaBin]->Draw("HIST same");  

    double valCut = ValueCutNominalVarFit(etaBin);
    TLine* cutLine = new TLine(valCut,0,valCut,_hSumm[kinBin][etaBin]->GetMaximum());
    cutLine->SetLineWidth(3);
    cutLine->Draw("same");

    textBin->Draw("same");
    textChi2->Draw("same");

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

void TTemplates::SaveYields()
{
  _fOutForSave->cd();

  for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){
    //write fake yields computed from fit
    //which is jets to gamma background
    TString strTot=_config.GetYieldsDDTemplateFakeName(_config.TOTAL,ieta);
    TH1F hTotFakeYield(strTot,strTot,1,_kinBinLims[0],_kinBinLims[_nKinBins]);
    TString str1D=_config.GetYieldsDDTemplateFakeName(_config.ONEDI,ieta);
    TH1F h1DFakeYield(str1D,str1D,_nKinBins,_kinBinLims);
    hTotFakeYield.SetBinContent(1,_nFakeYieldsVal[0][ieta]);
    hTotFakeYield.SetBinError(1,_nFakeYieldsErr[0][ieta]);
    for (int i=1; i<_nKinBins+1; i++){  
      h1DFakeYield.SetBinContent(i,_nFakeYieldsVal[i][ieta]);
      h1DFakeYield.SetBinError(i,_nFakeYieldsErr[i][ieta]);
    }
    hTotFakeYield.Write(strTot); 
    h1DFakeYield.Write(str1D); 

    //write true yields computed from fit
    //which is true gamma background plus signal
    strTot=_config.GetYieldsDDTemplateTrueName(_config.TOTAL,ieta);
    TH1F hTotTrueYield(strTot,strTot,1,_kinBinLims[0],_kinBinLims[_nKinBins]);
    str1D=_config.GetYieldsDDTemplateTrueName(_config.ONEDI,ieta);
    TH1F h1DTrueYield(str1D,str1D,_nKinBins,_kinBinLims);
    hTotTrueYield.SetBinContent(1,_nTrueYieldsVal[0][ieta]);
    hTotTrueYield.SetBinError(1,_nTrueYieldsErr[0][ieta]);
    for (int i=1; i<_nKinBins+1; i++){  
      h1DTrueYield.SetBinContent(i,_nTrueYieldsVal[i][ieta]);
      h1DTrueYield.SetBinError(i,_nTrueYieldsErr[i][ieta]);
    }
    hTotTrueYield.Write(strTot); 
    h1DTrueYield.Write(str1D); 
    if (ieta==_config.COMMON) continue;
    for (int i=0; i<_nKinBins+1; i++){  
      _hTrue[i][ieta]->Write(TString("templateTrue")+StrLabelKin(i)+StrLabelEta(ieta));
      _hFake[i][ieta]->Write(TString("templateFake")+StrLabelKin(i)+StrLabelEta(ieta));
      _hData[i][ieta]->Write(TString("dataToFit")+StrLabelKin(i)+StrLabelEta(ieta));
      _plotter[i][ieta]->Write(TString("plotter")+StrLabelKin(i)+StrLabelEta(ieta));
    }//end of loop over i
  }//end of loop over ieta
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
  TCut cut;
  if (etaBin==_config.BARREL) 
    cut=_photon.RangeBarrel();
  else if (etaBin==_config.ENDCAP) 
    cut=_photon.RangeEndcap();
  else if (etaBin==_config.COMMON) 
    cut=(_photon.RangeEndcap()&&_photon.RangeBarrel());
  else
    cut="0";
  return cut;
}

double TTemplates::ValueCutNominalVarFit(int etaBin){
  if (_varFit=="phoSigmaIEtaIEta"){
    if (etaBin==_config.BARREL) return _photon.GetPhoSigmaIEtaIEtaCutB(_year,_phoWP);
    else if (etaBin==_config.ENDCAP) return _photon.GetPhoSigmaIEtaIEtaCutE(_year,_phoWP);
    else return 1;
  }
  if (_varFit=="phoSCRChIsoCorr"){
    return _photon.GetPhoSCRChIsoCorrCut();
  }
  return 100000;
}
