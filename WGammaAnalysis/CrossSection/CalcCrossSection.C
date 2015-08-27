#include "CalcCrossSection.h" 
  //this class
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TMathTools.h"
#include "../Unfolding/Unfolding.h"
#include "../RooUnfold/src/RooUnfold.h"
  //this package
  //currently in this package
#include <string>
#include <iostream>
#include <iomanip>
  //standard C++ class
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TVectorF.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLine.h"
  //ROOT class

CalcCrossSection::CalcCrossSection(int channel, int vgamma, int blind, string configFile)
{
  _INPUT = new TAllInputSamples(channel, vgamma, configFile);
  _channel=channel;
  _vgamma=vgamma;
  _blind=blind;
  _fOut = new TFile(_config.GetCrossSectionFileName(_channel,_vgamma),"recreate");
  for (int iSource=0; iSource<_INPUT->nSources_; iSource++){
       if (_INPUT->allInputs_[iSource].sample_==_config.DATA){
           _lumi=_INPUT->allInputs_[iSource].lumiTotal_;
           if (blind==_config.BLIND_PRESCALE) 
             _lumi=_lumi/_config.GetBlindPrescale();
           break;
       }
  }// end of iSource
}// end of CalcCrossSection

CalcCrossSection::~CalcCrossSection()
{

}// end of ~CalcCrossSection()

void CalcCrossSection::PrintLatexAll()
{

  TString fName=_config.GetAccXEffFileName(_channel, _vgamma);
  TFile* fTheory = new TFile(fName);
  TH1F* hTheory1D = (TH1F*)fTheory->Get(_config.GetTheoryCSname(_config.ONEDI));
  TH1F* hTheoryTot = (TH1F*)fTheory->Get(_config.GetTheoryCSname(_config.TOTAL));

  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;


    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Cross section and errors}"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih
    if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA){
      std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|}"<<std::endl;
      std::cout<<"    bin & d\\sigma/dP_{T} &d\\sigma/dP_{T} & err & err syst & err syst & accXeff\\\\ "<<std::endl;
      std::cout<<"    lims & MC based &    meas.       & stat & $I_{ch}$ vs $\\sigma_{i\\eta i\\eta}$ & $e\\rightarrow\\gamma$ stat. & MC stat \\\\ \\hline"<<std::endl;
   }
    else{
      std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|}"<<std::endl;
      std::cout<<"    bin & d\\sigma/dP_{T} &d\\sigma/dP_{T} & err & err syst & accXeff\\\\ "<<std::endl;
      std::cout<<"    lims & MC based &    meas.       & stat & $I_{ch}$ vs $\\sigma_{i\\eta i\\eta}$ & MC stat\\\\ \\hline"<<std::endl;
    }


      std::cout<<"    ";
      std::cout<<std::setprecision(0)<<"total & ";

      std::cout<<std::setprecision(0)<<1000*hTheoryTot->GetBinContent(1)<<" & ";

      std::cout<<std::setprecision(0)<<_yCSstat.crossSectionTOT->GetBinContent(1)<<" & ";

      std::cout<<std::setprecision(0)<<_yCSstat.crossSectionTOT->GetBinError(1)<<" & ";

      std::cout<<std::setprecision(0)<<_yCSsyst_CHISOvsSIHIH.crossSectionTOT->GetBinError(1)<<" & ";

      std::cout<<std::setprecision(0)<<_yCSsyst_accXeff_MCstat.crossSectionTOT->GetBinError(1);

      if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA)
        std::cout<<std::setprecision(0)<<" & "<<_yCSsyst_etogStat.crossSectionTOT->GetBinError(1);
      std::cout<<" \\\\ \\hline"<<std::endl;
    //loop over pt bins
    for (int ib=1; ib<=_yCSstat.crossSection1D->GetNbinsX(); ib++){

      if (ib==1) std::cout<<"%";

      std::cout<<"    ";
      std::cout<<std::setprecision(0)<<_yCSstat.crossSection1D->GetBinLowEdge(ib)<<"-"<<_yCSstat.crossSection1D->GetBinLowEdge(ib)+_yCSstat.crossSection1D->GetBinWidth(ib)<<" & ";

      float cont = _yCSstat.crossSection1D->GetBinContent(ib);
      float pres=0;
      if (cont<10) pres=1; //0.1
      if (cont<1)  pres=2;
      if (cont<0.1) pres=3;

      std::cout<<std::setprecision(pres)<<1000*hTheory1D->GetBinContent(ib)<<" & ";
   
      std::cout<<std::setprecision(pres)<<cont<<" & ";

      std::cout<<std::setprecision(pres)<<_yCSstat.crossSection1D->GetBinError(ib)<<" & ";

      std::cout<<std::setprecision(pres)<<_yCSsyst_CHISOvsSIHIH.crossSection1D->GetBinError(ib)<<" & ";

      std::cout<<std::setprecision(pres)<<_yCSsyst_accXeff_MCstat.crossSection1D->GetBinError(ib);

      if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA)
        std::cout<<std::setprecision(pres)<<" & "<<_yCSsyst_etogStat.crossSection1D->GetBinError(ib);

      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:sc_and_syst}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;
}//end of PrintLatexAll()

