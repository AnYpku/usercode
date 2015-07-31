#include "TTemplates.h"
  //this class
//#include "../Include/TPhotonCuts.h"
#include "../Include/TMathTools.h"
//#include "../Configuration/TConfiguration.h"
//#include "../Configuration/TInputSample.h"
//#include "../Configuration/TAllInputSamples.h"

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
#include "TF1.h"
#include "TLatex.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TVectorD.h"

using namespace RooFit ;

TTemplates::TTemplates()
{
}

TTemplates::TTemplates(TemplatesPars pars)
{
  SetPars(pars);
  _fOutTemp = new TFile("fOutTemp.root","recreate");
}

TTemplates::~TTemplates()
{
  //if (_pars.fOutForSave->IsOpen()) _pars.fOutForSave->Close();
}

void TTemplates::SetPars(TemplatesPars pars)
{
  _pars=pars;
  _pars.cutWeight=_pars.varWeight;

  _pars.fOutForSave = new TFile(_pars.strFileOutName,"recreate");
}

void TTemplates::ComputeBackground(bool noPrint, bool noPlot)
{
  if (!noPrint) PrintPars();
  for (int ikin=0; ikin<=_pars.nKinBins; ikin++){
  //for (int ikin=0; ikin<=1; ikin++){
    bool ok = ComputeBackgroundOne(ikin,_COMMON,noPrint);
    if (!ok) return;
  }
  if (!noPlot) PlotTemplates();
  PrintYieldsAndChi2();
  PrintLatex();
  SaveYields();
}// end of ComputeBackground()

void TTemplates::PrintPars()
{
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Templates parameters"<<std::endl;
  std::cout<<"varKin: "<<_pars.varKin<<std::endl;
  std::cout<<"varTrueTempl: "<<_pars.varTrueTempl<<std::endl;
  std::cout<<"varFakeTempl: "<<_pars.varFakeTempl<<std::endl;
  std::cout<<"varSideband: "<<_pars.varSideband<<std::endl;
  std::cout<<"varPhoEta: "<<_pars.varPhoEta<<std::endl;
  std::cout<<"varWeight: "<<_pars.varWeight<<std::endl;
//  std::cout<<"fTrue: "<<_pars.fTrue->GetTitle()<<std::endl;
//  std::cout<<"fFake: "<<_pars.fFake->GetTitle()<<std::endl;
//  std::cout<<"fData: "<<_pars.fData->GetTitle()<<std::endl;
//  std::cout<<"fSign: "<<_pars.fSign->GetTitle()<<std::endl;
  std::cout<<"nKinBins="<<_pars.nKinBins<<std::endl;
  std::cout<<"kin bin lims; nFitBins; fit range; sideband"<<std::endl;
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
    std::cout<<StrLabelEta(ieta)<<std::endl;
    for (int ik=0; ik<=_pars.nKinBins; ik++){
      std::cout<<StrLabelKin(ik)<<"; ";
      std::cout<<std::setprecision(0)<<_pars.nFitBins[ik][ieta]<<"; ";
      std::cout<<std::setprecision(3)<<_pars.minVarFit[ik][ieta]<<"-"<<_pars.maxVarFit[ik][ieta]<<"; "<<_pars.sideband[ik][ieta]<<"-"<<_pars.sidebandUp[ik][ieta];
      std::cout<<std::endl;
    }//end of loop over ik
    std::cout<<"cutNominal="<<_pars.cutNominal[ieta].GetTitle()<<std::endl;
    std::cout<<"cutNominalExceptSidebandVar="<<_pars.cutNominalExceptSidebandVar[ieta].GetTitle()<<std::endl;
    std::cout<<"cutSidebandVarNominalRange="<<_pars.cutSidebandVarNominalRange[ieta].GetTitle()<<std::endl;
  }//end of loop over ieta
  std::cout<<"cutAdd="<<_pars.cutAdd.GetTitle()<<std::endl;
  std::cout<<"cutBarrel="<<_pars.cutBarrel.GetTitle()<<std::endl;
  std::cout<<"cutEndcap="<<_pars.cutEndcap.GetTitle()<<std::endl;
  std::cout<<"cutWeight="<<_pars.cutWeight.GetTitle()<<std::endl;
  std::cout<<"noLeakSubtr="<<_pars.noLeakSubtr<<std::endl;
  std::cout<<"isRooFit="<<_pars.isRooFit<<std::endl;
  std::cout<<"strFileOutName="<<_pars.strFileOutName<<std::endl;
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    std::cout<<StrLabelEta(ieta)<<":"<<std::endl;
    std::cout<<"strTrueYieldsTot="<<_pars.strTrueYieldsTot[ieta]<<std::endl;
    std::cout<<"strTrueYields1D="<<_pars.strTrueYields1D[ieta]<<std::endl;
    std::cout<<"strFakeYieldsTot="<<_pars.strFakeYieldsTot[ieta]<<std::endl;
    std::cout<<"strFakeYields1D="<<_pars.strFakeYields1D[ieta]<<std::endl;
  }//end of loop over ieta
  std::cout<<"|||| end of Templates parameters"<<std::endl;
  std::cout<<"==============================="<<std::endl;
}// end of TTemplates::PrintPars()

void TTemplates::PrintLatex()
{
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
    TString strEta = StrLabelEta(ieta);
    strEta.ReplaceAll("_","");

    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{"<<strEta<<"}"<<std::endl;
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|}"<<std::endl;
    std::cout<<"    bin & N of & fit & sideband & leak\\\\ "<<std::endl;
    std::cout<<"    lims & fit bins & range & & fraction, \\% \\\\ \\hline"<<std::endl;
    for (int ik=1; ik<=_pars.nKinBins; ik++){
      std::cout<<"    ";
      std::cout<<std::setprecision(0)<<_pars.kinBinLims[ik-1]<<"-"<<_pars.kinBinLims[ik]<<" & ";
      std::cout<<std::setprecision(0)<<_pars.nFitBins[ik][ieta]<<" & ";
      int presFit, presSb;
      if (_pars.maxVarFit[ik][ieta]-_pars.minVarFit[ik][ieta]<1) {presFit=3; presSb=1;}
      else {presFit=1; presSb=3;}
      std::cout<<std::setprecision(presFit)<<_pars.minVarFit[ik][ieta]<<"-"<<_pars.maxVarFit[ik][ieta]<<" & ";
      std::cout<<std::setprecision(presSb)<<_pars.sideband[ik][ieta]<<"-"<<_pars.sidebandUp[ik][ieta]<<" & ";
      std::cout<<std::setprecision(1)<<_leakFraction[ik][ieta];
      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ik
    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:fit_pars_"<<strEta<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;
  }//end of loop over ieta
  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;
}// end of TTemplates::PrintLatex()

bool TTemplates::ComputeBackgroundOne(int ikin, int ieta, bool noPrint)
{
  // if ieta == _COMMON - compute BARREL, ENDCAP and SUM
  // otherwise compute just for ieta
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<"||||========== ComputeBackgroundOne - "<<StrLabelKin(ikin)<<" starts"<<std::endl;
  }

  for (int ieta1=_BARREL; ieta1<=_ENDCAP; ieta1++){
    if ((ieta!=_COMMON) && (ieta!=ieta1)) continue;
    bool ok = SetHists(ikin, ieta1, noPrint);
    if (!noPrint) std::cout<<"SetHists is done; ok="<<ok<<std::endl;
    if (!ok) return 0;
    if (!noPrint) std::cout<<"Will do FitOne; ieta="<<StrLabelEta(ieta)<<", ieta1="<<StrLabelEta(ieta1)<<std::endl;
    if (_pars.isRooFit) FitOneRooFit(ikin, ieta1, noPrint);
    else FitOneROOT(ikin, ieta1, noPrint);
  }//end of loop over ieta

  ComputeYieldOneKinBin(ikin,ieta,noPrint);


  if (!noPrint){
    std::cout<<"||||========== ComputeBackgroundOne - "<<StrLabelKin(ikin)<<" ends"<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<std::endl;
  }
  return 1;
}// end of ComputeBackgroundOne

