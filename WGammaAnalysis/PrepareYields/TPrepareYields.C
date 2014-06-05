#include "TPrepareYields.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TFullCuts.h"
#include "../Include/TPhotonCuts.h"

#include <string>
#include <vector>
#include <iostream>

#include "TFile.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVectorD.h"
#include "TLine.h"

TPrepareYields::TPrepareYields(int channel,int blind,TString varKin, int nKinBins, float* kinBinLims, bool isMetCutOptimization, int phoWP, TString phoIsoBase)
{
  _channel=channel;
  _blind=blind;
  _isMetCutOptimization=isMetCutOptimization;
  _cutAdd="1";
  if (isMetCutOptimization)
    _cutAdd=_emptyPhoton.RangePhoton(phoWP, phoIsoBase);
  _INPUT=new TAllInputSamples(_channel,"../Configuration/config.txt");
  _fOut=0;
  _dataYields=0;
  _sigMCYields=0;
  _sigMCGenYields=0;
  _bkgDDFakeGammaYields=0;
  _signalDataYields=0;
//  _legend=0;
  _canvDDvsMC=0;
  _canvTotalDATAvsSIGplusBKG=0;
  _canvSignalDATAvsMC=0;
  _varKin=varKin;
  _nKinBins=nKinBins;
  _kinBinLims = new float[nKinBins+1];
  for (int i=0; i<=nKinBins; i++){
    _kinBinLims[i]=kinBinLims[i];
  }
  _doLogX=0; _doLogY=0;
  _varKinLabel=_varKin;
  if (_varKin=="phoEt" || _varKin=="phoSCEt") _varKinLabel="Pt^{#gamma}, GeV";
//  if (_kinBinLims[0]==0) _doLogX=1;
//  else if (_kinBinLims[nKinBins]/_kinBinLims[0]>10) _doLogX=1;
//  else _doLogX=0;
  if (_varKin=="phoEt" || _varKin=="phoSCEt"){
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
  SubtractBackground();
  PrintYields();
  StoreYields();
  CompareFakeBkgDDvsMC();
  CompareTotalDATAvsSIGplusBKG();
  CompareTotalDATAvsMC();
  CompareSignalDATAvsMC();
}

void TPrepareYields::SetYields()
{
  TString fOutName;
  fOutName = _config.GetYieldsFileName(_channel);
  _fOut = new TFile(fOutName,"recreate");
  for (int i=0; i<_INPUT->nSources_; i++){
    SetYieldsOneSource(i);
  }
  SetYieldsDDBkgTemplate();
}

void TPrepareYields::SetYieldsOneSource(int iSource)
{


  TString yieldsHistName;

  int sample = _INPUT->allInputs_[iSource].sample_;
  TString strSourceName = _INPUT->allInputs_[iSource].sourceName_;

  TCut cutWeight="weight";
  if (_blind==_config.BLIND_PRESCALE && sample!=_config.DATA){
    TString strCutWeight="weight";
    strCutWeight+="*";
    strCutWeight+=1.0/_config.GetBlindPrescale();
    cutWeight=strCutWeight;
  }

  int selStage=_config.FULLY;
  if (_isMetCutOptimization) selStage=_config.PRELIMINARY_FOR_MET_CUT;
  TString fInName = _config.GetSelectedName(selStage,_channel,_blind,sample,strSourceName);
  TFile fIn(fInName);
  if (!fIn.IsOpen()){
    std::cout<<"file "<<fInName<<" is not open"<<std::endl;
    return;
  }
  _fOut->cd();
  TTree* tr = (TTree*)fIn.Get("selectedEvents");
  TString yieldsName  = (_config.GetYieldsSelectedName(_config.ONEDI,_INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_));

  TString yieldsGenName = _config.GetYieldsSelectedSignalMCGenName(_config.ONEDI);

  _fOut->cd();
  if (_INPUT->allInputs_[iSource].sample_==TConfiguration::DATA){
    _dataYields = new TH1F(yieldsName,yieldsName,
                _nKinBins,_kinBinLims);
    _dataYields->Sumw2();
    tr->Draw(_varKin+TString(">>")+yieldsName,_cutAdd*cutWeight,"goff");
    std::cout<<"set _dataYieldTot: "<<std::endl;
    SetTotalYield(tr, _cutAdd*cutWeight, _dataYieldTot, _dataYieldTotErr);
  }
  else if (_INPUT->allInputs_[iSource].sample_==TConfiguration::SIGMC){
    _sigMCYields = new TH1F(yieldsName,yieldsName,
                _nKinBins,_kinBinLims);
    _sigMCYields->Sumw2();
    tr->Draw(_varKin+TString(">>")+yieldsName,_cutAdd*cutWeight,"goff");
    _sigMCGenYields = new TH1F(yieldsGenName,yieldsGenName,
                _nKinBins,_kinBinLims);
    _sigMCGenYields->Sumw2();
    tr->Draw(TString("phoGenEt>>")+yieldsGenName,_cutAdd*cutWeight,"goff");
    std::cout<<"set _sigMCYieldTot: "<<std::endl;
    SetTotalYield(tr, _cutAdd*cutWeight, _sigMCYieldTot, _sigMCYieldTotErr);
    std::cout<<"set _sigMCGenYieldTot: "<<std::endl;
    SetTotalYield(tr, _cutAdd*cutWeight, _sigMCGenYieldTot, _sigMCGenYieldTotErr);
  }
  else if (_INPUT->allInputs_[iSource].sample_==TConfiguration::BKGMC){
    _vecBkgMCYields.push_back( new TH1F(yieldsName,yieldsName,
                _nKinBins,_kinBinLims));
    _vecBkgMCYields.back()->Sumw2();
    TCut cut;
    if (_INPUT->allInputs_[iSource].sourceName_!="Wjets_to_lnu" &&
        _INPUT->allInputs_[iSource].sourceName_!="DYjets_to_ll" &&
        _INPUT->allInputs_[iSource].sourceName_!="ttbarjets")
        cut = cutWeight*(_emptyPhoton.RangeGenFakePhoton()&&_cutAdd);
    else cut = _cutAdd*cutWeight;
    tr->Draw(_varKin+TString(">>")+yieldsName,cut,"goff");
    float val;
    float err;
    std::cout<<"set _vecBkgMCYieldTot: #"<<iSource<<std::endl;
    SetTotalYield(tr, cut, val, err);
    _vecBkgMCYieldTot.push_back(val);
    _vecBkgMCYieldTotErr.push_back(err);

    _vecBkgMCTrueGammaYields.push_back( new TH1F(yieldsName+TString("true"),yieldsName+TString("true"),
                _nKinBins,_kinBinLims));
    _vecBkgMCTrueGammaYields.back()->Sumw2();
    if (_INPUT->allInputs_[iSource].sourceName_!="Wjets_to_lnu" &&
        _INPUT->allInputs_[iSource].sourceName_!="DYjets_to_ll" &&
        _INPUT->allInputs_[iSource].sourceName_!="ttbarjets"){
      tr->Draw(_varKin+TString(">>")+yieldsName+TString("true"),cutWeight*(_emptyPhoton.RangeGenTruePhoton()&&_cutAdd),"goff");
 
      std::cout<<"set _vecBkgMCTrueGammaYieldTot: #"<<iSource<<std::endl;
      SetTotalYield(tr,cutWeight*(_emptyPhoton.RangeGenTruePhoton()&&_cutAdd), val, err);
      _vecBkgMCTrueGammaYieldTot.push_back(val);
      _vecBkgMCTrueGammaYieldTotErr.push_back(err);    
    }
    else{
      _vecBkgMCTrueGammaYieldTot.push_back(0);
      _vecBkgMCTrueGammaYieldTotErr.push_back(0);
    }
    _vecBkgMCFakeGammaYields.push_back( new TH1F(yieldsName+TString("fake"),yieldsName+TString("fake"),
                _nKinBins,_kinBinLims));
    _vecBkgMCFakeGammaYields.back()->Sumw2();
    tr->Draw(_varKin+TString(">>")+yieldsName+TString("fake"),cutWeight*(_emptyPhoton.RangeGenFakePhoton()&&_cutAdd),"goff");
    std::cout<<"set _vecBkgMCFakeGammaYieldTot: #"<<iSource<<std::endl;
    SetTotalYield(tr,cutWeight*(_emptyPhoton.RangeGenFakePhoton()&&_cutAdd), val, err);
    _vecBkgMCFakeGammaYieldTot.push_back(val);
    _vecBkgMCFakeGammaYieldTotErr.push_back(err);  
  }
}

void TPrepareYields::SetTotalYield(TTree* tr, TCut cut, float& val, float& err)
{
  _floatingHist = new TH1F("floatingHist","hist for temprorary storage of total yields",1,-1,10000);//inputFileNumber
  _floatingHist->Sumw2();
  tr->Draw("inputFileNumber>>floatingHist",cut,"goff");
  val = _floatingHist->GetBinContent(1);
  err = _floatingHist->GetBinError(1);
  std::cout<<"val+-err="<<val<<"+-"<<err<<std::endl;
  delete _floatingHist;
}

void TPrepareYields::SetYieldsDDBkgTemplate()
{
  TFile* fDDBkgTemplate= new TFile(_config.GetDDTemplateBkgFileName(_channel));
  if (!fDDBkgTemplate->IsOpen()){
    std::cout<<"file "<<_config.GetDDTemplateBkgFileName(_channel)<<" is not open"<<std::endl;
    return;
  }
  TString nameYield1D = _config.GetYieldsDDTemplateBkgName(_config.ONEDI);
  _fOut->cd();
  _bkgDDFakeGammaYields = (TH1F*)fDDBkgTemplate->Get(nameYield1D);
 
  TString nameYieldTot = _config.GetYieldsDDTemplateBkgName(_config.TOTAL);
  TH1F* hT = (TH1F*)fDDBkgTemplate->Get(nameYieldTot);
  _bkgDDFakeGammaYieldTot = hT->GetBinContent(1);
  _bkgDDFakeGammaYieldTotErr = hT->GetBinError(1);
  if (_blind==_config.BLIND_PRESCALE){
    _bkgDDFakeGammaYieldTot*=1.0/_config.GetBlindPrescale();
    _bkgDDFakeGammaYieldTotErr*=1.0/_config.GetBlindPrescale();
    for (int ib=1; ib<=_bkgDDFakeGammaYields->GetNbinsX(); ib++){
      float cont=_bkgDDFakeGammaYields->GetBinContent(ib);
      float err=_bkgDDFakeGammaYields->GetBinError(ib);
      _bkgDDFakeGammaYields->SetBinContent(ib,cont*1.0/_config.GetBlindPrescale());
      _bkgDDFakeGammaYields->SetBinError(ib,err*1.0/_config.GetBlindPrescale());
    }
  }

  std::cout<<"_bkgDDFakeGammaYields:"<<std::endl;
  _bkgDDFakeGammaYields->Print();
  for (int ib=1; ib<=_bkgDDFakeGammaYields->GetNbinsX(); ib++){
    std::cout<<ib<<": "<<_bkgDDFakeGammaYields->GetBinContent(ib)<<"+-"<<_bkgDDFakeGammaYields->GetBinError(ib)<<std::endl;
  }
}

void TPrepareYields::SubtractBackground()
{
  std::cout<<"#####################"<<std::endl;
  std::cout<<"Subtract Background"<<std::endl;
  std::cout<<"_bkgDDFakeGammaYields:"<<std::endl;
  _bkgDDFakeGammaYields->Print();
  for (int ib=1; ib<=_bkgDDFakeGammaYields->GetNbinsX(); ib++){
    std::cout<<ib<<": "<<_bkgDDFakeGammaYields->GetBinContent(ib)<<"+-"<<_bkgDDFakeGammaYields->GetBinError(ib)<<std::endl;
  }

  TString yieldsName  = (_config.GetYieldsSignalName(_config.ONEDI));
//  yieldsName+="sig";
  _signalDataYields = (TH1F*)_dataYields->Clone(yieldsName);
  _signalDataYields->SetTitle(yieldsName);
  std::cout<<"_bkgDDFakeGammaYields:"<<std::endl;
  _bkgDDFakeGammaYields->Print();
  for (int ib=1; ib<=_bkgDDFakeGammaYields->GetNbinsX(); ib++){
    std::cout<<ib<<": "<<_bkgDDFakeGammaYields->GetBinContent(ib)<<"+-"<<_bkgDDFakeGammaYields->GetBinError(ib)<<std::endl;
  }

  int nBins = _signalDataYields->GetNbinsX();
//  float yields[nBins];
//  float errsSquared[nBins];
  _signalDataYields->Add(_bkgDDFakeGammaYields,-1);

  _signalDataYieldTot=_dataYieldTot-_bkgDDFakeGammaYieldTot;

  _signalDataYieldTotErr= _dataYieldTotErr*_dataYieldTotErr+_bkgDDFakeGammaYieldTotErr*_bkgDDFakeGammaYieldTotErr;
  for (int i=0; i<_vecBkgMCTrueGammaYields.size(); i++){
        if (_INPUT->allInputs_[i+2].sourceName_=="Wjets_to_lnu") 
          continue;
        if (_INPUT->allInputs_[i+2].sourceName_=="ttbarjets") 
          continue;
        if (_INPUT->allInputs_[i+2].sourceName_=="DYjets_to_ll") 
          continue;
        _signalDataYields->Add(_vecBkgMCTrueGammaYields[i],-1);
        _signalDataYieldTot-=_vecBkgMCTrueGammaYieldTot[i];
        _signalDataYieldTotErr+=_vecBkgMCTrueGammaYieldTotErr[i]*_vecBkgMCTrueGammaYieldTotErr[i];
  }
  _signalDataYieldTotErr=sqrt(_signalDataYieldTotErr);
  std::cout<<"#####################"<<std::endl;
}

void TPrepareYields::CompareFakeBkgDDvsMC()
{
  _fOut->cd();
  _canvDDvsMC = new TCanvas("FakeDDvsMC","FakeDDvsMC",800,800);
  if (_doLogX) _canvDDvsMC->SetLogx();
  if (_doLogY) _canvDDvsMC->SetLogy();
  TLegend *legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHists","jets #rightarrow #gamma  bkg, DD vs MC estimation");
  TH1F* hSum=(TH1F*)_bkgDDFakeGammaYields->Clone("hSum_fakeGammaMC");; 

  int iFirst=0;
  for (int i=0; i<_vecBkgMCFakeGammaYields.size(); i++){
        if (_INPUT->allInputs_[i+2].sourceName_=="Wg_to_taunu") 
          continue;
        if (_INPUT->allInputs_[i+2].sourceName_=="ttbarg") 
          continue;
        if (_INPUT->allInputs_[i+2].sourceName_=="Zg") 
          continue;
        int color = _INPUT->allInputs_[i+2].color_;
        _vecBkgMCFakeGammaYields[i]->SetLineColor(color);
        _vecBkgMCFakeGammaYields[i]->SetFillColor(color);
        _vecBkgMCFakeGammaYields[i]->SetMarkerColor(color);
        _vecBkgMCFakeGammaYields[i]->SetStats(0);
        mcHists->Add(_vecBkgMCFakeGammaYields[i]);
        hSum->Add(_vecBkgMCFakeGammaYields[i]);
        legend->AddEntry(_vecBkgMCFakeGammaYields[i],"MC: "+_INPUT->allInputs_[i+2].sourceName_);
  }

 hSum->Add(_bkgDDFakeGammaYields,-1);
 legend->AddEntry(_bkgDDFakeGammaYields,"Data Driven");

 legend->SetLineColor(1);
 legend->SetLineWidth(2);
 legend->SetFillColor(0);

  _bkgDDFakeGammaYields->SetLineWidth(2);
  _bkgDDFakeGammaYields->SetStats(0);

  _bkgDDFakeGammaYields->SetTitle("jets #rightarrow #gamma background: DD vs MC");
  CompareStackVsHist(_bkgDDFakeGammaYields, hSum, legend, _canvDDvsMC, 1, mcHists);
}

void TPrepareYields::CompareTotalDATAvsSIGplusBKG()
{
  _fOut->cd();
  _canvTotalDATAvsSIGplusBKG = new TCanvas("TotalDATAvsSIGplusBKG","TotalDATAvsSIGplusBKG",800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs SIG plus BKG");

  _bkgDDFakeGammaYields->SetLineColor(4);
  _bkgDDFakeGammaYields->SetFillColor(4);
  _bkgDDFakeGammaYields->SetMarkerColor(4);
  _bkgDDFakeGammaYields->SetStats(0);
  mcHists->Add(_bkgDDFakeGammaYields);
  TH1F* hSum = (TH1F*)_bkgDDFakeGammaYields->Clone("hSumSignalPlusBkg");
  legend->AddEntry(_bkgDDFakeGammaYields,"DD fake gamma");

  for (int i=0; i<_vecBkgMCTrueGammaYields.size(); i++){
        if (_INPUT->allInputs_[i+2].sourceName_=="Wjets_to_lnu") 
          continue;
        if (_INPUT->allInputs_[i+2].sourceName_=="ttbarjets") 
          continue;
        if (_INPUT->allInputs_[i+2].sourceName_=="DYjets_to_ll") 
          continue;
        int color = _INPUT->allInputs_[i+2].color_;
        _vecBkgMCTrueGammaYields[i]->SetLineColor(color);
        _vecBkgMCTrueGammaYields[i]->SetFillColor(color);
        _vecBkgMCTrueGammaYields[i]->SetMarkerColor(color);
        _vecBkgMCTrueGammaYields[i]->SetStats(0);
        mcHists->Add(_vecBkgMCTrueGammaYields[i]);
        hSum->Add(_vecBkgMCTrueGammaYields[i]);
        TString strLeg = "MC: "+_INPUT->allInputs_[i+2].sourceLatexLabel_;
        strLeg.ReplaceAll("jets","#gamma");
        legend->AddEntry(_vecBkgMCTrueGammaYields[i],strLeg);
  }

  int colMC = _INPUT->allInputs_[1].color_;
  _sigMCYields->SetLineColor(colMC);
  _sigMCYields->SetFillColor(colMC);
  _sigMCYields->SetMarkerColor(colMC);
  _sigMCYields->SetStats(colMC);
  mcHists->Add(_sigMCYields);
  hSum->Add(_sigMCYields);
  legend->AddEntry(_sigMCYields,"signal MC");


  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);

  legend->AddEntry(_dataYields,"data");

  _dataYields->SetTitle("Total DATA vs signal MC + estimated Bkg");
  CompareStackVsHist(_dataYields, hSum, legend, _canvTotalDATAvsSIGplusBKG, 1, mcHists);
}

void TPrepareYields::CompareTotalDATAvsMC()
{
  _fOut->cd();
  _canvTotalDATAvsMC = new TCanvas("TotalDATAvsMC","TotalDATAvsMC",800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs MC");
  TH1F* hSum=(TH1F*)_sigMCYields->Clone("hSum_WholeMC");
  int iFirst=0;

  for (int i=0; i<_vecBkgMCTrueGammaYields.size(); i++){
        //if (_INPUT->allInputs_[i+2].sourceName_=="Wjets_to_lnu" ||
        //    _INPUT->allInputs_[i+2].sourceName_=="ttbarjets" ||
        //    _INPUT->allInputs_[i+2].sourceName_=="DYjets_to_ll" ||
        //    _INPUT->allInputs_[i+2].sourceName_=="multibosons"); 
        //else continue;
        int color = _INPUT->allInputs_[i+2].color_;
        _vecBkgMCYields[i]->SetLineColor(color);
        _vecBkgMCYields[i]->SetFillColor(color);
        _vecBkgMCYields[i]->SetMarkerColor(color);
        _vecBkgMCYields[i]->SetStats(0);
        mcHists->Add(_vecBkgMCYields[i]);
        hSum->Add(_vecBkgMCYields[i]);
        TString strLeg = "MC: "+_INPUT->allInputs_[i+2].sourceLatexLabel_;
        legend->AddEntry(_vecBkgMCYields[i],strLeg);
  }



  mcHists->Add(_sigMCYields);

  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);
//  mcHists->GetXaxis()->SetMoreLogLabels();
  _dataYields->SetStats(0);
  legend->AddEntry(_dataYields,"data");

  _dataYields->SetTitle("Total DATA vs total MC");
  CompareStackVsHist(_dataYields, hSum, legend, _canvTotalDATAvsMC, 1, mcHists);
}

void TPrepareYields::CompareSignalDATAvsMC()
{
  _canvSignalDATAvsMC = new TCanvas("SignalDATAvsMC","SignalDATAvsMC",800,800);

  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  legend->AddEntry(_signalDataYields,"signal data");
  legend->AddEntry(_sigMCYields,"signal MC");
  legend->SetLineColor(1);
  legend->SetLineWidth(2);
  legend->SetFillColor(0);

  _signalDataYields->SetTitle("signal DATA vs signal MC"); 
  CompareStackVsHist(_signalDataYields,_sigMCYields,legend,_canvSignalDATAvsMC);
}

void TPrepareYields::CompareStackVsHist(TH1F* hist1, TH1F* hist2, TLegend* legend, TCanvas* canv, bool isStack, THStack* stack)
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
  float min = 0.1*hist1->GetMinimum();
  if (min>0.1*hist2->GetMinimum())
    min=0.1*hist2->GetMinimum();
  if (!_doLogX) min=0;
  max=1.2*max;

  if (isStack) hist2->SetLineColor(2);
  if (isStack) hist2->SetFillStyle(0);
  hist1->GetYaxis()->SetRangeUser(min,max);
  hist1->SetStats(0);
  hist1->Draw("P");
  if (isStack) stack->Draw("HIST same"); 
  else hist2->Draw("HIST same");

//  if (isStack) hist2->Draw("HIST same");
//  legend->AddEntry(hist2,"hsum","l");

  hist1->SetLineWidth(2);
  hist1->Draw("EP same");
  legend->Draw("same");

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

}