void CalcCrossSection::Calc()
{
//  _yCSsyst_RealTemplStat.errType=ERR_SYST;
//  _yCSsyst_FakeTemplStat.errType=ERR_SYST;
//  _yCSsyst_etogStat.errType=ERR_SYST;

  //_yCSstat
  _yCSstat.errType=ERR_STAT;
  _yCSstat.name="yield_pm_stat";
  _yCSstat.title="yield+-stat";
  std::cout<<" _yCSstat "<<_yCSstat.title<<std::endl;
  GetSignalYields();
  ApplyUnfolding(_yCSstat);
  ApplyAccXEff(_yCSstat);
  DivideOverLumi(_yCSstat);
  DivideOverBinWidth(_yCSstat);
  Plot(_yCSstat);

  //_yCSsyst_CHISOvsSIHIH
  _yCSsyst_CHISOvsSIHIH.errType=ERR_SYST;
  _yCSsyst_CHISOvsSIHIH.title="|N_{Ich}-N_{#sigma{i#eta i#eta}}|";
  _yCSsyst_CHISOvsSIHIH.name="syst_CHISOvsSIHIH";
  std::cout<<" _yCSsyst_CHISOvsSIHIH "<<_yCSsyst_CHISOvsSIHIH.title<<std::endl;
  GetYieldsSyst(_yCSsyst_CHISOvsSIHIH, 
    _config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_OVERLAY, "phoEt"), 
    _config.GetSystCHISOvsSIHIHname(_config.ONEDI,_config.COMMON), 
    _config.GetSystCHISOvsSIHIHname(_config.TOTAL,_config.COMMON));
  ApplyUnfolding(_yCSsyst_CHISOvsSIHIH);
  ApplyAccXEff(_yCSsyst_CHISOvsSIHIH);
  DivideOverLumi(_yCSsyst_CHISOvsSIHIH);
  DivideOverBinWidth(_yCSsyst_CHISOvsSIHIH);  

  //  _yCSsyst_etogStat
  if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA){
    _yCSsyst_etogStat.errType=ERR_SYST;  
    _yCSsyst_etogStat.title="e#rightarrow#gamma, samples stat";
    _yCSsyst_etogStat.name="syst_etog_stat";
    std::cout<<" _yCSsyst_etogStat "<<_yCSsyst_etogStat.title<<std::endl;
    GetYieldsSyst(_yCSsyst_etogStat, 
    _config.GetDDBkgEtoGammaFileName("phoEt"), 
    _config.GetYieldsDDBkgEtoGamma(_config.ONEDI,_config.COMMON), 
    _config.GetYieldsDDBkgEtoGamma(_config.TOTAL,_config.COMMON));
    ApplyUnfolding(_yCSsyst_etogStat);
    ApplyAccXEff(_yCSsyst_etogStat);
    DivideOverLumi(_yCSsyst_etogStat);
    DivideOverBinWidth(_yCSsyst_etogStat);  
  }// end of if (_channel==_config.ELECTRON && _channel==_config.W_GAMMA)

  //  _yCSsyst_accXeff_MCstat.errType=ERR_SYST;
  _yCSsyst_accXeff_MCstat.errType=ERR_SYST;
  _yCSsyst_accXeff_MCstat.title="accXeff, MC stat";
  _yCSsyst_accXeff_MCstat.name="syst_accXeff_MCstat";
  TFile* f = new TFile(_config.GetAccXEffFileName(_channel,_vgamma));
  TH1F* hAccXEffTot = (TH1F*)f->Get(_config.GetAccXEffName(_config.TOTAL));
  TH1F* hAccXEff1D = (TH1F*)f->Get(_config.GetAccXEffName(_config.ONEDI));
  _yCSsyst_accXeff_MCstat.yieldTOT_accXeffCorr = (TH1F*)_yCSstat.yieldTOT_accXeffCorr->Clone("haccXeffTot");
  _yCSsyst_accXeff_MCstat.yields1D_accXeffCorr = (TH1F*)_yCSstat.yields1D_accXeffCorr->Clone("haccXeff1D");
  float accXeffErr = hAccXEffTot->GetBinError(1);
  float accXeffVal = hAccXEffTot->GetBinContent(1);
  float yieldVal = _yCSstat.yieldTOT_unfolded->GetBinContent(1);
  _yCSsyst_accXeff_MCstat.yieldTOT_accXeffCorr->SetBinError(1,yieldVal*accXeffErr/(accXeffVal*accXeffVal));
  for (int ib=1; ib<_yCSsyst_accXeff_MCstat.yields1D_accXeffCorr->GetNbinsX(); ib++){
    accXeffErr = hAccXEff1D->GetBinError(ib);
    accXeffVal = hAccXEff1D->GetBinContent(ib);
    yieldVal = _yCSstat.yields1D_unfolded->GetBinContent(ib);
    _yCSsyst_accXeff_MCstat.yields1D_accXeffCorr->SetBinError(ib,yieldVal*accXeffErr/(accXeffVal*accXeffVal));
  }//end of loop over ib  
  DivideOverLumi(_yCSsyst_accXeff_MCstat);
  DivideOverBinWidth(_yCSsyst_accXeff_MCstat);  

  // print table of all uncerntainties in Latex format
  PrintLatexAll();

}// Calc()

