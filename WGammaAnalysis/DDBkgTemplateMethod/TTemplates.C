#include "TTemplates.h"
  //this class
#include "../Include/TPhotonCuts.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"

#include <string>
#include <vector>

#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooChi2Var.h"
#include "RooAbsReal.h"
#include "TCanvas.h"

using namespace RooFit ;

TTemplates::TTemplates(int channel)
{
  channel_=channel;
  fOutForSave_ = new TFile(config_.GetFractionsDDTemplateBkgFileName(channel_),"recreate");
}

TTemplates::~TTemplates()
{
}

void TTemplates::ComputeBackground()
{
  SetHists();
  //PrintHists();
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    FitOne(i,config_.BARREL);
    FitOne(i,config_.ENDCAP);
  }
  PrintChi2();
  SaveBkgYields();
}

void TTemplates::SetHists()
{
  fSig_ = new TFile(config_.GetSelectedPreliminaryName(channel_,config_.SIGMC));
  fBkg_ = new TFile(config_.GetSelectedPreliminaryName(channel_,config_.DATA));

  TTree* treeSignalInitial = (TTree *)fSig_->Get("selectedEvents");
  TTree* treeBkgrawInitial = (TTree *)fBkg_->Get("selectedEvents");

  fOutForSave_->cd();
  int nBins = config_.GetNPhoPtBins();
  //trData_ = new TTree*[nBins];
  //trSignal_ = new TTree*[nBins];
  //trBkg_ = new TTree*[nBins];
  //trLeakage_ = new TTree*[nBins];
  hBarrelData_ = new TH1F*[nBins];
  hBarrelSignal_ = new TH1F*[nBins];
  hBarrelBkg_ = new TH1F*[nBins];
  hBarrelLeakage_ = new TH1F*[nBins];
  hEndcapData_ = new TH1F*[nBins];
  hEndcapSignal_ = new TH1F*[nBins];
  hEndcapBkg_ = new TH1F*[nBins];
  hEndcapLeakage_ = new TH1F*[nBins];
  TPhotonCuts emptyPhoton;
  vector <float> phoPtBinLimits = config_.GetPhoPtBinsLimits();
  for (int i=0; i<nBins; i++){
    TString ptBinCutStr;
    ptBinCutStr="phoEt>";
    ptBinCutStr+=phoPtBinLimits[i];
    ptBinCutStr+=" && phoEt<=";
    ptBinCutStr+=phoPtBinLimits[i+1];
    TCut ptBinCut(ptBinCutStr);
    TCut phoChIsoCut = emptyPhoton.RangePhoChIso();
    TCut barrelCut = emptyPhoton.RangeBarrel();
    TCut endcapCut = emptyPhoton.RangeEndcap();
    TString name;
    TCut cut;
    TString cutStr;

    name="hBarrelSig";
    name+=i;
    cut = barrelCut && ptBinCut && phoChIsoCut; 
    cutStr=cut.GetTitle();
    hBarrelSignal_[i] = new TH1F(name,name,
      nBinsBarrel_,sIEtaIEtaBarrelMin_,sIEtaIEtaBarrelMax_);
    treeSignalInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hBarrelBkg";
    name+=i;
    cut = barrelCut && ptBinCut && !phoChIsoCut;
    cutStr=cut.GetTitle();
    hBarrelBkg_[i] = new TH1F(name,name,
      nBinsBarrel_,sIEtaIEtaBarrelMin_,sIEtaIEtaBarrelMax_);
    treeBkgrawInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hBarrelLeakage";
    name+=i;
    cut = barrelCut && ptBinCut && !phoChIsoCut;
    cutStr=cut.GetTitle();
    hBarrelLeakage_[i] = new TH1F(name,name,
      nBinsBarrel_,sIEtaIEtaBarrelMin_,sIEtaIEtaBarrelMax_);
    treeSignalInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hBarrelData";
    name+=i;
    cut = barrelCut && ptBinCut && phoChIsoCut;
    cutStr=cut.GetTitle();
    hBarrelData_[i] = new TH1F(name,name,
      nBinsBarrel_,sIEtaIEtaBarrelMin_,sIEtaIEtaBarrelMax_);
    treeBkgrawInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hEndcapSig";
    name+=i;
    cut = endcapCut && ptBinCut && phoChIsoCut; 
    cutStr=cut.GetTitle();
    hEndcapSignal_[i] = new TH1F(name,name,
      nBinsEndcap_,sIEtaIEtaEndcapMin_,sIEtaIEtaEndcapMax_);
    treeSignalInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hEndcapBkg";
    name+=i;
    cut = endcapCut && ptBinCut && !phoChIsoCut;
    cutStr=cut.GetTitle();
    hEndcapBkg_[i] = new TH1F(name,name,
      nBinsEndcap_,sIEtaIEtaEndcapMin_,sIEtaIEtaEndcapMax_);
    treeBkgrawInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hEndcapLeakage";
    name+=i;
    cut = endcapCut && ptBinCut && !phoChIsoCut;
    cutStr=cut.GetTitle();
    hEndcapLeakage_[i] = new TH1F(name,name,
      nBinsEndcap_,sIEtaIEtaEndcapMin_,sIEtaIEtaEndcapMax_);
    treeSignalInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    name="hEndcapData";
    name+=i;
    cut = endcapCut && ptBinCut && phoChIsoCut;
    cutStr=cut.GetTitle();
    hEndcapData_[i] = new TH1F(name,name,
      nBinsEndcap_,sIEtaIEtaEndcapMin_,sIEtaIEtaEndcapMax_);
    treeBkgrawInitial->Draw(TString("phoSigmaIEtaIEta>>")+name,"("+cutStr+")*weight","goff");

    //subtract leakage
    hBarrelBkg_[i]->Add(hBarrelLeakage_[i],-1);
    hEndcapBkg_[i]->Add(hEndcapLeakage_[i],-1);
  

  }//end of loop over eta bins

}

