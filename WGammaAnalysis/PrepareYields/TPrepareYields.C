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

TPrepareYields::TPrepareYields(int channel)
{
  _channel=channel;
  _INPUT=new TAllInputSamples(_channel,"../Configuration/config.txt");
  _fOut=0;
  _dataYields=0;
  _sigMCYields=0;
  _sigMCGenYields=0;
  _bkgDDFakeGammaYields=0;
  _signalDataYields=0;
  _legend=0;
  _canvDDvsMC=0;
  _canvTotalDATAvsSIGplusBKG=0;
  _canvSignalDATAvsMC=0;
  _phoPtBinLimits = new float[_config.GetNPhoPtBins()];
  _config.GetPhoPtBinsLimits(_phoPtBinLimits);
}

TPrepareYields::~TPrepareYields()
{
  delete _INPUT;
}

void TPrepareYields::PrepareYields()
{
  SetYields();
  SubtractBackground();
  CompareFakeBkgDDvsMC();
  CompareTotalDATAvsSIGplusBKG();
  CompareTotalDATAvsMC();
  CompareSignalDATAvsMC();
  PrintYields();
  StoreYields();
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

  TCut cutWeight="weight";

  TString yieldsHistName;

  int sample = _INPUT->allInputs_[iSource].sample_;
  TString strSourceName = _INPUT->allInputs_[iSource].sourceName_;

  TString fInName = _config.GetSelectedName(_config.FULLY,_channel,sample,strSourceName);
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
                _config.GetNPhoPtBins(),_phoPtBinLimits);
    tr->Draw(TString("phoEt>>")+yieldsName,cutWeight,"goff");
    std::cout<<"set _dataYieldTot: "<<std::endl;
    SetTotalYield(tr, "weight", _dataYieldTot, _dataYieldTotErr);
  }
  else if (_INPUT->allInputs_[iSource].sample_==TConfiguration::SIGMC){
    _sigMCYields = new TH1F(yieldsName,yieldsName,
                _config.GetNPhoPtBins(),_phoPtBinLimits);
    tr->Draw(TString("phoEt>>")+yieldsName,cutWeight,"goff");
    _sigMCGenYields = new TH1F(yieldsGenName,yieldsGenName,
                _config.GetNPhoPtBins(),_phoPtBinLimits);
    tr->Draw(TString("phoGenEt>>")+yieldsGenName,cutWeight,"goff");
    std::cout<<"set _sigMCYieldTot: "<<std::endl;
    SetTotalYield(tr, "weight", _sigMCYieldTot, _sigMCYieldTotErr);
    std::cout<<"set _sigMCGenYieldTot: "<<std::endl;
    SetTotalYield(tr, "weight", _sigMCGenYieldTot, _sigMCGenYieldTotErr);
  }
  else if (_INPUT->allInputs_[iSource].sample_==TConfiguration::BKGMC){

    _vecBkgMCYields.push_back( new TH1F(yieldsName,yieldsName,
                _config.GetNPhoPtBins(),_phoPtBinLimits));
    tr->Draw(TString("phoEt>>")+yieldsName,cutWeight,"goff");
    float val;
    float err;
    std::cout<<"set _vecBkgMCYieldTot: #"<<iSource<<std::endl;
    SetTotalYield(tr, "weight", val, err);
    _vecBkgMCYieldTot.push_back(val);
    _vecBkgMCYieldTotErr.push_back(err);

    _vecBkgMCTrueGammaYields.push_back( new TH1F(yieldsName+TString("true"),yieldsName+TString("true"),
                _config.GetNPhoPtBins(),_phoPtBinLimits));
    if (_INPUT->allInputs_[iSource].sourceName_!="Wjets_to_lnu" &&
        _INPUT->allInputs_[iSource].sourceName_!="DYjets_to_ll" &&
        _INPUT->allInputs_[iSource].sourceName_!="ttbarjets"){
      tr->Draw(TString("phoEt>>")+yieldsName+TString("true"),cutWeight*_emptyPhoton.RangeGenTruePhoton(),"goff");
 
      std::cout<<"set _vecBkgMCTrueGammaYieldTot: #"<<iSource<<std::endl;
      SetTotalYield(tr,cutWeight*_emptyPhoton.RangeGenTruePhoton(), val, err);
      _vecBkgMCTrueGammaYieldTot.push_back(val);
      _vecBkgMCTrueGammaYieldTotErr.push_back(err);    
    }
    else{
      _vecBkgMCTrueGammaYieldTot.push_back(0);
      _vecBkgMCTrueGammaYieldTotErr.push_back(0);
    }
    _vecBkgMCFakeGammaYields.push_back( new TH1F(yieldsName+TString("fake"),yieldsName+TString("fake"),
                _config.GetNPhoPtBins(),_phoPtBinLimits));
    tr->Draw(TString("phoEt>>")+yieldsName+TString("fake"),cutWeight*_emptyPhoton.RangeGenFakePhoton(),"goff");
    std::cout<<"set _vecBkgMCFakeGammaYieldTot: #"<<iSource<<std::endl;
    SetTotalYield(tr,cutWeight*_emptyPhoton.RangeGenFakePhoton(), val, err);
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
  std::cout<<"_bkgDDFakeGammaYields:"<<std::endl;
  _bkgDDFakeGammaYields->Print();
  for (int ib=1; ib<=_bkgDDFakeGammaYields->GetNbinsX(); ib++){
    std::cout<<ib<<": "<<_bkgDDFakeGammaYields->GetBinContent(ib)<<"+-"<<_bkgDDFakeGammaYields->GetBinError(ib)<<std::endl;
  }
  
  TString nameYieldTot = _config.GetYieldsDDTemplateBkgName(_config.TOTAL);
  TH1F* hT = (TH1F*)fDDBkgTemplate->Get(nameYieldTot);
  _bkgDDFakeGammaYieldTot = hT->GetBinContent(1);
  _bkgDDFakeGammaYieldTotErr = hT->GetBinError(1);
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

  TString yieldsName  = (_config.GetYieldsSelectedName(_config.ONEDI,_config.DATA));
  yieldsName+="sig";
  _signalDataYields = (TH1F*)_dataYields->Clone(yieldsName);
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
  _canvDDvsMC = new TCanvas("DDvsMC","DDvsMC");
  _canvDDvsMC->SetLogx();
  _canvDDvsMC->SetLogy();
  _legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHists","jets #rightarrow #gamma  bkg, DD vs MC estimation");

  for (int i=0; i<_vecBkgMCFakeGammaYields.size(); i++){
        int color = _INPUT->allInputs_[i+2].color_;
        _vecBkgMCFakeGammaYields[i]->SetLineColor(color);
        _vecBkgMCFakeGammaYields[i]->SetFillColor(color);
        _vecBkgMCFakeGammaYields[i]->SetMarkerColor(color);
        _vecBkgMCFakeGammaYields[i]->SetStats(0);
        mcHists->Add(_vecBkgMCFakeGammaYields[i]);
        _legend->AddEntry(_vecBkgMCFakeGammaYields[i],"MC: "+_INPUT->allInputs_[i+2].sourceName_);
  }
 _legend->AddEntry(_bkgDDFakeGammaYields,"Data Driven");

 _legend->SetLineColor(1);
 _legend->SetLineWidth(2);
 _legend->SetFillColor(0);
//  mcHists->GetXaxis()->SetMoreLogLabels();
  _bkgDDFakeGammaYields->SetLineWidth(2);
  _bkgDDFakeGammaYields->SetStats(0);
  _bkgDDFakeGammaYields->Draw("P");
  mcHists->Draw("same");
  _bkgDDFakeGammaYields->Draw("EP same");
  _legend->Draw("same");

}