bool TTemplates::SetHists(int ikin, int ieta, bool noPrint){

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"SetHists for: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
  }

  _pars.unitOrigFit[ikin][ieta] = (_pars.maxVarFit[ikin][ieta]-_pars.minVarFit[ikin][ieta])/_pars.nFitBins[ikin][ieta];
  NewHistograms(ikin, ieta, noPrint);
  bool isOk = SetFakeTemplate(ikin, ieta, noPrint);
  if (!isOk) return 0;
  SetTrueTemplate(ikin, ieta, noPrint);
  SetDataAndSignHists(ikin, ieta, noPrint);
  bool badBins[150];
  bool hasBadBins=CheckTemplates(ikin, ieta, badBins, noPrint);
  if (!noPrint)     std::cout<<"CheckTemplates done; hasBadBins="<<hasBadBins<<std::endl;

  while (hasBadBins){
    std::cout<<"try to rebin"<<std::endl;
    DeleteHistograms(ikin, ieta);
    RebinTemplates(ikin, ieta, badBins, noPrint);
    NewHistograms(ikin, ieta, noPrint);
    bool isOk = SetFakeTemplate(ikin, ieta, noPrint);
    if (!isOk) return 0;
    SetTrueTemplate(ikin, ieta, noPrint);
    SetDataAndSignHists(ikin, ieta, noPrint);  
    hasBadBins=CheckTemplates(ikin, ieta, badBins, noPrint); 
    if (_pars.nFitBins[ikin][ieta]<3) return 0; 
  }

  if (!noPrint)     std::cout<<"Will copy templates to reference hists"<<std::endl;

  _hFakeReference[ikin][ieta] = (TH1D*)_hFake[ikin][ieta]->Clone(_hFake[ikin][ieta]->GetName()+TString("_Ref"));
  _hFakeReference[ikin][ieta]->SetTitle(_hFakeReference[ikin][ieta]->GetName());
  _hTrueReference[ikin][ieta] = (TH1D*)_hTrue[ikin][ieta]->Clone(_hTrue[ikin][ieta]->GetName()+TString("_Ref"));
  _hTrueReference[ikin][ieta]->SetTitle(_hTrueReference[ikin][ieta]->GetName());

  if (!noPrint)     std::cout<<"copied templates to reference hists"<<std::endl;

  if (!noPrint)     std::cout<<std::endl;
  return 1;
}//end of SetHists()

void TTemplates::SetTemplate(int ikin, int ieta, bool isTrueGamma, TH1D* hTemplate, TCut cutExceptKin, bool noPrint, TH1D* hLeak)
{
  TCut cutKinTrue=CutKinBin(ikin); 
  if (_pars.thresholdCombineTrueTemplates<_pars.kinBinLims[0]) cutKinTrue="1";
  else if (_pars.kinBinLims[ikin-1]>_pars.thresholdCombineTrueTemplates && ikin>0){
     TString strCutKin=_pars.varKin;
     strCutKin+=">";
     strCutKin+=_pars.thresholdCombineTrueTemplates;
     cutKinTrue=TCut(strCutKin);
  }
  TCut cutKinFake=CutKinBin(ikin); 
  if (_pars.thresholdCombineFakeTemplates<_pars.kinBinLims[0]) cutKinFake="1";
  else if (_pars.kinBinLims[ikin-1]>_pars.thresholdCombineFakeTemplates && ikin>0){
       TString strCutKin=_pars.varKin;
       strCutKin+=">";
       strCutKin+=_pars.thresholdCombineFakeTemplates;
       cutKinFake=TCut(strCutKin);
  }

  TString name=hTemplate->GetName();
  TCut cutTrue=cutExceptKin && cutKinTrue;
  TCut cutFake=cutExceptKin && cutKinFake;

  if (isTrueGamma) _pars.treeTrue->Draw(_pars.varTrueTempl+(">>")+name,cutTrue*_pars.cutWeight,"goff");

  else _pars.treeFake->Draw(_pars.varFakeTempl+(">>")+name,cutFake*_pars.cutWeight,"goff");
  if (!noPrint) {
    std::cout<<std::endl;
    std::cout<<"hTemplate->GetName()="<<hTemplate->GetName()<<std::endl;
    std::cout<<"hTemplate->GetSumOfWeights()="<<hTemplate->GetSumOfWeights()<<std::endl;
    if (isTrueGamma) std::cout<<"_pars.treeTrue->GetEntries(cut)="<<_pars.treeTrue->GetEntries(cutTrue*_pars.cutWeight)<<std::endl;
    else std::cout<<"_pars.treeFake->GetEntries(cut)="<<_pars.treeFake->GetEntries(cutFake*_pars.cutWeight)<<std::endl;
    std::cout<<"cutKinTrue="<<cutKinTrue.GetTitle()<<std::endl;
    std::cout<<"cutKinFake="<<cutKinFake.GetTitle()<<std::endl;    
  }

  if (isTrueGamma) return;
  if (_pars.noLeakSubtr) return;


  TString strNameLeak=hLeak->GetName();

//  _pars.treeSign->Draw(_pars.varFakeTempl+TString(">>")+strNameLeak,cutFake*_pars.cutWeight,"goff");
//  float normFromSign = hLeak->GetSumOfWeights();
//  _pars.treeTrue->Draw(_pars.varTrueTempl+TString(">>")+strNameLeak,cutTrue*_pars.cutWeight,"goff");  
//  float normFromTrue = hLeak->GetSumOfWeights();
//  float norm;
//  if (normFromTrue==0) float norm = 0; else norm = normFromSign/normFromTrue;
//  std::cout<<"hLeak normalization: normFromSign="<<normFromSign<<", normFromTrue="<<normFromTrue<<std::endl;
//  hTemplate->Add(hLeak,-norm);

  _pars.treeSign->Draw(_pars.varFakeTempl+TString(">>")+strNameLeak,cutFake*_pars.cutWeight,"goff");
  hTemplate->Add(hLeak,-1);

  if (!noPrint){

    std::cout<<"(cutTrue*_pars.cutWeight)->GetTitle()="<<(cutTrue*_pars.cutWeight).GetTitle()<<std::endl;  
    std::cout<<"(cutFake*_pars.cutWeight)->GetTitle()="<<(cutFake*_pars.cutWeight).GetTitle()<<std::endl;  
    hLeak->Print();
    std::cout<<"hLeak->GetName()="<<hLeak->GetName()<<std::endl;  
//    std::cout<<"hLeak->GetSumOfWeights()*norm="<<hLeak->GetSumOfWeights()*norm<<std::endl;  
    std::cout<<"_pars.treeSign->GetEntries(cutFake)="<<_pars.treeSign->GetEntries(cutFake*_pars.cutWeight)<<std::endl; 
    std::cout<<"hTemplate->GetSumOfWeights()="<<hTemplate->GetSumOfWeights()<<std::endl; 
/*
    TH1F* histTempr = new TH1F("histTempr","histTempr",1,_pars.treeSign->GetMinimum(_pars.varWeight)-1,_pars.treeSign->GetMaximum(_pars.varWeight)+1);
    histTempr->Sumw2();
    _pars.treeSign->Draw(_pars.varWeight+TString(">>histTempr"),cut*_pars.cutWeight,"goff");
    std::cout<<std::fixed;
    std::cout<<hLeak->GetTitle()<<" Template, mean weight =";
    std::cout<<std::setprecision(2)<<histTempr->GetMean()<<"+-";
    std::cout<<std::setprecision(2)<<histTempr->GetRMS()<<"; ";
    std::cout<<"_pars.treeSign->GetEntries(cut*_pars.cutWeight)="<<_pars.treeSign->GetEntries(cut*_pars.cutWeight);
    std::cout<<std::endl;
    std::cout<<std::endl;
    delete histTempr;
*/
  }
}

