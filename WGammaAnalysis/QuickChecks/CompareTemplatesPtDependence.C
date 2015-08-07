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

void DrawTemplates(bool isTrue, TTree* tr, TString strName, TString strLabel, TString varIso, TTree* trLeak=0)
{

// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  const int nPtBins = 5;
  const double lims[nPtBins+1]={10,15,20,25,55,500};
//  const int nPtBins = 8;
//  const double lims[nPtBins+1]={10,15,20,25,30,35,45,55,500};
  //const int nPtBins = 4;
  //const double lims[nPtBins+1]={15,20,25,30,35};
  //const int nPtBins = 12;
  //const double lims[nPtBins+1]={15,20,25,30,35,45,55,65,75,85,95,120,500};
  int colors[50]={1,2,4,6,kOrange+4,kAzure+7,kGreen+1,kViolet-7};
  TCut cutEta[2];
  cutEta[0]="phoSCEta>-1.4442 && phoSCEta<1.4442";
  cutEta[1]="(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)";

  TCut cutSihih[2];
  cutSihih[0]="phoSigmaIEtaIEta<0.011"; 
  cutSihih[1]="phoSigmaIEtaIEta<0.033";

  TCut cutWeight="weight";

  TH1F* hists[nPtBins][2];
  TH1F* histsLeak[nPtBins][2];

  TFile* fOut = new TFile("fOut.root","recreate"); 

  for (int ieta=0; ieta<=1; ieta++){
    float max = 0;
    TString canvNameBase;
    if(isTrue) canvNameBase="cCompareTemplatesPtDepTrue";
    else canvNameBase="cCompareTemplatesPtDepFake";
    canvNameBase+=strName;
    if (ieta==0) canvNameBase+="_Barrel_";
    if (ieta==1) canvNameBase+="_Endcap_";
    TCanvas* canv = new TCanvas(canvNameBase,canvNameBase, 600, 600);
    TLegend* leg;
    leg = new TLegend(0.53,0.37,0.87,0.87);


    for (int ipt=0; ipt<nPtBins; ipt++){
      TCut cutPt =  CutPt(lims[ipt],lims[ipt+1]);
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

      TString hName=TString("h_")+strAffix;

      float shift = 0.05*ipt*(up-low)/nBins;
      shift=0;
      hists[ipt][ieta] = new TH1F(hName,hName,nBins,low+shift,up+shift);
      tr->Draw(varIso+TString(">>")+hists[ipt][ieta]->GetName(),cut*cutWeight,"goff");
      if (!isTrue) histsLeak[ipt][ieta] = new TH1F(hName+TString("_leak"),hName+TString("_leak"),nBins,low+shift,up+shift);
      if (!isTrue) trLeak->Draw(varIso+TString(">>")+histsLeak[ipt][ieta]->GetName(),cut*cutWeight,"goff");

      if (!isTrue){
        std::cout<<std::endl;
        std::cout<<strAffix<<std::endl;
        std::cout<<"tr->GetEntries(cut*cutWeight)="<<tr->GetEntries(cut*cutWeight)<<std::endl;
        std::cout<<"trLeak->GetEntries(cut*cutWeight)="<<trLeak->GetEntries(cut*cutWeight)<<std::endl;
        std::cout<<"hFake+hLeak: "<<hists[ipt][ieta]->GetSumOfWeights()<<std::endl;
        std::cout<<"hLeak: "<<histsLeak[ipt][ieta]->GetSumOfWeights()<<std::endl;
      }

      if (!isTrue) hists[ipt][ieta]->Add(histsLeak[ipt][ieta],-1);

      if (!isTrue){
        std::cout<<"(hFake+hLeak)-hLeak: "<<hists[ipt][ieta]->GetSumOfWeights()<<std::endl;
        std::cout<<std::endl;
      }

      hists[ipt][ieta]->SetLineWidth(2);
      hists[ipt][ieta]->SetLineColor(colors[ipt]);

      NormalizeHist(hists[ipt][ieta]);

      TString str_hLabel=hName;
      str_hLabel.ReplaceAll("h__pt","P_{T}^{#gamma}: ");
      str_hLabel.ReplaceAll("to","-");
      str_hLabel.ReplaceAll("_Barrel_","");
      str_hLabel.ReplaceAll("_Endcap_","");
      str_hLabel+=" GeV";
      leg->AddEntry(hists[ipt][ieta],str_hLabel,"l");

      for (int ib=1; ib<=hists[ipt][ieta]->GetNbinsX(); ib++)
        if (hists[ipt][ieta]->GetBinContent(ib)>max) max=hists[ipt][ieta]->GetBinContent(ib);

    }// end of loop over ipt
    hists[0][ieta]->SetStats(0);
    hists[0][ieta]->GetYaxis()->SetRangeUser(0.0,1.1*max);
    TString axLabel;
    if (varIso=="phoSCRChIsoCorr") axLabel="I_{ch-SCR}, GeV";
    if (varIso=="phoRandConeChIsoCorr") axLabel="I_{ch-RandCone}, GeV";
    if (varIso=="phoPFChIsoCorr") axLabel="I_{chHad}^{#gamma}, GeV";
    hists[0][ieta]->GetXaxis()->SetTitle(axLabel);
    hists[0][ieta]->Draw("EP");
    for (int ipt=0; ipt<nPtBins; ipt++){
       hists[ipt][ieta]->Draw("EP same");
    }
    leg->Draw("same");

    TString strEta;
      if (ieta==0) strEta=" Barrel";
      if (ieta==1) strEta+=" Endcap";
    if (isTrue) hists[0][ieta]->SetTitle(TString("Real-#gamma templates, ")+strLabel+strEta);
    else hists[0][ieta]->SetTitle(TString("Fake-#gamma templates, ")+strLabel+strEta);
    hists[0][ieta]->Draw("EP same");


    TString strSave="../WGammaOutput";
    if(isTrue) strSave+="/MERGED/Plots/QuickChecks/";
    else strSave+="/ChannelsMERGED_ZGamma/Plots/QuickChecks/";
    strSave+=canv->GetName()+TString(".png");
    canv->SaveAs(strSave);
    strSave.ReplaceAll(".png",".pdf");
    canv->SaveAs(strSave);     
  }//end of loop over ieta  

}// end of DrawTemplates

