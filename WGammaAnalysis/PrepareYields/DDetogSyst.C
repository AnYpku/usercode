#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TMath.h"
#include "TH1F.h"
#include "TString.h"

  TH1F* h_signal[3];
  TH1F* h_etog[3];
  TH1F* h_etogNoWMtCut[3];

  TFile* fOut;

bool CheckBinning(int nBins, TString strL, TH1F* h)
{
  if (h->GetNbinsX()!=nBins){
    std::cout<<strL<<": h->GetNbinsX()="<<h->GetNbinsX()<<", nBins="<<nBins<<std::endl;
    return 0;
  }
  return 1;
}//end of CheckBinning

bool GetAndCheckHists(TString str_f_signal, TString str_f_etog, TString str_f_etogNoWMtCut, int ieta, TString strEta)
{

  TFile* f_signal = new TFile(str_f_signal);
  TFile* f_etog = new TFile(str_f_etog);
  TFile* f_etogNoWMtCut = new TFile(str_f_etogNoWMtCut);

  if (!(f_signal->IsOpen())) {return 0;}
  if (!(f_etog->IsOpen())) {return 0;}
  if (!(f_etogNoWMtCut->IsOpen())) {return 0;}

  fOut->cd();

  TString hName=TString("yieldsBkgSubtrData_ONEDI_")+strEta;
  h_signal[ieta] = (TH1F*)f_signal->Get(hName);
  hName=TString("yieldsBkgEtoGammaONEDI")+strEta;
  h_etog[ieta] = (TH1F*)f_etog->Get(hName);
  h_etogNoWMtCut[ieta] = (TH1F*)f_etogNoWMtCut->Get(hName);

  std::cout<<"hists are set"<<std::endl;
  h_signal[ieta]->Print();
  h_etog[ieta]->Print();
  h_etogNoWMtCut[ieta]->Print();

  // check if binning is always same

  int nBins= h_signal[0]->GetNbinsX(); bool isOk;
  isOk=CheckBinning(nBins, "h_signal "+strEta, h_signal[ieta]);
  isOk=CheckBinning(nBins, "h_etog "+strEta, h_etog[ieta]);
  isOk=CheckBinning(nBins, "h_etogNoWMtCut "+strEta, h_etogNoWMtCut[ieta]);

  if (!isOk) return 0;

  return 1;
}//end of GetAndCheckHists


void DDetogSyst()
{
  TString f_signal,f_etog, f_etogWMtCut;


  TString str_f_signal="../WGammaOutput/ELECTRON_WGamma/YieldsAndBackground/yields_WGamma__phoEt_.root";
  TString str_f_etog="../WGammaOutput/ELECTRON_WGamma/YieldsAndBackground/DDEtoGamma_WGamma_phoEt_.root";
  TString str_f_etogNoWMtCut="../WGammaOutput/ELECTRON_WGamma/YieldsAndBackground/DDEtoGamma_WGamma_phoEt__noWMTcut.root";
  

  fOut = new TFile("../WGammaOutput/ELECTRON_WGamma/YieldsAndBackground/DDetog_Syst_phoEt.root","RECREATE");

  bool isOk1 = GetAndCheckHists(str_f_signal, str_f_etog, str_f_etogNoWMtCut, 0, "BARREL");
  bool isOk2 = GetAndCheckHists(str_f_signal, str_f_etog, str_f_etogNoWMtCut, 1, "ENDCAP");

  TH1F* hYieldONEDI[3];
  TH1F* hYieldTOTAL[3];
  
  for (int ieta=0; ieta<=1; ieta++){
    TString str;
    if (ieta==0) str="BARREL";
    if (ieta==1) str="ENDCAP";
    hYieldONEDI[ieta]=(TH1F*)h_signal[ieta]->Clone();
    
    for (int ib=1; ib<=hYieldONEDI[ieta]->GetNbinsX(); ib++){
      float diff=h_etog[ieta]->GetBinContent(ib)-h_etogNoWMtCut[ieta]->GetBinContent(ib);
      hYieldONEDI[ieta]->SetBinError(ib,fabs(diff));
    }//end of loop over ib
    hYieldONEDI[ieta]->Write();
    
  }//end of loop over ieta
  hYieldONEDI[2]=(TH1F*)hYieldONEDI[0]->Clone("yieldsBkgSubtrData_ONEDI_COMMON");
  hYieldONEDI[2]->Add(hYieldONEDI[1]);
  hYieldONEDI[2]->Write();
  for (int ieta=0; ieta<=2; ieta++){
    TString str;
    if (ieta==0) str="BARREL";
    if (ieta==1) str="ENDCAP";
    if (ieta==2) str="COMMON";
    hYieldTOTAL[ieta]=new TH1F("yieldsBkgSubtrData_TOTAL_"+str,"yieldsBkgSubtrData_TOTAL_"+str,1,15,500);
    float cont=0; float err=0;
    for (int ib=2; ib<=hYieldONEDI[ieta]->GetNbinsX(); ib++){
      cont+=hYieldONEDI[ieta]->GetBinContent(ib);
      err+=hYieldONEDI[ieta]->GetBinError(ib)*hYieldONEDI[ieta]->GetBinError(ib);
      std::cout<<"ib="<<ib<<", cont+-err="<<hYieldONEDI[ieta]->GetBinContent(ib)<<"+-"<<hYieldONEDI[ieta]->GetBinError(ib)<<std::endl;
    }//end of loop over ib
    err = sqrt(err);
    hYieldTOTAL[ieta]->SetBinContent(1,cont);
    hYieldTOTAL[ieta]->SetBinError(1,err);
      std::cout<<"total: cont+-err="<<cont<<"+-"<<err<<std::endl;
    hYieldTOTAL[ieta]->Write();
  }//end of loop over ieta
  
}//end of DDetogSyst()

