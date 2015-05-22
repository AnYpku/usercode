#include "TComputeSystDueToSbVariation.h"

#include "TH1F.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TText.h"

#include <iostream>
#include <iomanip>
#include <vector>

TComputeSystDueToSbVariation::TComputeSystDueToSbVariation(TConfiguration::AnalysisParameters anPars)
{
  _anPars=anPars;
}

TComputeSystDueToSbVariation::~TComputeSystDueToSbVariation()
{
  //if (_pars.fOutForSave->IsOpen()) _pars.fOutForSave->Close();
}

bool TComputeSystDueToSbVariation::ComputeSyst()
{
  GetHistsAndVectors();
  for (int ieta=0; ieta<=1; ieta++){
    for (int ikin=1; ikin<=_anPars.nKinBins; ikin++){
      ComputeSystOneBin(METH2,ieta,ikin);
    }
  }
  PrintLatexTableMeth1();
  return 1;
}// end of TComputeSystDueToSbVariation::ComputeSyst

bool TComputeSystDueToSbVariation::ComputeSystOneBin(int meth, int ieta, int ikin)
{
  // U_i = f_d_i - b_i x f_d, b_i = f_MC_i / f_MCtruth
  _f_MCtruth[ikin][ieta]=_hMCtruth[ieta]->GetBinContent(ikin);
  if (_f_MCtruth[ikin][ieta]<0.0001) {std::cout<<"f_MCtruth=0;"<<std::endl; return 0;}
  _f_d[ikin][ieta]=-1;
  _f_MC[ikin][ieta]=-1;
  float sbL=-1;
  float sbU=-1;

  sbL=_hSbLselected[ieta]->GetBinContent(ikin);
  sbU=_hSbUselected[ieta]->GetBinContent(ikin);

  int N_Sb_data = _vecSbL_Data[ikin][ieta]->GetNrows();
  int N_Sb_MC = _vecSbL_MC[ikin][ieta]->GetNrows();

  // find f_d
  for (int isb=0; isb<N_Sb_data; isb++){
      if (fabs(_vecSbL_Data[ikin][ieta]->operator()(isb)-_hSbLselected[ieta]->GetBinContent(ikin))<0.0001 &&
          fabs(_vecSbU_Data[ikin][ieta]->operator()(isb)-_hSbUselected[ieta]->GetBinContent(ikin))<0.0001)
      _f_d[ikin][ieta]=_vecFitRes_Data[ikin][ieta]->operator()(isb);
  }//end of loop over isb

  // find f_MC
  for (int isb=0; isb<N_Sb_MC; isb++){
      if (fabs(_vecSbL_MC[ikin][ieta]->operator()(isb)-_hSbLselected[ieta]->GetBinContent(ikin))<0.0001 &&
          fabs(_vecSbU_MC[ikin][ieta]->operator()(isb)-_hSbUselected[ieta]->GetBinContent(ikin))<0.0001)
      _f_MC[ikin][ieta]=_vecFitRes_MC[ikin][ieta]->operator()(isb);
  }//end of loop over isb

  std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<std::endl;
  std::cout<<"f_MCtruth="<<_f_MCtruth[ikin][ieta]<<", f_MC="<<_f_MC[ikin][ieta]<<", f_d="<<_f_d[ikin][ieta]<<", sb="<<sbL<<"-"<<sbU<<std::endl;

  TString hUiName = "syst_hUi";
  hUiName+=StrLabelKin(ikin);
  hUiName+=StrLabelEta(ieta);
//  std::cout<<setprecision(1);
  vector <float> vecUi;
  float Ui_min=100000;
  float Ui_max=-100000;

  float KTthresh[6]={0.3,0.2,0.1,0.05,0.01,0.005};
  int ikt;
  for (ikt=0; ikt<6; ikt++){
    Ui_min=100000;
    Ui_max=-100000;
    vecUi.clear();
    FillVecUi(vecUi, Ui_min, Ui_max, meth, ikin, ieta, N_Sb_data, N_Sb_MC, KTthresh[ikt]);
    if (vecUi.size()>5 || ikt==5) break;
  }


  _fOut->cd();

  std::cout<<"Ui_min = "<<Ui_min<<", Ui_max = "<<Ui_max<<", n of points = "<<vecUi.size()<<", KTthresh["<<ikt<<"] = "<<KTthresh[ikt]<<std::endl;
  int nBins = vecUi.size()/4;
  if (nBins<5) nBins=5;
  TH1F* hUi = new TH1F(hUiName,hUiName,nBins,Ui_min,Ui_max);
  TString hUiTitle = hUiName;

  _RMSMeth1[ikin][ieta]=0; _meanMeth1[ikin][ieta]=0;
  for (int iv=0; iv<vecUi.size(); iv++){
    hUi->Fill(vecUi.at(iv));
    _meanMeth1[ikin][ieta]+=vecUi.at(iv);
  }
  _meanMeth1[ikin][ieta]=_meanMeth1[ikin][ieta]/vecUi.size();
  for (int iv=0; iv<vecUi.size(); iv++){
    _RMSMeth1[ikin][ieta]+=(_meanMeth1[ikin][ieta]-vecUi.at(iv))*(_meanMeth1[ikin][ieta]-vecUi.at(iv));
  }  
  _RMSMeth1[ikin][ieta] = sqrt(_RMSMeth1[ikin][ieta]/(vecUi.size()));

  _U0[ikin][ieta] = _f_d[ikin][ieta] - _f_d[ikin][ieta]*_f_MC[ikin][ieta]/_f_MCtruth[ikin][ieta];
  _systMeth1[ikin][ieta] = sqrt(_U0[ikin][ieta]*_U0[ikin][ieta]+_RMSMeth1[ikin][ieta]*_RMSMeth1[ikin][ieta]);
  _systRelMeth1[ikin][ieta] = (100*_systMeth1[ikin][ieta]/_f_d[ikin][ieta]);
  TString strPars = "f_d = ";  strPars+=(int)_f_d[ikin][ieta]; 
  strPars+=", f_MC = ";  strPars+=(int)_f_MC[ikin][ieta]; 
  strPars+=", f_MCtruth = ";  strPars+=(int)_f_MCtruth[ikin][ieta]; 
  strPars+=", U0 = ";  strPars+=(int)_U0[ikin][ieta]; 
  TText* textPars; 
  textPars =new TText(0.15,0.85,strPars);
  textPars->SetNDC();

  TString strSyst="syst = ";   strSyst+=(int)_systMeth1[ikin][ieta];
  strSyst+=" = ";   strSyst+=(int)_systRelMeth1[ikin][ieta]; strSyst+="%";
  strSyst+=", mean = ";   strSyst+=(int)_meanMeth1[ikin][ieta];
  strSyst+=", RMS = ";   strSyst+=(int)_RMSMeth1[ikin][ieta];
  TText* textSyst; 
  textSyst =new TText(0.15,0.75,strSyst);
  textSyst->SetNDC();

  TString canvName = hUiName;
  TCanvas *c = new TCanvas(hUiName, hUiName);
  hUi->SetTitle(hUiTitle);
  hUi->SetLineWidth(2);
  hUi->SetLineColor(2);
//  hUi->GetXaxis()->SetRangeUser(Ui_min-0.5*fabs(Ui_min),Ui_max+0.5*fabs(Ui_max));
  hUi->Draw("EP");
  hUi->SetStats(0);
  if (meth==METH1){
    TLine* line = new TLine(_U0[ikin][ieta],0,_U0[ikin][ieta],1000);
    line->SetLineWidth(2);
    line->SetLineColor(2);
    line->Draw("same");
  }
  textPars->Draw("same");
  textSyst->Draw("same");

  _f_d[ikin][2]=_f_d[ikin][0]+_f_d[ikin][1];
  _f_MC[ikin][2]=_f_MC[ikin][0]+_f_MC[ikin][1];
  _f_MCtruth[ikin][2]=_f_MCtruth[ikin][0]+_f_MCtruth[ikin][1];
  _systMeth1[ikin][2]=sqrt(_systMeth1[ikin][0]*_systMeth1[ikin][0]+_systMeth1[ikin][1]*_systMeth1[ikin][1]);
  _systRelMeth1[ikin][2]=_systMeth1[ikin][2]/_f_d[ikin][2];
  
  TString strSaveName=_config.GetPlotsDirName(_anPars.channel, _anPars.vgamma, _config.PLOTS_TEMPL_FITS_SYST);
  strSaveName+=hUiName;
  strSaveName+=".png";
  c->SaveAs(strSaveName);
  strSaveName.ReplaceAll(".png",".pdf");
  c->SaveAs(strSaveName);
  return 1;
}// end of TComputeSystDueToSbVariation::ComputeSystMeth1OneBin

