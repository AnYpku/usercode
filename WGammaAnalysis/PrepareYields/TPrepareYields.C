#include "TPrepareYields.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TFullCuts.h"
#include "../Include/TPhotonCuts.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVectorD.h"
#include "TLine.h"
#include "TLatex.h"

TPrepareYields::TPrepareYields(int year,int channel,int vgamma,int blind,TString varKin, int nKinBins, float* kinBinLims,int phoWP, bool noDDBkgComputation)
{
  _channel=channel;
  _vgamma=vgamma;
  _blind=blind;
  _noDDBkgComputation=noDDBkgComputation;
  _INPUT=new TAllInputSamples(_channel,_vgamma,"../Configuration/config.txt");
  _fOut=0;
  for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){
    _dataYields[ieta]=0;
    _sigMCYields[ieta]=0;
    _sigMCGenYields[ieta]=0;
    _DDFakeGammaYields[ieta]=0;
    _DDTrueGammaYields[ieta]=0;
    _signalDataYields[ieta]=0;
    _canvFakeDDvsMC[ieta]=0;
    _canvTrueDDvsMC[ieta]=0;
    _canvTotalDATAvsDDFakePlusTrue[ieta]=0;
    _canvTotalDATAvsSIGplusBKG1[ieta]=0;
    _canvTotalDATAvsSIGplusBKG2[ieta]=0;
    _canvSignalDATAvsMC[ieta]=0;
  }
  _varKin=varKin;
  _nKinBins=nKinBins;
  _kinBinLims = new float[nKinBins+1];
  for (int i=0; i<=nKinBins; i++){
    _kinBinLims[i]=kinBinLims[i];
  }
  TString strCutKin=varKin;
  strCutKin+=">=";
  strCutKin+=_kinBinLims[0];
  strCutKin+=" && ";
  strCutKin+=varKin;
  strCutKin+="<=";
  strCutKin+=_kinBinLims[nKinBins];
  _cutKin=strCutKin;
  std::cout<<"_cutKin="<<_cutKin.GetTitle()<<std::endl;
  _doLogX=0; _doLogY=0;
  _varKinLabel=_varKin;
  if (_varKin=="phoEt" || _varKin=="phoSCEt") _varKinLabel="Pt^{#gamma}, GeV";
//  if (_kinBinLims[0]==0) _doLogX=1;
//  else if (_kinBinLims[nKinBins]/_kinBinLims[0]>10) _doLogX=1;
//  else _doLogX=0;
  if (_varKin=="phoEt" || _varKin=="phoSCEt" || _varKin=="phoEtNoPhosphor"){
    _doLogX=1;
    _doLogY=1;
  }
  if (_kinBinLims[0]!=0)
    if (fabs(_kinBinLims[nKinBins])/fabs(_kinBinLims[0])<10){
      _doLogX=0;
      _doLogY=0;
    }
//  else _doLogY=0;
}

TPrepareYields::~TPrepareYields()
{
  delete _INPUT;
}

void TPrepareYields::PrepareYields()
{
  SetYields();
  if (!_noDDBkgComputation) SubtractBackground();
  for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){
    if (!_noDDBkgComputation) CompareFakeBkgDDvsMC(ieta);
    if (!_noDDBkgComputation) CompareTrueBkgDDvsMC(ieta);
    if (!_noDDBkgComputation) CompareTotalDATAvsDDFakePlusTrue(ieta);
    if (!_noDDBkgComputation) CompareTotalDATAvsSIGplusBKG(ieta);
    CompareTotalDATAvsMC(ieta);
    if (!_noDDBkgComputation) CompareSignalDATAvsMC(ieta);
  }
  if (!_noDDBkgComputation) StoreYields();
  PrintYields();
}

void TPrepareYields::SetYields()
{
  TString fOutName;
  fOutName = _config.GetYieldsFileName(_channel,_vgamma,_varKin);
  _fOut = new TFile(fOutName,"recreate");
  for (int i=0; i<_INPUT->nSources_; i++){
    for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++) 
      SetYieldsOneSource(i,ieta);
  }
  for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++) 
    if (!_noDDBkgComputation) SetYieldsDDBkgTemplate(ieta);
}

