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
#include "TLatex.h"
#include "TText.h"
#include "TString.h"
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

void CalcCrossSection::PrintLatexAll_ErrInPercent()
{

  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;

  std::cout<<"||||========== Table with err in %"<<std::endl;


    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Relative errors [\\%]. ";
    std::cout<<_config.StrChannel(_channel)<<" "<<_config.StrVgType(_vgamma)<<"}"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih

    // print |c|c|c|.....|c|
    std::cout<<"  \\begin{tabular}{|c|";
    for (int errT=0; errT<Nerrs; errT++) 
      if (_yCSarray[errT].errType!=ERR_NONE) std::cout<<"c|";
    std::cout<<"}"<<std::endl;

    // print strUp
    std::cout<<"    bin ";
    for (int errT=0; errT<Nerrs; errT++) 
      if (_yCSarray[errT].errType!=ERR_NONE) std::cout<<" & "<<_yCSarray[errT].strUp;
    std::cout<<"\\\\"<<std::endl;
    // print strDown
    std::cout<<"    lims ";
    for (int errT=0; errT<Nerrs; errT++) 
      if (_yCSarray[errT].errType!=ERR_NONE) std::cout<<" & "<<_yCSarray[errT].strDown;
    std::cout<<"\\\\ \\hline"<<std::endl;

      std::cout<<"    ";
      std::cout<<std::setprecision(0)<<"total ";
    std::cout<<std::setprecision(0);
    for (int errT=0; errT<Nerrs; errT++) 
      if (_yCSarray[errT].errType!=ERR_NONE) 
        std::cout<<" & "<<100*_yCSarray[errT].crossSectionTOT->GetBinError(1)/_yCSarray[ERR_STAT].crossSectionTOT->GetBinContent(1);

      std::cout<<" \\\\ \\hline"<<std::endl;
    //loop over pt bins
    for (int ib=1; ib<=_yCSarray[ERR_STAT].crossSection1D->GetNbinsX(); ib++){

      if (ib==1) std::cout<<"%";

      std::cout<<"    ";
      std::cout<<_yCSarray[ERR_STAT].crossSection1D->GetBinLowEdge(ib)<<"-";
      std::cout<<_yCSarray[ERR_STAT].crossSection1D->GetBinLowEdge(ib)+_yCSarray[ERR_STAT].crossSection1D->GetBinWidth(ib);

      for (int errT=0; errT<Nerrs; errT++) 
        if (_yCSarray[errT].errType!=ERR_NONE) 
          std::cout<<" & "<<100*_yCSarray[errT].crossSection1D->GetBinError(ib)/_yCSarray[ERR_STAT].crossSection1D->GetBinContent(ib);

      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:systInPercent_";
    std::cout<<_config.StrChannel(_channel)<<"_"<<_config.StrVgType(_vgamma);
    std::cout<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;

}//end of PrintLatexAll_ErrInPercent()

void CalcCrossSection::PrintLatexAll_MeasVsMCbased()
{
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;

  std::cout<<"||||========== Table with meas vs MCbased"<<std::endl;

  TString fName=_config.GetAccXEffFileName(_channel, _vgamma);
  TFile* fTheory = new TFile(fName);
  TH1F* hTheory1D = (TH1F*)fTheory->Get(_config.GetTheoryCSname(_config.ONEDI));
  TH1F* hTheoryTot = (TH1F*)fTheory->Get(_config.GetTheoryCSname(_config.TOTAL));
    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Cross section and errors}"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih
    std::cout<<"  \\begin{tabular}{|c|c|c|}"<<std::endl;
    std::cout<<"    bin & $d\\sigma/dP_{T}$ &$d\\sigma/dP_{T}$ \\\\ "<<std::endl;
    std::cout<<"    lims & MC based &    meas.       \\\\ \\hline"<<std::endl;



      std::cout<<"    ";
      std::cout<<std::setprecision(0)<<"total & ";

      std::cout<<std::setprecision(0)<<1000*hTheoryTot->GetBinContent(1)<<" & $";

      std::cout<<std::setprecision(0)<<_yCSarray[ERR_STAT].crossSectionTOT->GetBinContent(1)<<" \\pm ";

      std::cout<<std::setprecision(0)<<_yCSarray[ERR_STAT].crossSectionTOT->GetBinError(1)<<" \\pm ";

      std::cout<<std::setprecision(0)<<_yCSarray[ERR_SYST_SUM].crossSectionTOT->GetBinError(1)<<"$";

      std::cout<<" \\\\ \\hline"<<std::endl;
    //loop over pt bins
    for (int ib=1; ib<=_yCSarray[ERR_STAT].crossSection1D->GetNbinsX(); ib++){

      if (ib==1) std::cout<<"%";

      std::cout<<"    ";
      std::cout<<std::setprecision(0)<<_yCSarray[ERR_STAT].crossSection1D->GetBinLowEdge(ib)<<"-"<<_yCSarray[ERR_STAT].crossSection1D->GetBinLowEdge(ib)+_yCSarray[ERR_STAT].crossSection1D->GetBinWidth(ib)<<" & ";

      float cont = _yCSarray[ERR_STAT].crossSection1D->GetBinContent(ib);
      float pres=0;
      if (cont<10) pres=1; //0.1
      if (cont<1)  pres=2;
      if (cont<0.1) pres=3;

      std::cout<<std::setprecision(pres)<<1000*hTheory1D->GetBinContent(ib)<<" & $";
   
      std::cout<<std::setprecision(pres)<<cont<<" \\pm ";

      std::cout<<std::setprecision(pres)<<_yCSarray[ERR_STAT].crossSection1D->GetBinError(ib)<<" \\pm ";

      std::cout<<std::setprecision(pres)<<_yCSarray[ERR_SYST_SUM].crossSection1D->GetBinError(ib)<<"$";



      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:sc_mc_vs_meas_";
    std::cout<<_config.StrChannel(_channel)<<"_"<<_config.StrVgType(_vgamma);
    std::cout<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;

}//end of PrintLatexAll()

void CalcCrossSection::Calc()
{

  int errT;

  for (errT=0; errT<Nerrs; errT++){
    _yCSarray[errT].errType=ERR_NONE;
  }

  errT=ERR_STAT;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].name="yield_pm_stat";
  _yCSarray[errT].title="yield+-stat";
  _yCSarray[errT].strUp="err";
  _yCSarray[errT].strDown="stat";
  std::cout<<"ERR_STAT "<<_yCSarray[errT].title<<std::endl;
  GetSignalYields(ERR_STAT);
  ApplyUnfolding(0,_yCSarray[errT]);
  ApplyAccXEff(_yCSarray[errT]);
  DivideOverLumi(_yCSarray[errT]);
  DivideOverBinWidth(_yCSarray[errT]);

  errT=ERR_SYST_CHISOvsSIHIH;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].name="syst_CHISOvsSIHIH";
  _yCSarray[errT].title="|N_{Ich}-N_{#sigma{i#eta i#eta}}|";
  _yCSarray[errT].strUp="syst";
  _yCSarray[errT].strDown="$|N_{Ich}-N_{\\sigma{i\\eta i\\eta}}|$";
  std::cout<<"ERR_SYST_CHISOvsSIHIH "<<_yCSarray[errT].title<<std::endl;
  TString strCh, strVg;
  if (_channel==_config.MUON) strCh="MUON";
  if (_channel==_config.ELECTRON) strCh="ELECTRON";
  if (_vgamma==_config.W_GAMMA) strVg="WGamma";
  if (_vgamma==_config.Z_GAMMA) strVg="ZGamma";
  TString strF="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_SystCHISOvsSIHIH_phoEt.root";
  GetYieldsSyst(_yCSarray[errT], 
    strF,
    "yieldsDDTrueONEDICOMMON",
    "yieldsDDTrueTOTALCOMMON");