bool TTemplates::SetFakeTemplate(int ikin, int ieta, bool noPrint)
{
  if (!noPrint){ 
    std::cout<<std::endl;
    std::cout<<"Will SetFakeTemplate:"<<std::endl;
    std::cout<<std::endl;
  }

  TCut cutExceptKin = _pars.cutAdd && CutEtaBin(ieta) && SidebandCut(ikin,ieta);
  if (!noPrint) {
    std::cout<<"cutSideband="<<SidebandCut(ikin,ieta).GetTitle()<<std::endl;
    std::cout<<"fake template cutExceptKin=(cutAdd && cutEta && cutSideband)*cutWeight="<<cutExceptKin.GetTitle()<<std::endl;
  }
  if (!noPrint) std::cout<<"Fake template cutExceptKin="<<cutExceptKin.GetTitle()<<std::endl;
  SetTemplate(ikin,ieta,0,_hFake[ikin][ieta], cutExceptKin, noPrint, _hLeak[ikin][ieta]);
  // "0" for fake gamma template
  if (_hFake[ikin][ieta]->GetSumOfWeights()!=0){
    float num = 100.0*_hLeak[ikin][ieta]->GetSumOfWeights();
    float den = _hLeak[ikin][ieta]->GetSumOfWeights()+_hFake[ikin][ieta]->GetSumOfWeights();
    _leakFraction[ikin][ieta]=num/den;
  }
  if (_hFake[ikin][ieta]->GetSumOfWeights()==0){
    std::cout<<"_hFake[ikin][ieta]->GetSumOfWeights()==0; return 0;"<<std::endl;
    return 0;
  }
  if (_pars.isMCclosureMode && _leakFraction[ikin][ieta]>_acceptableLeakFraction[ikin][ieta]){
    std::cout<<"_hFake[ikin][ieta]->GetSumOfWeights()="<<_hFake[ikin][ieta]->GetSumOfWeights()<<"; ";
    std::cout<<"_hLeak[ikin][ieta]->GetSumOfWeights()="<<_hLeak[ikin][ieta]->GetSumOfWeights()<<"; ";
    std::cout<<"_leakFraction[ikin][ieta]="<<_leakFraction[ikin][ieta]<<"; ";
    std::cout<<"_acceptableFraction[ikin][ieta]="<<_acceptableLeakFraction[ikin][ieta]<<"; ";
    std::cout<<"return 0;"<<std::endl;
    return 0;
  }
  return 1;
}// end of SetFakeTemplate

void TTemplates::SetTrueTemplate(int ikin, int ieta, bool noPrint)
{
  if (!noPrint){ 
    std::cout<<std::endl;
    std::cout<<"Will SetTrueTemplate:"<<std::endl;
    std::cout<<std::endl;
  }

  TCut cutExceptKin = _pars.cutAdd && CutEtaBin(ieta) && SidebandVarNominalCut(ieta);// && cutSignal;;
  if (!noPrint) {
    std::cout<<"cutEta="<<CutEtaBin(ieta).GetTitle()<<std::endl;
    std::cout<<"cutWeight="<<_pars.cutWeight.GetTitle()<<std::endl;
    std::cout<<"cutAdd="<<_pars.cutAdd.GetTitle()<<std::endl;
    std::cout<<"SidebandVarNominalCut(ieta)="<<SidebandVarNominalCut(ieta)<<std::endl;
    std::cout<<"true template cutExceptKin=(cutAdd && cutEta && SidebandVarNominalCut())*cutWeight="<<cutExceptKin.GetTitle()<<std::endl;
  }
//  _fOutTemp->cd();
  SetTemplate(ikin, ieta, 1, _hTrue[ikin][ieta],cutExceptKin,noPrint);
  // "1" for true gamma template
}//end of SetTrueTemplate

void TTemplates::SetDataAndSignHists(int ikin, int ieta, bool noPrint){
  if (!noPrint){ 
    std::cout<<std::endl;
    std::cout<<"Will SetDataAndSignHists:"<<std::endl;
    std::cout<<std::endl;
  }
  TCut sidebandCut = SidebandCut(ikin,ieta);
  TCut etaCut = CutEtaBin(ieta);
  TCut kinCut = CutKinBin(ikin);
  TCut sidebandVarNominalCut = SidebandVarNominalCut(ieta);
  TCut cut = _pars.cutAdd && kinCut && etaCut && sidebandVarNominalCut;
  if (!noPrint){
    std::cout<<"etaCut="<<etaCut.GetTitle()<<std::endl;
    std::cout<<"kinCut="<<kinCut.GetTitle()<<std::endl;
    std::cout<<"sidebandVarNominalCut="<<sidebandVarNominalCut.GetTitle()<<std::endl;
    std::cout<<"data hist cut=(cutAdd && cutEta && cutKin && SidebandVarNominalCut())*cutWeight="<<cut.GetTitle()<<std::endl;
    std::cout<<"_pars.treeData->GetEntries(cut)="<<_pars.treeData->GetEntries(cut)<<std::endl;
  }

	
//  TH1D* histTemp = (TH1D*)_hData[ikin][ieta]->Clone(); 
//  histTemp->SetName(TString("histTemp")+StrLabelKin(ikin)+StrLabelEta(ieta));
  TString varDraw=_pars.varFit+TString(">>")+_hData[ikin][ieta]->GetName();
  _pars.treeData->Draw(varDraw,cut*_pars.cutWeight,"goff");
  TH1F* hTemp = new TH1F(TString("hTemp")+StrLabelKin(ikin)+StrLabelEta(ieta),TString("hTemp")+StrLabelKin(ikin)+StrLabelEta(ieta),1,-3,3);
  _pars.treeData->Draw(_pars.varPhoEta+TString(">>hTemp")+StrLabelKin(ikin)+StrLabelEta(ieta),(cut && _pars.cutNominal[ieta])*_pars.cutWeight,"goff");
  _nDataFullSelVal[ikin][ieta]=hTemp->GetBinContent(1);
  _nDataFullSelErr[ikin][ieta]=hTemp->GetBinError(1); 
  std::cout<<StrLabelKin(ikin)+StrLabelEta(ieta)<<std::endl;
  std::cout<<"treeData, full selection, nEntries: "<<_pars.treeData->GetEntries((cut && _pars.cutNominal[ieta])*_pars.cutWeight)<<std::endl;
  std::cout<<"nDataFullSel = "<<_nDataFullSelVal[ikin][ieta]<<" +- "<<_nDataFullSelErr[ikin][ieta]<<std::endl;
  delete hTemp;

  //std::cout<<StrLabelEta(ieta1)<<": "<<std::endl;

  if (!noPrint){
    std::cout<<"varDraw="<<varDraw<<std::endl;
    std::cout<<"_hData bin lims: ";
    for (int ib=1; ib<=_hData[ikin][ieta]->GetNbinsX(); ib++)
      std::cout<<_hData[ikin][ieta]->GetBinLowEdge(ib)<<", ";
    std::cout<<_hData[ikin][ieta]->GetBinLowEdge(_hData[ikin][ieta]->GetNbinsX())+_hData[ikin][ieta]->GetBinWidth(_hData[ikin][ieta]->GetNbinsX());
    std::cout<<std::endl;
    _hData[ikin][ieta]->Print();
  }
  varDraw=_pars.varFit+TString(">>")+_hSign[ikin][ieta]->GetName();
  _pars.treeSign->Draw(varDraw,cut*_pars.cutWeight,"goff");
  if (!noPrint){
    std::cout<<"varDraw="<<varDraw<<std::endl;
    std::cout<<"_hSign bin lims: ";
    for (int ib=1; ib<=_hSign[ikin][ieta]->GetNbinsX(); ib++)
      std::cout<<_hSign[ikin][ieta]->GetBinLowEdge(ib)<<", ";
    std::cout<<_hSign[ikin][ieta]->GetBinLowEdge(_hSign[ikin][ieta]->GetNbinsX())+_hSign[ikin][ieta]->GetBinWidth(_hSign[ikin][ieta]->GetNbinsX());
    std::cout<<std::endl;
    _hSign[ikin][ieta]->Print();
  }

  
  varDraw=_pars.varFit+TString(">>")+_hFakeMCtruth[ikin][ieta]->GetName();
  _pars.treeFakeRef->Draw(varDraw,cut*_pars.cutWeight,"goff");

  _testMCtruthKolmogorov[ikin][ieta]=_hFakeMCtruth[ikin][ieta]->KolmogorovTest(_hFake[ikin][ieta]);
  _testMCtruthChi2[ikin][ieta]=_hFakeMCtruth[ikin][ieta]->Chi2Test(_hFake[ikin][ieta],"WW");
  std::cout<<"Compare fake to MCtruth hist (relevant for MC-closure only)"<<std::endl;
  std::cout<<"KolmogorovTest="<<_testMCtruthKolmogorov[ikin][ieta]<<std::endl;
  std::cout<<"Chi2Test="<<_testMCtruthChi2[ikin][ieta]<<std::endl;

  TH1F* histTemp = new TH1F("histTemp","histTemp",1,_pars.treeData->GetMinimum(_pars.varWeight)-1,_pars.treeData->GetMaximum(_pars.varWeight)+1);
  histTemp->Sumw2();
  _pars.treeData->Draw(_pars.varWeight+TString(">>histTemp"),cut,"goff");
  std::cout<<std::endl;
  std::cout<<std::fixed;
  std::cout<<"Data Hist, mean weight =";
  std::cout<<std::setprecision(2)<<histTemp->GetMean()<<"+-";
  std::cout<<std::setprecision(2)<<histTemp->GetRMS()<<"; ";
  std::cout<<std::endl;
  std::cout<<std::endl;
  delete histTemp; 

}//end of SetDataAndSignHists