void TPrepareYields::CompareTotalDATAvsSIGplusBKG()
{
  _canvTotalDATAvsSIGplusBKG = new TCanvas("TotalDATAvsSIGplusBKG","TotalDATAvsSIGplusBKG");
  _canvTotalDATAvsSIGplusBKG->SetLogx();
  _canvTotalDATAvsSIGplusBKG->SetLogy();
  _legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs SIG plus BKG");

  _bkgDDFakeGammaYields->SetLineColor(4);
  _bkgDDFakeGammaYields->SetFillColor(4);
  _bkgDDFakeGammaYields->SetMarkerColor(4);
  _bkgDDFakeGammaYields->SetStats(0);
  mcHists->Add(_bkgDDFakeGammaYields);
  _legend->AddEntry(_bkgDDFakeGammaYields,"DD fake gamma");

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
        TString strLeg = "MC: "+_INPUT->allInputs_[i+2].sourceLatexLabel_;
        strLeg.ReplaceAll("jets","#gamma");
        _legend->AddEntry(_vecBkgMCTrueGammaYields[i],strLeg);
  }

  int colMC = _INPUT->allInputs_[1].color_;
  _sigMCYields->SetLineColor(colMC);
  _sigMCYields->SetFillColor(colMC);
  _sigMCYields->SetMarkerColor(colMC);
  _sigMCYields->SetStats(colMC);
  mcHists->Add(_sigMCYields);
  _legend->AddEntry(_sigMCYields,"signal MC");


 _legend->SetLineColor(1);
 _legend->SetLineWidth(2);
 _legend->SetFillColor(0);
