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

void DrawTemplates(TString strCanvBase, TTree* tr, TString varTemp, TCut cutEta)
{

// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  // const int nPtBins = 2;
  //const double lims[nPtBins+1]={15,20,25};
  const int nPtBins = 3;
  const double lims[nPtBins+1]={15,20,35,500};

  TCut cutWeight="weight";

  TH1F* hists[nPtBins][2];

  TFile* fOut = new TFile("fOut.root","recreate"); 

  std::cout<<"ready to loop over ipt"<<std::endl;

  for (int ipt=0; ipt<nPtBins; ipt++){
    
      TCut cutPt =  CutPt(lims[ipt],lims[ipt+1]);
      TCut cut = cutPt && cutEta;
      TString strAffix="_pt";
      strAffix+=lims[ipt];
      strAffix+="to";
      strAffix+=lims[ipt+1];
      
      std::cout<<"strAffix="<<strAffix<<std::endl;
      
      int nBins=21;
      float low = -0.9;
      float up = 20.1;

      if (varTemp=="phoPFChIsoCorr"){
        if (strCanvBase.Contains("Wjets")){nBins=21; low=-0.9; up=20.1;}
        if (strCanvBase.Contains("Wg")){nBins=7; low=-0.9; up=6.1;}
      }
      if (varTemp=="phoSigmaIEtaIEta") {
        if (strCanvBase.Contains("BARREL")){nBins=32; low=0.005; up=0.021;}
        if (strCanvBase.Contains("ENDCAP")){nBins=25; low=0.019; up=0.069;}
      }

      TCanvas* canv = new TCanvas(strCanvBase+strAffix,strCanvBase+strAffix, 600, 600);
      
      TLegend* leg;
      leg = new TLegend(0.50,0.70,0.90,0.90);
      float max = 0;
      
        TString hName="h"+strCanvBase+strAffix;
        hists[ipt][0] = new TH1F(hName,hName,nBins,low,up);
        TCut cutWMt="WMt>40 && WMt<70";
        tr->Draw(varTemp+TString(">>")+hists[ipt][0]->GetName(),(cut&&cutWMt)*cutWeight,"goff");
	std::cout<<"cut="<<(cut&&cutWMt).GetTitle()<<", tr->GetEntries(cut)="<<std::endl;
	std::cout<<tr->GetEntries((cut&&cutWMt)*cutWeight)<<std::endl;
        hists[ipt][0]->Print();
        hists[ipt][0]->SetLineWidth(2);
        hists[ipt][0]->SetLineColor(1);
	NormalizeHist(hists[ipt][0]);
        leg->AddEntry(hists[ipt][0],"40<WMt<70 GeV","l");

        hName="h1"+strCanvBase+strAffix;
        hists[ipt][1] = new TH1F(hName,hName,nBins,low,up);
        cutWMt="WMt>70";
        tr->Draw(varTemp+TString(">>")+hists[ipt][1]->GetName(),(cut&&cutWMt)*cutWeight,"goff");
	std::cout<<"cut="<<(cut&&cutWMt).GetTitle()<<", tr->GetEntries(cut)="<<std::endl;
	std::cout<<tr->GetEntries((cut&&cutWMt)*cutWeight)<<std::endl;
        hists[ipt][1]->Print();
        hists[ipt][1]->SetLineWidth(2);
        hists[ipt][1]->SetLineColor(2);
	NormalizeHist(hists[ipt][1]);
        leg->AddEntry(hists[ipt][1],"WMt>70 GeV","l");

      hists[ipt][0]->SetStats(0);
      if (varTemp=="phoPFChIsoCorr") hists[ipt][0]->GetXaxis()->SetTitle("I_{ch}, GeV");
      if (varTemp=="phoSigmaIEtaIEta") hists[ipt][0]->GetXaxis()->SetTitle("#sigma_{i #etai #eta}");
      hists[ipt][0]->Draw("EP");
      hists[ipt][1]->Draw("EP same");
      
      leg->Draw("same");
      
      TString strTitle=strCanvBase+strAffix;
      strTitle.ReplaceAll("cTemplatesVsWMt","");
      strTitle.ReplaceAll("phoSigmaIEtaIEta","");
      strTitle.ReplaceAll("phoPFChIsoCorr","");
      hists[ipt][0]->SetTitle(strTitle);

      TString strSave="../WGammaOutput";
      strSave+="/ChannelsMERGED_WGamma/Plots/QuickChecks/";
      strSave+=canv->GetName();

      canv->SaveAs(strSave+TString(".png"));
      canv->SaveAs(strSave+TString(".pdf"));      
 
  }//end of loop over ipt  
  
}// end of DrawTemplates

void TemplatesVsWMt_MC()
{

  TString fName[2]={"../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root","../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root"};
  TString strWlabel[2]={"Wg","Wjets"};

  TString varTemp[2]={"phoPFChIsoCorr","phoSigmaIEtaIEta"};

  TString strEta[2]={"phoSCEta<1.4442 && phoSCEta>-1.4442","(phoSCEta>-2.5 && phoSCEta<-1.566) || (phoSCEta<2.5 && phoSCEta>1.566)"};
  TString strEtaLabel[2]={"BARREL","ENDCAP"};

  TString strSb[2][2];
  strSb[0][0]="phoSigmaIEtaIEta<0.011";
  strSb[0][1]="phoPFChIsoCorr<1.5";
  strSb[1][0]="phoSigmaIEtaIEta<0.033";
  strSb[1][1]="phoPFChIsoCorr<1.2";

  for (int iW=0; iW<=1; iW++){
    TFile* f = new TFile(fName[iW]);  
    TTree* tr = (TTree*)f->Get("selectedEvents");
    for (int iVarTemp=0; iVarTemp<=1; iVarTemp++){
      for (int ieta=0; ieta<=1; ieta++){
        TString strCanvBase="cTemplatesVsWMt_"+strWlabel[iW]+"_"+varTemp[iVarTemp]+"_"+strEtaLabel[ieta];
        TCut cutEta(strEta[ieta]); TCut cutSb(strSb[ieta][iVarTemp]);
        TCut cut = cutEta && cutSb;
      	DrawTemplates(strCanvBase, tr, varTemp[iVarTemp], cut);
      }//end of loop over ieta
    }//end of loop over iVarTemp
  }//end of loop over iW

}// end of TemplatesVsWMt_MC