bool TComputeSystDueToSbVariation::FillVecUi(vector <float> &vecUi, float& Ui_min, float& Ui_max, int meth, int ikin, int ieta, int N_Sb_data, int N_Sb_MC, float KTthresh)
{
  Ui_min=100000;
  Ui_max=-100000;

  for (int isb_data=0; isb_data<N_Sb_data; isb_data++){
    for (int isb_MC=0; isb_MC<N_Sb_MC; isb_MC++){
      if (!(fabs(_vecSbL_MC[ikin][ieta]->operator()(isb_MC)-_vecSbL_Data[ikin][ieta]->operator()(isb_data))<0.0001 &&
            fabs(_vecSbU_MC[ikin][ieta]->operator()(isb_MC)-_vecSbU_Data[ikin][ieta]->operator()(isb_data))<0.0001 ))
        continue;
      float f_d_i = _vecFitRes_Data[ikin][ieta]->operator()(isb_data);
      float f_MC_i = _vecFitRes_MC[ikin][ieta]->operator()(isb_MC);
      float Ui;
      if (meth==METH1){
        if (f_MC_i/_f_MC[ikin][ieta]>2.0 || f_MC_i/_f_MC[ikin][ieta]<0.5) continue;
        Ui = f_d_i - _f_d[ikin][ieta]*f_MC_i/_f_MCtruth[ikin][ieta]; 
      }
      if (meth==METH2){
        if (_vecKolmogorov_MC[ikin][ieta]->operator()(isb_MC)<KTthresh) continue;
        Ui = f_d_i - _f_d[ikin][ieta];
      }
      vecUi.push_back(Ui);
//      int Ui_int = (int)Ui;
//      std::cout<<Ui_int<<"; ";
//      hUi->Fill(Ui);
      if (Ui<Ui_min) Ui_min=Ui;
      if (Ui>Ui_max) Ui_max=Ui;
    }//end of loop over isb_MC
  }//end of loop over isb_data
}//end of FillVecUiFillVecUi()