//    _config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_OVERLAY, "phoEt"), 
//    _config.GetSystCHISOvsSIHIHname(_config.ONEDI,_config.COMMON), 
//    _config.GetSystCHISOvsSIHIHname(_config.TOTAL,_config.COMMON));
  ApplyUnfolding(0,_yCSarray[errT]);
  ApplyAccXEff(_yCSarray[errT]);
  DivideOverLumi(_yCSarray[errT]);
  DivideOverBinWidth(_yCSarray[errT]);  

  errT=ERR_SYST_ZgMC_Norm;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].name="syst_ZgMC_Norm";
  _yCSarray[errT].title="Z#gamma MC norm";
  _yCSarray[errT].strUp="Zg MC";
  _yCSarray[errT].strDown="norm";
  std::cout<<"ERR_SYST_ZgMC_Norm "<<_yCSarray[errT].title<<std::endl;
  strF="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_"+strVg+"_TEMPL_CHISO_phoEt__SystZgNorm.root";
  GetYieldsSyst(_yCSarray[errT], 
    strF,
    "yieldsDDTrueONEDICOMMON",
    "yieldsDDTrueTOTALCOMMON");
//    _config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_OVERLAY, "phoEt"), 
//    _config.GetSystCHISOvsSIHIHname(_config.ONEDI,_config.COMMON), 
//    _config.GetSystCHISOvsSIHIHname(_config.TOTAL,_config.COMMON));
  ApplyUnfolding(0,_yCSarray[errT]);
  ApplyAccXEff(_yCSarray[errT]);
  DivideOverLumi(_yCSarray[errT]);
  DivideOverBinWidth(_yCSarray[errT]); 

