#include "TFile.h"
#include "TH1F.h"
#include <iostream>

void CompareJets()
{

  std::cout<<"I will compare jets"<<std::endl;

  TFile* fDataCHISO = new TFile("../WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_phoEt_.root");
  TFile* fDataSIHIH = new TFile("../WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_phoEt_.root");
  TFile* fMCclCHISO = new TFile("../WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_CHISO_phoEt__MCclosure.root");
  TFile* fMCclSIHIH = new TFile("../WGammaOutput/MUON_WGamma/YieldsAndBackground/yields_WGamma_TEMPL_SIHIH_phoEt__MCclosure.root");

  TH1F* hDataCHISO[2];
  TH1F* hDataSIHIH[2]; 
  TH1F* hMCclCHISO[2]; 
  TH1F* hMCclSIHIH[2]; 

  TH1F* hMCWjets[2];
  TH1F* hMCDYjets[2];
  TH1F* hMCttjets[2];
  TH1F* hMCsumJets[2];

  hDataCHISO[0] = (TH1F*)fDataCHISO->Get("yieldsDDFakeONEDIBARREL");
  hDataSIHIH[0] = (TH1F*)fDataSIHIH->Get("yieldsDDFakeONEDIBARREL");
  hMCclCHISO[0] = (TH1F*)fMCclCHISO->Get("yieldsDDFakeONEDIBARREL");
  hMCclSIHIH[0] = (TH1F*)fMCclSIHIH->Get("yieldsDDFakeONEDIBARREL");

  hDataCHISO[1] = (TH1F*)fDataCHISO->Get("yieldsDDFakeONEDIENDCAP");
  hDataSIHIH[1] = (TH1F*)fDataSIHIH->Get("yieldsDDFakeONEDIENDCAP");
  hMCclCHISO[1] = (TH1F*)fMCclCHISO->Get("yieldsDDFakeONEDIENDCAP");
  hMCclSIHIH[1] = (TH1F*)fMCclSIHIH->Get("yieldsDDFakeONEDIENDCAP");

  hMCWjets[0] = (TH1F*)fDataCHISO->Get("histWjets_to_lnu_1D__Barrel_");
  hMCDYjets[0] = (TH1F*)fDataCHISO->Get("histDYjets_to_ll_1D__Barrel_");
  hMCttjets[0] = (TH1F*)fDataCHISO->Get("histttbarjets_1D__Barrel_");
  hMCsumJets[0] = (TH1F*) hMCWjets[0]->Clone("hMCsumJets_1D__Barrel_");
  hMCsumJets[0]->Add( hMCDYjets[0]);
  hMCsumJets[0]->Add( hMCttjets[0]);

  hMCWjets[1] = (TH1F*)fDataCHISO->Get("histWjets_to_lnu_1D__Endcap_");
  hMCDYjets[1] = (TH1F*)fDataCHISO->Get("histDYjets_to_ll_1D__Endcap_");
  hMCttjets[1] = (TH1F*)fDataCHISO->Get("histttbarjets_1D__Endcap_");
  hMCsumJets[1] = (TH1F*) hMCWjets[1]->Clone("hMCsumJets_1D__Endcap_");
  hMCsumJets[1]->Add( hMCDYjets[1]);
  hMCsumJets[1]->Add( hMCttjets[1]);


  for (int ieta=0; ieta<=1; ieta++){
    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Jets$\\rightarrow\\gamma$ background. Different ways to fit. }"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|c|}"<<std::endl;
    std::cout<<"    bin &  MC   & data  & data  & MC cl. & MC cl.\\\\ "<<std::endl;
    std::cout<<"    lims & pred & chiso & sihih & chiso  & sihih \\\\ \\hline"<<std::endl;
  
    for (int ib=1; ib<=hDataCHISO[0]->GetNbinsX(); ib++){
      std::cout<<hDataCHISO[ieta]->GetBinLowEdge(ib)<<"-"<<hDataCHISO[0]->GetBinLowEdge(ib)+hDataCHISO[0]->GetBinWidth(ib)<<" & ";
      std::cout<<(int)hMCsumJets[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)hMCsumJets[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)hDataCHISO[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)hDataCHISO[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)hDataSIHIH[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)hDataSIHIH[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)hMCclCHISO[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)hMCclCHISO[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)hMCclSIHIH[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)hMCclSIHIH[ieta]->GetBinError(ib)<<"  ";
      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:Jets_to_gamma_diff_ways_to_fit_"<<ieta<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;
  }//end of loop over ieta

}// end of CompareJets()
