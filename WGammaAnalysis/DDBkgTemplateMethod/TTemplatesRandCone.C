#include "TTemplatesRandCone.h"
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

TTemplatesRandCone::TTemplatesRandCone(int channel, int vgamma, int phoWP, TString varKin, int nKinBins, float* kinBinLims, bool isMetCutOptimization)
{

  _cutWeight="weight";

  _channel=channel;
  _vgamma=vgamma;
  _phoWP=phoWP;
  _INPUT = new TAllInputSamples(_channel,_vgamma,"../Configuration/config.txt");

  _varKin=varKin;
  _nKinBins=nKinBins;
  _kinBinLims = new float[nKinBins+1];
  for (int i=0; i<=nKinBins; i++){
    _kinBinLims[i]=kinBinLims[i];
  }
  _fOutForSave = new TFile(_config.GetDDTemplateFileName(_channel,_vgamma,_varKin),"recreate");
  _isMetCutOptimization=isMetCutOptimization;
  if (_isMetCutOptimization) _selectionStage=_config.PRELIMINARY_FOR_MET_CUT;
  else _selectionStage=_config.PRELIMINARY_FOR_TEMPLATE_METHOD;

  _fData = new TFile(_config.GetSelectedName(_selectionStage,_channel,_vgamma,_config.UNBLIND,_config.DATA));
  _treeData = (TTree *)_fData->Get("selectedEvents");

  //set # of bins histograms for fitting 
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      _nFitBins[ikin][ieta] = 20;
      _maxVarFit[ikin][ieta] = 20;
      if (varKin=="phoEt" && kinBinLims[ikin]>60){
        _nFitBins[ikin][ieta] = 10;
        _maxVarFit[ikin][ieta] = 16;
      }
    }
  }

}

TTemplatesRandCone::~TTemplatesRandCone()
{
}

void TTemplatesRandCone::ComputeBackground(bool noPrint, bool noPlot)
{
  for (int ikin=0; ikin<=_nKinBins; ikin++){
    ComputeBackgroundOne(ikin,noPrint,noPlot);
  }
  if (!noPlot) PlotTemplates();
  PrintYieldsAndChi2();
  SaveYields();
}

void TTemplatesRandCone::ComputeBackgroundOne(int ikin, bool noPrint, bool noPlot)
{
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<"||||========== ComputeBackgroundOne - "<<StrLabelKin(ikin)<<" starts"<<std::endl;
  }
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    SetHists(ikin, ieta, noPrint);
  }//end of loop over ieta
  ComputeYieldOneKinBin(ikin,noPrint);
  if (!noPrint){
    std::cout<<"||||========== ComputeBackgroundOne - "<<StrLabelKin(ikin)<<" ends"<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<std::endl;
  }
}