void TTemplates::PrintHists()
{
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    hBarrelData_[i]->Print();
    hBarrelSignal_[i]->Print();
    hBarrelBkg_[i]->Print();
    hBarrelLeakage_[i]->Print();
    hEndcapData_[i]->Print();
    hEndcapSignal_[i]->Print();
    hEndcapBkg_[i]->Print();
    hEndcapLeakage_[i]->Print();
  }
}

void TTemplates::FitOne(int ptBin, int etaBin)
{
  // RooFit variables
  float varMin, varMax;
  int nBins;
  TH1F* hSig = 0;
  TH1F* hBkg = 0;
  TH1F* hData= 0;
  if (etaBin==config_.BARREL){
    varMin = sIEtaIEtaBarrelMin_;
    varMax = sIEtaIEtaBarrelMax_;
    nBins = nBinsBarrel_;
    hSig  = hBarrelSignal_[ptBin];
    hBkg  = hBarrelBkg_[ptBin];
    hData = hBarrelData_[ptBin];
  }
  if (etaBin==config_.ENDCAP){
    varMin = sIEtaIEtaEndcapMin_;
    varMax = sIEtaIEtaEndcapMax_;
    nBins = nBinsEndcap_;
    hSig  = hEndcapSignal_[ptBin];
    hBkg  = hEndcapBkg_[ptBin];
    hData = hEndcapData_[ptBin];
  }
  RooRealVar sihihVar("phoSigmaIEtaIEta","#sigma_{i#etai#eta}", varMin, varMax);
  RooArgList sihihArgList;
  sihihArgList.add(sihihVar);
  RooArgSet sihihArgSet;
  sihihArgSet.add(sihihVar);
  //create PDFs
  RooDataHist signalDataHist("signalDataHist","signal RooDataHist", sihihArgList, hSig);
  RooHistPdf signalPdf("signalPdf","#sigma_{i#etai#eta} of signal", sihihArgSet, signalDataHist);
  RooDataHist backgroundDataHist("backgroundDataHist", "background RooDataHist", sihihArgList, hBkg);
  RooHistPdf backgroundPdf("backgroundPdf","#sigma_{i#etai#eta} of background", sihihArgSet, backgroundDataHist);

  //data
  RooDataHist dataDataHist("data sihih", "#sigma_{i#etai#eta} in Data", sihihArgList, hData);
  //signal fraction parameter
  RooRealVar bkgFractionVar("bkg fraction","bkg fraction", 0.5, 0.0, 1.0);
  RooAddPdf sumPdf("totalPdf","signal and background", backgroundPdf, signalPdf, bkgFractionVar);
  //fit
  sumPdf.fitTo( dataDataHist );
  //calc chi squared
  RooChi2Var chi2("chi2","chi2",sumPdf,dataDataHist);

  TPhotonCuts emptyPhoton;
  float sihihCut = 0;
  if (etaBin==config_.BARREL) sihihCut = emptyPhoton.GetPhoSigmaIEtaIEtaCutB(emptyPhoton.GetWP());
  else if (etaBin==config_.ENDCAP) sihihCut = emptyPhoton.GetPhoSigmaIEtaIEtaCutE(emptyPhoton.GetWP());
  sihihVar.setRange("inSelection",0,sihihCut);
  RooAbsReal* integralSumPdf = sumPdf.createIntegral(sihihVar,NormSet(sihihVar),Range("inSelection"));
  RooAbsReal* integralBkgPdf = backgroundPdf.createIntegral(sihihVar,NormSet(sihihVar),Range("inSelection"));

  if (etaBin==config_.BARREL){
    chi2Barrel_.push_back(chi2.getVal()/nBinsBarrel_);
    if (integralSumPdf->getVal()==0) bkgFractionBarrel_.push_back(-1);
    else bkgFractionBarrel_.push_back(integralBkgPdf->getVal()/integralSumPdf->getVal());
  }
  else if (etaBin==config_.ENDCAP){
    chi2Endcap_.push_back(chi2.getVal()/nBinsEndcap_); 
    if (integralSumPdf->getVal()==0) bkgFractionEndcap_.push_back(-1);
    else bkgFractionEndcap_.push_back(integralBkgPdf->getVal()/integralSumPdf->getVal());
  }
  //plot results
  TString cName="c";
  cName+=etaBin;
  cName+=ptBin;
  TCanvas* c1 = new TCanvas(cName,cName, 800, 600);
  RooPlot* plotter = new RooPlot(sihihVar,varMin,varMax,nBins); 
  dataDataHist.plotOn(plotter);
  sumPdf.plotOn(plotter,Name("sum"),LineColor(kRed));
  sumPdf.plotOn(plotter,Components("signalPdf"), Name("signal"),
  LineColor(kGreen));
  sumPdf.plotOn(plotter, Components("backgroundPdf"),Name("background"),LineColor(kBlue));
  sumPdf.paramOn(plotter);

  plotter->Draw();
  TString plotName(config_.GetTemplatePicNameBase(ptBin,etaBin));
  plotName+=ptBin;
  plotName+="_";
  plotName+=etaBin;
  plotName+=".png";
  fOutForSave_->cd();
  c1->Write(config_.GetTemplatePicNameBase(ptBin,etaBin));
  delete plotter;
}