void CalcCrossSection::GetSignalYields()
{
  TFile* fSig = new TFile(_config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_CHISO, "phoEt"));
  std::cout<<"file with signal yields: "<<_config.GetYieldsFileName(_channel,_vgamma,_config.TEMPL_CHISO,"phoEt")<<std::endl;
  std::cout<<"total yields: "<<_config.GetYieldsBkgSubtrDataName(_config.TOTAL)<<std::endl;
  std::cout<<"1D yields: "<<_config.GetYieldsBkgSubtrDataName(_config.ONEDI)<<std::endl;
  _yCSstat.yieldTOT_bkgSubtr=(TH1F*)fSig->Get(_config.GetYieldsBkgSubtrDataName(_config.TOTAL));
  _yCSstat.yields1D_bkgSubtr=(TH1F*)fSig->Get(_config.GetYieldsBkgSubtrDataName(_config.ONEDI));
  _yCSstat.yields1D_bkgSubtr->Print();
  _fOut->cd();
  Print("Bkg Subtr Yields:",_yCSstat.yieldTOT_bkgSubtr,_yCSstat.yields1D_bkgSubtr);
}// end of GetSignalYields()

void CalcCrossSection::GetYieldsSyst(FromYieldToCS& yCS, TString strFile, TString str1D, TString strTOT)
{
  // Get errors from file where errors are saved
  TFile* fSig = new TFile(strFile);
  std::cout<<"file with syst errors: "<<strFile<<std::endl;
  std::cout<<"total errors: "<<strTOT<<std::endl;
  std::cout<<"1D errors: "<<str1D<<std::endl;
  yCS.yieldTOT_bkgSubtr=(TH1F*)fSig->Get(strTOT);
  yCS.yields1D_bkgSubtr=(TH1F*)fSig->Get(str1D);

  yCS.yields1D_bkgSubtr->Print();
  _fOut->cd();
  // Assign histogram content to be equal to yields
  yCS.yieldTOT_bkgSubtr->SetBinContent(1,_yCSstat.yieldTOT_bkgSubtr->GetBinContent(1));
  for (int ib=1; ib<_yCSstat.yields1D_bkgSubtr->GetNbinsX(); ib++){
    yCS.yields1D_bkgSubtr->SetBinContent(ib,_yCSstat.yields1D_bkgSubtr->GetBinContent(ib));
  }//end of loop over ib
  Print("Bkg Subtr Yields:",yCS.yieldTOT_bkgSubtr,yCS.yields1D_bkgSubtr);

}// end of GetYieldsSyst()