void TTemplatesRandCone::SetHists(int ikin, int ieta, bool noPrint){
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"SetHists for: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
  }
  TString name=TString("h")+StrLabelKin(ikin)+StrLabelEta(ieta); 
  
  TCut cutSideband=SidebandCut();
  TCut cutSignal=SidebandVarNominalCut();

  TCut cutEta = CutEtaBin(ieta);
  TCut cutKin = Cutikin(ikin);
  TCut cut;

  int nFitBins=_nFitBins[ikin][ieta];
  double max=_maxVarFit[_nBinsMax][3];
  double unit = max/nFitBins;
  double min=0-unit;
  nFitBins++;
  // one bin [-unit,0] is reserved for events for which Iso==0 

  double fitBinLims[nFitBins+1];

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"bin lims:";
  }
  for (int i=0; i<=nFitBins; i++){
    fitBinLims[i]=min+i*unit;
    if (!noPrint) std::cout<<fitBinLims[i]<<", ";
  }
  std::cout<<std::endl;

  //_kinBinLims[ikin] is upper limit
  //_kinBinLims[ikin-1] is lower limit
  if (_varKin=="phoEt" && (_kinBinLims[ikin]>79 && _kinBinLims[ikin-1]>59))
    cutKin="phoEt>60";

  name+="_True"; 
  cut = cutEta && cutKin && cutSignal;;
  if (!noPrint) {
    std::cout<<"cutEta="<<cutEta.GetTitle()<<std::endl;
    std::cout<<"cutKin="<<cutKin.GetTitle()<<std::endl;
    std::cout<<"cutSignal="<<cutSignal.GetTitle()<<std::endl;
    std::cout<<"cutWeight="<<_cutWeight.GetTitle()<<std::endl;
    std::cout<<"sign template cut=(cutEta && cutKin && cutSignal)*cutWeight="<<cut.GetTitle()<<std::endl;
  }

  _fOutForSave->cd();
  _hTrue[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hTrue[ikin][ieta]->Sumw2();
  SetTrueTemplate(_hTrue[ikin][ieta],cut,noPrint);

  name.ReplaceAll("_True","_Fake");
  cut = cutEta && cutKin && cutSideband;
  if (!noPrint) std::cout<<"Fake template cut="<<cut.GetTitle()<<std::endl;

  _hFake[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hFake[ikin][ieta]->Sumw2();
  name.ReplaceAll("_Fake","_Data");
  _hData[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hData[ikin][ieta]->Sumw2();

  SetFakeTemplate(_hFake[ikin][ieta], _hLeak[ikin][ieta], hLeakTemp, cut, noPrint);

  _hFakeReference[ikin][ieta] = (TH1D*)_hFake[ikin][ieta]->Clone(_hFake[ikin][ieta]->GetName()+TString("_Ref"));
  _hFakeReference[ikin][ieta]->SetTitle(_hFakeReference[ikin][ieta]->GetName());
  _hTrueReference[ikin][ieta] = (TH1D*)_hTrue[ikin][ieta]->Clone(_hTrue[ikin][ieta]->GetName()+TString("_Ref"));
  _hTrueReference[ikin][ieta]->SetTitle(_hTrueReference[ikin][ieta]->GetName());

  if (!noPrint)     std::cout<<std::endl;
}

void TTemplatesRandCone::SetTrueTemplate(TH1D* hTrue, TCut cut, bool noPrint)
{
  TString name=hTrue->GetName();
  _treeData->Draw(("phoRandConeChIsoCorr>>")+name,cut*_cutWeight,"goff");

  TH1F* histTemp = new TH1F("hist","hist",1,_treeData->GetMinimum("weight"),_treeData->GetMaximum("weight"));
  histTemp->Sumw2();
  _treeData->Draw("weight>>hist",cut,"goff");
  std::cout<<std::endl;
  std::cout<<std::fixed;
  std::cout<<"True Template, mean weight =";
  std::cout<<std::setprecision(2)<<histTemp->GetMean()<<"+-";
  std::cout<<std::setprecision(2)<<histTemp->GetRMS()<<"; ";
  std::cout<<std::endl;
  std::cout<<std::endl;
  delete histTemp;

}

void TTemplatesRandCone::SetFakeTemplate(TH1D* hFake, TCut cut, bool noPrint)
{
  TString name=hTrue->GetName();
  _treeData->Draw(("phoSCRChIsoCorr>>")+name,cut*_cutWeight,"goff");

  TH1F* histTemp = new TH1F("hist","hist",1,_treeData->GetMinimum("weight"),_treeData->GetMaximum("weight"));
  histTemp->Sumw2();
  _treeData->Draw("weight>>hist",cut,"goff");
  std::cout<<std::endl;
  std::cout<<std::fixed;
  std::cout<<"True Template, mean weight =";
  std::cout<<std::setprecision(2)<<histTemp->GetMean()<<"+-";
  std::cout<<std::setprecision(2)<<histTemp->GetRMS()<<"; ";
  std::cout<<std::endl;
  std::cout<<std::endl;
  delete histTemp;
}

void TTemplatesRandCone::RandomizeTemplates(int ikin, int ieta)
{
  for (int ib=1; ib<_hTrueReference[ikin][ieta]->GetNbinsX(); ib++){
    float mean = _hTrueReference[ikin][ieta]->GetBinContent(ib);
    float sigma = _hTrueReference[ikin][ieta]->GetBinError(ib);
    _hTrue[ikin][ieta]->SetBinContent(ib,_random.Gaus(mean,sigma));
    _hTrue[ikin][ieta]->SetBinError(ib,sigma);
  }
  for (int ib=1; ib<_hFakeReference[ikin][ieta]->GetNbinsX(); ib++){
    float mean = _hFakeReference[ikin][ieta]->GetBinContent(ib);
    float sigma = _hFakeReference[ikin][ieta]->GetBinError(ib);
    _hFake[ikin][ieta]->SetBinContent(ib,_random.Gaus(mean,sigma));
    _hFake[ikin][ieta]->SetBinError(ib,sigma);
  }
}

void TTemplatesRandCone::DeleteThreeHists(int ikin, int ieta){

  delete _hTrue[ikin][ieta];
  delete _hFake[ikin][ieta]; 

}

TString TTemplatesRandCone::StrLabelEta(int ieta){
  if (ieta==_config.BARREL) return "_Barrel_";
  else if (ieta==_config.ENDCAP) return "_Endcap_";
  else if (ieta==_config.COMMON) return "_EtaCommon_";
  return "_EtaUnknown_";
}

TString TTemplatesRandCone::StrLabelKin(int ikin){
  if (ikin<0 || ikin>_nKinBins) return TString("_")+_varKin+TString("Unknown_");
  if (ikin==0) return TString("_")+_varKin+TString("Total_");
  TString str="_";
  str+=_varKin;
  str+=_kinBinLims[ikin-1];
  str+="to";
  str+=_kinBinLims[ikin];
  str+="_";
  return str;
}

void TTemplatesRandCone::FitOne(int ikin, int ieta, bool noPrint, bool noPlot)
{

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"FitOne: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<std::endl;
  }

  double varMin=0;
  double varMax=0;
  int nBins=0;

  nBins = _hFake[ikin][ieta]->GetNbinsX();
  varMin = _hFake[ikin][ieta]->GetBinLowEdge(1);
  varMax = _hFake[ikin][ieta]->GetBinLowEdge(nBins)+_hFake[ikin][ieta]->GetBinWidth(nBins);

  double minVarSideband = 0;
  double maxVarSideband = 100000;
  double minPhoEta = -2.6;
  double maxPhoEta = 2.6;
  double minKin = -100000;
  double maxKin =  100000;
  double minWeight = 0;
  double maxWeight = 1;

  maxVarSideband = _treeData->GetMaximum("phoSigmaIEtaIEta");
  minVarSideband = _treeData->GetMinimum("phoSigmaIEtaIEta");
  maxPhoEta = _treeData->GetMaximum("phoSCEta");
  minPhoEta = _treeData->GetMinimum("phoSCEta");
  maxKin = _treeData->GetMaximum(_varKin);
  minKin = _treeData->GetMinimum(_varKin);
  maxWeight = _treeSign->GetMaximum("weight");
  minWeight = _treeSign->GetMinimum("weight");
  if (!noPrint){
    std::cout<<"sideband (phoSigmaIEtaIEta) range: "<<minVarSideband<<" - "<<maxVarSideband<<std::endl;
    std::cout<<"phoSCEta range: "<<minPhoEta<<" - "<<maxPhoEta<<std::endl;
    std::cout<<_varKin<<" range: "<<minKin<<" - "<<maxKin<<std::endl;
    std::cout<<"weight range: "<<minWeight<<" - "<<maxWeight<<std::endl;
  }


  TCut sidebandCut = SidebandCut();
  TCut etaCut = CutEtaBin(ieta);
  TCut kinCut = CutKinBin(ikin);
  TCut sidebandVarNominalCut = SidebandVarNominalCut();
  if (!noPrint){
    std::cout<<"etaCut="<<etaCut.GetTitle()<<std::endl;
    std::cout<<"kinCut="<<kinCut.GetTitle()<<std::endl;
    std::cout<<"sidebandVarNominalCut="<<sidebandVarNonimalCut.GetTitle()<<std::endl;
  }
  TCut cut = kinCut && etaCut && sidebandVarNominalCut;
	
  TString varDraw=_varFit;
  varDraw+=">>";
  varDraw+= _hData[ikin][ieta]->GetName();

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
    _hData[ikin][ieta]->Print();
    std::cout<<"true template hist TH1D* hist: "<<std::endl;
    _hTrue[ikin][ieta]->Print();
    std::cout<<"fake template hist TH1D* hist: "<<std::endl;
    _hFake[ikin][ieta]->Print();
  }
  //RooDataSet dataSet("dataSet","dataSet",_treeData,argSetData,cut);

  //create total pdf
  int nMax = _treeData->GetEntries(cut);
  int nTrueStart = 0.5*nMax;
  int nFakeStart = 0.5*nMax;

  RooRealVar rooVarFit(_varFit,_labelVarFit, varMin, varMax);
  if (nMax<=0){
    if (!noPrint) std::cout<<"ATTENTION: No data events for bin "<<kinCut<<", "<<etaCut<<std::endl;
    _nFakeFromFitVal[ikin][ieta]=0;
    _nFakeFromFitErr[ikin][ieta]=0;
    _nTrueFromFitVal[ikin][ieta]=0;
    _nTrueFromFitErr[ikin][ieta]=0;
    _plotter[ikin][ieta] = new RooPlot(rooVarFit,varMin,varMax,nBins);
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
  if (!noPrint){
    std::cout<<"Histograms (hTrue, hTrueReference, hFake, hFakeReference, hLeak, hData):"<<std::endl;
    PrintOneHistogramBinByBin(_hTrue, ikin, ieta);
    PrintOneHistogramBinByBin(_hTrueReference, ikin, ieta);
    PrintOneHistogramBinByBin(_hFake, ikin, ieta);
    PrintOneHistogramBinByBin(_hFakeReference, ikin, ieta);
    PrintOneHistogramBinByBin(_hData, ikin, ieta);
  }
  RooDataHist dataHist("hist", "data set converted to hist", argList, _hData[ikin][ieta]);
  //create Fake and True PDFs
  RooDataHist trueDataHist("trueDataHist","true RooDataHist", argList, _hTrue[ikin][ieta]);
  RooHistPdf truePdf("truePdf",_varFit+TString(" of true"), argSet, trueDataHist);
  RooDataHist fakeDataHist("fakeDataHist", "fake RooDataHist", argList, _hFake[ikin][ieta]);
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
  _chi2ToNDF[ikin][ieta] = chi2.getVal()/(nBins-2);

  //get nFake values from fit
  _nFakeFromFitVal[ikin][ieta]=rooNFake.getVal();
  _nFakeFromFitErr[ikin][ieta]=rooNFake.getError();
  _nTrueFromFitVal[ikin][ieta]=rooNTrue.getVal();
  _nTrueFromFitErr[ikin][ieta]=rooNTrue.getError();
  
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"nFake="<<rooNFake.getVal()<<"+-"<<rooNFake.getError()<<", nTrue="<<rooNTrue.getVal()<<"+-"<<rooNTrue.getError()<<", chi2="<<chi2.getVal()/(nBins-2)<<std::endl;
    std::cout<<std::endl;
  }

  //for plotting
  _plotter[ikin][ieta] = new RooPlot(rooVarFit,varMin,varMax,nBins); 
  dataHist.plotOn(_plotter[ikin][ieta]);
  fullPdf.plotOn(_plotter[ikin][ieta],Name("sum"),LineColor(kRed));
  fullPdf.plotOn(_plotter[ikin][ieta],Components("truePdf"), Name("true"),
  LineColor(kGreen),LineStyle(9));
  fullPdf.plotOn(_plotter[ikin][ieta], Components("fakePdf"),Name("fake"),LineColor(kBlue),LineStyle(9));
  fullPdf.paramOn(_plotter[ikin][ieta]);
}

