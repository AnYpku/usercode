#include "../Configuration/TConfiguration.h"

#include "iostream.h"
#include "iomanip.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1F.h"
#include "TString.h"

  TH1F* h_d_chiso;
  TH1F* h_d_sihih;
  TH1F* h_M_chiso;
  TH1F* h_M_sihih;
  TH1F* h_d_chisoMCsig;
  TH1F* h_d_sihihMCsig;
  TH1F* h_M_chisoMCsig;
  TH1F* h_M_sihihMCsig;

  TH1F* h_d_chiso_templStat;
  TH1F* h_d_sihih_templStat;
  TH1F* h_M_chiso_templStat;
  TH1F* h_M_sihih_templStat;

  TH1F* h_Yield_Meth1;
  TH1F* h_Yield_Meth2[3];

  TFile* fOut;

bool CheckBinning(int nBins, TString strL, TH1F* h)
{
  if (h->GetNbinsX()!=nBins){
    std::cout<<strL<<": h->GetNbinsX()="<<h->GetNbinsX()<<", nBins="<<nBins<<std::endl;
    return 0;
  }
  return 1;
}//end of CheckBinning

bool CheckSignalMC(TH1F* h1, TH1F* h2, TH1F* h3, TH1F* h4)
{
  for(int ib=1; ib<=h1->GetNbinsX(); ib++){
    float cont1=h1->GetBinContent(ib);    float err1=h1->GetBinError(ib);
    float cont2=h2->GetBinContent(ib);    float err2=h2->GetBinError(ib);
    float cont3=h3->GetBinContent(ib);    float err3=h3->GetBinError(ib);
    float cont4=h4->GetBinContent(ib);    float err4=h4->GetBinError(ib);
    if ( fabs(cont4-cont1)>0.001 || fabs(cont3-cont1)>0.001 || fabs(cont2-cont1)>0.001 ||
         fabs(err4-err1)>0.001 || fabs(err3-err1)>0.001 || fabs(err2-err1)>0.001){
       std::cout<<"cont1="<<cont1<<"+-"<<err1<<" ";
       std::cout<<"cont2="<<cont2<<"+-"<<err2<<" ";
       std::cout<<"cont3="<<cont3<<"+-"<<err3<<" ";
       std::cout<<"cont4="<<cont4<<"+-"<<err4<<" ";
       return 0;
    }
  }//end of loop over ib
  return 1;
}//end of CheckSignalMC