void CalcCrossSection::ApplyUnfolding(FromYieldToCS& yCS)
{
  Unfolding unf(_channel,_vgamma);
  bool isOk = unf.PrepareMigrationMatrix();
  if (!isOk){
    std::cout<<"ERROR: PrepareMigrationMatrix() for Unfolding failed"<<std::endl;
    return;
  }
//  TH1D sign1D;
//  _signalYields1D->Copy(sign1D);
  TH1D* signInput1D = (TH1D*)yCS.yields1D_bkgSubtr;
  TH1D* signUnfolded1D = (TH1D*)signInput1D->Clone("hUnfolded");
  isOk = unf.ApplyRooUnfold(signInput1D,signUnfolded1D,RooUnfold::kInvert,yCS.name);
  if (!isOk){
    std::cout<<"ERROR: ApplyRooUnfold() for Unfolding failed"<<std::endl;
    return;
  } 
  unf.PlotAndStore(yCS.name);
  Print("Yields before Unfolding:",yCS.yieldTOT_bkgSubtr,yCS.yields1D_bkgSubtr);
  yCS.yields1D_unfolded=(TH1F*)signUnfolded1D->Clone("hUnfolded1D");
  yCS.yieldTOT_unfolded=(TH1F*)yCS.yieldTOT_bkgSubtr->Clone("hBkgSubtrTOT"); // no unfolding for total
  Print("Unfolded Yields: ",yCS.yieldTOT_unfolded,yCS.yields1D_unfolded);
//  unf.PlotAndStore();
}// end of ApplyUnfolding()

void CalcCrossSection::ApplyAccXEff(FromYieldToCS& yCS)
{
  TFile* f = new TFile(_config.GetAccXEffFileName(_channel,_vgamma));
  TH1F* hAccXEffTot = (TH1F*)f->Get(_config.GetAccXEffName(_config.TOTAL));
  TH1F* hAccXEff1D = (TH1F*)f->Get(_config.GetAccXEffName(_config.ONEDI));
  yCS.yieldTOT_accXeffCorr=(TH1F*)yCS.yieldTOT_unfolded->Clone("hAccXEffCorrTOT");
  yCS.yields1D_accXeffCorr=(TH1F*)yCS.yields1D_unfolded->Clone("hAccXEffCorr1D");

  //When divide over accXeff, only stat error is propagated
  //error bars on accXeff itself should be propagated separately 
  //as syst due to limited MC statistics

  //Divide over accXeff Total:
  float accXeffTot = hAccXEffTot->GetBinContent(1);
  float yieldTot = yCS.yieldTOT_accXeffCorr->GetBinContent(1);
  float errTot = yCS.yieldTOT_accXeffCorr->GetBinError(1);
  yCS.yieldTOT_accXeffCorr->SetBinContent(1,yieldTot/accXeffTot);
  yCS.yieldTOT_accXeffCorr->SetBinError(1,errTot/accXeffTot);

  //Divide over accXeff 1D:
  for (int ib=1; ib<=yCS.yields1D_accXeffCorr->GetNbinsX(); ib++){
    float accXeff1D = hAccXEff1D->GetBinContent(ib);
    float yields1D = yCS.yields1D_accXeffCorr->GetBinContent(ib);
    float err1D = yCS.yields1D_accXeffCorr->GetBinError(ib);
    yCS.yields1D_accXeffCorr->SetBinContent(ib,yields1D/accXeff1D);    
    yCS.yields1D_accXeffCorr->SetBinError(ib,err1D/accXeff1D);  
  }// end of loop over ib

//  yCS.yieldTOT_accXeffCorr->Divide(hAccXEffTot);
//  yCS.yields1D_accXeffCorr->Divide(hAccXEff1D);

  Print("Acc X Eff Corrected Yields: ",yCS.yieldTOT_accXeffCorr,yCS.yields1D_accXeffCorr);
}// end of ApplyAccXEff()

