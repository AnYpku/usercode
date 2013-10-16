#include "TPrepareYields.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TFullCuts.h"
#include "../Include/TPhotonCuts.h"

#include <string>
#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"

TPrepareYields::TPrepareYields(int channel)
{
  channel_=channel;
  INPUT_=new TAllInputSamples(channel_,"../Configuration/config.txt");

}

TPrepareYields::~TPrepareYields()
{
  delete INPUT_;
}

void TPrepareYields::PrepareYields()
{
  SetYields();
  SubtractBackground();
  CompareBkgDDandMC();
  StoreYields();
}

void TPrepareYields::SetYields()
{
  TString fOutName;
  fOutName = config_.GetYieldsFileName(channel_);
  fOut_ = new TFile(fOutName,"recreate");
  for (int i=0; i<INPUT_->nSources_; i++){
    SetYieldsOneSource(i);
  }
  SetYieldsDDBkgTemplate();
}

void TPrepareYields::SetYieldsOneSource(int iSource)
{
  TString fInName;
  TString yieldsHistName;


  fInName = config_.GetSelectedFullyName(channel_,INPUT_->allInputs_[iSource].sample_,INPUT_->allInputs_[iSource].sourceName_);

  TFile fIn(fInName);
  TTree* tr = (TTree*)fIn.Get("selectedEvents");

  vector <float> vecPtBins = config_.GetPhoPtBinsLimits();
  float ptBinsLimits[config_.GetNPhoPtBins()+1];
  for (int i=0; i<config_.GetNPhoPtBins()+1; i++)
    ptBinsLimits[i] = vecPtBins[i];
  TString yieldsName  = (config_.GetYieldsSelectedHistName(INPUT_->allInputs_[iSource].sample_,config_.COMMON,INPUT_->allInputs_[iSource].sourceName_));
  TString yieldsBName = (config_.GetYieldsSelectedHistName(INPUT_->allInputs_[iSource].sample_,config_.BARREL,INPUT_->allInputs_[iSource].sourceName_));
  TString yieldsEName = (config_.GetYieldsSelectedHistName(INPUT_->allInputs_[iSource].sample_,config_.ENDCAP,INPUT_->allInputs_[iSource].sourceName_));

  TString yieldsGenName = config_.GetYieldsSelectedSignalMCGenHistName();

  fOut_->cd();

  if (INPUT_->allInputs_[iSource].sample_==TInputSample::DATA){
    dataYields_ = new TH1F(yieldsName,yieldsName,
                config_.GetNPhoPtBins(),ptBinsLimits);
    dataYieldsB_ = new TH1F(yieldsBName,yieldsBName,
                config_.GetNPhoPtBins(),ptBinsLimits);
    dataYieldsE_ = new TH1F(yieldsEName,yieldsEName,
                config_.GetNPhoPtBins(),ptBinsLimits);
  }
  else if (INPUT_->allInputs_[iSource].sample_==TInputSample::SIGMC){
    sigMCYields_ = new TH1F(yieldsName,yieldsName,
                config_.GetNPhoPtBins(),ptBinsLimits);
    sigMCYieldsB_ = new TH1F(yieldsBName,yieldsBName,
                config_.GetNPhoPtBins(),ptBinsLimits);
    sigMCYieldsE_ = new TH1F(yieldsEName,yieldsEName,
                config_.GetNPhoPtBins(),ptBinsLimits);
    sigMCGenYields_ = new TH1F(yieldsGenName,yieldsGenName,
                config_.GetNPhoPtBins(),ptBinsLimits);

  }
  else if (INPUT_->allInputs_[iSource].sample_==TInputSample::BKGMC){
    vecBkgMCYields_.push_back( new TH1F(yieldsName,yieldsName,
                config_.GetNPhoPtBins(),ptBinsLimits));
    vecBkgMCYieldsB_.push_back( new TH1F(yieldsBName,yieldsBName,
                config_.GetNPhoPtBins(),ptBinsLimits));
    vecBkgMCYieldsE_.push_back( new TH1F(yieldsEName,yieldsEName,
                config_.GetNPhoPtBins(),ptBinsLimits));
  }

  //TFullCuts fullCuts;
  TPhotonCuts emptyPhoton;
  //TCut cut = fullCuts.ExtraCut("phoSigmaIEtaIEta", "phoPFChIsoCorr", "phoEta");
  //TString cutStr = cut.GetTitle();
  tr->Draw(TString("phoEt>>")+yieldsName,"(1)*weight","goff");
  if (INPUT_->allInputs_[iSource].sample_==TInputSample::SIGMC){
      tr->Draw(TString("phoGenEt>>")+yieldsGenName,"(1)*weight","goff");
  }
  
  TCut cutB = emptyPhoton.IsBarrel("phoEta");
  TString cutStrB = cutB.GetTitle();
  tr->Draw(TString("phoEt>>")+yieldsBName,"("+cutStrB+")*weight","goff");

  TCut cutE = emptyPhoton.IsEndcap("phoEta");
  TString cutStrE = cutE.GetTitle();
  tr->Draw(TString("phoEt>>")+yieldsEName,"("+cutStrE+")*weight","goff");
}