//  errT=ERR_SYST_TemplStat;
//  _yCSarray[errT].errType=errT;
//  _yCSarray[errT].title="template statistics";
//  _yCSarray[errT].name="syst_templStat";
//  _yCSarray[errT].strUp="templ";
//  _yCSarray[errT].strDown="stat";
//  std::cout<<"ERR_SYST_TemplStat "<<_yCSarray[errT].title<<std::endl;
//  TString strName=_config.GetDDTemplateFileName(_channel,_vgamma,_config.TEMPL_CHISO,"phoEt");
//  strName.ReplaceAll(".root","_SystRand.root");
//  GetYieldsSyst(_yCSarray[errT], 
//    strName, 
//    _config.GetYieldsDDTemplateTrueName(_config.ONEDI,_config.COMMON), 
//    _config.GetYieldsDDTemplateTrueName(_config.ONEDI,_config.COMMON));
//  ApplyUnfolding(0,_yCSarray[errT]);
//  ApplyAccXEff(_yCSarray[errT]);
//  DivideOverLumi(_yCSarray[errT]);
//  DivideOverBinWidth(_yCSarray[errT]);  

  if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA){
    errT=ERR_SYST_etogStat;
    _yCSarray[errT].errType=errT;  
    _yCSarray[errT].title="e#rightarrow#gamma, samples stat";
    _yCSarray[errT].name="syst_etog_stat";
    _yCSarray[errT].strUp="e\\rightarrow\\gamma samp.";
    _yCSarray[errT].strDown="stat";
    std::cout<<"ERR_SYST_etogStat "<<_yCSarray[errT].title<<std::endl;
    GetYieldsSyst(_yCSarray[errT], 
    _config.GetDDBkgEtoGammaFileName("phoEt"), 
    _config.GetYieldsDDBkgEtoGamma(_config.ONEDI,_config.COMMON), 
    _config.GetYieldsDDBkgEtoGamma(_config.TOTAL,_config.COMMON));
    ApplyUnfolding(0,_yCSarray[errT]);
    ApplyAccXEff(_yCSarray[errT]);
    DivideOverLumi(_yCSarray[errT]);
    DivideOverBinWidth(_yCSarray[errT]);  
  }// end of if (_channel==_config.ELECTRON && _channel==_config.W_GAMMA)

  errT=ERR_SYST_BkgSubtrZgWgtaunu;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].name="syst_real_g_bkg_subtr";
  _yCSarray[errT].title="real #gamma bkg subtr";
  _yCSarray[errT].strUp="real \\gamma";
  _yCSarray[errT].strDown="bkg";
  std::cout<<"ERR_SYST_BkgSubtrZgWgtaunu "<<_yCSarray[errT].title<<std::endl;
  strF="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/yields_"+strVg+"__phoEt_.root";
  GetYieldsSyst(_yCSarray[errT], 
    strF,
    "systBkgSubtrZgWgtaunu_ONEDI_COMMON",
    "systBkgSubtrZgWgtaunu_TOTAL_COMMON");