bool GetAndCheckHists(TString f_data_chiso, TString f_data_sihih, TString f_MCcl_chiso, TString f_MCcl_sihih, TString f_data_chiso_templStat, TString f_data_sihih_templStat, TString f_MCcl_chiso_templStat, TString f_MCcl_sihih_templStat, TString strEta)
{

  TFile* f_d_chiso = new TFile(f_data_chiso);
  TFile* f_d_sihih = new TFile(f_data_sihih);
  TFile* f_M_chiso = new TFile(f_MCcl_chiso);
  TFile* f_M_sihih = new TFile(f_MCcl_sihih);
  TFile* f_d_chiso_templStat = new TFile(f_data_chiso_templStat);
  TFile* f_d_sihih_templStat = new TFile(f_data_sihih_templStat);
  TFile* f_M_chiso_templStat = new TFile(f_MCcl_chiso_templStat);
  TFile* f_M_sihih_templStat = new TFile(f_MCcl_sihih_templStat);

  if (!(f_d_chiso->IsOpen())) {return 0;}
  if (!(f_d_sihih->IsOpen())) {return 0;}
  if (!(f_M_chiso->IsOpen())) {return 0;}
  if (!(f_M_sihih->IsOpen())) {return 0;}
  if (!(f_d_chiso_templStat->IsOpen())) {return 0;}
  if (!(f_d_sihih_templStat->IsOpen())) {return 0;}
  if (!(f_M_chiso_templStat->IsOpen())) {return 0;}
  if (!(f_M_sihih_templStat->IsOpen())) {return 0;}

  TString hName=TString("yieldsBkgSubtrData_ONEDI_")+strEta;
  TString hNameMCsig=TString("yieldsSelected_SIGMC__ONEDI_")+strEta;


fOut->cd();

  h_d_chiso = (TH1F*)f_d_chiso->Get(hName);
  h_d_sihih = (TH1F*)f_d_sihih->Get(hName);
  h_M_chiso = (TH1F*)f_M_chiso->Get(hName);
  h_M_sihih = (TH1F*)f_M_sihih->Get(hName);
  h_d_chisoMCsig = (TH1F*)f_d_chiso->Get(hNameMCsig);
  h_d_sihihMCsig = (TH1F*)f_d_sihih->Get(hNameMCsig);
  h_M_chisoMCsig = (TH1F*)f_M_chiso->Get(hNameMCsig);
  h_M_sihihMCsig = (TH1F*)f_M_sihih->Get(hNameMCsig);

  hName=TString("yieldsDDTrueONEDI")+strEta;
  h_d_chiso_templStat = (TH1F*)f_d_chiso_templStat->Get(hName);
  h_d_sihih_templStat = (TH1F*)f_d_sihih_templStat->Get(hName);
  h_M_chiso_templStat = (TH1F*)f_M_chiso_templStat->Get(hName);
  h_M_sihih_templStat = (TH1F*)f_M_sihih_templStat->Get(hName);

  std::cout<<"hists are set"<<std::endl;
  h_d_chiso->Print();
  h_d_sihih->Print();
  h_M_chiso->Print();
  h_M_sihih->Print();
  h_d_chisoMCsig->Print();
  h_d_sihihMCsig->Print();
  h_M_chisoMCsig->Print();
  h_M_sihihMCsig->Print();

  // check if binning is always same

  int nBins=h_d_chiso->GetNbinsX(); bool isOk;
  isOk=CheckBinning(nBins, "h_d_chiso", h_d_chiso);
  isOk=CheckBinning(nBins, "h_d_sihih", h_d_sihih);
  isOk=CheckBinning(nBins, "h_M_chiso", h_M_chiso);
  isOk=CheckBinning(nBins, "h_M_sihih", h_M_sihih);
  isOk=CheckBinning(nBins, "h_d_chisoMCsig", h_d_chisoMCsig);
  isOk=CheckBinning(nBins, "h_d_sihihMCsig", h_d_sihihMCsig);
  isOk=CheckBinning(nBins, "h_M_chisoMCsig", h_M_chisoMCsig);
  isOk=CheckBinning(nBins, "h_M_sihihMCsig", h_M_sihihMCsig);
  isOk=CheckBinning(nBins, "h_d_chiso_templStat", h_d_chiso_templStat);
  isOk=CheckBinning(nBins, "h_d_sihih_templStat", h_d_sihih_templStat);
  isOk=CheckBinning(nBins, "h_M_chiso_templStat", h_M_chiso_templStat);
  isOk=CheckBinning(nBins, "h_M_sihih_templStat", h_M_sihih_templStat);
  if (!isOk) return 0;

  // check is signal MC is always same
  isOk=CheckSignalMC(h_d_chisoMCsig,h_d_sihihMCsig,h_M_chisoMCsig,h_M_sihihMCsig);
  if (!isOk) return 0;

  return 1;
}//end of GetAndCheckHists