void TPrepareYields::SetYieldsDDBkgTemplate()
{
  TFile fFractions(config_.GetFractionsDDTemplateBkgFileName(channel_));
  TH1F* frB = (TH1F*)fFractions.Get(config_.GetFractionsDDTemplateBkgHistName(config_.BARREL));
  TH1F* frE = (TH1F*)fFractions.Get(config_.GetFractionsDDTemplateBkgHistName(config_.ENDCAP));
  vector <float> vecPtBins = config_.GetPhoPtBinsLimits();
  float ptBinsLimits[config_.GetNPhoPtBins()+1];
  for (int i=0; i<config_.GetNPhoPtBins()+1; i++)
    ptBinsLimits[i] = vecPtBins[i];

  fOut_->cd();
  TString yieldsName = config_.GetYieldsDDTemplateBkgHistName(config_.COMMON);
  TString yieldsNameB= config_.GetYieldsDDTemplateBkgHistName(config_.BARREL);
  TString yieldsNameE= config_.GetYieldsDDTemplateBkgHistName(config_.ENDCAP);

  bkgDDYields_ = new TH1F(yieldsName,yieldsName,
                config_.GetNPhoPtBins(),ptBinsLimits);
  bkgDDYieldsB_ = new TH1F(yieldsNameB,yieldsNameB,
                config_.GetNPhoPtBins(),ptBinsLimits);
  bkgDDYieldsE_ = new TH1F(yieldsNameE,yieldsNameE,
                config_.GetNPhoPtBins(),ptBinsLimits);

  bkgDDYieldsB_->Multiply(dataYieldsB_,frB);
  bkgDDYieldsE_->Multiply(dataYieldsE_,frE);
  bkgDDYields_->Add(bkgDDYieldsB_,bkgDDYieldsE_);
}

void TPrepareYields::SubtractBackground()
{
}

void TPrepareYields::CompareBkgDDandMC()
{
  canvDDvsMC_ = new TCanvas("DDvsMC","DDvsMC");
  canvDDvsMC_->SetLogx();
  canvDDvsMC_->SetLogy();
  legend_ = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHists","jets #rightarrow #gamma  bkg, DD vs MC estimation");
  for (int i=2; i<INPUT_->nSources_; i++){
    if (INPUT_->allInputs_[i].sourceName_=="Wjets_to_lnu" ||
        INPUT_->allInputs_[i].sourceName_=="DYjets_to_ll" ||
        INPUT_->allInputs_[i].sourceName_=="ttbarjets")
      {
        int color = INPUT_->allInputs_[i].color_;
        vecBkgMCYields_[i-2]->SetLineColor(color);
        vecBkgMCYields_[i-2]->SetFillColor(color);
        vecBkgMCYields_[i-2]->SetMarkerColor(color);
        vecBkgMCYields_[i-2]->SetStats(0);
        mcHists->Add(vecBkgMCYields_[i-2]);
        legend_->AddEntry(vecBkgMCYields_[i-2],"MC: "+INPUT_->allInputs_[i].sourceName_);
      }
  }
 legend_->AddEntry(bkgDDYields_,"Data Driven");
 legend_->SetLineColor(1);
 legend_->SetLineWidth(2);
 legend_->SetFillColor(0);
 // mcHists->GetXaxis()->SetMoreLogLabels();
  mcHists->Draw();
  bkgDDYields_->Draw("EP same");
  legend_->Draw("same");
}

void TPrepareYields::CompareYieldsDATAandMC()
{
}

void TPrepareYields::StoreYields()
{
  fOut_->cd();
  dataYields_->Write();
  dataYieldsB_->Write();
  dataYieldsE_->Write();
  sigMCYields_->Write();
  sigMCYieldsB_->Write();
  sigMCYieldsE_->Write();
  sigMCGenYields_->Write();
//  sigMCGenYieldsB_->Write();
//  sigMCGenYieldsE_->Write();
  for (unsigned int i=0; i<vecBkgMCYields_.size(); i++){
    vecBkgMCYields_[i]->Write();
    vecBkgMCYieldsB_[i]->Write();
    vecBkgMCYieldsE_[i]->Write();
  }
  bkgDDYields_->Write();
  bkgDDYieldsB_->Write();
  bkgDDYieldsE_->Write();  

  canvDDvsMC_->Write();
}