//    _config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_OVERLAY, "phoEt"), 
//    _config.GetSystCHISOvsSIHIHname(_config.ONEDI,_config.COMMON), 
//    _config.GetSystCHISOvsSIHIHname(_config.TOTAL,_config.COMMON));
  ApplyUnfolding(0,_yCSarray[errT]);
  ApplyAccXEff(_yCSarray[errT]);
  DivideOverLumi(_yCSarray[errT]);
  DivideOverBinWidth(_yCSarray[errT]);

  errT=ERR_SYST_accXeff_MCstat;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].title="accXeff, MC stat";
  _yCSarray[errT].name="syst_accXeff_MCstat";
    _yCSarray[errT].strUp="accXeff";
    _yCSarray[errT].strDown="MC stat";
    std::cout<<"ERR_SYST_accXeff_MCstat "<<_yCSarray[errT].title<<std::endl;
  TFile* f = new TFile(_config.GetAccXEffFileName(_channel,_vgamma));
  TH1F* hAccXEffTot = (TH1F*)f->Get(_config.GetAccXEffName(_config.TOTAL));
  TH1F* hAccXEff1D = (TH1F*)f->Get(_config.GetAccXEffName(_config.ONEDI));
  _yCSarray[errT].yieldTOT_accXeffCorr = (TH1F*)_yCSarray[ERR_STAT].yieldTOT_accXeffCorr->Clone("haccXeffTot");
  _yCSarray[errT].yields1D_accXeffCorr = (TH1F*)_yCSarray[ERR_STAT].yields1D_accXeffCorr->Clone("haccXeff1D");
  float accXeffErr = hAccXEffTot->GetBinError(1);
  float accXeffVal = hAccXEffTot->GetBinContent(1);
  float yieldVal = _yCSarray[ERR_STAT].yieldTOT_unfolded->GetBinContent(1);
  _yCSarray[errT].yieldTOT_accXeffCorr->SetBinError(1,yieldVal*accXeffErr/(accXeffVal*accXeffVal));
  for (int ib=1; ib<=_yCSarray[errT].yields1D_accXeffCorr->GetNbinsX(); ib++){
    accXeffErr = hAccXEff1D->GetBinError(ib);
    accXeffVal = hAccXEff1D->GetBinContent(ib);
    yieldVal = _yCSarray[ERR_STAT].yields1D_unfolded->GetBinContent(ib);
    _yCSarray[errT].yields1D_accXeffCorr->SetBinError(ib,yieldVal*accXeffErr/(accXeffVal*accXeffVal));
  }//end of loop over ib  
  DivideOverLumi(_yCSarray[errT]);
  DivideOverBinWidth(_yCSarray[errT]);  


  errT=ERR_SYST_UNF_MCstat;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].name="unf_MC_stat";
  _yCSarray[errT].title="unf, MC stat";
  _yCSarray[errT].strUp="unf";
  _yCSarray[errT].strDown="MC stat";
  std::cout<<"ERR_SYST_UNF_MCstat "<<_yCSarray[errT].title<<std::endl;
  GetSignalYields(ERR_SYST_UNF_MCstat);
  ApplyUnfolding(1,_yCSarray[errT]);
  ApplyAccXEff(_yCSarray[errT]);
  DivideOverLumi(_yCSarray[errT]);
  DivideOverBinWidth(_yCSarray[errT]);


  errT=ERR_SYST_LUMI;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].title="lumi +-2.6%";
  _yCSarray[errT].name="syst_lumi";
  _yCSarray[errT].strUp="syst";
  _yCSarray[errT].strDown="lumi";
  std::cout<<"ERR_SYST_LUMI "<<_yCSarray[errT].title<<std::endl;
  _yCSarray[errT].crossSectionTOT = (TH1F*)_yCSarray[ERR_STAT].crossSectionTOT->Clone("hCSlumiSystTot");
  _yCSarray[errT].crossSection1D = (TH1F*)_yCSarray[ERR_STAT].crossSection1D->Clone("hCSlumiSyst1D");
  float cs = _yCSarray[ERR_STAT].crossSectionTOT->GetBinContent(1);
  _yCSarray[errT].crossSectionTOT->SetBinError(1,_lumiErr*cs);
  for (int ib=1; ib<=_yCSarray[errT].crossSection1D->GetNbinsX(); ib++){
    cs = _yCSarray[ERR_STAT].crossSection1D->GetBinContent(ib);
    _yCSarray[errT].crossSection1D->SetBinError(ib,_lumiErr*cs);
  }//end of loop over ib  
  Print("Lumi syst, cs: ",_yCSarray[errT].crossSectionTOT,_yCSarray[errT].crossSection1D);

  if (_vgamma==_config.W_GAMMA){
    
    errT=ERR_SYST_WMtCut;
    _yCSarray[errT].errType=errT;  
    _yCSarray[errT].title="WMt cut +-5GeV";
    _yCSarray[errT].name="syst_WMt_cut";
    _yCSarray[errT].strUp="WMt";
    _yCSarray[errT].strDown="cut";
    std::cout<<"ERR_SYST_WMtCut "<<_yCSarray[errT].title<<std::endl;
    ComputeSystByAnalysisVariation(errT, "WGammaAnalysisAux21_WMtLow", "WGammaAnalysisAux22_WMtUp");
    
    errT=ERR_SYST_PUweight;
    _yCSarray[errT].errType=errT;  
    _yCSarray[errT].title="PUweight +-5%";
    _yCSarray[errT].name="syst_PUweight";
    _yCSarray[errT].strUp="PU";
    _yCSarray[errT].strDown="weight";
    std::cout<<"ERR_SYST_PUweight "<<_yCSarray[errT].title<<std::endl;
    ComputeSystByAnalysisVariation(errT, "WGammaAnalysisAux23_PUreweight_minus5pc", "WGammaAnalysisAux24_PUreweight_plus5pc");

    
      errT=ERR_SYST_SFs;
      _yCSarray[errT].errType=errT;  
      _yCSarray[errT].title="SFs +-1sigma";
      _yCSarray[errT].name="syst_SFs";
      _yCSarray[errT].strUp="SFs";
      _yCSarray[errT].strDown="err";
      std::cout<<"ERR_SYST_SFs "<<_yCSarray[errT].title<<std::endl;
      ComputeSystByAnalysisVariation(errT, "WGammaAnalysisAux25_ApplySF_minusSigma", "WGammaAnalysisAux26_ApplySF_plusSigma");
    

  }// end of if (_channel==_config.MUON && _channel==_config.W_GAMMA)

  errT=ERR_SYST_SUM;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].title="sum of syst";
  _yCSarray[errT].name="syst_sum";
  _yCSarray[errT].strUp="syst";
  _yCSarray[errT].strDown="total";
  std::cout<<"ERR_SYST_SUM "<<_yCSarray[errT].title<<std::endl;
  bool sumStarted=0;