void ComputeSystHists(TString strKin, TString strEta)
{

  fOut->cd();

//  TH1F* h_Yield_Meth1;
//  TH1F* h_Yield_Meth2;

  int ieta;
  if (strEta=="BARREL") ieta=0;
  if (strEta=="ENDCAP") ieta=1;

  h_Yield_Meth1 = (TH1F*)h_d_chiso->Clone(TString("h_Syst_Meth1")+strEta);
  h_Yield_Meth2[ieta] = (TH1F*)h_d_chiso->Clone(TString("h_Syst_Meth2")+strEta);

  for (int ib=1; ib<=h_d_chiso->GetNbinsX(); ib++){

    float errMeth1_1 = fabs(h_d_chiso->GetBinContent(ib) - h_d_sihih->GetBinContent(ib));
    float errMeth1_2 = h_d_chiso_templStat->GetBinError(ib);
    float errMeth1 = sqrt(errMeth1_1*errMeth1_1+errMeth1_2*errMeth1_2);
    h_Yield_Meth1->SetBinError(ib,errMeth1);

    float w1, x1, w2, x2;
    x1 = h_d_chiso->GetBinContent(ib);
    x2 = h_d_sihih->GetBinContent(ib);
    w1 = 1./(TMath::Power(h_M_chiso->GetBinContent(ib)-h_M_chisoMCsig->GetBinContent(ib),2)+TMath::Power(h_d_chiso_templStat->GetBinError(ib),2));
    w2 = 1./(TMath::Power(h_M_sihih->GetBinContent(ib)-h_M_chisoMCsig->GetBinContent(ib),2)+TMath::Power(h_d_sihih_templStat->GetBinError(ib),2));
    float valMeth2 = (w1*x1+w2*x2)/(w1+w2);
    float errMeth2 = 1./sqrt(w1+w2);
    h_Yield_Meth2[ieta]->SetBinContent(ib,valMeth2);
    h_Yield_Meth2[ieta]->SetBinError(ib,errMeth2);
  }//end of loop over ib

  h_Yield_Meth2[ieta]->SetTitle("yieldsDDTrueONEDI"+strEta);
  h_Yield_Meth2[ieta]->Write("yieldsDDTrueONEDI"+strEta);
  
//  if (channel==conf.MUON) strCh="MUON";
//  if (channel==conf.ELECTRON) strCh="ELECTRON";

  std::cout<<"\\begin{table}[h]"<<std::endl;
  std::cout<<"  \\scriptsize"<<std::endl;
  std::cout<<"  \\begin{center}"<<std::endl;
  std::cout<<"  \\caption{Syst. due to different ways to fit. "<<strKin<<" "<<strEta<<"}"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih
  std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|c|}"<<std::endl;
  std::cout<<"    bin &  MC   & data  & data  & MC cl. & MC cl. & yield & yield\\\\ "<<std::endl;
  std::cout<<"    lims & pred & chiso & sihih & chiso  & sihih  & meth1 & meth2 \\\\ \\hline"<<std::endl;
  for (int ib=1; ib<=h_d_chisoMCsig->GetNbinsX(); ib++){

//      if (ib==1) std::cout<<"%";

      std::cout<<"    ";
      std::cout<<h_d_chisoMCsig->GetBinLowEdge(ib)<<"-"<<h_d_chisoMCsig->GetBinLowEdge(ib)+h_d_chisoMCsig->GetBinWidth(ib)<<" & ";

      std::cout<<"$"<<(int)h_d_chisoMCsig->GetBinContent(ib)<<"\\pm"<<(int)h_d_chisoMCsig->GetBinError(ib)<<"$ & ";
      std::cout<<"$"<<(int)h_d_chiso->GetBinContent(ib)<<"\\pm"<<(int)sqrt(h_d_chiso->GetBinError(ib)*h_d_chiso->GetBinError(ib)+h_d_chiso_templStat->GetBinError(ib)*h_d_chiso_templStat->GetBinError(ib))<<"$ & ";
      std::cout<<"$"<<(int)h_d_sihih->GetBinContent(ib)<<"\\pm"<<(int)sqrt(h_d_sihih->GetBinError(ib)*h_d_sihih->GetBinError(ib)+h_d_sihih_templStat->GetBinError(ib)*h_d_sihih_templStat->GetBinError(ib))<<"$ & ";

      std::cout<<"$"<<(int)h_M_chiso->GetBinContent(ib)<<"\\pm"<<(int)sqrt(h_M_chiso->GetBinError(ib)*h_M_chiso->GetBinError(ib)+h_M_chiso_templStat->GetBinError(ib)*h_M_chiso_templStat->GetBinError(ib))<<"$ & ";
      std::cout<<"$"<<(int)h_M_sihih->GetBinContent(ib)<<"\\pm"<<(int)sqrt(h_M_sihih->GetBinError(ib)*h_M_sihih->GetBinError(ib)+h_M_sihih_templStat->GetBinError(ib)*h_M_sihih_templStat->GetBinError(ib))<<"$ &";

      std::cout<<"$"<<(int)h_Yield_Meth1->GetBinContent(ib)<<"\\pm"<<(int)h_Yield_Meth1->GetBinError(ib)<<"$ &";
      std::cout<<"$"<<(int)h_Yield_Meth2[ieta]->GetBinContent(ib)<<"\\pm"<<(int)h_Yield_Meth2[ieta]->GetBinError(ib)<<"$ ";

      std::cout<<" \\\\ \\hline"<<std::endl;
  }//end of loop over ib

  std::cout<<"  \\end{tabular}"<<std::endl;
  std::cout<<"  \\label{tab:diff_ways_to_fit_";
  std::cout<<strKin<<"_"<<strEta;
  std::cout<<"}"<<std::endl;
  std::cout<<"  \\end{center}"<<std::endl;
  std::cout<<"\\end{table}"<<std::endl;


  return;
}//end of ComputeSystHists()

void PrintTableOne(TString strKin, TString strEta)
{
  fOut->cd();
//  h_d_chiso->Print();
//  h_d_sihih->Print();
//  h_M_chiso->Print();
//  h_M_sihih->Print();
//  h_d_chisoMCsig->Print();
//  h_d_sihihMCsig->Print();
//  h_M_chisoMCsig->Print();
//  h_M_sihihMCsig->Print();




}//end of PrintOneTable()