void TPrepareYields::SetYieldsOneSource(int iSource, int ieta)
{
  TString yieldsHistName;
  int sample = _INPUT->allInputs_[iSource].sample_;
  TString strSourceName = _INPUT->allInputs_[iSource].sourceName_;
  _cutWeight="weight";
  if (_blind==_config.BLIND_PRESCALE && sample!=_config.DATA){
    TString strCutWeight="weight";
    strCutWeight+="*";
    strCutWeight+=1.0/_config.GetBlindPrescale();
    _cutWeight=strCutWeight;
  }
  int selStage=_config.FULLY;
//  int selStage=_config.FSR;
  TString fInName = _config.GetSelectedName(selStage,_channel,_vgamma,_blind,sample,strSourceName);
  TFile fIn(fInName);
  if (!fIn.IsOpen()){
    std::cout<<"file "<<fInName<<" is not open"<<std::endl;
    return;
  }

  _fOut->cd();
  TTree* tr = (TTree*)fIn.Get("selectedEvents");
  TString yieldsName  = (_config.GetYieldsSelectedName(_config.ONEDI,ieta,_INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_));

  TString yieldsGenName = _config.GetYieldsSelectedSignalMCGenName(_config.ONEDI,ieta);

  _fOut->cd();
  if (_INPUT->allInputs_[iSource].sample_==TConfiguration::DATA){
    SetYieldsDATA(tr,yieldsName,iSource,ieta);
  }
  else if (_INPUT->allInputs_[iSource].sample_==TConfiguration::SIGMC){
    SetYieldsSIGMC(tr,yieldsName,yieldsGenName,iSource,ieta);
  }
  else if (_INPUT->allInputs_[iSource].sample_==TConfiguration::BKGMC){

    SetYieldsBKGMC_all(tr,yieldsName,iSource,ieta);

    if (_INPUT->allInputs_[iSource].sourceName_!="Wjets_to_lnu" &&
        _INPUT->allInputs_[iSource].sourceName_!="DYjets_to_ll" &&
        _INPUT->allInputs_[iSource].sourceName_!="ttbarjets"){

       SetYieldsBKGMC_true(tr,yieldsName,iSource,ieta);
    }
    if (_INPUT->allInputs_[iSource].sourceName_=="Wjets_to_lnu" ||
        _INPUT->allInputs_[iSource].sourceName_=="DYjets_to_ll" ||
        _INPUT->allInputs_[iSource].sourceName_=="ttbarjets" ||
        _INPUT->allInputs_[iSource].sourceName_=="multibosons"){

       SetYieldsBKGMC_fake(tr,yieldsName,iSource,ieta);
    }

  }//else if sample_==TConfiguration::BKGMC
}

void TPrepareYields::SetYieldsDATA(TTree* tr, TString yieldsName, int iSource, int ieta)
{
    _dataYields[ieta] = new TH1F(yieldsName,yieldsName,_nKinBins,_kinBinLims);
    _dataYields[ieta]->Sumw2();
    if (ieta==_config.COMMON) _strDATAName=_INPUT->allInputs_[iSource].sourceName_;
    if (ieta==_config.COMMON) _strDATALabel=_INPUT->allInputs_[iSource].sourceLatexLabel_;
    if (ieta==_config.COMMON) _colorDATA=_INPUT->allInputs_[iSource].color_;
    tr->Draw(_varKin+TString(">>")+yieldsName,(CutEta(ieta))*_cutWeight,"goff");
    std::cout<<"set _dataYieldTot: "<<std::endl;
    SetTotalYield(tr, CutEta(ieta) && _cutKin, _dataYieldTot[ieta], _dataYieldTotErr[ieta]);
}

void TPrepareYields::SetYieldsSIGMC(TTree* tr, TString yieldsName, TString yieldsGenName, int iSource, int ieta)
{
    _sigMCYields[ieta] = new TH1F(yieldsName,yieldsName,_nKinBins,_kinBinLims);
    _sigMCYields[ieta]->Sumw2();
    tr->Draw(_varKin+TString(">>")+yieldsName,( CutEta(ieta))*_cutWeight,"goff");
    _sigMCGenYields[ieta] = new TH1F(yieldsGenName,yieldsGenName,_nKinBins,_kinBinLims);
    _sigMCGenYields[ieta]->Sumw2();
    if (ieta==_config.COMMON) _strSIGMCName=_INPUT->allInputs_[iSource].sourceName_;
    if (ieta==_config.COMMON) _strSIGMCLabel=_INPUT->allInputs_[iSource].sourceLatexLabel_;
    if (ieta==_config.COMMON) _colorSIGMC=_INPUT->allInputs_[iSource].color_;
    tr->Draw(TString("phoGenEt>>")+yieldsGenName,(CutEta(ieta))*_cutWeight,"goff");
    std::cout<<"set _sigMCYieldTot: "<<std::endl;
    SetTotalYield(tr, (CutEta(ieta) && _cutKin), _sigMCYieldTot[ieta], _sigMCYieldTotErr[ieta]);
    std::cout<<"set _sigMCGenYieldTot: "<<std::endl;
    SetTotalYield(tr, ( CutEta(ieta) && _cutKin), _sigMCGenYieldTot[ieta], _sigMCGenYieldTotErr[ieta]);
}

void TPrepareYields::SetYieldsBKGMC_all(TTree* tr, TString yieldsName, int iSource, int ieta)
{
    _vecBkgMCYields[ieta].push_back( new TH1F(yieldsName,yieldsName,_nKinBins,_kinBinLims));
    _vecBkgMCYields[ieta].back()->Sumw2();
    if (ieta==_config.COMMON) _vecStrBkgMCNames.push_back(_INPUT->allInputs_[iSource].sourceName_);
    if (ieta==_config.COMMON) _vecStrBkgLabels.push_back(_INPUT->allInputs_[iSource].sourceLatexLabel_);
    if (ieta==_config.COMMON) _colorsBkg.push_back(_INPUT->allInputs_[iSource].color_);
    if (ieta==_config.COMMON) std::cout<<"iSource="<<iSource<<", source name="<<_vecStrBkgMCNames.back()<<std::endl;
    tr->Draw(_varKin+TString(">>")+yieldsName,( CutEta(ieta))*_cutWeight,"goff");
    float val;
    float err;
    std::cout<<"set _vecBkgMCYieldTot: #"<<iSource<<std::endl;
    SetTotalYield(tr, CutEta(ieta) && _cutKin, val, err);
    _vecBkgMCYieldTot[ieta].push_back(val);
    _vecBkgMCYieldTotErr[ieta].push_back(err);
}