void TPrepareYields::PrintYields()
{
  int nBins = _nKinBins;

  //BKG MC yields
  std::cout<<"BKG MC yields"<<std::endl;
  std::cout<<"Total: ";
  for (int i=0; i<_vecBkgMCYieldTot.size(); i++)
    std::cout<<_vecBkgMCYieldTot[i]<<" ";
  std::cout<<std::endl;
  for (int ib=1; ib<=nBins; ib++){
    std::cout<<"ib="<<ib<<": ";
    for (int i=0; i<_vecBkgMCYields.size(); i++)
      std::cout<<_vecBkgMCYields[i]->GetBinContent(ib)<<" ";
    std::cout<<std::endl;
  }

  //BKG MC fake gamma yields
  std::cout<<"BKG MC fake gamma yields"<<std::endl;
  std::cout<<"Total: ";
  for (int i=0; i<_vecBkgMCFakeGammaYieldTot.size(); i++)
    std::cout<<_vecBkgMCFakeGammaYieldTot[i]<<" ";
  std::cout<<std::endl;
  for (int ib=1; ib<=nBins; ib++){
    std::cout<<"ib="<<ib<<": ";
    for (int i=0; i<_vecBkgMCFakeGammaYields.size(); i++)
      std::cout<<_vecBkgMCFakeGammaYields[i]->GetBinContent(ib)<<" ";
    std::cout<<std::endl;
  }

  //BKG MC true gamma yields
  std::cout<<"BKG MC true gamma yields"<<std::endl;
  std::cout<<"Total: ";
  for (int i=0; i<_vecBkgMCTrueGammaYieldTot.size(); i++)
    std::cout<<_vecBkgMCTrueGammaYieldTot[i]<<" ";
  std::cout<<std::endl;
  for (int ib=1; ib<=nBins; ib++){
    std::cout<<"ib="<<ib<<": ";
    for (int i=0; i<_vecBkgMCTrueGammaYields.size(); i++)
      std::cout<<_vecBkgMCTrueGammaYields[i]->GetBinContent(ib)<<" ";
    std::cout<<std::endl;
  }

  //DATA
  std::cout<<"DATA"<<std::endl;
  std::cout<<"Total: "<<_dataYieldTot<<"+-"<<_dataYieldTotErr<<std::endl;
  for (int ib=1; ib<=nBins; ib++)
    std::cout<<"ib="<<ib<<"    "<<_dataYields->GetBinContent(ib)<<"+-"<<_dataYields->GetBinError(ib)<<std::endl;

  //signal MC
  std::cout<<"signal MC"<<std::endl;
  std::cout<<"Total: "<<_sigMCYieldTot<<"+-"<<_sigMCYieldTotErr<<std::endl;
  for (int ib=1; ib<=nBins; ib++)
    std::cout<<"ib="<<ib<<"    "<<_sigMCYields->GetBinContent(ib)<<"+-"<<_sigMCYields->GetBinError(ib)<<std::endl;

  //DD bkg
  std::cout<<"DD bkg"<<std::endl;
  std::cout<<"Total: "<<_bkgDDFakeGammaYieldTot<<"+-"<<_bkgDDFakeGammaYieldTotErr<<std::endl;
  for (int ib=1; ib<=nBins; ib++)
    std::cout<<"ib="<<ib<<"    "<<_bkgDDFakeGammaYields->GetBinContent(ib)<<"+-"<<_bkgDDFakeGammaYields->GetBinError(ib)<<std::endl;

  //signal DATA vs signal MC
  std::cout<<"          signal DATA        signal MC"<<std::endl;
  std::cout<<"Total: "<<_signalDataYieldTot<<"+-"<<_signalDataYieldTotErr<<";   "<<_sigMCYieldTot<<"+-"<<_sigMCYieldTotErr<<std::endl;
  for (int ib=1; ib<=nBins; ib++)
    std::cout<<"ib="<<ib<<"    "<<_signalDataYields->GetBinContent(ib)<<"+-"<<_signalDataYields->GetBinError(ib)<<";   "<<_sigMCYields->GetBinContent(ib)<<"+-"<<_sigMCYields->GetBinError(ib)<<std::endl;
}

void TPrepareYields::StoreYields()
{
  _fOut->cd();
  _signalDataYields->Write();
//  _canvDDvsMC->Write();
  TString yieldsName  = (_config.GetYieldsSignalName(_config.TOTAL));
  TH1F hT = TH1F(yieldsName,yieldsName,1,_config.GetPhoPtMin(),_config.GetPhoPtMax());
  hT.SetBinContent(1,_signalDataYieldTot);
  hT.SetBinError(1,_signalDataYieldTotErr);
  hT.Write(yieldsName);
}