void TTemplatesRandCone::ComputeYieldOneKinBin(int ikin, bool noPrint)
{

  _nFakeYieldsVal[ikin][_config.BARREL]=_nFakeFromFitVal[ikin][_config.BARREL];
  _nFakeYieldsErr[ikin][_config.BARREL]=_nFakeFromFitErr[ikin][_config.BARREL];
  _nFakeYieldsVal[ikin][_config.ENDCAP]=_nFakeFromFitVal[ikin][_config.ENDCAP];
  _nFakeYieldsErr[ikin][_config.ENDCAP]=_nFakeFromFitErr[ikin][_config.ENDCAP];
  _nFakeYieldsVal[ikin][_config.COMMON]=_nFakeYieldsVal[ikin][_config.BARREL]+_nFakeYieldsVal[ikin][_config.ENDCAP];
  _nFakeYieldsErr[ikin][_config.COMMON]=sqrt(_nFakeYieldsVal[ikin][_config.BARREL]*_nFakeYieldsVal[ikin][_config.BARREL]+_nFakeYieldsVal[ikin][_config.ENDCAP]*_nFakeYieldsVal[ikin][_config.ENDCAP]);

  _nTrueYieldsVal[ikin][_config.BARREL]=_nTrueFromFitVal[ikin][_config.BARREL];
  _nTrueYieldsErr[ikin][_config.BARREL]=_nTrueFromFitErr[ikin][_config.BARREL];
  _nTrueYieldsVal[ikin][_config.ENDCAP]=_nTrueFromFitVal[ikin][_config.ENDCAP];
  _nTrueYieldsErr[ikin][_config.ENDCAP]=_nTrueFromFitErr[ikin][_config.ENDCAP];
  _nTrueYieldsVal[ikin][_config.COMMON]=_nTrueYieldsVal[ikin][_config.BARREL]+_nTrueYieldsVal[ikin][_config.ENDCAP];
  _nTrueYieldsErr[ikin][_config.COMMON]=sqrt(_nTrueYieldsVal[ikin][_config.BARREL]*_nTrueYieldsVal[ikin][_config.BARREL]+_nTrueYieldsVal[ikin][_config.ENDCAP]*_nTrueYieldsVal[ikin][_config.ENDCAP]);

  if (!noPrint){
    std::cout<<"Yields in "<<StrKinLabel(ikin)<<": "<<std::endl
    for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){
      std::cout<<StrEtaLabel(ieta)<<": nTrue="<<_nTrueYieldsVal[ikin][ieta]<<"+-"<<_nTrueYieldsErr[ikin][ieta]<<"; nFake="<<_nFakeYieldsVal[ikin][ieta]<<"+-"<<_nFakeYieldsErr[ikin][ieta]<<std::endl       
    }  
  }
}