//  _yCSarray[errT].crossSectionTOT=(TH1F*)_yCSarray[ERR_STAT].crossSectionTOT->Clone();
//  _yCSarray[errT].crossSection1D=(TH1F*)_yCSarray[ERR_STAT].crossSection1D->Clone();
  float errTot=0;
  for (int ieT=0; ieT<Nerrs; ieT++) {
    if (_yCSarray[ieT].errType==ERR_STAT || _yCSarray[ieT].errType>=ERR_SYST_SUM) continue;
    if (!sumStarted){
       _yCSarray[errT].crossSectionTOT=(TH1F*)_yCSarray[ieT].crossSectionTOT->Clone();
       _yCSarray[errT].crossSection1D=(TH1F*)_yCSarray[ieT].crossSection1D->Clone();
       sumStarted=1;
    }
    else{
//      _yCSarray[errT].crossSectionTOT->Add(_yCSarray[ieT].crossSectionTOT); 
      _yCSarray[errT].crossSection1D->Add(_yCSarray[ieT].crossSection1D); 
    }    
    errTot+=_yCSarray[ieT].crossSectionTOT->GetBinError(1)*_yCSarray[ieT].crossSectionTOT->GetBinError(1);
  }//end of loop over ieT
  errTot=sqrt(errTot);
  _yCSarray[errT].crossSectionTOT->SetBinContent(1,_yCSarray[ERR_STAT].crossSectionTOT->GetBinContent(1));
  _yCSarray[errT].crossSectionTOT->SetBinError(1,errTot);
  for (int ib=1; ib<=_yCSarray[errT].crossSection1D->GetNbinsX(); ib++){
    _yCSarray[errT].crossSection1D->SetBinContent(ib,_yCSarray[ERR_STAT].crossSection1D->GetBinContent(ib));
  }//end of loop over ib  

  errT=ERR_SUM;
  _yCSarray[errT].errType=errT;
  _yCSarray[errT].title="sum of syst and stat";
  _yCSarray[errT].name="syst_and_stat_sum";
  _yCSarray[errT].strUp="syst + stat";
  _yCSarray[errT].strDown="total";
  std::cout<<"ERR_SUM "<<_yCSarray[errT].title<<std::endl;
  _yCSarray[errT].crossSectionTOT=(TH1F*)_yCSarray[ERR_STAT].crossSectionTOT->Clone();
  _yCSarray[errT].crossSection1D=(TH1F*)_yCSarray[ERR_STAT].crossSection1D->Clone();
  _yCSarray[errT].crossSectionTOT->Add(_yCSarray[ERR_SYST_SUM].crossSectionTOT); 
  _yCSarray[errT].crossSection1D->Add(_yCSarray[ERR_SYST_SUM].crossSection1D); 
  _yCSarray[errT].crossSectionTOT->SetBinContent(1,_yCSarray[ERR_STAT].crossSectionTOT->GetBinContent(1));
  for (int ib=1; ib<=_yCSarray[errT].crossSection1D->GetNbinsX(); ib++){
    _yCSarray[errT].crossSection1D->SetBinContent(ib,_yCSarray[ERR_STAT].crossSection1D->GetBinContent(ib));
  }//end of loop over ib 

  Plot();

  // print table of all uncerntainties in Latex format
  PrintLatexAll_ErrInPercent();
  PrintLatexAll_MeasVsMCbased();

}// Calc()