void CompareFakeTemplates()
{
  TFile* f1 = new TFile("../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root");
  TTree* tr1=(TTree*)f1->Get("selectedEvents");
  TFile* f1Leak = new TFile("../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGammaSIGMC.root");
  TTree* tr1Leak=(TTree*)f1Leak->Get("selectedEvents");
  DrawTemplates(0, tr1, "Zg_FakeTemplPtDep", "Z#gamma ISR I_{chHad}^{#gamma}", "phoPFChIsoCorr",tr1Leak);
}// end of CompareFakeTemplatesDiffSources

void CompareTrueTemplates()
{
  // real photon templates from RandCone
  //TFile* f1 = new TFile("../WGammaOutput/ChannelsMERGED_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  //TTree* tr1=(TTree*)f1->Get("selectedEvents");
  //DrawTemplates(1, tr1, "Wg_TrueTemplPtDep", "W#gamma I_{ch-RandCone}", "phoRandConeChIsoCorr");

  // real photon templates from Zg FSR
  TFile* f2 = new TFile("../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root");
  TTree* tr2=(TTree*)f2->Get("selectedEvents");
  DrawTemplates(1, tr2, "ZgFSR_TrueTemplPtDep", "Z#gamma FSR I_{chHad}^{#gamma}", "phoPFChIsoCorr");

  // real photon templates from Zg MC
//  TFile* f3 = new TFile("../WGammaOutput/ChannelsMERGED_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root");
//  TTree* tr3=(TTree*)f3->Get("selectedEvents");
//  DrawTemplates(1, tr3, "ZgMC_TrueTemplPtDep_PFnoSCR", "Z#gamma (MC) I_{chHad}^{#gamma}", "phoPFChIsoCorr");
  
  //void DrawTemplates(bool isTrue, TTree* tr, TString strName, TString strLabel, TString varIso, TTree* trLeak=0)


}// end of CompareTrueTemplatesDiffSources