void TPrepareYields::SetYieldsBKGMC_true(TTree* tr, TString yieldsName, int iSource, int ieta){
    _vecBkgMCTrueGammaYields[ieta].push_back( new TH1F(yieldsName+TString("true"),yieldsName+TString("true"),_nKinBins,_kinBinLims));
    _vecBkgMCTrueGammaYields[ieta].back()->Sumw2();
    if (ieta==_config.COMMON) _vecStrBkgMCTrueNames.push_back(_INPUT->allInputs_[iSource].sourceName_);
    if (ieta==_config.COMMON) _vecStrBkgTrueLabels.push_back(_INPUT->allInputs_[iSource].sourceLatexLabel_);
    if (ieta==_config.COMMON) _colorsBkgTrue.push_back(_INPUT->allInputs_[iSource].color_);
    if (ieta==_config.COMMON) std::cout<<"iSource="<<iSource<<", source name="<<_vecStrBkgMCTrueNames.back()<<std::endl;
    tr->Draw(_varKin+TString(">>")+yieldsName+TString("true"),_cutWeight*(_emptyPhoton.RangeGenTruePhoton()&&CutEta(ieta)),"goff");
    std::cout<<"set _vecBkgMCTrueGammaYieldTot: #"<<iSource<<std::endl;
    float val, err;
    SetTotalYield(tr,_emptyPhoton.RangeGenTruePhoton() && CutEta(ieta)&& _cutKin, val, err);
    _vecBkgMCTrueGammaYieldTot[ieta].push_back(val);
    _vecBkgMCTrueGammaYieldTotErr[ieta].push_back(err);    
}

void TPrepareYields::SetYieldsBKGMC_fake(TTree* tr, TString yieldsName, int iSource, int ieta){
    _vecBkgMCFakeGammaYields[ieta].push_back( new TH1F(yieldsName+TString("fake"),yieldsName+TString("fake"),_nKinBins,_kinBinLims));
    _vecBkgMCFakeGammaYields[ieta].back()->Sumw2();
    if (ieta==_config.COMMON) _vecStrBkgMCFakeNames.push_back(_INPUT->allInputs_[iSource].sourceName_);
    if (ieta==_config.COMMON) _vecStrBkgFakeLabels.push_back(_INPUT->allInputs_[iSource].sourceLatexLabel_);
    if (ieta==_config.COMMON) _colorsBkgFake.push_back(_INPUT->allInputs_[iSource].color_);
    if (ieta==_config.COMMON) std::cout<<"iSource="<<iSource<<", source name="<<_vecStrBkgMCFakeNames.back()<<std::endl;
    tr->Draw(_varKin+TString(">>")+yieldsName+TString("fake"),_cutWeight*(CutEta(ieta)),"goff");
    std::cout<<"set _vecBkgMCFakeGammaYieldTot: #"<<iSource<<std::endl;
    float val, err;
    SetTotalYield(tr,CutEta(ieta)&& _cutKin, val, err);
    _vecBkgMCFakeGammaYieldTot[ieta].push_back(val);
    _vecBkgMCFakeGammaYieldTotErr[ieta].push_back(err);  
}

void TPrepareYields::SetTotalYield(TTree* tr, TCut cut, float& val, float& err)
{
  _floatingHist = new TH1F("floatingHist","hist for temprorary storage of total yields",1,-1,10000);//inputFileNumber
  _floatingHist->Sumw2();
  tr->Draw("inputFileNumber>>floatingHist",cut*_cutWeight,"goff");
  val = _floatingHist->GetBinContent(1);
  err = _floatingHist->GetBinError(1);
  std::cout<<"val+-err="<<val<<"+-"<<err<<std::endl;
  delete _floatingHist;
}