bool TComputeSystDueToSbVariation::GetHistsAndVectors()
{
  TString strfName = _config.GetDDTemplateFileName(_anPars.channel,_anPars.vgamma,_config.TEMPL_CHISO,_anPars.varKin);
  TString strfNameData = strfName.ReplaceAll(".root","_Syst.root");
  TString strfNameMC = strfName.ReplaceAll("_Syst.root","_Syst_MCclosure.root");

  TString strMCtruthFile = _config.GetYieldsMCtruthFileName(_anPars.channel, _anPars.vgamma, _anPars.varKin);

  TString strFSbs = _config.GetSidebandsFileName(_anPars.channel, _anPars.vgamma, _config.TEMPL_CHISO, _anPars.varKin);

  std::cout<<"fSbVarData="<<strfNameData<<std::endl;
  std::cout<<"fSbVarMC="<<strfNameMC<<std::endl;
  std::cout<<"fMCtruth="<<strMCtruthFile<<std::endl;
  std::cout<<"fSbs="<<strFSbs<<std::endl;
  _fSbVarData = new TFile(strfNameData);
  _fSbVarMC = new TFile(strfNameMC);
  _fMCtruth = new TFile(strMCtruthFile);
  _fSbs = new TFile(strFSbs);
  for (int ieta=0; ieta<=1; ieta++){
    if (ieta==_config.BARREL) _hMCtruth[ieta]=(TH1F*)_fMCtruth->Get("hSum_MCtruth__Barrel_TrueGamma");
    if (ieta==_config.ENDCAP) _hMCtruth[ieta]=(TH1F*)_fMCtruth->Get("hSum_MCtruth__Endcap_TrueGamma");
    _hSbLselected[ieta] = (TH1F*)_fSbs->Get(_config.GetSidebandsLowerHistName(ieta));
    _hSbUselected[ieta] = (TH1F*)_fSbs->Get(_config.GetSidebandsUpperHistName(ieta));
    //std::cout<<"hMCtruth hist name = "<<
    if (_anPars.nKinBins!=_hMCtruth[ieta]->GetNbinsX()){
      std::cout<<"binning mismatch: _anPars.nKinBins="<<_anPars.nKinBins<<", _hMCtruth["<<ieta<<"]->GetNbinsX()"<<std::endl;
      return 0;
    }
    if (_anPars.nKinBins!=_hSbLselected[ieta]->GetNbinsX()){
      std::cout<<"binning mismatch: _anPars.nKinBins="<<_anPars.nKinBins<<", _hSbLselected["<<ieta<<"]->GetNbinsX()"<<std::endl;
      return 0;
    }
    if (_anPars.nKinBins!=_hSbUselected[ieta]->GetNbinsX()){
      std::cout<<"binning mismatch: _anPars.nKinBins="<<_anPars.nKinBins<<", _hSbUselected["<<ieta<<"]->GetNbinsX()"<<std::endl;
      return 0;
    }
    for (int ikin=1; ikin<=_anPars.nKinBins; ikin++){
      _vecSbL_Data[ikin][ieta]=(TVectorD*)_fSbVarData->Get(TString("vec_sbL")+StrLabelKin(ikin)+StrLabelEta(ieta));
      _vecSbU_Data[ikin][ieta]=(TVectorD*)_fSbVarData->Get(TString("vec_sbU")+StrLabelKin(ikin)+StrLabelEta(ieta));
      _vecSbL_MC[ikin][ieta]=(TVectorD*)_fSbVarMC->Get(TString("vec_sbL")+StrLabelKin(ikin)+StrLabelEta(ieta));
      _vecSbU_MC[ikin][ieta]=(TVectorD*)_fSbVarMC->Get(TString("vec_sbU")+StrLabelKin(ikin)+StrLabelEta(ieta));
//      if (fabs(_vecSbL_Data[ikin][ieta]-_vecSbL_MC[ikin][ieta])<0.0001 || 
//          fabs(_vecSbU_Data[ikin][ieta]-_vecSbU_MC[ikin][ieta])<0.0001){
//        std::cout<<"Sideband data vs MC mismatch for ikin="<<ikin<<", ieta="<<ieta;
//        std::cout<<": Sb_Data="<<_vecSbL_Data[ikin][ieta]<<"-"<<_vecSbU_Data[ikin][ieta];
//        std::cout<<", Sb_MC="<<_vecSbL_MC[ikin][ieta]<<"-"<<_vecSbU_MC[ikin][ieta];
//        std::cout<<std::endl;
//      }
      _vecFitRes_MC[ikin][ieta]=(TVectorD*)_fSbVarMC->Get(TString("vec_trueVal")+StrLabelKin(ikin)+StrLabelEta(ieta));
      _vecFitRes_Data[ikin][ieta]=(TVectorD*)_fSbVarData->Get(TString("vec_trueVal")+StrLabelKin(ikin)+StrLabelEta(ieta));
      _vecKolmogorov_MC[ikin][ieta]=(TVectorD*)_fSbVarMC->Get(TString("vec_MCtruthKolmogorov")+StrLabelKin(ikin)+StrLabelEta(ieta));
    }//end of loop over ikin
  }//end of loop over ieta
  _fOut=new TFile("fOut.root","recreate");
  return 1;
}// end of TComputeSystDueToSbVariation::GetHistsAndVectors