void CombineCHISOandSIHIH(int channel, int vgamma)
{
  TString f_data_chiso,f_data_sihih,
          f_MCcl_chiso,f_MCcl_sihih,
          f_data_chiso_templStat,f_data_sihih_templStat,
          f_MCcl_chiso_templStat,f_MCcl_sihih_templStat;

  TString strCh, strVg;
  TConfiguration conf;
  if (channel==conf.MUON) strCh="MUON";
  if (channel==conf.ELECTRON) strCh="ELECTRON";
  if (vgamma==conf.W_GAMMA) strVg="WGamma";
  if (vgamma==conf.Z_GAMMA) strVg="ZGamma";

  f_data_chiso="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/yields_"+strVg+"_TEMPL_CHISO_phoEt_.root";
  f_data_sihih="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/yields_"+strVg+"_TEMPL_SIHIH_phoEt_.root";
  f_MCcl_chiso="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/yields_"+strVg+"_TEMPL_CHISO_phoEt__MCclosure.root";
  f_MCcl_sihih="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/yields_"+strVg+"_TEMPL_SIHIH_phoEt__MCclosure.root";

  f_data_chiso_templStat="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_"+strVg+"_TEMPL_CHISO_phoEt__SystRand.root";
  f_data_sihih_templStat="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_"+strVg+"_TEMPL_SIHIH_phoEt__SystRand.root";
  f_MCcl_chiso_templStat="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_"+strVg+"_TEMPL_CHISO_phoEt__MCclosure_SystRand_MCclosure.root";
  f_MCcl_sihih_templStat="../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_"+strVg+"_TEMPL_SIHIH_phoEt__MCclosure_SystRand_MCclosure.root";

  fOut = new TFile("../WGammaOutput/"+strCh+"_"+strVg+"/YieldsAndBackground/DDTemplate_SystCHISOvsSIHIH_phoEt.root","RECREATE");

  bool isOk1 = GetAndCheckHists(f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, f_data_chiso_templStat, f_data_sihih_templStat, f_MCcl_chiso_templStat, f_MCcl_sihih_templStat, "BARREL");
  if (isOk1) {ComputeSystHists("phoEt", "BARREL"); PrintTableOne("phoEt", "BARREL");}
  bool isOk2 = GetAndCheckHists(f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, f_data_chiso_templStat, f_data_sihih_templStat, f_MCcl_chiso_templStat, f_MCcl_sihih_templStat,"ENDCAP");
  if (isOk2) {ComputeSystHists("phoEt", "ENDCAP"); PrintTableOne("phoEt", "ENDCAP");}

  h_Yield_Meth2[0]->Print();
  h_Yield_Meth2[1]->Print();
  h_Yield_Meth2[2]=(TH1F*)h_Yield_Meth2[0]->Clone("yieldsDDTrueONEDICOMMON");
  h_Yield_Meth2[2]->Add(h_Yield_Meth2[1]);
  h_Yield_Meth2[2]->SetTitle("yieldsDDTrueONEDICOMMON");
  h_Yield_Meth2[2]->Write();
  h_Yield_Meth2[2]->Print();

  TH1F* hYieldTOTAL[3];
  for (int ieta=0; ieta<=2; ieta++){
    TString str;
    if (ieta==0) str="BARREL";
    if (ieta==1) str="ENDCAP";
    if (ieta==2) str="COMMON";
    hYieldTOTAL[ieta]=new TH1F("yieldsDDTrueTOTAL"+str,"yieldsDDTrueTOTAL"+str,1,15,500);
    float cont=0; float err=0;
    for (int ib=2; ib<=h_Yield_Meth2[ieta]->GetNbinsX(); ib++){
      cont+=h_Yield_Meth2[ieta]->GetBinContent(ib);
      err+=h_Yield_Meth2[ieta]->GetBinError(ib)*h_Yield_Meth2[ieta]->GetBinError(ib);
    }//end of loop over ib
    err = sqrt(err);
    hYieldTOTAL[ieta]->SetBinContent(1,cont);
    hYieldTOTAL[ieta]->SetBinError(1,err);
    hYieldTOTAL[ieta]->Write();
  }//end of loop over ieta

//  f_data_chiso="../../WGammaAnalysisAux15_Fit_WMt_bins/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_WMt_.root";
//  f_data_sihih="../../WGammaAnalysisAux15_Fit_WMt_bins/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_WMt_.root";
//  f_MCcl_chiso="../../WGammaAnalysisAux18_Fit_WMt_bins_MCclosure/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_WMt_.root";
//  f_MCcl_sihih="../../WGammaAnalysisAux18_Fit_WMt_bins_MCclosure/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_WMt_.root";
//  PrintTableOne("WMt", f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, "BARREL");
//  PrintTableOne("WMt", f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, "ENDCAP");


}//end of CombineCHISOandSIHIH()