void TTemplatesRandCone::PrintYieldsAndChi2()
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
  PrintHistogramsBinByBin(_hTrueReference);
  PrintHistogramsBinByBin(_hFakeReference);
  std::cout<<std::endl;

  std::cout<<"Chi2/NDF for different bins: "<<std::endl;
  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": chi2/ndf=";
      std::cout<<_chi2ToNDF[ikin][ieta]<<",  nFitBins="<<_nFitBins[ikin][ieta]<<"; ";
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

}

void TTemplatesRandCone::PrintHistogramsBinByBin(TH1D* hist[_nBinsMax][3]){
  std::cout<<"TH1D* hists "<<hist[0][0]->GetTitle()<<": "<<std::endl;
  std::cout<<std::setprecision(0)<<std::fixed;
  for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
    for (int ikin=0; ikin<_nKinBins+1; ikin++)
      PrintOneHistogramBinByBin(hist, ikin, ieta);
  }
}

void TTemplatesRandCone::PrintOneHistogramBinByBin(TH1D* hist[_nBinsMax][3], int ikin, int ieta){
  std::cout<<hist[ikin][ieta]->GetTitle()<<": ";
//  std::cout<<StrLabelEta(ieta)<<StrLabelKin(ikin)<<": ";
  float sum=0;
  std::cout<<std::setprecision(0)<<std::fixed;
  for (int ifit=1; ifit<=hist[ikin][ieta]->GetNbinsX(); ifit++){
    std::cout<<std::setw(6)<<hist[ikin][ieta]->GetBinContent(ifit)<<"; ";
    sum+=hist[ikin][ieta]->GetBinContent(ifit);
  }
  std::cout<<"sum="<<sum<<std::endl;
}