bool TTemplates::CheckTemplates(int ikin, int ieta, bool badBins[150], bool noPrint){
  if (!noPrint){ 
    std::cout<<std::endl;
    std::cout<<"Will CheckTemplates:";
    std::cout<<std::endl;
  }
  int nBins = _hData[ikin][ieta]->GetNbinsX();
  //bool badBins[nBins];
  bool hasBadBin=0;

  int NeffEv=10;
  if (nBins<=11) NeffEv=5;
  if (nBins<=8) NeffEv=3;
  if (nBins<=6) NeffEv=1;
  if (nBins<=3) NeffEv=0.01;
  
  float errRateAllowed=sqrt(NeffEv)/NeffEv;

  for (int i=1; i<=nBins; i++){
    float contD=_hData[ikin][ieta]->GetBinContent(i);
    float contT=_hTrue[ikin][ieta]->GetBinContent(i);
    float contF=_hFake[ikin][ieta]->GetBinContent(i);
    
    float errD=_hData[ikin][ieta]->GetBinError(i);

    badBins[i]=0;
    if (contD<0) {std::cout<<i<<": contD<0; "; badBins[i]=1; hasBadBin=1;}
    if (contT<0) {std::cout<<i<<": contT<0; "; badBins[i]=1; hasBadBin=1;}
    if (contF<0) {std::cout<<i<<": contF<0; "; badBins[i]=1; hasBadBin=1;}
    if (contD==0 && (contT+contF)>0) {std::cout<<i<<": contD==0;"; badBins[i]=1; hasBadBin=1;}
    if (contD>0 && (contT+contF)==0) {std::cout<<i<<": contT+contF==0;"; badBins[i]=1; hasBadBin=1;}
    if (errD>errRateAllowed*contD) {
      // corresponds to 10 events for unweighted hist
      std::cout<<i<<": errD>"<<errRateAllowed<<"*contD="<<errD<<">"<<errRateAllowed<<"*"<<contD;
      std::cout<<"="<<errRateAllowed*contD<<"; ";
      badBins[i]=1; hasBadBin=1;
    }//end of errD>0.32*contD
  }//end of loop over i
  std::cout<<std::endl;
  if (!hasBadBin) {
    if (!noPrint) std::cout<<"CheckTemplates: all bins are OK"<<std::endl;
    return hasBadBin;
  }
  return hasBadBin;
}//end of CheckTemplates

bool TTemplates::RebinTemplates(int ikin, int ieta, bool badBins[150], bool noPrint){
  if (!noPrint){ 
    std::cout<<std::endl;
    std::cout<<"Will RebinTemplates:"<<std::endl;
    std::cout<<std::endl;
  }
  bool rebinningFound=0;
  int nFitBins=_pars.nFitBins[ikin][ieta];
  double max=_pars.maxVarFit[ikin][ieta];
  double min=_pars.minVarFit[ikin][ieta];
  double unit = (max-min)/nFitBins;
  std::cout<<"Old binning: nFitBins="<<std::setprecision(0)<<nFitBins;
  std::cout<<std::setprecision(4)<<", unit="<<unit<<", min="<<min<<", max="<<max<<std::endl;
  std::cout<<"bad bins: ";
  for (int i=1; i<=nFitBins; i++)
      std::cout<<badBins[i]<<" ";
  std::cout<<std::endl;
  int firstBadBin=0;
//  int firstGoodBin=0;
  for (int i=1; i<=nFitBins; i++){
    if (badBins[i]) {
      if (!firstBadBin) firstBadBin=i;
    }
  }// end of loop over i
  if (1.0*(nFitBins-firstBadBin)/nFitBins<0.25) {
    std::cout<<"Rebin by rejecting tail; firstBadBin = "<<firstBadBin<<", (nBins-badBin)/nBins = "<<1.0*(nFitBins-firstBadBin)/nFitBins<<std::endl;
    _pars.nFitBins[ikin][ieta]=firstBadBin-1;
    _pars.minVarFit[ikin][ieta]=min;
    _pars.maxVarFit[ikin][ieta]=min+unit*(firstBadBin-1);  
//    return 0;
  }
  else{
    std::cout<<"Rebin by increasing bin width"<<std::endl;
    unit = unit+_pars.unitOrigFit[ikin][ieta];
    _pars.minVarFit[ikin][ieta]=min;
    _pars.nFitBins[ikin][ieta]=_pars.nFitBins[ikin][ieta]/2+1;
    _pars.maxVarFit[ikin][ieta]=min+unit*_pars.nFitBins[ikin][ieta];     
  }
  rebinningFound=1;  

  nFitBins=_pars.nFitBins[ikin][ieta];
  max=_pars.maxVarFit[ikin][ieta];
  min=_pars.minVarFit[ikin][ieta];
  unit = (max-min)/nFitBins;
  std::cout<<"New binning: nFitBins="<<std::setprecision(0)<<nFitBins;
  std::cout<<std::setprecision(4)<<", unit="<<unit<<", min="<<min<<", max="<<max<<std::endl;
  return rebinningFound;
}//end of RebinTemplates

