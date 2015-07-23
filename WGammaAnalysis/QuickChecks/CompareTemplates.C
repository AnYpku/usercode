#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "Rtypes.h"

#include <iostream>

TCut CutPt(float lowPt, float upPt)
{
  TString strPt = "phoEt>";
  strPt+=lowPt;
  strPt+= "&& phoEt<=";
  strPt+=upPt;
  TCut cut(strPt);
  return cut;  
}// end of CutPt

void NormalizeHist(TH1F* h)
{
  int Nb=h->GetNbinsX();
  float norm = 0;
  for (int ib=1; ib<=Nb; ib++){
    norm+=h->GetBinContent(ib);
  }// end of ib
  for (int ib=1; ib<=Nb; ib++){
    float cont = h->GetBinContent(ib);
    float err = h->GetBinError(ib);
    h->SetBinContent(ib,cont/norm);
    h->SetBinError(ib,err/norm);
  }// end of ib
}// end of NomalizeHist

void DrawTemplates(int nSources, bool isTrue, TString canvNameBase, TTree* tr[50], int colors[50], TString hLabels[50], TString varIso[50])
{

// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  const int nPtBins = 7;
  const double lims[nPtBins+1]={15,20,25,30,35,45,55,500};
  //const int nPtBins = 12;
  //const double lims[nPtBins+1]={15,20,25,30,35,45,55,65,75,85,95,120,500};
  TCut cutEta[2];
  cutEta[0]="phoSCEta>-1.4442 && phoSCEta<1.4442";
  cutEta[1]="(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)";

  TCut cutSihih[2];
  cutSihih[0]="phoSigmaIEtaIEta<0.011"; 
  cutSihih[1]="phoSigmaIEtaIEta<0.033";

  TCut cutWeight="weight";

  TH1F* hists[nPtBins][2][2*nSources];

  TFile* fOut = new TFile("fOut.root","recreate"); 

  std::cout<<"ready to llop over ipt"<<std::endl;

  for (int ipt=0; ipt<nPtBins; ipt++){
    TCut cutPt =  CutPt(lims[ipt],lims[ipt+1]);
    for (int ieta=0; ieta<=1; ieta++){
      TCut cut = cutPt && cutEta[ieta] && cutSihih[ieta];
      TString strAffix="_pt";
      strAffix+=lims[ipt];
      strAffix+="to";
      strAffix+=lims[ipt+1];
      if (ieta==0) strAffix+="_Barrel_";
      if (ieta==1) strAffix+="_Endcap_";

      std::cout<<"strAffix="<<strAffix<<std::endl;

      int nBins=21;
      float low = -0.9;
      float up = 20.1;
      if (isTrue){nBins=7; low=-0.9; up=6.1;};

      TCanvas* canv = new TCanvas(canvNameBase+strAffix,canvNameBase+strAffix, 600, 600);
      TLegend* leg;
      if (isTrue) leg = new TLegend(0.50,0.50,0.90,0.90);
      else leg = new TLegend(0.50,0.60,0.85,0.85);
      float max = 0;

//      std::cout<<"ready to loop over is"<<std::endl;

      for (int is=0; is<nSources; is++){

        TString hName=hLabels[is];
        hName.ReplaceAll("-","");
        hName.ReplaceAll("(","");
        hName.ReplaceAll(")","");
        hName.ReplaceAll(":","");
        hName.ReplaceAll(" ","");
        hName+=strAffix;

        int ind_is=is;
        int ind=is;
        if (!isTrue) ind=2*is;

        float shift = 0.05*ind_is*(up-low)/nBins;
        shift=0;
        hists[ipt][ieta][ind] = new TH1F(hName,hLabels[ind_is],nBins,low+shift,up+shift);
        if (!isTrue) hists[ipt][ieta][ind+1] = new TH1F(hName+TString("_leak"),hLabels[ind_is]+TString("_leak"),nBins,low+shift,up+shift);
        tr[ind]->Draw(varIso[ind]+TString(">>")+hists[ipt][ieta][ind]->GetName(),cut*cutWeight,"goff");
        if (!isTrue) tr[ind+1]->Draw(varIso[ind+1]+TString(">>")+hists[ipt][ieta][ind+1]->GetName(),cut*cutWeight,"goff");

        if (!isTrue){
          std::cout<<std::endl;
          std::cout<<strAffix<<std::endl;
          std::cout<<"hFake+hLeak: "<<hists[ipt][ieta][ind]->GetSumOfWeights()<<std::endl;
          std::cout<<"hLeak: "<<hists[ipt][ieta][ind+1]->GetSumOfWeights()<<std::endl;
        }


        if (!isTrue) hists[ipt][ieta][ind]->Add(hists[ipt][ieta][ind+1],-1);

        if (!isTrue){
          std::cout<<"(hFake+hLeak)-hLeak: "<<hists[ipt][ieta][ind]->GetSumOfWeights()<<std::endl;
          std::cout<<std::endl;
        }

        hists[ipt][ieta][ind]->SetLineWidth(2);
        hists[ipt][ieta][ind]->SetLineColor(colors[ind_is]);

        NormalizeHist(hists[ipt][ieta][ind]);

        leg->AddEntry(hists[ipt][ieta][ind],hLabels[ind_is],"l");
//        std::cout<<"ready to loop over ib"<<std::endl;
        for (int ib=1; ib<=hists[ipt][ieta][is]->GetNbinsX(); ib++)
          if (hists[ipt][ieta][ind]->GetBinContent(ib)>max) max=hists[ipt][ieta][ind]->GetBinContent(ib);

      }//end of loop over is


      hists[ipt][ieta][0]->SetStats(0);
      hists[ipt][ieta][0]->GetYaxis()->SetRangeUser(0.0,1.1*max);
      if (isTrue) hists[ipt][ieta][0]->GetXaxis()->SetTitle("I_{ch-SCR}(I_{ch-RandCone}), GeV");
      else hists[ipt][ieta][0]->GetXaxis()->SetTitle("I_{ch-SCR}, GeV");
      hists[ipt][ieta][0]->Draw("EP");
      for (int is=1; is<nSources; is++){
        int ind=is;
        if (!isTrue) ind=2*is;
        hists[ipt][ieta][ind]->Draw("EP same");
      }
      leg->Draw("same");

      if (isTrue) hists[ipt][ieta][0]->SetTitle(TString("Real-#gamma templates, ")+strAffix);
      else hists[ipt][ieta][0]->SetTitle(TString("Fake-#gamma templates, ")+strAffix);
      hists[ipt][ieta][0]->Draw("EP same");


      TString strSave="../WGammaOutput";
      if(isTrue) strSave+="/MERGED/Plots/QuickChecks/";
      else strSave+="/ChannelsMERGED_ZGamma/Plots/QuickChecks/";
      strSave+=canv->GetName();

      canv->SaveAs(strSave+TString(".png"));
      canv->SaveAs(strSave+TString(".pdf"));      

    }// end of loop over ieta
  }//end of loop over ipt  

}// end of DrawTemplates