void TPrepareYields::SetYieldsDDBkgTemplate(int ieta)
{
  TFile* fDDBkgTemplate= new TFile(_config.GetDDTemplateFileName(_channel,_vgamma,_varKin));
  if (!fDDBkgTemplate->IsOpen()){
    std::cout<<"file "<<_config.GetDDTemplateFileName(_channel,_vgamma,_varKin)<<" is not open"<<std::endl;
    return;
  }
  _fOut->cd();
  _DDFakeGammaYields[ieta] = (TH1F*)fDDBkgTemplate->Get(_config.GetYieldsDDTemplateFakeName(_config.ONEDI,ieta));
  _DDTrueGammaYields[ieta] = (TH1F*)fDDBkgTemplate->Get(_config.GetYieldsDDTemplateTrueName(_config.ONEDI,ieta));
 
  TString nameYieldTot = _config.GetYieldsDDTemplateFakeName(_config.TOTAL,ieta);
  TH1F* hT = (TH1F*)fDDBkgTemplate->Get(nameYieldTot);
  _DDFakeGammaYieldTot[ieta] = hT->GetBinContent(1);
  _DDFakeGammaYieldTotErr[ieta] = hT->GetBinError(1);
//  if (_blind==_config.BLIND_PRESCALE){
//    _DDFakeGammaYieldTot[ieta]*=1.0/_config.GetBlindPrescale();
//    _DDFakeGammaYieldTotErr[ieta]*=1.0/_config.GetBlindPrescale();
//    for (int ib=1; ib<=_DDFakeGammaYields[ieta]->GetNbinsX(); ib++){
//      float cont=_DDFakeGammaYields[ieta]->GetBinContent(ib);
//      float err=_DDFakeGammaYields[ieta]->GetBinError(ib);
//      _DDFakeGammaYields[ieta]->SetBinContent(ib,cont*1.0/_config.GetBlindPrescale());
//      _DDFakeGammaYields[ieta]->SetBinError(ib,err*1.0/_config.GetBlindPrescale());
//    }
//  }
  nameYieldTot = _config.GetYieldsDDTemplateTrueName(_config.TOTAL,ieta);
  hT = (TH1F*)fDDBkgTemplate->Get(nameYieldTot);
  _DDTrueGammaYieldTot[ieta] = hT->GetBinContent(1);
  _DDTrueGammaYieldTotErr[ieta] = hT->GetBinError(1);
//  if (_blind==_config.BLIND_PRESCALE){
//    _DDTrueGammaYieldTot[ieta]*=1.0/_config.GetBlindPrescale();
//    _DDTrueGammaYieldTotErr[ieta]*=1.0/_config.GetBlindPrescale();
//    for (int ib=1; ib<=_DDTrueGammaYields[ieta]->GetNbinsX(); ib++){
//      float cont=_DDTrueGammaYields[ieta]->GetBinContent(ib);
//      float err=_DDTrueGammaYields[ieta]->GetBinError(ib);
//      _DDTrueGammaYields[ieta]->SetBinContent(ib,cont*1.0/_config.GetBlindPrescale());
//      _DDTrueGammaYields[ieta]->SetBinError(ib,err*1.0/_config.GetBlindPrescale());
//    }
//  }

  std::cout<<"_DDFakeGammaYields:"<<std::endl;
  _DDFakeGammaYields[ieta]->Print();
  for (int ib=1; ib<=_DDFakeGammaYields[ieta]->GetNbinsX(); ib++){
    std::cout<<ib<<": "<<_DDFakeGammaYields[ieta]->GetBinContent(ib)<<"+-"<<_DDFakeGammaYields[ieta]->GetBinError(ib)<<std::endl;
  }
  std::cout<<"_DDTrueGammaYields:"<<std::endl;
  _DDTrueGammaYields[ieta]->Print();
  for (int ib=1; ib<=_DDTrueGammaYields[ieta]->GetNbinsX(); ib++){
    std::cout<<ib<<": "<<_DDTrueGammaYields[ieta]->GetBinContent(ib)<<"+-"<<_DDTrueGammaYields[ieta]->GetBinError(ib)<<std::endl;
  }
}

void TPrepareYields::SubtractBackground()
{
  std::cout<<"#####################"<<std::endl;
  std::cout<<"Subtract Background"<<std::endl;
 
  for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){
    TString yieldsName  = (_config.GetYieldsSignalName(_config.ONEDI,ieta));
    _signalDataYields[ieta] = (TH1F*)_DDTrueGammaYields[ieta]->Clone(yieldsName);
    _signalDataYieldTot[ieta]=_DDTrueGammaYieldTot[ieta];
    _signalDataYieldTotErr[ieta]=_DDTrueGammaYieldTotErr[ieta]*_DDTrueGammaYieldTotErr[ieta];
    _signalDataYields[ieta]->SetTitle(yieldsName);
    int nBins = _signalDataYields[ieta]->GetNbinsX();
    for (int i=0; i<_vecBkgMCTrueGammaYields[ieta].size(); i++){
      _signalDataYields[ieta]->Add(_vecBkgMCTrueGammaYields[ieta][i],-1);
      _signalDataYieldTot[ieta]-=_vecBkgMCTrueGammaYieldTot[ieta][i];
      _signalDataYieldTotErr[ieta]+=_vecBkgMCTrueGammaYieldTotErr[ieta][i]*_vecBkgMCTrueGammaYieldTotErr[ieta][i];
    }
    _signalDataYieldTotErr[ieta]=sqrt(_signalDataYieldTotErr[ieta]);
  }//end of loop over ieta
  std::cout<<"#####################"<<std::endl;
}