bool TComputeSystDueToSbVariation::PrintLatexTableMeth1()
{
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;
  for (int ieta=0; ieta<=2; ieta++){
    TString strEta = StrLabelEta(ieta);
    strEta.ReplaceAll("_","");
    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{"<<_config.StrChannel(_anPars.channel)<<" ";
    std::cout<<_config.StrVgType(_anPars.vgamma)<<" "<<strEta<<"}"<<std::endl;

    if (ieta==0 || ieta==1){
      std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|}"<<std::endl;
      std::cout<<"    bin & side- & data & MC & MC    & $U\\_{0}$ & mean & RMS & Syst & Syst \\\\ "<<std::endl;
      std::cout<<"    lims & band & fit & fit & truth &       &      &     &      & rel, \\% \\\\ \\hline"<<std::endl;
    }
    else{
      std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|}"<<std::endl;
      std::cout<<"    bin &  data & MC & MC    &  Syst & Syst \\\\ "<<std::endl;
      std::cout<<"    lims & fit & fit & truth &       & rel, \\% \\\\ \\hline"<<std::endl;
    }

    for (int ik=1; ik<=_anPars.nKinBins; ik++){
      std::cout<<"    ";
      std::cout<<(int)_anPars.kinBinLims[ik-1]<<"-"<<(int)_anPars.kinBinLims[ik]<<" & ";//bin lims
      if (ieta!=2) {std::cout<<_hSbLselected[ieta]->GetBinContent(ik)<<"-";
        std::cout<<_hSbUselected[ieta]->GetBinContent(ik)<<" & "; }//sideband
      std::cout<<(int)_f_d[ik][ieta]<<" & ";// data fit
      std::cout<<(int)_f_MC[ik][ieta]<<" & ";// MC fit
      std::cout<<(int)_f_MCtruth[ik][ieta]<<" & ";// MC truth
      if (ieta!=2){
        std::cout<<(int)_U0[ik][ieta]<<" & ";// U0
        std::cout<<(int)_meanMeth1[ik][ieta]<<" & ";// mean
        std::cout<<(int)_RMSMeth1[ik][ieta]<<" & ";// RMS        
      }
      std::cout<<(int)_systMeth1[ik][ieta]<<" & ";
      std::cout<<(int)_systRelMeth1[ik][ieta]<<" ";
      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ik
    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:systSbVar_"<<_config.StrChannel(_anPars.channel)<<"_";
    std::cout<<_config.StrVgType(_anPars.vgamma)<<"_"<<strEta<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;
  }//end of loop over ieta
  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;
  return 1;
}// end of TComputeSystDueToSbVariation::PrintLatexTableMeth1

TString TComputeSystDueToSbVariation::StrLabelEta(int ieta){
  if (ieta==_config.BARREL) return "_Barrel_";
  else if (ieta==_config.ENDCAP) return "_Endcap_";
  else if (ieta==_config.COMMON) return "_EtaCommon_";
  return "_EtaUnknown_";
}// end of StrLabelEta

TString TComputeSystDueToSbVariation::StrLabelKin(int ikin){
  if (ikin<0 || ikin>_anPars.nKinBins) return TString("_")+_anPars.varKin+TString("Unknown_");
  if (ikin==0) return TString("_")+_anPars.varKin+TString("Total_");
  TString str="_";
  str+=_anPars.varKin;
  str+=_anPars.kinBinLims[ikin-1];
  str+="to";
  str+=_anPars.kinBinLims[ikin];
  str+="_";
  return str;
}// end of StrLabelKin