void TTemplatesRandCone::PlotTemplates()
{

  for (int ikin=0; ikin<_nKinBins+1; ikin++){
    for (int ieta=_config.BARREL; ieta<=_config.ENDCAP; ieta++){
      PlotOneTemplate(ikin, ieta);
    }//end of loop over ieta
  }//end of loop over ikin

}


void TTemplatesRandCone::PlotOneTemplate(int ikin, int ieta)
{

  TString strBin = StrLabelKin(ikin)+StrLabelEta(ieta);
  TString strChi = "#chi^{2}/ndf=";
  float chiFloat=(100*_chi2ToNDF[ikin][ieta]);
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
  strRatio+=ikin;
  strRatio+=ieta;
  TString strSum="hSum";
  strSum+=ikin;
  strSum+=ieta;
  _hRatio[ikin][ieta]=(TH1D*)_hData[ikin][ieta]->Clone(strRatio);
  _hRatio[ikin][ieta]->Sumw2();
  _hSumm[ikin][ieta]=(TH1D*)_hData[ikin][ieta]->Clone(strSum);
  
  float areaHistTrue=0;
  float areaHistFake=0;
  for (int ib=1; ib<=_hTrue[ikin][ieta]->GetNbinsX(); ib++){
    areaHistTrue+=_hTrue[ikin][ieta]->GetBinContent(ib);
    areaHistFake+=_hFake[ikin][ieta]->GetBinContent(ib);
  }

  for (int ib=1; ib<=_hTrue[ikin][ieta]->GetNbinsX(); ib++){

    float contS=0;
    float err1S=0;
    float err2S=0;
    float contB=0;
    float err1B=0;
    float err2B=0;

    if (areaHistTrue==0);
    else{
      contS = _hTrue[ikin][ieta]->GetBinContent(ib)*_nTrueFromFitVal[ikin][ieta]/areaHistTrue;
      err1S = (_hTrue[ikin][ieta]->GetBinError(ib)*_nTrueFromFitVal[ikin][ieta])/areaHistTrue;
      err2S = (_hTrue[ikin][ieta]->GetBinContent(ib)*_nTrueFromFitErr[ikin][ieta])/areaHistTrue;
    }
    if (areaHistFake==0);
    else {
      contB = _hFake[ikin][ieta]->GetBinContent(ib)*_nFakeFromFitVal[ikin][ieta]/areaHistFake;
      err1B = (_hFake[ikin][ieta]->GetBinError(ib)*_nFakeFromFitVal[ikin][ieta])/areaHistFake;
      err2B = (_hFake[ikin][ieta]->GetBinContent(ib)*_nFakeFromFitErr[ikin][ieta])/areaHistFake;
    }
//    _hTrue[ikin][ieta]->SetBinContent(ib,contS);
//    _hTrue[ikin][ieta]->SetBinError(ib,sqrt(err1S*err1S+err2S*err2S));

//    _hFake[ikin][ieta]->SetBinContent(ib,contB);
//    _hFake[ikin][ieta]->SetBinError(ib,sqrt(err1B*err1B+err2B*err2B));

    float err = sqrt(err1S*err1S+err2S*err2S+err1B*err1B+err2B*err2B);
    _hSumm[ikin][ieta]->SetBinContent(ib,contS+contB);
    _hSumm[ikin][ieta]->SetBinError(ib,err);

  }  

  _hRatio[ikin][ieta]->Divide(_hSumm[ikin][ieta]);


    TString cName="c";
    cName+=StrLabelKin(ikin);
    cName+=StrLabelEta(ieta);
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

    _plotter[ikin][ieta]->SetTitle("");
    _plotter[ikin][ieta]->Draw();

    _hSumm[ikin][ieta]->SetLineColor(7);
    _hSumm[ikin][ieta]->SetLineWidth(2);
    _hSumm[ikin][ieta]->SetLineStyle(3);
    _hSumm[ikin][ieta]->Draw("HIST same");  

    double valCut = ValueCutNominalVarFit(ieta);
    TLine* cutLine = new TLine(valCut,0,valCut,_hSumm[ikin][ieta]->GetMaximum());
    cutLine->SetLineWidth(3);
    cutLine->Draw("same");

    textBin->Draw("same");
    textChi2->Draw("same");

    pad2->cd();
    _hRatio[ikin][ieta]->SetLineWidth(2);
    _hRatio[ikin][ieta]->SetStats(0);
    _hRatio[ikin][ieta]->GetYaxis()->SetLabelSize(0.1);
    _hRatio[ikin][ieta]->GetXaxis()->SetLabelSize(0.1);
    _hRatio[ikin][ieta]->GetXaxis()->SetTitleOffset(1.0);
    _hRatio[ikin][ieta]->GetXaxis()->SetTitleSize(0.12);
    _hRatio[ikin][ieta]->SetTitle(TString("; ")+_labelVarFit+TString(" ;"));
    _hRatio[ikin][ieta]->Draw();
    int nBins = _hRatio[ikin][ieta]->GetNbinsX();
    TLine* line = new TLine(_hRatio[ikin][ieta]->GetBinLowEdge(1),1,_hRatio[ikin][ieta]->GetBinLowEdge(nBins)+_hRatio[ikin][ieta]->GetBinWidth(nBins),1);
    line->SetLineWidth(2);
    line->SetLineStyle(9);
    line->Draw("same");
   
    cName+=".png";
    cName.ReplaceAll("-1","total");
    c1->SaveAs(cName);

//  cName+="plotter";
//  TCanvas* c2 = new TCanvas(cName);
//  _plotter[ikin][ieta]->Draw();
}