void CalcCrossSection::ComputeSystByAnalysisVariation(int errT, TString strDir1, TString strDir2){
    _yCSarray[errT].crossSectionTOT = (TH1F*)_yCSarray[ERR_STAT].crossSectionTOT->Clone(TString("hCS")+_yCSarray[errT].name+TString("Tot"));
    _yCSarray[errT].crossSection1D = (TH1F*)_yCSarray[ERR_STAT].crossSection1D->Clone(TString("hCS")+_yCSarray[errT].name+TString("1D"));
    TString str1 = _config.GetCrossSectionFileName(_channel, _vgamma);
    str1.ReplaceAll("../WGammaOutput/",TString("../../")+strDir1+TString("/WGammaOutput/"));
    TString str2 = _config.GetCrossSectionFileName(_channel, _vgamma);
    str2.ReplaceAll("../WGammaOutput/",TString("../../")+strDir2+TString("/WGammaOutput/"));
    TFile f1(str1);
    TFile f2(str2);
    if (!f1.IsOpen() || !f2.IsOpen()) {_yCSarray[errT].errType=ERR_NONE;}
    else{
      std::cout<<"syst "<<_yCSarray[errT].title<<" will be computed"<<std::endl;
      TH1F* cs1D_1 = (TH1F*)f1.Get(_config.GetCSname(_channel, _config.ONEDI));
      TH1F* csTOT_1 = (TH1F*)f1.Get(_config.GetCSname(_channel, _config.TOTAL));
      TH1F* cs1D_2 = (TH1F*)f2.Get(_config.GetCSname(_channel, _config.ONEDI));
      TH1F* csTOT_2 = (TH1F*)f2.Get(_config.GetCSname(_channel, _config.TOTAL));

      float spreadTot1 = fabs(csTOT_1->GetBinContent(1)-csTOT_2->GetBinContent(1));
      float spreadTot2 = fabs(csTOT_1->GetBinContent(1)-_yCSarray[errT].crossSectionTOT->GetBinContent(1));
      float spreadTot3 = fabs(_yCSarray[errT].crossSectionTOT->GetBinContent(1)-csTOT_2->GetBinContent(1));
      float spreadTot;
      if (spreadTot1>spreadTot2) spreadTot=spreadTot1;
      else spreadTot=spreadTot2;
      if (spreadTot3>spreadTot) spreadTot=spreadTot3;
      _yCSarray[errT].crossSectionTOT->SetBinError(1,spreadTot);
      for (int ib=1; ib<=_yCSarray[errT].crossSection1D->GetNbinsX(); ib++){
        float spread1D1 = fabs(cs1D_1->GetBinContent(ib)-cs1D_2->GetBinContent(ib));
        float spread1D2 = fabs(cs1D_1->GetBinContent(ib)-_yCSarray[errT].crossSection1D->GetBinContent(ib));
        float spread1D3 = fabs(_yCSarray[errT].crossSection1D->GetBinContent(ib)-cs1D_2->GetBinContent(ib));
        float spread1D;
        if (spread1D1>spread1D2) spread1D=spread1D1;
        else spread1D=spread1D2;
        if (spread1D3>spread1D) spread1D=spread1D3;
        _yCSarray[errT].crossSection1D->SetBinError(ib,spread1D);
      }//end of loop over ib 

    }
    Print(_yCSarray[errT].title+TString(", cs: "),_yCSarray[errT].crossSectionTOT,_yCSarray[errT].crossSection1D);
}// end of ComputeSystByAnalysisVariation()

void CalcCrossSection::GetSignalYields(int errT)
{
  TFile* fSig = new TFile(_config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_CHISO, "phoEt"));
  std::cout<<"file with signal yields: "<<_config.GetYieldsFileName(_channel,_vgamma,_config.TEMPL_CHISO,"phoEt")<<std::endl;
  std::cout<<"total yields: "<<_config.GetYieldsBkgSubtrDataName(_config.TOTAL)<<std::endl;
  std::cout<<"1D yields: "<<_config.GetYieldsBkgSubtrDataName(_config.ONEDI)<<std::endl;
  _yCSarray[errT].yieldTOT_bkgSubtr=(TH1F*)fSig->Get(_config.GetYieldsBkgSubtrDataName(_config.TOTAL));
  _yCSarray[errT].yields1D_bkgSubtr=(TH1F*)fSig->Get(_config.GetYieldsBkgSubtrDataName(_config.ONEDI));
  _yCSarray[errT].yields1D_bkgSubtr->Print();
 
