#include "TTemplates.h"
#include "TTemplatesSyst.h"
#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"
//this package

#include <iostream>
//standard C++

#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TCut.h"
//ROOT

void AuxTemplatesPickSideband(TConfiguration::AnalysisParameters &anPars)
{
  TConfiguration conf;
  TTemplates temp;

  std::cout<<std::endl;
  std::cout<<"AuxTemplatesPickSideband for"<<std::endl;
  std::cout<<conf.StrChannel(anPars.channel)<<" ";
  std::cout<<conf.StrVgType(anPars.vgamma)<<" ";
  std::cout<<conf.StrTempl(anPars.templFits)<<std::endl;
  std::cout<<std::endl;

  TString strMCtruth = conf.GetYieldsMCtruthFileName(anPars.channel, anPars.vgamma, anPars.varKin);
  TFile fMCtruth(strMCtruth);
  TH1F* hTrueMCtruth[2];
  hTrueMCtruth[conf.BARREL] = (TH1F*)fMCtruth.Get("hSum_MCtruth__Barrel_TrueGamma");
  hTrueMCtruth[conf.ENDCAP] = (TH1F*)fMCtruth.Get("hSum_MCtruth__Endcap_TrueGamma");
  if (hTrueMCtruth[conf.BARREL]->GetNbinsX()!=anPars.nKinBins || hTrueMCtruth[conf.ENDCAP]->GetNbinsX()!=anPars.nKinBins){
    std::cout<<"ERROR in AuxTemplatesPickSideband: binning doesn't match. anPars.nKinBins="<<anPars.nKinBins;
    std::cout<<", hTrueMCtruth[conf.BARREL]->GetNbinsX()="<<hTrueMCtruth[conf.BARREL]->GetNbinsX();
    std::cout<<", hTrueMCtruth[conf.ENDCAP]->GetNbinsX()="<<hTrueMCtruth[conf.ENDCAP]->GetNbinsX();
    std::cout<<std::endl;
    return;
  }

  TString strF = conf.GetDDTemplateFileName(anPars.channel,anPars.vgamma,anPars.templFits,anPars.varKin);
  strF.ReplaceAll(".root","_Syst_MCclosure.root");
  TFile fInpMCclosure(strF);     
  TVectorD* vecSbL[50][2];
  TVectorD* vecSbU[50][2];
  TVectorD* vecTrueVal[50][2];
  TVectorD* vecTrueErr[50][2];
  double selSbL[anPars.nKinBins][2];
  double selSbU[anPars.nKinBins][2];
  double selTrueVal[anPars.nKinBins][2];
  double selTrueErr[anPars.nKinBins][2];
  std::cout<<"True Vals based on MC closure:"<<std::endl;
  float scale=1.0;
  //  if (anPars.vgamma==conf.W_GAMMA){
  //    scale=conf.GetBlindPrescale();
  //  }
  std::cout<<"scale="<<scale<<", channel="<<conf.StrChannel(anPars.channel)<<std::endl;
  for (int ikin=1; ikin<=anPars.nKinBins; ikin++){
//  for (int ikin=1; ikin<=1; ikin++){
    for (int ieta=conf.BARREL; ieta<=conf.ENDCAP; ieta++){
      TString strLabelKin=temp.StrLabelKin(ikin,anPars.nKinBins,  anPars.kinBinLims, anPars.varKin);
      vecSbL[ikin][ieta]=(TVectorD *)fInpMCclosure.Get(TString("vec_sbL")+strLabelKin+temp.StrLabelEta(ieta));
      vecSbU[ikin][ieta]=(TVectorD *)fInpMCclosure.Get(TString("vec_sbU")+strLabelKin+temp.StrLabelEta(ieta));
      vecTrueVal[ikin][ieta]=(TVectorD *)fInpMCclosure.Get(TString("vec_trueVal")+strLabelKin+temp.StrLabelEta(ieta));
      //      vecTrueErr[ikin][ieta]=(TVectorD *)fInpMCclosure.Get(TString("vec_trueErr")+strLabelKin+temp.StrLabelEta(ieta));
      std::cout<<strLabelKin+temp.StrLabelEta(ieta)<<std::endl;

      double diff=10000;
      for (int ival=0; ival<vecSbL[ikin][ieta]->GetNrows(); ival++){
        std::cout<<"sb = "<<vecSbL[ikin][ieta]->operator()(ival)<<" - "<<vecSbU[ikin][ieta]->operator()(ival)<<", ";
        std::cout<<"true yield = "<<vecTrueVal[ikin][ieta]->operator()(ival);
        std::cout<<std::endl;
        float diff1 = fabs(vecTrueVal[ikin][ieta]->operator()(ival)-scale*hTrueMCtruth[ieta]->GetBinContent(ikin));
        if (diff1<diff){
          selSbL[ikin][ieta]=vecSbL[ikin][ieta]->operator()(ival);
          selSbU[ikin][ieta]=vecSbU[ikin][ieta]->operator()(ival);
          selTrueVal[ikin][ieta]=vecTrueVal[ikin][ieta]->operator()(ival);
	  //          selTrueErr[ikin][ieta]=vecTrueErr[ikin][ieta]->operator()(ival);
          diff=diff1;
        }
      }// end of loop over ival

      std::cout<<"MC truth = "<<scale*hTrueMCtruth[ieta]->GetBinContent(ikin);
      std::cout<<", bin="<<hTrueMCtruth[ieta]->GetBinLowEdge(ikin);
      std::cout<<"-"<<hTrueMCtruth[ieta]->GetBinLowEdge(ikin)+hTrueMCtruth[ieta]->GetBinWidth(ikin);
      std::cout<<std::endl;
      std::cout<<"true yield = "<<selTrueVal[ikin][ieta]<<", sb="<<selSbL[ikin][ieta]<<"-"<<selSbU[ikin][ieta]<<std::endl;
    }// end of loop over ieta
  }// end of loop over ikin

  std::cout<<std::endl;

  for (int ieta=conf.BARREL; ieta<=conf.ENDCAP; ieta++){
    std::cout<<temp.StrLabelEta(ieta)<<std::endl;
    for (int ikin=1; ikin<=anPars.nKinBins; ikin++){
//  for (int ikin=1; ikin<=1; ikin++){
      TString strLabelKin=temp.StrLabelKin(ikin,anPars.nKinBins,  anPars.kinBinLims, anPars.varKin);
      std::cout<<strLabelKin<<": ";
      std::cout<<"sb="<<selSbL[ikin][ieta]<<"-"<<selSbU[ikin][ieta]<<", true yield = "<<selTrueVal[ikin][ieta];
      std::cout<<" ( MC truth = "<<scale*hTrueMCtruth[ieta]->GetBinContent(ikin);
      std::cout<<", bin="<<hTrueMCtruth[ieta]->GetBinLowEdge(ikin);
      std::cout<<"-"<<hTrueMCtruth[ieta]->GetBinLowEdge(ikin)+hTrueMCtruth[ieta]->GetBinWidth(ikin)<<" ) ";
      std::cout<<std::endl;
    }// end of loop over ikin
  }// end of loop over ieta

  // Write to file
  TFile* fOut = new TFile(conf.GetSidebandsFileName(anPars.channel, anPars.vgamma, anPars.templFits, anPars.varKin),"recreate");
  fOut->cd();
  for (int ieta=conf.BARREL; ieta<=conf.ENDCAP; ieta++){
    TString strSbLower = conf.GetSidebandsLowerHistName(ieta);
    TString strSbUpper = conf.GetSidebandsUpperHistName(ieta);
    TString strMCtruthTrue = conf.GetMCtruthTrueHistName(ieta);
    TString strFromMCfitTrue = conf.GetFromMCfitTrueHistName(ieta);

    hTrueMCtruth[ieta]->SetTitle(strMCtruthTrue);
    hTrueMCtruth[ieta]->Write(strMCtruthTrue);
    TH1F* hSbLower = new TH1F(strSbLower,strSbLower,anPars.nKinBins,anPars.kinBinLims);
    TH1F* hSbUpper = new TH1F(strSbUpper,strSbUpper,anPars.nKinBins,anPars.kinBinLims);
    TH1F* hFromMCfitTrue = new TH1F(strFromMCfitTrue,strFromMCfitTrue,anPars.nKinBins,anPars.kinBinLims);
    for (int ikin=1; ikin<=anPars.nKinBins; ikin++){
      hSbLower->SetBinContent(ikin,selSbL[ikin][ieta]);
      hSbLower->SetBinError(ikin,0.01*selSbL[ikin][ieta]);
      hSbUpper->SetBinContent(ikin,selSbU[ikin][ieta]);
      hSbUpper->SetBinError(ikin,0.01*selSbU[ikin][ieta]);
      hFromMCfitTrue->SetBinContent(ikin,selTrueVal[ikin][ieta]);
      // hFromMCfitTrue->SetBinError(ikin,selTrueErr[ikin][ieta]);
      hFromMCfitTrue->SetBinError(ikin,sqrt(selTrueVal[ikin][ieta]));
    }//end of loop over ikin
    hSbLower->Write();
    hSbUpper->Write();
    hFromMCfitTrue->Write();
  }//end of loop over ieta
}//end  of AuxTemplatesPickSideband

// for one eta bin
void Redo2DPlotsMCclosure(TH1F* hSbLower, TH1F* hSbUpper, TH1F* hFromMCfitTrue, TH1F* hTrueMCtruth)
{

}// end of Redo2DPlotsMCclosure
