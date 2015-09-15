#include "iostream.h"
#include "iomanip.h"
#include "TFile.h"
#include "TH1F.h"
#include "TString.h"

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

void PrintTableOne(TString strKin, TString f_data_chiso, TString f_data_sihih, TString f_MCcl_chiso, TString f_MCcl_sihih, TString strEta)
{

  TFile f_d_chiso(f_data_chiso);
  TFile f_d_sihih(f_data_sihih);
  TFile f_M_chiso(f_MCcl_chiso);
  TFile f_M_sihih(f_MCcl_sihih);

  if (!(f_d_chiso.IsOpen())) return;
  if (!(f_d_sihih.IsOpen())) return;
  if (!(f_M_chiso.IsOpen())) return;
  if (!(f_M_sihih.IsOpen())) return;

  TString hName=TString("yieldsBkgSubtrData_ONEDI_")+strEta;
  TString hNameMCsig=TString("yieldsSelected_SIGMC__ONEDI_")+strEta;

  TFile* fOut = new TFile("fOut.root");

  TH1F* h_d_chiso = (TH1F*)f_d_chiso.Get(hName);
  TH1F* h_d_sihih = (TH1F*)f_d_sihih.Get(hName);
  TH1F* h_M_chiso = (TH1F*)f_M_chiso.Get(hName);
  TH1F* h_M_sihih = (TH1F*)f_M_sihih.Get(hName);
  TH1F* h_d_chisoMCsig = (TH1F*)f_d_chiso.Get(hNameMCsig);
  TH1F* h_d_sihihMCsig = (TH1F*)f_d_sihih.Get(hNameMCsig);
  TH1F* h_M_chisoMCsig = (TH1F*)f_M_chiso.Get(hNameMCsig);
  TH1F* h_M_sihihMCsig = (TH1F*)f_M_sihih.Get(hNameMCsig);

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
  if (!isOk) return;

  // check is signal MC is always same
  isOk=CheckSignalMC(h_d_chisoMCsig,h_d_sihihMCsig,h_M_chisoMCsig,h_M_sihihMCsig);
  if (!isOk) return;

    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Syst. due to different ways to fit. Muon "<<strKin<<" "<<strEta<<"}"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|}"<<std::endl;
    std::cout<<"    bin &  MC   & data  & data  & MC cl. & MC cl. \\\\ "<<std::endl;
    std::cout<<"    lims & pred & chiso & sihih & chiso  & sihih  \\\\ \\hline"<<std::endl;
    for (int ib=1; ib<=h_d_chisoMCsig->GetNbinsX(); ib++){

//      if (ib==1) std::cout<<"%";

      std::cout<<"    ";
      std::cout<<h_d_chisoMCsig->GetBinLowEdge(ib)<<"-"<<h_d_chisoMCsig->GetBinLowEdge(ib)+h_d_chisoMCsig->GetBinWidth(ib)<<" & ";

      std::cout<<"$"<<(int)h_d_chisoMCsig->GetBinContent(ib)<<"\\pm"<<(int)h_d_chisoMCsig->GetBinError(ib)<<"$ & ";
      std::cout<<"$"<<(int)h_d_chiso->GetBinContent(ib)<<"\\pm"<<(int)h_d_chiso->GetBinError(ib)<<"$ & ";
      std::cout<<"$"<<(int)h_d_sihih->GetBinContent(ib)<<"\\pm"<<(int)h_d_sihih->GetBinError(ib)<<"$ & ";
      std::cout<<"$"<<(int)h_M_chiso->GetBinContent(ib)<<"\\pm"<<(int)h_M_chiso->GetBinError(ib)<<"$ & ";
      std::cout<<"$"<<(int)h_M_sihih->GetBinContent(ib)<<"\\pm"<<(int)h_M_sihih->GetBinError(ib)<<"$ ";

      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:diff_ways_to_fit_muon_";
    std::cout<<strKin<<"_"<<strEta;
    std::cout<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;



}//end of PrintOneTable()

void PrintTables()
{
  TString f_data_chiso,f_data_sihih,f_MCcl_chiso,f_MCcl_sihih;

  f_data_chiso="../WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_phoEt_.root";
  f_data_sihih="../WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_phoEt_.root";
  f_MCcl_chiso="../../WGammaAnalysisAux20_Fit_phoEt_bins_MCclosure/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_phoEt_.root";
  f_MCcl_sihih="../../WGammaAnalysisAux20_Fit_phoEt_bins_MCclosure/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_phoEt_.root";
  PrintTableOne("phoEt", f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, "BARREL");
  PrintTableOne("phoEt", f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, "ENDCAP");

  f_data_chiso="../../WGammaAnalysisAux15_Fit_WMt_bins/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_WMt_.root";
  f_data_sihih="../../WGammaAnalysisAux15_Fit_WMt_bins/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_WMt_.root";
  f_MCcl_chiso="../../WGammaAnalysisAux18_Fit_WMt_bins_MCclosure/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_WMt_.root";
  f_MCcl_sihih="../../WGammaAnalysisAux18_Fit_WMt_bins_MCclosure/WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_WMt_.root";
  PrintTableOne("WMt", f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, "BARREL");
  PrintTableOne("WMt", f_data_chiso, f_data_sihih, f_MCcl_chiso, f_MCcl_sihih, "ENDCAP");


}//end of PrintTables()