void TPrepareYields::CompareFakeBkgDDvsMC(int ieta)
{
  _fOut->cd();
  TString canvName="FakeDDvsMC";
  canvName+=StrLabelEta(ieta);
  _canvFakeDDvsMC[ieta] = new TCanvas(canvName,canvName,800,800);
  if (_doLogX) _canvFakeDDvsMC[ieta]->SetLogx();
  if (_doLogY) _canvFakeDDvsMC[ieta]->SetLogy();
  TLegend *legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHists","jets #rightarrow #gamma  bkg, DD vs MC estimation");
  TH1F* hSum=(TH1F*)_DDFakeGammaYields[ieta]->Clone("hSum_fakeGammaMC");; 
  hSum->Reset();
  
  for (int i=0; i<_vecBkgMCFakeGammaYields[ieta].size(); i++){
    _vecBkgMCFakeGammaYields[ieta][i]->SetLineColor(_colorsBkgFake[i]);
    _vecBkgMCFakeGammaYields[ieta][i]->SetFillColor(_colorsBkgFake[i]);
    _vecBkgMCFakeGammaYields[ieta][i]->SetMarkerColor(_colorsBkgFake[i]);
    _vecBkgMCFakeGammaYields[ieta][i]->SetStats(0);
    mcHists->Add(_vecBkgMCFakeGammaYields[ieta][i]);
    hSum->Add(_vecBkgMCFakeGammaYields[ieta][i]);
    legend->AddEntry(_vecBkgMCFakeGammaYields[ieta][i],"MC: "+_vecStrBkgFakeLabels[i]);
  }
 
  legend->AddEntry(_DDFakeGammaYields[ieta],"Data Driven");
  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);

  _DDFakeGammaYields[ieta]->SetLineWidth(2);
  _DDFakeGammaYields[ieta]->SetStats(0);
  _DDFakeGammaYields[ieta]->SetTitle("");
  CompareStackVsHist("jets #rightarrow #gamma background: DD vs MC",_DDFakeGammaYields[ieta], hSum, legend, _canvFakeDDvsMC[ieta], 1, mcHists);
}

void TPrepareYields::CompareTrueBkgDDvsMC(int ieta)
{
  _fOut->cd();
  TString canvName="TrueDDvsMC";
  canvName+=StrLabelEta(ieta);
  _canvTrueDDvsMC[ieta] = new TCanvas(canvName,canvName,800,800);
  if (_doLogX) _canvTrueDDvsMC[ieta]->SetLogx();
  if (_doLogY) _canvTrueDDvsMC[ieta]->SetLogy();
  TLegend *legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHists","jets #rightarrow #gamma  bkg, DD vs MC estimation");
  TH1F* hSum=(TH1F*)_sigMCYields[ieta]->Clone("hSum_trueGammaMC");
  _sigMCYields[ieta]->SetLineColor(_colorSIGMC);
  _sigMCYields[ieta]->SetFillColor(_colorSIGMC);
  _sigMCYields[ieta]->SetMarkerColor(_colorSIGMC);
  mcHists->Add(_sigMCYields[ieta]);
  
  for (int i=0; i<_vecBkgMCTrueGammaYields[ieta].size(); i++){
    _vecBkgMCTrueGammaYields[ieta][i]->SetLineColor(_colorsBkgTrue[i]);
    _vecBkgMCTrueGammaYields[ieta][i]->SetFillColor(_colorsBkgTrue[i]);
    _vecBkgMCTrueGammaYields[ieta][i]->SetMarkerColor(_colorsBkgTrue[i]);
    _vecBkgMCTrueGammaYields[ieta][i]->SetStats(0);
    mcHists->Add(_vecBkgMCTrueGammaYields[ieta][i]);
    hSum->Add(_vecBkgMCTrueGammaYields[ieta][i]);
    legend->AddEntry(_vecBkgMCTrueGammaYields[ieta][i],"MC: "+_vecStrBkgTrueLabels[i]);
  }

 
 legend->AddEntry(_DDTrueGammaYields[ieta],"Data Driven");

 legend->SetLineColor(1);
 legend->SetLineWidth(2);
 legend->SetFillColor(0);

  _DDTrueGammaYields[ieta]->SetLineWidth(2);
  _DDTrueGammaYields[ieta]->SetStats(0);

  _DDTrueGammaYields[ieta]->SetTitle("");
  CompareStackVsHist("signal + true gamma background: DD vs MC",_DDTrueGammaYields[ieta], hSum, legend, _canvTrueDDvsMC[ieta], 1, mcHists);
}

void TPrepareYields::CompareTotalDATAvsDDFakePlusTrue(int ieta)
{
  _fOut->cd();
  TString canvName="TotalDATAvsDDFakePlusTrue";
  canvName+=StrLabelEta(ieta);
  _canvTotalDATAvsDDFakePlusTrue[ieta] = new TCanvas(canvName,canvName,800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs DD Fake + True");

  mcHists->Add(_DDFakeGammaYields[ieta]);
  mcHists->Add(_DDTrueGammaYields[ieta]);
  TH1F* hSum = (TH1F*)_DDFakeGammaYields[ieta]->Clone("hSumDDFakePlusTrue");
  hSum->Add(_DDTrueGammaYields[ieta]);
  legend->AddEntry(_DDFakeGammaYields[ieta],"DD fake gamma");
  legend->AddEntry(_DDTrueGammaYields[ieta],"DD true gamma");
  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);

  legend->AddEntry(_dataYields[ieta],"data");
  _DDFakeGammaYields[ieta]->SetLineColor(4);
  _DDFakeGammaYields[ieta]->SetFillColor(4);
  _DDFakeGammaYields[ieta]->SetMarkerColor(4);
  _DDFakeGammaYields[ieta]->SetStats(0);
  _DDTrueGammaYields[ieta]->SetLineColor(8);
  _DDTrueGammaYields[ieta]->SetFillColor(8);
  _DDTrueGammaYields[ieta]->SetMarkerColor(8);
  _DDTrueGammaYields[ieta]->SetStats(0);

  _dataYields[ieta]->SetTitle("");
  CompareStackVsHist("Data vs DD fake + true", _dataYields[ieta], hSum, legend, _canvTotalDATAvsDDFakePlusTrue[ieta], 1, mcHists);

}