void CalcCrossSection::DivideOverLumi(FromYieldToCS& yCS)
{
  yCS.yieldTOT_overLumi=(TH1F*)yCS.yieldTOT_accXeffCorr->Clone("hOverLumiTOT");
  yCS.yields1D_overLumi=(TH1F*)yCS.yields1D_accXeffCorr->Clone("hOverLumi1D");
  float cont=yCS.yieldTOT_accXeffCorr->GetBinContent(1);
  float err=yCS.yieldTOT_accXeffCorr->GetBinError(1);
  yCS.yieldTOT_overLumi->SetBinContent(1,1000*cont/_lumi);
  yCS.yieldTOT_overLumi->SetBinError(1,1000*err/_lumi);
  for (int ib=1; ib<=yCS.yields1D_accXeffCorr->GetNbinsX(); ib++){
    cont=yCS.yields1D_overLumi->GetBinContent(ib);
    err=yCS.yields1D_overLumi->GetBinError(ib);
    yCS.yields1D_overLumi->SetBinContent(ib,cont/_lumi);
    yCS.yields1D_overLumi->SetBinError(ib,err/_lumi);
  }//end of loop over ib
  Print("Yields over Lumi (pb): ",yCS.yieldTOT_overLumi,yCS.yields1D_overLumi);
}// end of DivideOverLumi()

void CalcCrossSection::DivideOverBinWidth(FromYieldToCS& yCS)
{
  yCS.crossSectionTOT=(TH1F*)yCS.yieldTOT_overLumi->Clone("crossSectionTOT");
  yCS.crossSection1D=(TH1F*)yCS.yields1D_overLumi->Clone("crossSection1D");
  for (int ib=1; ib<=yCS.yields1D_overLumi->GetNbinsX(); ib++){
    float cont=yCS.yields1D_overLumi->GetBinContent(ib);
    float err=yCS.yields1D_overLumi->GetBinError(ib);
    float width=yCS.yields1D_overLumi->GetBinWidth(ib);
    yCS.crossSection1D->SetBinContent(ib,1000*cont/width);
    yCS.crossSection1D->SetBinError(ib,1000*err/width); 
  }// end of loop over ib
  std::cout<<"yCS title: "<<yCS.title<<std::endl;
  Print("Total (pb) and differential(pb/GeV) cross sections: ",yCS.crossSectionTOT,yCS.crossSection1D);
}// end of DivideOverBinWidth()