void TTemplates::PrintChi2()
{
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    std::cout<<"ptBin="<<i<<", etaBin=0, chi2="<<chi2Barrel_[i]<<", bkg frac="<<bkgFractionBarrel_[i]<<std::endl;
    std::cout<<"ptBin="<<i<<", etaBin=1, chi2="<<chi2Endcap_[i]<<", bkg frac="<<bkgFractionEndcap_[i]<<std::endl;
  }
}

void TTemplates::SaveBkgYields()
{
  fOutForSave_->cd();
  TString bkgFractionsBName = (config_.GetFractionsDDTemplateBkgHistName(config_.BARREL));
  TString bkgFractionsEName = (config_.GetFractionsDDTemplateBkgHistName(config_.ENDCAP));
  vector <float> vecPtBins = config_.GetPhoPtBinsLimits();
  float ptBinsLimits[config_.GetNPhoPtBins()+1];
  for (int i=0; i<config_.GetNPhoPtBins()+1; i++)
    ptBinsLimits[i] = vecPtBins[i];
  fractionsBkgB_ = new TH1F(bkgFractionsBName,bkgFractionsBName,
                config_.GetNPhoPtBins(),ptBinsLimits);
  fractionsBkgE_ = new TH1F(bkgFractionsEName,bkgFractionsEName,
                config_.GetNPhoPtBins(),ptBinsLimits);
  for (int i=1; i<config_.GetNPhoPtBins()+1; i++){
    fractionsBkgB_->SetBinContent(i,bkgFractionBarrel_[i-1]);
    fractionsBkgE_->SetBinContent(i,bkgFractionEndcap_[i-1]);

    hBarrelBkg_[i-1]->Write();
    hEndcapBkg_[i-1]->Write();
    hBarrelSignal_[i-1]->Write();
    hEndcapSignal_[i-1]->Write();
  }

  fractionsBkgB_->Write();
  fractionsBkgE_->Write();

  delete fOutForSave_;
}