void TPrepareYields::CompareTotalDATAvsSIGplusBKG(int ieta)
{
  _fOut->cd();
  TString canvName="TotalDATAvsSIGplusBKG1";
  canvName+=StrLabelEta(ieta);
  _canvTotalDATAvsSIGplusBKG1[ieta] = new TCanvas(canvName,canvName,800,800);
  canvName="TotalDATAvsSIGplusBKG2";
  canvName+=StrLabelEta(ieta);
  _canvTotalDATAvsSIGplusBKG2[ieta] = new TCanvas(canvName,canvName,800,800);
  TLegend* legend1 = new TLegend(0.7,0.7,0.95,0.95);
  TLegend* legend2 = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists1 = new THStack("mcHistsTot1","DATA vs SIG plus BKG");
  THStack* mcHists2 = new THStack("mcHistsTot2","DATA vs SIG plus BKG");

  _DDFakeGammaYields[ieta]->SetLineColor(4);
  _DDFakeGammaYields[ieta]->SetFillColor(4);
  _DDFakeGammaYields[ieta]->SetMarkerColor(4);
  _DDFakeGammaYields[ieta]->SetStats(0);
  mcHists1->Add(_DDFakeGammaYields[ieta]);
  mcHists2->Add(_DDFakeGammaYields[ieta]);
  TH1F* hSum = (TH1F*)_DDFakeGammaYields[ieta]->Clone("hSumSignalPlusBkg");
  legend1->AddEntry(_DDFakeGammaYields[ieta],"DD fake gamma");
  legend2->AddEntry(_DDFakeGammaYields[ieta],"DD fake gamma");
  TH1F* hSumTrue = (TH1F*)_DDFakeGammaYields[ieta]->Clone("hSumTrue");
  hSumTrue->Reset();


  for (int i=0; i<_vecBkgMCTrueGammaYields[ieta].size(); i++){
        _vecBkgMCTrueGammaYields[ieta][i]->SetLineColor(_colorsBkgTrue[i]);
        _vecBkgMCTrueGammaYields[ieta][i]->SetFillColor(_colorsBkgTrue[i]);
        _vecBkgMCTrueGammaYields[ieta][i]->SetMarkerColor(_colorsBkgTrue[i]);
        _vecBkgMCTrueGammaYields[ieta][i]->SetStats(0);
        mcHists1->Add(_vecBkgMCTrueGammaYields[ieta][i]);
        hSum->Add(_vecBkgMCTrueGammaYields[ieta][i]);
        hSumTrue->Add(_vecBkgMCTrueGammaYields[ieta][i]);
        TString strLeg = "MC: "+_vecStrBkgTrueLabels[i];
        //strLeg.ReplaceAll("jets","#gamma");
        legend1->AddEntry(_vecBkgMCTrueGammaYields[ieta][i],strLeg);
  }
  hSumTrue->SetLineColor(8);
  hSumTrue->SetFillColor(8);
  hSumTrue->SetMarkerColor(8);
  hSumTrue->SetStats(0);
  mcHists2->Add(hSumTrue);
  legend2->AddEntry(hSumTrue,"MC: true gamma bkg");
  _sigMCYields[ieta]->SetLineColor(_colorSIGMC);
  _sigMCYields[ieta]->SetFillColor(_colorSIGMC);
  _sigMCYields[ieta]->SetMarkerColor(_colorSIGMC);
  _sigMCYields[ieta]->SetStats(0);
  mcHists1->Add(_sigMCYields[ieta]);
  mcHists2->Add(_sigMCYields[ieta]);
  hSum->Add(_sigMCYields[ieta]);
  legend1->AddEntry(_sigMCYields[ieta],"signal MC");
  legend2->AddEntry(_sigMCYields[ieta],"signal MC");


  legend1->SetLineColor(1);
  legend1->SetLineWidth(2);
  legend1->SetFillColor(0);

  legend1->AddEntry(_dataYields[ieta],"data");

  legend2->SetLineColor(1);
  legend2->SetLineWidth(2);
  legend2->SetFillColor(0);

  legend2->AddEntry(_dataYields[ieta],"data");

  _dataYields[ieta]->SetTitle("");
  CompareStackVsHist("Data vs Signal + Bkgr (sources listed)", _dataYields[ieta], hSum, legend1, _canvTotalDATAvsSIGplusBKG1[ieta], 1, mcHists1);
  CompareStackVsHist("Data vs Signal + Bkgr",_dataYields[ieta], hSum, legend2, _canvTotalDATAvsSIGplusBKG2[ieta], 1, mcHists2);
}