void CompareFakeTemplates()
{
  const int nSources=2;
  TString fName[50]={
"../WGammaOutput/MUON_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/MUON_ZGamma/FsrExcludedSelected/selected_ZGammaSIGMC.root",
"../WGammaOutput/ELECTRON_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/ELECTRON_ZGamma/FsrExcludedSelected/selected_ZGammaSIGMC.root"
};
  TFile* f[50];
  TTree* tr[50];
  for (int i=0; i<nSources; i++){
    f[2*i]=new TFile(fName[2*i]);
    tr[2*i]=(TTree*)f[2*i]->Get("selectedEvents");
    f[2*i+1]=new TFile(fName[2*i+1]);
    tr[2*i+1]=(TTree*)f[2*i+1]->Get("selectedEvents");
  }
  int colors[50]={1,4};
  TString hLabels[50]={"Z#gamma ISR, #mu, I_{ch-SCR}", "Z#gamma ISR, e, I_{ch-SCR}"};
  TString varIso[50]={"phoSCRChIsoCorr","phoSCRChIsoCorr","phoSCRChIsoCorr","phoSCRChIsoCorr"};
  DrawTemplates(nSources, 0, "cCompareTemplatesFake", tr, colors, hLabels, varIso);
}// end of CompareFakeTemplates

void CompareTrueTemplates()
{
  const int nSources=4;
  TString fName[50]={
"../WGammaOutput/ChannelsMERGED_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root",
"../WGammaOutput/ChannelsMERGED_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root"
};
  TFile* f[50];
  TTree* tr[50];
  for (int i=0; i<nSources; i++){
    f[i]=new TFile(fName[i]);
    tr[i]=(TTree*)f[i]->Get("selectedEvents");
  }
  int colors[50]={1,2,4,kOrange+4,6,kGreen+1,kAzure+7,kViolet-7};
  TString hLabels[50]={"W#gamma, I_{ch-RandCone}", "Z#gamma, I_{ch-RandCone}", "Z#gamma FSR, I_{ch-RandCone}", "Z#gamma FSR, I_{ch-SCR}"};
  TString varIso[50]={"phoRandConeChIsoCorr","phoRandConeChIsoCorr","phoRandConeChIsoCorr","phoSCRChIsoCorr"};
  DrawTemplates(nSources, 1, "cCompareTemplatesTrue", tr, colors, hLabels, varIso);
}// end of CompareTrueTemplates

void CompareTrueTemplatesDATAvsMC()
{
  const int nSources=4;
  TString fName[50]={
"../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGammaSIGMC.root",
"../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGammaSIGMC.root",
"../WGammaOutput/ChannelsMERGED_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root"
};
  TFile* f[50];
  TTree* tr[50];
  for (int i=0; i<nSources; i++){
    f[i]=new TFile(fName[i]);
    tr[i]=(TTree*)f[i]->Get("selectedEvents");
  }
  int colors[50]={4,1,2,kOrange+4,6,kGreen+1,kAzure+7,kViolet-7};
  TString hLabels[50]={"Z#gamma FSR data", "Z#gamma FSR MC", "Z#gamma ISR MC","W#gamma MC"};
  TString varIso[50]={"phoPFChIsoCorr","phoPFChIsoCorr","phoPFChIsoCorr","phoPFChIsoCorr"};
  DrawTemplates(nSources, 1, "cCompareTemplatesTrueDATAvsMC", tr, colors, hLabels, varIso);
}// end of CompareTrueTemplates