//  mcHists->GetXaxis()->SetMoreLogLabels();
  _dataYields->SetStats(0);
  _legend->AddEntry(_dataYields,"data");
  _dataYields->Draw("P");
  mcHists->Draw("HIST same");
  _dataYields->SetLineWidth(2);
  _dataYields->Draw("EP same");
  _legend->Draw("same");
}

void TPrepareYields::CompareSignalDATAvsMC()
{
  _canvSignalDATAvsMC = new TCanvas("SignalDATAvsMC","SignalDATAvsMC");
  _canvSignalDATAvsMC->SetLogx();
  _canvSignalDATAvsMC->SetLogy();
  _legend = new TLegend(0.7,0.7,0.95,0.95);

  _legend->AddEntry(_signalDataYields,"signal data");
  _legend->AddEntry(_sigMCYields,"signal MC");

  _legend->SetLineColor(1);
  _legend->SetLineWidth(2);
  _legend->SetFillColor(0);
 
  _signalDataYields->SetStats(0);
  _signalDataYields->Draw("P");
  _sigMCYields->Draw("HIST same");
  _signalDataYields->SetLineWidth(2);
  _signalDataYields->Draw("EP same");
  _legend->Draw("same");
}

void TPrepareYields::CompareTotalDATAvsMC()
{
  _canvTotalDATAvsMC = new TCanvas("TotalDATAvsMC","TotalDATAvsMC");
  _canvTotalDATAvsMC->SetLogx();
  _canvTotalDATAvsMC->SetLogy();
  _legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs MC");

  for (int i=0; i<_vecBkgMCTrueGammaYields.size(); i++){
        if (_INPUT->allInputs_[i+2].sourceName_=="Wjets_to_lnu" ||
            _INPUT->allInputs_[i+2].sourceName_=="ttbarjets" ||
            _INPUT->allInputs_[i+2].sourceName_=="DYjets_to_ll" ||
            _INPUT->allInputs_[i+2].sourceName_=="multibosons"); 
        else continue;
        int color = _INPUT->allInputs_[i+2].color_;
        _vecBkgMCYields[i]->SetLineColor(color);
        _vecBkgMCYields[i]->SetFillColor(color);
        _vecBkgMCYields[i]->SetMarkerColor(color);
        _vecBkgMCYields[i]->SetStats(0);
        mcHists->Add(_vecBkgMCYields[i]);
        TString strLeg = "MC: "+_INPUT->allInputs_[i+2].sourceLatexLabel_;
        _legend->AddEntry(_vecBkgMCYields[i],strLeg);
  }

 _legend->SetLineColor(1);
 _legend->SetLineWidth(2);
 _legend->SetFillColor(0);
//  mcHists->GetXaxis()->SetMoreLogLabels();
  _dataYields->SetStats(0);
  _legend->AddEntry(_dataYields,"data");
  _dataYields->Draw("P");
  mcHists->Draw("HIST same");
  _dataYields->SetLineWidth(2);
  _dataYields->Draw("EP same");
  _legend->Draw("same");
}


void TPrepareYields::PrintYields()
{
  int nBins = _config.GetNPhoPtBins();


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
  _canvDDvsMC->Write();
  TString yieldsName  = (_config.GetYieldsSelectedName(_config.TOTAL,_config.DATA));
  TH1F hT = TH1F(yieldsName,yieldsName,1,_config.GetPhoPtMin(),_config.GetPhoPtMax());
  hT.SetBinContent(1,_signalDataYieldTot);
  hT.SetBinError(1,_signalDataYieldTotErr);
  hT.Write(yieldsName);
}