void TPrepareYields::CompareTotalDATAvsMC(int ieta)
{
  _fOut->cd();
  TString canvName="TotalDATAvsMC";
  canvName+=StrLabelEta(ieta);
  _canvTotalDATAvsMC [ieta]= new TCanvas(canvName,canvName,800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs MC");
  TH1F* hSum=(TH1F*)_sigMCYields[ieta]->Clone("hSum_WholeMC");
  

  for (int i=0; i<_vecBkgMCYields[ieta].size(); i++){
    std::cout<<"CompareTotalDATAvsMC: processing iSource "<<_INPUT->allInputs_[i+2].sourceName_<<std::endl;
    _vecBkgMCYields[ieta][i]->SetLineColor(_colorsBkg[i]);
    _vecBkgMCYields[ieta][i]->SetFillColor(_colorsBkg[i]);
    _vecBkgMCYields[ieta][i]->SetMarkerColor(_colorsBkg[i]);
    _vecBkgMCYields[ieta][i]->SetStats(0);
    mcHists->Add(_vecBkgMCYields[ieta][i]);
    hSum->Add(_vecBkgMCYields[ieta][i]);
    TString strLeg = "MC: "+_vecStrBkgLabels[i];
    legend->AddEntry(_vecBkgMCYields[ieta][i],strLeg);
  }



  mcHists->Add(_sigMCYields[ieta]);
  int color = _INPUT->allInputs_[1].color_;
  _sigMCYields[ieta]->SetLineColor(color);
  _sigMCYields[ieta]->SetFillColor(color);
  _sigMCYields[ieta]->SetMarkerColor(color);
  TString strLegSig = "sigMC: "+_strSIGMCLabel;
  legend->AddEntry(_sigMCYields[ieta],strLegSig);

  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);
//  mcHists->GetXaxis()->SetMoreLogLabels();
  _dataYields[ieta]->SetStats(0);
  legend->AddEntry(_dataYields[ieta],"data");

  _dataYields[ieta]->SetTitle("");
  CompareStackVsHist("Total DATA vs total MC", _dataYields[ieta], hSum, legend, _canvTotalDATAvsMC[ieta], 1, mcHists);
}

void TPrepareYields::CompareSignalDATAvsMC(int ieta)
{
  TString canvName="SignalDATAvsMC";
  canvName+=StrLabelEta(ieta);
  _canvSignalDATAvsMC[ieta] = new TCanvas(canvName,canvName,800,800);

  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  legend->AddEntry(_signalDataYields[ieta],"signal data");
  legend->AddEntry(_sigMCYields[ieta],"signal MC");
  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);

  _signalDataYields[ieta]->SetTitle(""); 
  CompareStackVsHist("bkrg subtracted DATA vs signal MC", _signalDataYields[ieta],_sigMCYields[ieta],legend,_canvSignalDATAvsMC[ieta]);
}

void TPrepareYields::CompareStackVsHist(TString plotTitle, TH1F* hist1, TH1F* hist2, TLegend* legend, TCanvas* canv, bool isStack, THStack* stack)
//if isStack then hist2 must be sum of histograms in the stack
{
  canv->Divide(1,2);
  TPad* pad1 = (TPad*)canv->GetPad(1);
  TPad* pad2 = (TPad*)canv->GetPad(2);
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
  if (_doLogX) pad1->SetLogx();
  if (_doLogY) pad1->SetLogy();
  pad1->cd();

  float max = hist1->GetMaximum();
  if (hist2->GetMaximum()>max)
    max=hist2->GetMaximum();
  float min = 0.3*hist1->GetMinimum();
  if (min>0.3*hist2->GetMinimum())
    min=0.3*hist2->GetMinimum();
  if (!_doLogX) min=0;
  max=1.2*max;

  if (isStack) hist2->SetLineColor(2);
  if (isStack) hist2->SetFillStyle(0);
  hist1->GetYaxis()->SetRangeUser(min,max);
  hist1->SetStats(0);
  if (_doLogY) {hist1->GetYaxis()->SetMoreLogLabels(); hist1->GetYaxis()->SetNoExponent();}
  hist1->Draw("P");
  if (isStack) stack->Draw("HIST same"); 
  else hist2->Draw("HIST same");

//  if (isStack) hist2->Draw("HIST same");
//  legend->AddEntry(hist2,"hsum","l");

  hist1->SetLineWidth(2);
  hist1->Draw("EP same");
  legend->Draw("same");

  TLatex* latexTitle = new TLatex(0.15,0.95,plotTitle);
  latexTitle->SetNDC();
  latexTitle->Draw("same");

  pad2->cd();
  if (_doLogX) pad2->SetLogx();
  TString hRName = "hRatio";
  hRName+=hist1->GetTitle();
  hRName+=hist2->GetTitle();
  TH1F* hRatio = (TH1F*)hist1->Clone(hRName);
  hRatio->Divide(hist2);
  max=1;min=1;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    if (hRatio->GetBinContent(ib)+hRatio->GetBinError(ib)>max) 
      max=hRatio->GetBinContent(ib)+hRatio->GetBinError(ib);
    if (hRatio->GetBinContent(ib)-hRatio->GetBinError(ib)<min) 
      min=hRatio->GetBinContent(ib)-hRatio->GetBinError(ib);
  }

  std::cout<<"ratio min = "<<min<<", ratio max = "<<max<<std::endl;
  hRatio->GetYaxis()->SetRangeUser(min,max);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->GetXaxis()->SetMoreLogLabels();
  hRatio->GetXaxis()->SetNoExponent();

  hRatio->Draw();
  hRatio->SetTitle(TString("; ")+_varKinLabel+TString(" ;"));
  int nBins = hRatio->GetNbinsX();
  TLine* line = new TLine(hRatio->GetBinLowEdge(1),1,hRatio->GetBinLowEdge(nBins)+hRatio->GetBinWidth(nBins),1);
  line->SetLineWidth(2);
  line->SetLineStyle(9);
  line->Draw("same");

  TString nameForSave="canv";
  nameForSave+="_";
  nameForSave+=_varKin;
  nameForSave+="_";
  nameForSave+=canv->GetTitle();
  nameForSave.ReplaceAll(" ","");
  nameForSave+=".png";
  canv->SaveAs(nameForSave);
}