void CalcCrossSection::Plot(FromYieldToCS& yCS)
{
  TString canvTitle="c_CS_";
  canvTitle+=_config.StrChannel(_channel);
  canvTitle+="_";
  canvTitle+=_config.StrVgType(_vgamma);
  canvTitle+="_";
  canvTitle+=_config.StrBlindType(_blind);
  TCanvas* canv=new TCanvas(canvTitle,canvTitle,800,800);
  canv->Divide(2,1);
  TPad* pad1 = (TPad*)canv->GetPad(1);
  TPad* pad2 = (TPad*)canv->GetPad(2);
  pad1->SetLogx();
  pad1->SetLogy();
  pad2->SetLogx();
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
  TString fName=_config.GetAccXEffFileName(_channel, _vgamma);
  TFile* fTheory = new TFile(fName);
  TH1F* hTheory = (TH1F*)fTheory->Get(_config.GetTheoryCSname(_config.ONEDI));
  // Multiply by 1000, pb -> fb
  for (int ib=1; ib<=yCS.crossSection1D->GetNbinsX(); ib++){
    float cont; float err;
    cont = yCS.crossSection1D->GetBinContent(ib);
    err = yCS.crossSection1D->GetBinError(ib);
//    yCS.crossSection1D->SetBinContent(ib,1000*cont);
//    yCS.crossSection1D->SetBinError(ib,1000*err);
    cont = hTheory->GetBinContent(ib);
    err = hTheory->GetBinError(ib);
    hTheory->SetBinContent(ib,1000*cont);
    hTheory->SetBinError(ib,1000*err);
  }// end of loop over ib
   Print("Total (pb) and differential(fb/GeV) cross sections: ", yCS.crossSectionTOT, yCS.crossSection1D);
  pad1->cd();
  yCS.crossSection1D->GetXaxis()->SetNoExponent();
  yCS.crossSection1D->GetXaxis()->SetMoreLogLabels();
  yCS.crossSection1D->SetLineWidth(2);
  TString strTitle="PRELIMINARY: d#sigma/dP_{T}^{#gamma}, fb/GeV, ";
  strTitle+=_config.StrChannel(_channel);
  strTitle+=" ";
  strTitle+=_config.StrVgType(_vgamma);
  strTitle+=" ";
  strTitle+=_config.StrBlindType(_blind);
  yCS.crossSection1D->SetTitle(strTitle);
  yCS.crossSection1D->SetStats(0);
  yCS.crossSection1D->Draw();
  // Plot theory Cross section
  hTheory->SetLineWidth(2);
  hTheory->SetLineColor(2);
  hTheory->Draw("same");
  yCS.crossSection1D->Draw("same");
  TLegend *leg = new TLegend(0.70,0.70,0.95,0.95);
  leg->AddEntry(yCS.crossSection1D,"measured CS","l");
  leg->AddEntry(hTheory,"theory CS","l");
  leg->Draw("same");

  pad2->cd();
  TH1F* hRatio = (TH1F*) yCS.crossSection1D->Clone("hRatio");
  hRatio->Divide(hTheory);
  float max=1.1; float min=0.9;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    if (hRatio->GetBinContent(ib)+hRatio->GetBinError(ib)>max) 
      max=hRatio->GetBinContent(ib)+hRatio->GetBinError(ib);
    if (hRatio->GetBinContent(ib)-hRatio->GetBinError(ib)<min) 
      min=hRatio->GetBinContent(ib)-hRatio->GetBinError(ib);
  }// end of loop over ib
  hRatio->SetStats(0);
  hRatio->SetLineWidth(2);
  hRatio->GetYaxis()->SetRangeUser(min,max);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->GetXaxis()->SetMoreLogLabels();
  hRatio->GetXaxis()->SetNoExponent();
  hRatio->SetTitle("; phoEt, GeV;");
  hRatio->Draw();

  int nBins = hRatio->GetNbinsX();
  TLine* line = new TLine(hRatio->GetBinLowEdge(1),1,hRatio->GetBinLowEdge(nBins)+hRatio->GetBinWidth(nBins),1);
  line->SetLineWidth(2);
  line->SetLineStyle(9);
  line->Draw("same");

  TString nameSave=_config.GetPlotsDirName(_channel,_vgamma,_config.PLOTS_CROSS_SECTION);
  nameSave+=canv->GetTitle();
  nameSave+=".png";
  canv->SaveAs(nameSave);
  nameSave.ReplaceAll(".png",".pdf");
  canv->SaveAs(nameSave);
  nameSave.ReplaceAll(".pdf",".root");
  canv->SaveAs(nameSave);

  _fOut->cd();
  yCS.crossSectionTOT->Write(_config.GetCSname(_channel,_config.TOTAL)); 
  yCS.crossSection1D->Write(_config.GetCSname(_channel,_config.ONEDI));
  hTheory->Write(_config.GetTheoryCSname(_config.ONEDI));
  
}// end of Plot()

void CalcCrossSection::Print(TString strYields, TH1F* hTot, TH1F* h1D)
{
  std::cout<<std::endl;
  std::cout<<strYields<<std::endl;
  std::cout<<"tot: "<<hTot->GetBinContent(1)<<"+-"<<hTot->GetBinError(1)<<std::endl;
  std::cout<<"1D: "<<std::endl;
  for (int ib=1; ib<=h1D->GetNbinsX(); ib++){
    if (hTot->GetBinContent(1)<15000) std::cout<<std::setprecision(2);
    std::cout<<h1D->GetBinLowEdge(ib)<<"-"<<h1D->GetBinLowEdge(ib)+h1D->GetBinWidth(ib)<<": "<<h1D->GetBinContent(ib)<<"+-"<<h1D->GetBinError(ib)<<std::endl;
  }// end of ib
  std::cout<<std::endl;
}// end of Print()
