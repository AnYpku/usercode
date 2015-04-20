#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include <iomanip>

TCut CutPt(float lowPt, float upPt)
{
  TString strPt = "phoEt>";
  strPt+=lowPt;
  strPt+= "&& phoEt<=";
  strPt+=upPt;
  TCut cut(strPt);
  return cut;  
}// end of CutPt

void CalcEff(TTree* tr, TCut cutTotal, TCut cutExtra, float& eff, float& err)
{
  TFile f("fOut.root","recreate");
  TCut cutWeight="weight";

  TH1F* hPassed = new TH1F("hPassed","hPassed",1,-3.0,3.0);
  hPassed->Sumw2();
  tr->Draw("phoSCEta>>hPassed",(cutTotal && cutExtra)*cutWeight,"goff");
  TH1F* hTotal = new TH1F("hTotal","hTotal",1,-3.0,3.0);
  hTotal->Sumw2();
  tr->Draw("phoSCEta>>hTotal",cutTotal*cutWeight,"goff");
  TH1F* hEff = (TH1F*)hPassed->Clone();
  hEff->Divide(hTotal);
//  hEff->Draw("EP");
  eff=hEff->GetBinContent(1);
  err=hEff->GetBinError(1);
  delete hPassed;
  delete hTotal;
}// end of CalcEff

float ErrDivision(float num, float den, float dnum, float dden)
{
  if (fabs(den)<0.001) return -1;
  float err = num*num*dden*dden+dnum*dnum*den*den;
  err=sqrt(err);
  err=err/(den*den);
  return err;
}//end of EffDivision

void PrintEffAndSF(float eff_data, float err_data, float eff_sign, float err_sign)
{ 
  float sf, sf_err;
      if (fabs(eff_sign)>0.001){ 
        sf=eff_data/eff_sign;
        sf_err=ErrDivision(eff_data, eff_sign, err_data, err_sign);
 //       std::cout<<"sf_err_PF"<<sf_err_PF<<std::endl;
      }
      else {sf=-1;sf_err=-1;}

      std::cout<<std::setprecision(2)<<" $"<<eff_data<<"\\pm";
      if (err_data>0.1) std::cout<<std::setprecision(2)<<err_data<<"$ ";
      else std::cout<<std::setprecision(1)<<err_data<<"$ ";
      std::cout<<" & ";
      std::cout<<std::setprecision(2)<<" $"<<eff_sign<<"\\pm";
      if (err_sign>0.1) std::cout<<std::setprecision(2)<<err_sign<<"$ ";
      else std::cout<<std::setprecision(1)<<err_sign<<"$ ";
      std::cout<<" & ";
      std::cout<<std::setprecision(2)<<" $"<<sf<<"\\pm";;
      if (sf_err>0.1) std::cout<<std::setprecision(2)<<sf_err<<"$ ";
      else std::cout<<std::setprecision(1)<<sf_err<<"$ ";

}//end of PrintEffandSF

void CheckPFvsSCRIchScaleFactors()
{
  TFile fData("../WGammaOutput/MERGED/selected_ZGamma_FSR_Data.root");
  TFile fSign("../WGammaOutput/MERGED/selected_ZGamma_FSR_SIGMC.root");

  TTree* trData = (TTree*)fData.Get("selectedEvents");
  TTree* trSign = (TTree*)fSign.Get("selectedEvents");

// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  const int nPtBins = 7;
  const double lims[nPtBins+1]={15,20,25,30,35,45,55,500};
  TCut cutEta[2];
  cutEta[0]="phoSCEta>-1.4442 && phoSCEta<1.4442";
  cutEta[1]="(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)";
  TCut cutSihih[2];
  cutSihih[0]="phoSigmaIEtaIEta<0.011";
  cutSihih[1]="phoSigmaIEtaIEta<0.033";

  TCut cutPFChIso[2];
  cutPFChIso[0]="phoPFChIsoCorr<1.5";
  cutPFChIso[1]="phoPFChIsoCorr<1.2";
  TCut cutSCRChIso[2];
  cutSCRChIso[0]="phoSCRChIsoCorr<1.5";
  cutSCRChIso[1]="phoSCRChIsoCorr<1.2";

  for (int ieta=0; ieta<=1; ieta++){

    TString strEta;
    if (ieta==0) strEta="Barrel";
    if (ieta==1) strEta="Endcap";

    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{ eff of $I_{ch}$ PF vs SCR, "<<strEta<<"}"<<std::endl;
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|}"<<std::endl;


    std::cout<<" & PF & PF & PF & SCR & SCR & SCR \\\\"<<std::endl;
    std::cout<<" bin & $eff_{data}$ & $eff_{sign}$ & $eff_{data}/eff_{sign}$ & $eff_{data}$ & $eff_{sign}$ & $eff_{data}/eff_{sign}$ \\\\ \\hline"<<std::endl;

    for (int ipt=0; ipt<nPtBins; ipt++){
      TCut cutPt =  CutPt(lims[ipt],lims[ipt+1]);
      TCut cut = cutPt && cutEta[ieta] && cutSihih[ieta];
      TString strPt="";
      strPt+=lims[ipt];
      strPt+="-";
      strPt+=lims[ipt+1];
      std::cout<<strPt<<" & ";

      float eff_data_PF,err_data_PF,eff_sign_PF,err_sign_PF;
      float eff_data_SCR,err_data_SCR,eff_sign_SCR,err_sign_SCR;
      CalcEff(trData,cut,cutPFChIso[ieta],eff_data_PF,err_data_PF);
      CalcEff(trSign,cut,cutPFChIso[ieta],eff_sign_PF,err_sign_PF);
      CalcEff(trData,cut,cutSCRChIso[ieta],eff_data_SCR,err_data_SCR);
      CalcEff(trSign,cut,cutSCRChIso[ieta],eff_sign_SCR,err_sign_SCR);

      PrintEffAndSF(eff_data_PF, err_data_PF, eff_sign_PF, err_sign_PF);
      std::cout<<" & ";
      PrintEffAndSF(eff_data_SCR, err_data_SCR, eff_sign_SCR, err_sign_SCR);

      std::cout<<" \\\\ \\hline "<<std::endl;

    }//end of loop over ipt

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:sf_PFvsSCR_"<<strEta<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  }//end of loop over ieta
}// end of CheckMuonVsElectronTemplates