void TTemplates::NewHistograms(int ikin, int ieta, bool noPrint){

  if (!noPrint){ 
    std::cout<<std::endl;
    std::cout<<"Will do NewHistograms:"<<std::endl;
    std::cout<<std::endl;
  }

  TString name=TString("h")+StrLabelKin(ikin)+StrLabelEta(ieta); 
  
  int nFitBins=_pars.nFitBins[ikin][ieta];
  double max=_pars.maxVarFit[ikin][ieta];
  double min=_pars.minVarFit[ikin][ieta];
  double unit = (max-min)/nFitBins;
  // one bin [-unit,0] is reserved for events for which Iso==0 

  TString nameAffix="";
  int intmin=int(1000*min);
  int intmax=int(1000*max);
  nameAffix+=intmin;
  nameAffix+=intmax;
  nameAffix.ReplaceAll(".","");
  nameAffix.ReplaceAll("0","");
  nameAffix.ReplaceAll("99","");
  name+=nameAffix;

  double fitBinLims[nFitBins+1];

  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<std::setprecision(3)<<"min="<<min<<", max="<<max<<", unit="<<unit;
    std::cout<<std::setprecision(0)<<", nFitBins="<<nFitBins<<std::endl;
    std::cout<<std::setprecision(3)<<"bin lims:";
  }
  for (int i=0; i<=nFitBins; i++){
    fitBinLims[i]=min+i*unit;
    if (!noPrint) std::cout<<fitBinLims[i]<<", ";
  }
  std::cout<<std::endl;
  //_pars.kinBinLims[ikin] is upper limit
  //_pars.kinBinLims[ikin-1] is lower limit

  name+="_Fake"; 
  _hFake[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hFake[ikin][ieta]->Sumw2();
  name.ReplaceAll("_Fake","_Leak");
  _hLeak[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hLeak[ikin][ieta]->Sumw2();
  name.ReplaceAll("_Leak","_Data");
  _hData[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hData[ikin][ieta]->Sumw2();
  name.ReplaceAll("_Data","_Sign");
  _hSign[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hSign[ikin][ieta]->Sumw2();
  name.ReplaceAll("_Sign","_True");
  _hTrue[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hTrue[ikin][ieta]->Sumw2();
  name.ReplaceAll("_True","_FakeMCtruth");
  _hFakeMCtruth[ikin][ieta] = new TH1D(name,name,nFitBins,fitBinLims);
  _hFakeMCtruth[ikin][ieta]->Sumw2();
}// end of NewHistograms

void TTemplates::DeleteHistograms(int ikin, int ieta){

    std::cout<<std::endl;
    std::cout<<"Will do DeleteHistograms:"<<std::endl;
    std::cout<<std::endl;
  
  delete  _hFake[ikin][ieta];
  delete  _hLeak[ikin][ieta]; 
  delete  _hData[ikin][ieta]; 
  delete  _hSign[ikin][ieta]; 
  delete  _hTrue[ikin][ieta]; 
  delete  _hFakeMCtruth[ikin][ieta];
}// end of DeleteHistograms

void TTemplates::RandomizeTemplates(int ikin, int ieta)
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

TString TTemplates::StrLabelEta(int ieta){
  if (ieta==_BARREL) return "_Barrel_";
  else if (ieta==_ENDCAP) return "_Endcap_";
  else if (ieta==_COMMON) return "_EtaCommon_";
  return "_EtaUnknown_";
}

TString TTemplates::StrLabelKin(int ikin){
  if (ikin<0 || ikin>_pars.nKinBins) return TString("_")+_pars.varKin+TString("Unknown_");
  if (ikin==0) return TString("_")+_pars.varKin+TString("Total_");
  TString str="_";
  str+=_pars.varKin;
  str+=_pars.kinBinLims[ikin-1];
  str+="to";
  str+=_pars.kinBinLims[ikin];
  str+="_";
  return str;
}

TString TTemplates::StrLabelKin(int ikin, int nKinBins, float* kinBinLims, TString varKin)
{
  if (ikin<0 || ikin>nKinBins) return TString("_")+varKin+TString("Unknown_");
  if (ikin==0) return TString("_")+varKin+TString("Total_");
  TString str="_";
  str+=varKin;
  str+=kinBinLims[ikin-1];
  str+="to";
  str+=kinBinLims[ikin];
  str+="_";
  return str;
}

void TTemplates::FitOneROOT(int ikin, int ieta, bool noPrint, bool noPlot)
{
  float low = _hData[ikin][ieta]->GetBinLowEdge(1);
  int nBins = _hData[ikin][ieta]->GetNbinsX();
  float up = _hData[ikin][ieta]->GetBinLowEdge(nBins)+_hData[ikin][ieta]->GetBinWidth(nBins);

  _hTrueTemp=_hTrue[ikin][ieta];
  _hFakeTemp=_hFake[ikin][ieta];

  TF1* funcTempl = new TF1("FuncTempl",FuncTempl,low,up,2);

  float nMax = _hData[ikin][ieta]->GetSumOfWeights();
  float nSign = _hSign[ikin][ieta]->GetSumOfWeights();
  float nTrueStart = 0.5*nMax;//nSign;
  float nFakeStart = 0.5*nMax;//nMax - nSign;
  if (nTrueStart<=0 || nFakeStart<=0){nTrueStart=0.5*nMax; nFakeStart=0.5*nMax;}
  std::cout<<"some true and fake parameters: nTrueStart="<<nTrueStart<<", nFakeStart="<<nFakeStart<<", nSign="<<nSign<<", nMax="<<nMax<<std::endl;
  funcTempl->SetParameters(nTrueStart,nFakeStart);
  funcTempl->SetParLimits(0,0,nMax);
  funcTempl->SetParLimits(1,0,nMax);
  funcTempl->SetParNames("nTrue","nFake");
  
  _hData[ikin][ieta]->Fit("FuncTempl","L");
}// end of FitOneROOT

void TTemplates::FitOneRooFit(int ikin, int ieta, bool noPrint, bool noPlot)
{
  if (!noPrint){
    std::cout<<std::endl;
    std::cout<<"FitOneRooFit: "<<StrLabelKin(ikin)<<", "<<StrLabelEta(ieta)<<std::endl;
    std::cout<<std::endl;
  }

  if (!noPrint){
    std::cout<<"Will do _fOutTemp->cd()"<<std::endl;
  }
//  _fOutTemp->cd();
  if (!noPrint){
    std::cout<<"Done _fOutTemp->cd()"<<std::endl;
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

  maxVarSideband = _pars.treeData->GetMaximum(_pars.varSideband);
  minVarSideband = _pars.treeData->GetMinimum(_pars.varSideband);
  maxPhoEta = _pars.treeData->GetMaximum(_pars.varPhoEta);
  minPhoEta = _pars.treeData->GetMinimum(_pars.varPhoEta);
  maxKin = _pars.treeData->GetMaximum(_pars.varKin);
  minKin = _pars.treeData->GetMinimum(_pars.varKin);
  maxWeight = _pars.treeData->GetMaximum(_pars.varWeight);
  minWeight = _pars.treeData->GetMinimum(_pars.varWeight);
  if (!noPrint){
    std::cout<<"sideband ("<<_pars.varSideband<<") range: "<<minVarSideband<<" - "<<maxVarSideband<<std::endl;
    std::cout<<_pars.varPhoEta<<" range: "<<minPhoEta<<" - "<<maxPhoEta<<std::endl;
    std::cout<<_pars.varKin<<" range: "<<minKin<<" - "<<maxKin<<std::endl;
    std::cout<<_pars.varWeight<<" range: "<<minWeight<<" - "<<maxWeight<<std::endl;
  }

  if (!noPrint){
    std::cout<<"data hist TH1D* hist: "<<std::endl;
    _hData[ikin][ieta]->Print();
    std::cout<<"true template hist TH1D* hist: "<<std::endl;
    _hTrue[ikin][ieta]->Print();
    std::cout<<"fake template hist TH1D* hist: "<<std::endl;
    _hFake[ikin][ieta]->Print();
  }

  //create total pdf
  float nMax = _hData[ikin][ieta]->GetSumOfWeights();
  float nSign = _hSign[ikin][ieta]->GetSumOfWeights();
//  if (TString(_pars.fData->GetName()).Contains("blindPRESCALE")) nSign=nSign*0.05;
  float nTrueStart = nSign;
  float nFakeStart = nMax - nSign;
  if (nTrueStart<=0 || nFakeStart<=0){nTrueStart=0.5*nMax; nFakeStart=0.5*nMax;}
  std::cout<<"some true and fake parameters: nTrueStart="<<nTrueStart<<", nFakeStart="<<nFakeStart<<", nSign="<<nSign<<", nMax="<<nMax<<std::endl;

  RooRealVar rooVarFit(_pars.varFit,_pars.varFit, varMin, varMax);
  if (nMax<=0){
    if (!noPrint) std::cout<<"ATTENTION: No data events for bin "<<CutKinBin(ikin)<<", "<<CutEtaBin(ieta)<<std::endl;
    _nFakeFromFitVal[ikin][ieta]=0;
    _nFakeFromFitErr[ikin][ieta]=0;
    _nTrueFromFitVal[ikin][ieta]=0;
    _nTrueFromFitErr[ikin][ieta]=0;
    _plotter[ikin][ieta] = new RooPlot(rooVarFit,varMin,varMax,nBins);
    return;
  }
  RooRealVar rooVarSideband(_pars.varSideband,_pars.varSideband,minVarSideband,maxVarSideband);
  RooRealVar rooPhoEta(_pars.varPhoEta,_pars.varPhoEta,minPhoEta,maxPhoEta);
  RooRealVar rooKin(_pars.varKin,_pars.varKin,minKin,maxKin);
  RooRealVar rooWeight(_pars.varWeight,_pars.varWeight,minWeight,maxWeight);
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
    PrintOneHistogramBinByBin(_hLeak, ikin, ieta);
    PrintOneHistogramBinByBin(_hData, ikin, ieta);
    std::cout<<"_leakFraction[ikin][ieta]="<<_leakFraction[ikin][ieta]<<std::endl;
  }
  RooDataHist dataHist("hist", "data set converted to hist", argList, _hData[ikin][ieta]);
  //create Fake and True PDFs
  RooDataHist trueDataHist("trueDataHist","true RooDataHist", argList, _hTrue[ikin][ieta]);
  RooHistPdf truePdf("truePdf",_pars.varTrueTempl+TString(" of true"), argSet, trueDataHist);
  RooDataHist fakeDataHist("fakeDataHist", "fake RooDataHist", argList, _hFake[ikin][ieta]);
  RooHistPdf fakePdf("fakePdf",_pars.varTrueTempl+TString(" of fake"), argSet, fakeDataHist);

  //load data
  RooArgSet argSetData(rooVarFit,rooVarSideband,rooPhoEta,rooKin,rooWeight);
  RooRealVar rooNTrue("nTrue","n True",nTrueStart,0,nMax);
  RooExtendPdf eTruePdf("eTrue","extended True",truePdf,rooNTrue);
  RooRealVar rooNFake("nFake","n Fake",nFakeStart,0,nMax);
  RooExtendPdf eFakePdf("eFake","extended Fake",fakePdf,rooNFake);
//  RooAddPdf fullPdf("fitModel","fit model",RooArgList(eTruePdf,eFakePdf));
  RooAddPdf fullPdf("fitModel","fit model",RooArgList(truePdf,fakePdf),RooArgList(rooNTrue,rooNFake));
  std::cout<<"fullPdf.getNorm()="<<fullPdf.getNorm()<<std::endl;
  std::cout<<"dataHist.sumEntries()="<<dataHist.sumEntries()<<std::endl;

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

}// end of FitOneRooFit

void TTemplates::ComputeYieldOneKinBin(int ikin, int ieta, bool noPrint)
{

    // first, compute true yield
    ComputeTrueYield(ikin, ieta, noPrint, _hTrue[ikin],
	_nTrueYieldsVal[ikin], _nTrueYieldsErr[ikin], _nTrueFromFitVal[ikin], _nTrueFromFitErr[ikin]);
    // next, compute fake yield
    //ComputeOneYield(ikin, ieta, noPrint, 0, _hFake[ikin],
//	_nFakeYieldsVal[ikin], _nFakeYieldsErr[ikin], _nFakeFromFitVal[ikin], _nFakeFromFitErr[ikin]);

    for (int ieta1=_BARREL; ieta1<=_ENDCAP; ieta1++){
      _nFakeYieldsVal[ikin][ieta1]=_nDataFullSelVal[ikin][ieta1]-_nTrueYieldsVal[ikin][ieta1];
      _nFakeYieldsErr[ikin][ieta1]=sqrt(_nDataFullSelErr[ikin][ieta1]*_nDataFullSelErr[ikin][ieta1]+_nTrueYieldsErr[ikin][ieta1]*_nTrueYieldsErr[ikin][ieta1]);
      if ((ieta!=_COMMON) && (ieta!=ieta1)) continue;
    }  //end of loop over ieta1
  _nFakeYieldsVal[ikin][_COMMON]=_nFakeYieldsVal[ikin][_BARREL]+_nFakeYieldsVal[ikin][_ENDCAP];
  _nFakeYieldsErr[ikin][_COMMON]=sqrt(_nFakeYieldsErr[ikin][_BARREL]*_nFakeYieldsErr[ikin][_BARREL]+_nFakeYieldsErr[ikin][_ENDCAP]*_nFakeYieldsErr[ikin][_ENDCAP]);

  if (!noPrint){
    std::cout<<"Yields in "<<StrLabelKin(ikin)<<": "<<std::endl;
    for (int ieta1=_BARREL; ieta1<=_ENDCAP; ieta1++){
      if ((ieta!=_COMMON) && (ieta!=ieta1)) continue;
      std::cout<<setprecision(2)<<std::endl;
      std::cout<<StrLabelEta(ieta1)<<": "<<std::endl;
     
      std::cout<<"_hData="<<_hData[ikin][ieta1]->GetSumOfWeights()<<"; true+fake="<<_nTrueYieldsVal[ikin][ieta1]+_nFakeYieldsVal[ikin][ieta1]<<std::endl; 
      std::cout<<"_hData="<<_hData[ikin][ieta1]->GetSumOfWeights()<<"; true+fake from fit="<<_nTrueFromFitVal[ikin][ieta1]+_nFakeFromFitVal[ikin][ieta1]<<std::endl; 
    }  //end of loop over ieta1
  }//end of if (!noPrint)
}

void TTemplates::ComputeTrueYield(int ikin, int ieta, bool noPrint, TH1D* hist[2],
	double*  nYieldsVal,double* nYieldsErr,double* nFromFitVal, double* nFromFitErr)
{
  float eff[2];
  for (int ieta1=_BARREL; ieta1<=_ENDCAP; ieta1++){
    if (ieta!=_COMMON && ieta!=ieta1) continue;
    eff[ieta1]=EffFromTree(ikin, ieta1, noPrint);
    nYieldsVal[ieta1]=nFromFitVal[ieta1]*eff[ieta1];
    nYieldsErr[ieta1]=nFromFitErr[ieta1]*eff[ieta1];
  }//end of loop over ieta1
  if (ieta==_COMMON){
    nYieldsVal[_COMMON]=nYieldsVal[_BARREL]+nYieldsVal[_ENDCAP];
    double valsqr = nYieldsErr[_BARREL]*nYieldsErr[_BARREL]+nYieldsErr[_ENDCAP]*nYieldsErr[_ENDCAP];
    if (valsqr>=0) nYieldsErr[_COMMON]=sqrt(valsqr);
    else nYieldsErr[_COMMON]=0;
  }//end of if (ieta==_COMMON)
  if (!noPrint){
    for (int ieta1=_BARREL; ieta1<=_ENDCAP; ieta1++){
      if (ieta!=_COMMON && ieta!=ieta1) continue;
      std::cout<<"fromFit*eff=nTrue=("<<nFromFitVal[ieta1]<<"+-"<<nFromFitErr[ieta1]<<")*"<<eff[ieta1]<<"="<<nYieldsVal[ieta1]<<"+-"<<nYieldsErr[ieta1]<<";"<<std::endl;
    }//  loop over ieta
  }//  end of if (!noPrint)
}// end of ComputeOneYield

void TTemplates::PrintYieldsAndChi2()
{

  std::cout<<"Histograms:"<<std::endl;
  PrintHistogramsBinByBin(_hTrue);
  PrintHistogramsBinByBin(_hFake);
  PrintHistogramsBinByBin(_hTrueReference);
  PrintHistogramsBinByBin(_hFakeReference);
  std::cout<<std::endl;

  std::cout<<"Fake and True values from fit:"<<std::endl;
  std::cout<<"  bin            Fake-B     True-B  Leak%-B    Fake-E    True-E  Leak%-E"<<std::endl;
  for (int ikin=0; ikin<_pars.nKinBins+1; ikin++){
    std::cout<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
     std::cout<<std::setw(6)<<std::setprecision(0)<<std::fixed;
//      std::cout<<std::setw(6)<<std::setprecision(0)<<_nFakeFromFitVal[ikin][ieta]<<"+-";
//      std::cout<<std::setw(5)<<std::setprecision(0)<<_nFakeFromFitErr[ikin][ieta]<<"; ";
//      std::cout<<std::setw(6)<<std::setprecision(0)<<_nTrueFromFitVal[ikin][ieta]<<"+-";
//      std::cout<<std::setw(5)<<std::setprecision(0)<<_nTrueFromFitErr[ikin][ieta]<<"; ";
      std::cout<<_nFakeFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nFakeFromFitErr[ikin][ieta]<<"; ";
      std::cout<<_nTrueFromFitVal[ikin][ieta]<<"+-";
      std::cout<<_nTrueFromFitErr[ikin][ieta]<<"; ";
      if (_hFake[ikin][ieta]->GetSumOfWeights()!=0){
        _leakFraction[ikin][ieta]=100.0*_hLeak[ikin][ieta]->GetSumOfWeights()/(_hLeak[ikin][ieta]->GetSumOfWeights()+_hFakeReference[ikin][ieta]->GetSumOfWeights());
        std::cout<<_leakFraction[ikin][ieta]<<"%; ";
      }
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  std::cout<<"Chi2/NDF for different bins: "<<std::endl;
  for (int ikin=0; ikin<_pars.nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin)<<": ";
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": chi2/ndf=";
      std::cout<<_chi2ToNDF[ikin][ieta]<<",  nFitBins="<<_pars.nFitBins[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;  
  }
  std::cout<<std::endl;

  std::cout<<"Fake yields:"<<std::endl;
  for (int ikin=0; ikin<_pars.nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin);
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": "<<_nFakeYieldsVal[ikin][ieta]<<"+-"<<_nFakeYieldsErr[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"True yields:"<<std::endl;
  for (int ikin=0; ikin<_pars.nKinBins+1; ikin++){
    std::cout<<"ikin="<<ikin<<", "<<StrLabelKin(ikin);
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
      std::cout<<StrLabelEta(ieta)<<": "<<_nTrueYieldsVal[ikin][ieta]<<"+-"<<_nTrueYieldsErr[ikin][ieta]<<"; ";
    }
    std::cout<<std::endl;
  }

}

void TTemplates::PrintHistogramsBinByBin(TH1D* hist[nKinBinsMax][3]){
  std::cout<<"TH1D* hists "<<hist[0][0]->GetTitle()<<": "<<std::endl;
  std::cout<<std::setprecision(0)<<std::fixed;
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
    for (int ikin=0; ikin<_pars.nKinBins+1; ikin++)
      PrintOneHistogramBinByBin(hist, ikin, ieta);
  }
}

void TTemplates::PrintOneHistogramBinByBin(TH1D* hist[nKinBinsMax][3], int ikin, int ieta){
  std::cout<<hist[ikin][ieta]->GetTitle()<<": ";
//  std::cout<<StrLabelEta(ieta)<<StrLabelKin(ikin)<<": ";
  float sum=0;
  std::cout<<std::setprecision(0)<<std::fixed;
  for (int ifit=1; ifit<=hist[ikin][ieta]->GetNbinsX(); ifit++){
    std::cout<<hist[ikin][ieta]->GetBinContent(ifit)<<"; ";
    sum+=hist[ikin][ieta]->GetBinContent(ifit);
  }
  std::cout<<"sum="<<sum<<std::endl;
}

void TTemplates::PlotTemplates()
{

  for (int ikin=0; ikin<_pars.nKinBins+1; ikin++){
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
      PlotOneTemplate(ikin, ieta);
    }//end of loop over ieta
  }//end of loop over ikin

}


void TTemplates::PlotOneTemplate(int ikin, int ieta)
{

 _fOutTemp->cd();

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
  float areaHistFakeMCtruth=0;
  for (int ib=1; ib<=_hTrue[ikin][ieta]->GetNbinsX(); ib++){
    areaHistTrue+=_hTrue[ikin][ieta]->GetBinContent(ib);
    areaHistFake+=_hFake[ikin][ieta]->GetBinContent(ib);
    areaHistFakeMCtruth+=_hFakeMCtruth[ikin][ieta]->GetBinContent(ib);
  }

  for (int ib=1; ib<=_hSumm[ikin][ieta]->GetNbinsX(); ib++){

    float contT=0;
    float err1T=0;
    float err2T=0;
    float contF=0;
    float err1F=0;
    float err2F=0;

    if (areaHistTrue==0);
    else{
      contT = _hTrue[ikin][ieta]->GetBinContent(ib)*_nTrueFromFitVal[ikin][ieta]/areaHistTrue;
      err1T = (_hTrue[ikin][ieta]->GetBinError(ib)*_nTrueFromFitVal[ikin][ieta])/areaHistTrue;
      err2T = (_hTrue[ikin][ieta]->GetBinContent(ib)*_nTrueFromFitErr[ikin][ieta])/areaHistTrue;
      _hTrue[ikin][ieta]->SetBinContent(ib,contT);
      _hTrue[ikin][ieta]->SetBinError(ib,sqrt(err1T*err1T+err2T*err2T));
    }
    if (areaHistFake==0);
    else {
      contF = _hFake[ikin][ieta]->GetBinContent(ib)*_nFakeFromFitVal[ikin][ieta]/areaHistFake;
      err1F = (_hFake[ikin][ieta]->GetBinError(ib)*_nFakeFromFitVal[ikin][ieta])/areaHistFake;
      err2F = (_hFake[ikin][ieta]->GetBinContent(ib)*_nFakeFromFitErr[ikin][ieta])/areaHistFake;
      _hFake[ikin][ieta]->SetBinContent(ib,contF);
      _hFake[ikin][ieta]->SetBinError(ib,sqrt(err1F*err1F+err2F*err2F));
    }
//    _hTrue[ikin][ieta]->SetBinContent(ib,contS);
//    _hTrue[ikin][ieta]->SetBinError(ib,sqrt(err1S*err1S+err2S*err2S));

//    _hFake[ikin][ieta]->SetBinContent(ib,contB);
//    _hFake[ikin][ieta]->SetBinError(ib,sqrt(err1B*err1B+err2B*err2B));

    float err = sqrt(err1T*err1T+err2T*err2T+err1F*err1F+err2F*err2F);
    _hSumm[ikin][ieta]->SetBinContent(ib,contT+contF);
    _hSumm[ikin][ieta]->SetBinError(ib,err);
    if (areaHistFakeMCtruth==0);
    else {
      contF = _hFakeMCtruth[ikin][ieta]->GetBinContent(ib)*_nFakeFromFitVal[ikin][ieta]/areaHistFakeMCtruth;
      err1F = (_hFakeMCtruth[ikin][ieta]->GetBinError(ib)*_nFakeFromFitVal[ikin][ieta])/areaHistFakeMCtruth;
      err2F = (_hFakeMCtruth[ikin][ieta]->GetBinContent(ib)*_nFakeFromFitErr[ikin][ieta])/areaHistFakeMCtruth;
      _hFakeMCtruth[ikin][ieta]->SetBinContent(ib,contF);
      _hFakeMCtruth[ikin][ieta]->SetBinError(ib,sqrt(err1F*err1F+err2F*err2F));
    }

  }  

  _hRatio[ikin][ieta]->Divide(_hSumm[ikin][ieta]);


    TString cName=_pars.strPlotsDir+_pars.strPlotsBaseName;
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

    if (_pars.isRooFit){
      _plotter[ikin][ieta]->SetTitle("");
      _plotter[ikin][ieta]->Draw();
    }// end of if (_pars.isRooFit)

    _hSumm[ikin][ieta]->SetLineColor(7);
    _hSumm[ikin][ieta]->SetLineWidth(2);
    _hSumm[ikin][ieta]->Draw("HIST same"); 
    _hSumm[ikin][ieta]->SetLineColor(2);  
    _hSumm[ikin][ieta]->Draw("EP same"); 
    _hTrue[ikin][ieta]->SetLineWidth(2);
    _hTrue[ikin][ieta]->SetLineColor(3);
    _hTrue[ikin][ieta]->Draw("EP same");
    _hFake[ikin][ieta]->SetLineWidth(2);
    _hFake[ikin][ieta]->SetLineColor(4);
    _hFake[ikin][ieta]->Draw("EP same");
    _hData[ikin][ieta]->SetLineWidth(2);
    _hData[ikin][ieta]->Draw("EP same");

    if (_pars.isMCclosureMode){
      float sum=0; 
      _hFakeMCtruth[ikin][ieta]->SetLineWidth(2);
      _hFakeMCtruth[ikin][ieta]->SetLineColor(7);
      _hFakeMCtruth[ikin][ieta]->SetFillColor(7);
      _hFakeMCtruth[ikin][ieta]->SetFillStyle(3004);
      _hFakeMCtruth[ikin][ieta]->Draw("HIST same");
      _hFakeMCtruth[ikin][ieta]->Draw("EP same");
    }

    textBin->Draw("same");
    textChi2->Draw("same");

    pad2->cd();
    _hRatio[ikin][ieta]->SetLineWidth(2);
    _hRatio[ikin][ieta]->SetStats(0);
    _hRatio[ikin][ieta]->GetYaxis()->SetLabelSize(0.1);
    _hRatio[ikin][ieta]->GetXaxis()->SetLabelSize(0.1);
    _hRatio[ikin][ieta]->GetXaxis()->SetTitleOffset(1.0);
    _hRatio[ikin][ieta]->GetXaxis()->SetTitleSize(0.12);
    _hRatio[ikin][ieta]->SetTitle(TString("; ")+_pars.varFit+TString(" ;"));
    _hRatio[ikin][ieta]->Draw();
    int nBins = _hRatio[ikin][ieta]->GetNbinsX();
    TLine* line = new TLine(_hRatio[ikin][ieta]->GetBinLowEdge(1),1,_hRatio[ikin][ieta]->GetBinLowEdge(nBins)+_hRatio[ikin][ieta]->GetBinWidth(nBins),1);
    line->SetLineWidth(2);
    line->SetLineStyle(9);
    line->Draw("same");
   
    cName.ReplaceAll("-1","total");
    if (_pars.isRooFit) cName+="_RooFit";
    else cName+="_ROOTfit";
    if (_pars.isMCclosureMode) cName+="_MCclosure.png";
    else cName+=".png";
    c1->SaveAs(cName);
    cName.ReplaceAll(".png",".pdf");
    c1->SaveAs(cName);
    cName.ReplaceAll(".pdf",".root");
    c1->SaveAs(cName);

//  cName+="plotter";
//  TCanvas* c2 = new TCanvas(cName);
//  _plotter[ikin][ieta]->Draw();
}// end of plotOneTemplate

TCut TTemplates::SidebandCut(int ikin, int ieta)
{
  float lim=_pars.sideband[ikin][ieta]; 
  float limUp=_pars.sidebandUp[ikin][ieta]; 
  TString strCut=_pars.varSideband;
  strCut+=">";
  strCut+=lim;
  strCut+=" && ";
  strCut+=_pars.varSideband;
  strCut+="<";
  strCut+=limUp;
  TCut cut(strCut);
//  if (ieta==_BARREL) cut = cut && _pars.cutBarrel;
//  if (ieta==_ENDCAP) cut = cut && _pars.cutEndcap;
  return ( cut ); 
}

TCut TTemplates::SidebandVarNominalCut(int ieta1)
{
  return _pars.cutSidebandVarNominalRange[ieta1];
}

TCut TTemplates::FitVarFitRangeCut(int ikin, int ieta)
{
  float lim=_pars.maxVarFit[ikin][ieta]; 
  TString strCut=_pars.varFit;
  strCut+="<";
  strCut+=lim;
  TCut cut(strCut);
//  if (ieta==_BARREL) cut = cut && _pars.cutBarrel;
//  if (ieta==_ENDCAP) cut = cut && _pars.cutEndcap;
  return ( cut );
}

void TTemplates::SaveYields()
{
  _pars.fOutForSave->cd();

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    //write fake yields computed from fit
    //which is jets to gamma background
    TString strTot=_pars.strFakeYieldsTot[ieta];
    TH1F hTotFakeYield(strTot,strTot,1,_pars.kinBinLims[0],_pars.kinBinLims[_pars.nKinBins]);
    TString str1D=_pars.strFakeYields1D[ieta];
    TH1F h1DFakeYield(str1D,str1D,_pars.nKinBins,_pars.kinBinLims);
    hTotFakeYield.SetBinContent(1,_nFakeYieldsVal[0][ieta]);
    hTotFakeYield.SetBinError(1,_nFakeYieldsErr[0][ieta]);
    for (int i=1; i<_pars.nKinBins+1; i++){  
      h1DFakeYield.SetBinContent(i,_nFakeYieldsVal[i][ieta]);
      h1DFakeYield.SetBinError(i,_nFakeYieldsErr[i][ieta]);
    }
    hTotFakeYield.Write(strTot); 
    h1DFakeYield.Write(str1D); 

    //write true yields computed from fit
    //which is true gamma background plus signal
    strTot=_pars.strTrueYieldsTot[ieta];
    TH1F hTotTrueYield(strTot,strTot,1,_pars.kinBinLims[0],_pars.kinBinLims[_pars.nKinBins]);
    str1D=_pars.strTrueYields1D[ieta];
    TH1F h1DTrueYield(str1D,str1D,_pars.nKinBins,_pars.kinBinLims);
    hTotTrueYield.SetBinContent(1,_nTrueYieldsVal[0][ieta]);
    hTotTrueYield.SetBinError(1,_nTrueYieldsErr[0][ieta]);
    for (int i=1; i<_pars.nKinBins+1; i++){  
      h1DTrueYield.SetBinContent(i,_nTrueYieldsVal[i][ieta]);
      h1DTrueYield.SetBinError(i,_nTrueYieldsErr[i][ieta]);
    }
    hTotTrueYield.Write(strTot); 
    h1DTrueYield.Write(str1D); 
    if (ieta==_COMMON) continue;
    for (int i=0; i<_pars.nKinBins+1; i++){  
      _hTrue[i][ieta]->Write(TString("templateTrue")+StrLabelKin(i)+StrLabelEta(ieta));
      _hFake[i][ieta]->Write(TString("templateFake")+StrLabelKin(i)+StrLabelEta(ieta));
      _hData[i][ieta]->Write(TString("dataToFit")+StrLabelKin(i)+StrLabelEta(ieta));
      _plotter[i][ieta]->Write(TString("plotter")+StrLabelKin(i)+StrLabelEta(ieta));
    }//end of loop over i
  }//end of loop over ieta

  if (_pars.fOutForSave->IsOpen()) _pars.fOutForSave->Close();
}

TCut TTemplates::CutKinBin(int ikin){

  if (ikin<0 || ikin>_pars.nKinBins){
    TCut cut("0");
    return cut;
  }
  float min;
  float max;
  if (ikin==0){
    min=_pars.kinBinLims[0];
    max=_pars.kinBinLims[_pars.nKinBins];
  }
  else{
    min=_pars.kinBinLims[ikin-1];
    max=_pars.kinBinLims[ikin];
  }

  TString ikinCutStr;
  ikinCutStr=_pars.varKin;
  ikinCutStr+=">";
  ikinCutStr+=min;
  ikinCutStr+=" && ";
  ikinCutStr+=_pars.varKin;
  ikinCutStr+="<=";
  ikinCutStr+=max;
  TCut cut(ikinCutStr);
  return cut;
}

TCut TTemplates::CutEtaBin(int ieta){
  if (ieta==_BARREL)      return _pars.cutBarrel;
  else if (ieta==_ENDCAP) return _pars.cutEndcap;
  else if (ieta==_COMMON) return (_pars.cutBarrel || _pars.cutEndcap);
  return "0";
}

float TTemplates::EffFromTree(int ikin, int ieta, bool noPrint)
{
  // only for true gamma
  // int nPassed;
  // int nPassedFitVar;
  // int nTot;
  float eff;
  TCut cutSidebandVar;
  cutSidebandVar=_pars.cutSidebandVarNominalRange[ieta];

  TCut cutKin=CutKinBin(ikin); 
  if (_pars.thresholdCombineTrueTemplates<_pars.kinBinLims[0]) cutKin="1";
  else if (_pars.kinBinLims[ikin-1]>_pars.thresholdCombineTrueTemplates && ikin>0){
     TString strCutKin=_pars.varKin;
     strCutKin+=">";
     strCutKin+=_pars.thresholdCombineTrueTemplates;
     cutKin=TCut(strCutKin);
  }

  TCut cutTot = _pars.cutAdd && cutSidebandVar && CutEtaBin(ieta) && cutKin && FitVarFitRangeCut(ikin,ieta);
  TCut cutPassed = _pars.cutAdd && cutSidebandVar && CutEtaBin(ieta) && cutKin && _pars.cutNominalExceptSidebandVar[ieta];
  std::cout<<setprecision(2)<<std::endl;
  std::cout<<StrLabelEta(ieta)<<": "<<std::endl;
  std::cout<<"cutAdd="<<_pars.cutAdd.GetTitle()<<std::endl;
  std::cout<<"cutEta="<<CutEtaBin(ieta).GetTitle()<<std::endl;
  std::cout<<"cutKin="<<CutKinBin(ikin).GetTitle()<<std::endl;
  std::cout<<"_pars.cutSidebandVarNominalRange[ieta1]="<<_pars.cutSidebandVarNominalRange[ieta].GetTitle()<<std::endl;
  std::cout<<"SidebandCut(ikin,ieta1)="<<SidebandCut(ikin,ieta).GetTitle()<<std::endl;
  std::cout<<"cutSidebandVar="<<cutSidebandVar.GetTitle()<<std::endl;
  std::cout<<"cutNominalExceptSidebandVar="<<_pars.cutNominalExceptSidebandVar[ieta].GetTitle()<<std::endl;
  std::cout<<"FitVarFitRangeCut(ikin,ieta)="<<FitVarFitRangeCut(ikin,ieta).GetTitle()<<std::endl;
  std::cout<<"cutPassed="<<cutPassed.GetTitle()<<std::endl;
  std::cout<<"cutTot="<<cutTot.GetTitle()<<std::endl;
  TString nameHistT=TString("histPassedT")+StrLabelKin(ikin)+StrLabelEta(ieta);
  TH1F* hTruePassed = new TH1F(nameHistT,nameHistT,1,-3.0,3.0);
  _pars.treeTrue->Draw(_pars.varPhoEta+TString(">>")+nameHistT,cutPassed*_pars.cutWeight,"goff");
  nameHistT=TString("histTotT")+StrLabelKin(ikin)+StrLabelEta(ieta);
  TH1F* hTrueTot = new TH1F(nameHistT,nameHistT,1,-3.0,3.0);
  _pars.treeTrue->Draw(_pars.varPhoEta+TString(">>")+nameHistT,cutTot*_pars.cutWeight,"goff");
  TH1F* hEff = (TH1F*)hTruePassed->Clone();
  hEff->Divide(hTrueTot);
  eff=hTruePassed->GetBinContent(1)/hTrueTot->GetBinContent(1);
  TMathTools math;
  float effErr=math.ErrOfTwoIndependent("x1/(x1+x2)", hTruePassed->GetBinContent(1), hTrueTot->GetBinContent(1)-hTruePassed->GetBinContent(1), hTruePassed->GetBinError(1), hTrueTot->GetBinError(1));
  std::cout<<"hTruePassed->GetBinContent(1)="<<hTruePassed->GetBinContent(1)<<"+-"<<hTruePassed->GetBinError(1)<<std::endl;
  std::cout<<"hTrueTot->GetBinContent(1)="<<hTrueTot->GetBinContent(1)<<"+-"<<hTrueTot->GetBinError(1)<<std::endl;
  std::cout<<"EffFromTree="<<eff<<"+-"<<effErr<<std::endl;
  delete hTruePassed;
  delete hTrueTot;
  return eff;
}// end of EffFromTree