TCut TTemplatesRandCone::SidebandCut()
{
  TCut cutSidebandB="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.014";  
  TCut cutSidebandE="phoSigmaIEtaIEta>0.033 && phoSigmaIEtaIEta<0.044";
  return ( (_photon.IsBarrel()&&cutSidebandB)||(_photon.IsEndcap()&&cutSidebandE) ); 
}

TCut TTemplatesRandCone::SidebandVarNominalCut()
{
  return _photon.RangeSigmaIEtaIEta(2012, _phoWP);
}

void TTemplatesRandCone::SaveYields()
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

TCut TTemplatesRandCone::Cutikin(int ikin){

  if (ikin<0 || ikin>_nKinBins){
    TCut cut("0");
    return cut;
  }
  float min;
  float max;
  if (ikin==0){
    min=_kinBinLims[0];
    max=_kinBinLims[_nKinBins];
  }
  else{
    min=_kinBinLims[ikin-1];
    max=_kinBinLims[ikin];
  }

  TString ikinCutStr;
  ikinCutStr=_varKin;
  ikinCutStr+=">";
  ikinCutStr+=min;
  ikinCutStr+=" && ";
  ikinCutStr+=_varKin;
  ikinCutStr+="<=";
  ikinCutStr+=max;
  TCut cut(ikinCutStr);
  return cut;
}

TCut TTemplatesRandCone::CutEtaBin(int ieta){
  TCut cut;
  if (ieta==_config.BARREL) 
    cut=_photon.RangeBarrel();
  else if (ieta==_config.ENDCAP) 
    cut=_photon.RangeEndcap();
  else if (ieta==_config.COMMON) 
    cut=(_photon.RangeEndcap()&&_photon.RangeBarrel());
  else
    cut="0";
  return cut;
}

double TTemplatesRandCone::ValueCutNominalVarFit(int ieta){
  if (_varFit=="phoSigmaIEtaIEta"){
    if (ieta==_config.BARREL) return _photon.GetPhoSigmaIEtaIEtaCutB(2012,_phoWP);
    else if (ieta==_config.ENDCAP) return _photon.GetPhoSigmaIEtaIEtaCutE(2012,_phoWP);
    else return 1;
  }
  if (_varFit=="phoSCRChIsoCorr"){
    return _photon.GetPhoSCRChIsoCorrCut();
  }
  return 100000;
}