//  if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA){
//    TH1F* hSigMC = (TH1F*)fSig->Get("yieldsSelected_SIGMC__ONEDI_COMMON");
//    // take underflow bin from signal MC for WGamma electron channel
//   float cont = hSigMC->GetBinContent(1);
//    float err = sqrt(hSigMC->GetBinError(1)*hSigMC->GetBinError(1)+0.2*cont*0.2*cont);
//    _yCSstat.yields1D_bkgSubtr->SetBinContent(1,cont);
//    _yCSstat.yields1D_bkgSubtr->SetBinError(1,err);
//  }

  TString strCh, strVg;
  if (_channel==_config.MUON) strCh="MUON";
  if (_channel==_config.ELECTRON) strCh="ELECTRON";
  if (_vgamma==_config.W_GAMMA) strVg="WGamma";
  if (_vgamma==_config.Z_GAMMA) strVg="ZGamma";
  TString strF="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_SystCHISOvsSIHIH_phoEt.root";
  TFile* fSigMeth2 = new TFile(strF);
  TH1F* h1D = (TH1F*)fSigMeth2->Get("yieldsDDTrueONEDICOMMON");
  TH1F* hTot = (TH1F*)fSigMeth2->Get("yieldsDDTrueTOTALCOMMON");

  _yCSarray[errT].yieldTOT_bkgSubtr->SetBinContent(1,hTot->GetBinContent(1));
  for (int ib=1; ib<=h1D->GetNbinsX(); ib++){
    _yCSarray[errT].yields1D_bkgSubtr->SetBinContent(ib,h1D->GetBinContent(ib));
  }//end of loop over ib

  _fOut->cd();
  Print("Bkg Subtr Yields:",_yCSarray[errT].yieldTOT_bkgSubtr,_yCSarray[errT].yields1D_bkgSubtr);
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
  yCS.yieldTOT_bkgSubtr->SetBinContent(1,_yCSarray[ERR_STAT].yieldTOT_bkgSubtr->GetBinContent(1));
  for (int ib=1; ib<_yCSarray[ERR_STAT].yields1D_bkgSubtr->GetNbinsX(); ib++){
    yCS.yields1D_bkgSubtr->SetBinContent(ib,_yCSarray[ERR_STAT].yields1D_bkgSubtr->GetBinContent(ib));
  }//end of loop over ib

//  if (_channel==_config.ELECTRON && _vgamma==_config.W_GAMMA){
//    yCS.yields1D_bkgSubtr->SetBinError(1,_yCSstat.yields1D_bkgSubtr->GetBinError(1));
//  }

  Print("Bkg Subtr Yields:",yCS.yieldTOT_bkgSubtr,yCS.yields1D_bkgSubtr);

}// end of GetYieldsSyst()