void TPrepareYields::PrintYields()
{
  int nBins = _nKinBins;
  for (int ieta=_config.BARREL; ieta<=_config.COMMON; ieta++){

    std::cout<<"etaBin = "<<StrLabelEta(ieta)<<std::endl;

    //BKG MC yields
    std::cout<< "Bkg MC gamma: "<<std::endl;
    for (int i=0; i<_vecBkgMCYieldTot[ieta].size(); i++){
      TString name = _vecStrBkgMCNames[i];
      name+=": ";
      PrintYieldsOne(name, _vecBkgMCYieldTot[ieta][i], _vecBkgMCYieldTotErr[ieta][i], _vecBkgMCYields[ieta][i]);
    }

    PrintYieldsOne("DATA_______________", _dataYieldTot[ieta], _dataYieldTotErr[ieta], _dataYields[ieta]);

    if (!_noDDBkgComputation) 
      PrintYieldsOne("DD fake from fit___", _DDFakeGammaYieldTot[ieta], _DDFakeGammaYieldTotErr[ieta], _DDFakeGammaYields[ieta]);

    //BKG MC fake gamma yields
    std::cout<< "Bkg MC fake gamma: "<<std::endl;
    for (int i=0; i<_vecBkgMCFakeGammaYieldTot[ieta].size(); i++){
      TString name = _vecStrBkgMCFakeNames[i];
      name+=": ";
      PrintYieldsOne(name, _vecBkgMCFakeGammaYieldTot[ieta][i], _vecBkgMCFakeGammaYieldTotErr[ieta][i], _vecBkgMCFakeGammaYields[ieta][i]);
    }

    if (!_noDDBkgComputation)     
       PrintYieldsOne("DD true from fit___", _DDTrueGammaYieldTot[ieta], _DDTrueGammaYieldTotErr[ieta], _DDTrueGammaYields[ieta]);

    //BKG MC true gamma yields
    std::cout<< "Bkg MC true gamma: "<<std::endl;
    for (int i=0; i<_vecBkgMCTrueGammaYieldTot[ieta].size(); i++){
      TString name = _vecStrBkgMCTrueNames[i];
      name+=": ";
      PrintYieldsOne(name, _vecBkgMCTrueGammaYieldTot[ieta][i], _vecBkgMCTrueGammaYieldTotErr[ieta][i], _vecBkgMCTrueGammaYields[ieta][i]);
    }

    if (!_noDDBkgComputation)
      PrintYieldsOne("bkg subtracted DATA", _signalDataYieldTot[ieta], _signalDataYieldTotErr[ieta], _signalDataYields[ieta]);

    PrintYieldsOne("signal MC__________", _sigMCYieldTot[ieta], _sigMCYieldTotErr[ieta], _sigMCYields[ieta]);

  }//end of loop over ieta
}

void TPrepareYields::PrintYieldsOne(TString strYieldType, float totVal, float totErr, TH1F* hist)
{
  std::cout<<std::setprecision(0);
  std::cout<<std::setw(5);
  std::cout<<std::fixed;
  std::cout<<strYieldType<<": "<<totVal<<"+-"<<totErr<<"; ";
  for (int ib=1; ib<=hist->GetNbinsX(); ib++){
    std::cout<<hist->GetBinContent(ib)<<"+-"<<hist->GetBinError(ib)<<", ";
  }
  std::cout<<hist->GetSumOfWeights()<<std::endl;
}

void TPrepareYields::StoreYields()
{
  _fOut->cd();
  _signalDataYields[_config.COMMON]->Write();
//  _canvFakeDDvsMC->Write();
  TString yieldsName  = (_config.GetYieldsSignalName(_config.TOTAL));
  TH1F hT = TH1F(yieldsName,yieldsName,1,_config.GetPhoPtMin(),_config.GetPhoPtMax());
  hT.SetBinContent(1,_signalDataYieldTot[_config.COMMON]);
  hT.SetBinError(1,_signalDataYieldTotErr[_config.COMMON]);
  hT.Write(yieldsName);
}

TCut TPrepareYields::CutEta(int ieta)
{
  if (ieta==_config.BARREL) return _emptyPhoton.RangeBarrel();
  if (ieta==_config.ENDCAP) return _emptyPhoton.RangeEndcap();
  return "1";
}

TString TPrepareYields::StrLabelEta(int ieta)
{
  if (ieta==_config.BARREL) return "_Barrel_";
  if (ieta==_config.ENDCAP) return "_Endcap_";
  return "_EtaCommon_";
}
