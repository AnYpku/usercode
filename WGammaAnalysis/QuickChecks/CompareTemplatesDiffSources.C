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

void DrawTemplates(int nSources, bool isTrue, bool isWMtCutCheck, TTree* tr[50], int colors[50], TString hLabels[50])
{

// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  const int nPtBins = 7;
  const double lims[nPtBins+1]={15,20,25,30,35,45,55,500};
  TCut cutEta[2];
  cutEta[0]="phoSCEta>-1.4442 && phoSCEta<1.4442";
  cutEta[1]="(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)";

  TCut cutSihih[2];
  TString varIso;
  if (isTrue){
    cutSihih[0]="phoSigmaIEtaIEta<0.011"; 
    cutSihih[1]="phoSigmaIEtaIEta<0.033";
    varIso="phoRandConeChIsoCorr";
  }
  else{
    cutSihih[0]="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.014";
    cutSihih[1]="phoSigmaIEtaIEta>0.033 && phoSigmaIEtaIEta<0.038";
    varIso="phoSCRChIsoCorr";
  }

  TCut cutWeight="weight";
  TCut cutWMt="WMt>50";

  TH1F* hists[nPtBins][2][nSources];
  TH1F* histsWMt[nPtBins][2][nSources];


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

      TString canvNameBase;
      if(isTrue) canvNameBase="cCompareTemplatesTrue";
      else canvNameBase="cCompareTemplatesFake";
      if (isWMtCutCheck) canvNameBase+="_WMtCheck_";
      TCanvas* canv = new TCanvas(canvNameBase+strAffix,canvNameBase+strAffix, 600, 600);
      TLegend* leg = new TLegend(0.5,0.5,0.9,0.9);

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
        float shift = 0.05*is*(up-low)/nBins;
        hists[ipt][ieta][is] = new TH1F(hName,hLabels[is],nBins,low+shift,up+shift);

        tr[is]->Draw(varIso+TString(">>")+hists[ipt][ieta][is]->GetName(),cut*cutWeight,"goff");

        hists[ipt][ieta][is]->SetLineWidth(2);
        hists[ipt][ieta][is]->SetLineColor(colors[is]);

        NormalizeHist(hists[ipt][ieta][is]);

        leg->AddEntry(hists[ipt][ieta][is],hLabels[is],"l");
//        std::cout<<"ready to loop over ib"<<std::endl;
        for (int ib=1; ib<=hists[ipt][ieta][is]->GetNbinsX(); ib++)
          if (hists[ipt][ieta][is]->GetBinContent(ib)>max) max=hists[ipt][ieta][is]->GetBinContent(ib);

        if (!isWMtCutCheck) continue;
          hName+="WMt";
        histsWMt[ipt][ieta][is] = new TH1F(hName,hLabels[is]+cutWMt.GetTitle(),nBins,low+shift,up+shift);

        tr[is]->Draw(varIso+TString(">>")+histsWMt[ipt][ieta][is]->GetName(),(cut&&cutWMt)*cutWeight,"goff");

        histsWMt[ipt][ieta][is]->SetLineWidth(2);
        histsWMt[ipt][ieta][is]->SetLineColor(colors[nSources+is]);

        NormalizeHist(histsWMt[ipt][ieta][is]);

        leg->AddEntry(histsWMt[ipt][ieta][is],hLabels[is]+cutWMt.GetTitle(),"l");
//        std::cout<<"ready to loop over ib"<<std::endl;
        for (int ib=1; ib<=hists[ipt][ieta][is]->GetNbinsX(); ib++)
          if (histsWMt[ipt][ieta][is]->GetBinContent(ib)>max) max=hists[ipt][ieta][is]->GetBinContent(ib); 
      }//end of loop over is


      hists[ipt][ieta][0]->SetStats(0);
      hists[ipt][ieta][0]->GetYaxis()->SetRangeUser(0.0,1.1*max);
      hists[ipt][ieta][0]->GetXaxis()->SetTitle(varIso);
      hists[ipt][ieta][0]->Draw("EP");
      if (isWMtCutCheck) histsWMt[ipt][ieta][0]->Draw("EP same");
      for (int is=1; is<nSources; is++){
        hists[ipt][ieta][is]->Draw("EP same");
        if (isWMtCutCheck) histsWMt[ipt][ieta][is]->Draw("EP same");
      }
      leg->Draw("same");

      hists[ipt][ieta][0]->SetTitle(TString("fake templates, ")+strAffix);
      hists[ipt][ieta][0]->Draw("EP same");


      TString strSave="../WGammaOutput/MUON_WGamma/Plots/QuickChecks/";
      strSave+=canv->GetName()+TString(".png");

      canv->SaveAs(strSave);

    }// end of loop over ieta
  }//end of loop over ipt  

}// end of DrawTemplates

void CompareFakeTemplatesDiffSources()
{
  const int nSources=6;
  TString fName[50]={
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root",
"../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_DYjets_to_ll.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_ttbarjets.root",
"../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root"};
  TFile* f[50];
  TTree* tr[50];
  for (int i=0; i<nSources; i++){
    f[i]=new TFile(fName[i]);
    tr[i]=(TTree*)f[i]->Get("selectedEvents");
  }
  int colors[50]={1,2,kGreen+1,4,6,kOrange+4};
  TString hLabels[50]={"data: Wg", "data: Zg", "MC: Wjets(Wg)", "MC: DYjets(Wg)","MC: ttjets(Wg)","MC: DYjets(Zg)"};
  DrawTemplates(nSources, 0, 0, tr, colors, hLabels);
}// end of CompareFakeTemplatesDiffSources

void CompareTrueTemplatesDiffSources()
{
  const int nSources=8;
  TString fName[50]={
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root",
"../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Zg.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_ttbarg.root",
"../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root",
"../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root"};
  TFile* f[50];
  TTree* tr[50];
  for (int i=0; i<nSources; i++){
    f[i]=new TFile(fName[i]);
    tr[i]=(TTree*)f[i]->Get("selectedEvents");
  }
  int colors[50]={1,2,kGreen+1,4,6,kOrange+4,kAzure+7,kViolet-7};
  TString hLabels[50]={"data: Wg", "data: Zg", "MC: Wjets(Wg)", "MC: Wg(Wg)", "MC: Zg(Wg)", "MC: ttg(Wg)", "MC: DYjets(Zg)", "MC: Zg(Zg)"};
  DrawTemplates(nSources, 1, 0, tr, colors, hLabels);
}// end of CompareTrueTemplatesDiffSources

void CompareTrueTemplatesDiffSourcesWMtCutCheck()
{
  const int nSources=3;
  TString fName[50]={
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root",
"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Zg.root"};
  TFile* f[50];
  TTree* tr[50];
  for (int i=0; i<nSources; i++){
    f[i]=new TFile(fName[i]);
    tr[i]=(TTree*)f[i]->Get("selectedEvents");
  }
  int colors[50]={1,2,kGreen+1,4,6,kOrange+4};
  TString hLabels[50]={"MC: Wjets(Wg)", "MC: Wg(Wg)", "MC: Zg(Wg)"};
  DrawTemplates(nSources, 1, 1, tr, colors, hLabels);
}// end of CompareTrueTemplatesDiffSources