void CalcCrossSection::ApplyUnfolding(bool doSyst, FromYieldToCS& yCS)
{
  Unfolding unf(_channel,_vgamma);
  bool isOk = unf.PrepareMigrationMatrix();
  if (!isOk){
    std::cout<<"ERROR: PrepareMigrationMatrix() for Unfolding failed"<<std::endl;
    return;
  }

  TH1D* signInput1D = (TH1D*)yCS.yields1D_bkgSubtr;
  TString strUnfName = signInput1D->GetName();
  strUnfName+="_unf";
  TH1D* signUnfolded1D = (TH1D*)signInput1D->Clone(strUnfName);

  isOk = unf.ApplyRooUnfold(doSyst,signInput1D,signUnfolded1D,RooUnfold::kBayes,yCS.name);
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

void CalcCrossSection::Plot()
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
  for (int ib=1; ib<=_yCSarray[ERR_SUM].crossSection1D->GetNbinsX(); ib++){
    float cont; float err;
    cont = _yCSarray[ERR_SUM].crossSection1D->GetBinContent(ib);
    err = _yCSarray[ERR_SUM].crossSection1D->GetBinError(ib);
//    yCS.crossSection1D->SetBinContent(ib,1000*cont);
//    yCS.crossSection1D->SetBinError(ib,1000*err);
    cont = hTheory->GetBinContent(ib);
    err = hTheory->GetBinError(ib);
    hTheory->SetBinContent(ib,1000*cont);
    hTheory->SetBinError(ib,1000*err);
  }// end of loop over ib
   Print("Total (pb) and differential(fb/GeV) cross sections: ", _yCSarray[ERR_SUM].crossSectionTOT, _yCSarray[ERR_SUM].crossSection1D);
  pad1->cd();
  _yCSarray[ERR_SUM].crossSection1D->GetXaxis()->SetNoExponent();
  _yCSarray[ERR_SUM].crossSection1D->GetXaxis()->SetMoreLogLabels();
  _yCSarray[ERR_SUM].crossSection1D->SetLineWidth(2);

//  TString strTitle=" ";
//  strTitle+=_config.StrChannel(_channel);
//  strTitle+=" ";
//  strTitle+=_config.StrVgType(_vgamma);
//  strTitle+=" ";
//  strTitle+=_config.StrBlindType(_blind);
  _yCSarray[ERR_SUM].crossSection1D->SetTitle("; ; d#sigma/dP_{T}^{#gamma}, fb/GeV");
  _yCSarray[ERR_SUM].crossSection1D->SetStats(0);
//  _yCSarray[ERR_SUM].crossSection1D->GetYaxis()->SetTitle("d#sigma/dP_{T}^{#gamma}, fb/GeV");
  _yCSarray[ERR_SUM].crossSection1D->Draw("E1");
  _yCSarray[ERR_STAT].crossSection1D->SetLineWidth(2);
 // _yCSarray[ERR_STAT].crossSection1D->SetLineColor(4);
  _yCSarray[ERR_STAT].crossSection1D->Draw("E1 same");
  // Plot theory Cross section
  hTheory->SetLineWidth(2);
  hTheory->SetLineColor(2);
  hTheory->Draw("same");
  _yCSarray[ERR_SUM].crossSection1D->Draw("E1 same");
  _yCSarray[ERR_STAT].crossSection1D->Draw("E1 same");
  TLegend *leg = new TLegend(0.68,0.65,0.92,0.90, "d#sigma/dP_{T}^{#gamma}, fb/GeV");
  leg->SetFillColor(0);
  leg->AddEntry(_yCSarray[ERR_SUM].crossSection1D,"measured","l");
  leg->AddEntry(hTheory,"MC-based","l");
  leg->Draw("same");

  TString txt_CMS_Preliminary = "#scale[1.4]{#font[61]{CMS}} #font[52]{Preliminary}";
  TString strHeader=txt_CMS_Preliminary;
  TLatex* text = new TLatex(0.15,0.93,strHeader);
  text->SetNDC();
  text->SetTextSize(0.04);
  text->Draw("same");

  TString strTitle="";
  strTitle+=_config.StrChannel(_channel);
  strTitle+=" ";
  strTitle+=_config.StrVgType(_vgamma);
//  strTitle+=" ";
//  strTitle+=_config.StrBlindType(_blind);
  TText* text2 = new TText(0.35,0.83,strTitle);
  text2->SetNDC();
  text2->SetTextSize(0.04);
  text2->Draw("same");

  pad2->cd();
  TH1F* hRatio = (TH1F*) _yCSarray[ERR_SUM].crossSection1D->Clone("hRatio");
  hRatio->Divide(hTheory);
  float max=1.1; float min=0.9;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    if (hRatio->GetBinContent(ib)+hRatio->GetBinError(ib)>max) 
      max=hRatio->GetBinContent(ib)+hRatio->GetBinError(ib);
    if (hRatio->GetBinContent(ib)-hRatio->GetBinError(ib)<min) 
      min=hRatio->GetBinContent(ib)-hRatio->GetBinError(ib);
  }// end of loop over ib
  min = 0.2; max = 1.8;
  hRatio->SetStats(0);
  hRatio->SetLineWidth(2);
  hRatio->GetYaxis()->SetRangeUser(min,max);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->GetXaxis()->SetMoreLogLabels();
  hRatio->GetXaxis()->SetNoExponent();
  hRatio->SetTitle("; P_{T}^{#gamma}, GeV;");
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
  _yCSarray[ERR_SUM].crossSectionTOT->Write(_config.GetCSname(_channel,_config.TOTAL)); 
  _yCSarray[ERR_SUM].crossSection1D->Write(_config.GetCSname(_channel,_config.ONEDI));
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
