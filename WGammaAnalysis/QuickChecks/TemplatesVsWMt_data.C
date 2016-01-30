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

void DrawTemplates(TString strCanvBase, TTree* tr[2], TString varTemp, TCut cutEta)
{

// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  // const int nPtBins = 2;
  //const double lims[nPtBins+1]={15,20,25};
  const int nPtBins = 1;
  const double lims[nPtBins+1]={10,500};

  TCut cutWeight="weight";

  TH1F* hists[2][nPtBins][2];

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
        hists[0][ipt][0] = new TH1F(hName+"_Zg",hName,nBins,low,up);
        hists[1][ipt][0] = new TH1F(hName+"_Zj",hName,nBins,low,up);
        TCut cutWMt="1";
        tr[0]->Draw(varTemp+TString(">>")+hists[0][ipt][0]->GetName(),(cut&&cutWMt)*cutWeight,"goff");
        tr[1]->Draw(varTemp+TString(">>")+hists[1][ipt][0]->GetName(),(cut&&cutWMt)*cutWeight,"goff");
	std::cout<<"cut="<<(cut&&cutWMt).GetTitle()<<", tr[ZgMC]->GetEntries(cut)="<<std::endl;
	std::cout<<tr[0]->GetEntries((cut&&cutWMt)*cutWeight)<<std::endl;
	std::cout<<"cut="<<(cut&&cutWMt).GetTitle()<<", tr[ZgData]->GetEntries(cut)="<<std::endl;
	std::cout<<tr[1]->GetEntries((cut&&cutWMt)*cutWeight)<<std::endl;
	hists[1][ipt][0]->Add(hists[0][ipt][0],-1);
        hists[1][ipt][0]->Print();
        hists[1][ipt][0]->SetLineWidth(2);
        hists[1][ipt][0]->SetLineColor(1);
	NormalizeHist(hists[1][ipt][0]);
        leg->AddEntry(hists[1][ipt][0],"no MET cut","l");

        hName="h1"+strCanvBase+strAffix;
        hists[0][ipt][1] = new TH1F(hName+"_Zg",hName,nBins,low,up);
        hists[1][ipt][1] = new TH1F(hName+"_Zj",hName,nBins,low,up);
        cutWMt="pfMET>10 && (TMath::Abs(phoPhi-pfMETPhi)<0.5)";
        tr[0]->Draw(varTemp+TString(">>")+hists[0][ipt][1]->GetName(),(cut&&cutWMt)*cutWeight,"goff");
        tr[1]->Draw(varTemp+TString(">>")+hists[1][ipt][1]->GetName(),(cut&&cutWMt)*cutWeight,"goff");
	std::cout<<"cut="<<(cut&&cutWMt).GetTitle()<<", tr[ZgMC]->GetEntries(cut)="<<std::endl;
	std::cout<<tr[0]->GetEntries((cut&&cutWMt)*cutWeight)<<std::endl;
	std::cout<<"cut="<<(cut&&cutWMt).GetTitle()<<", tr[ZgData]->GetEntries(cut)="<<std::endl;
	std::cout<<tr[1]->GetEntries((cut&&cutWMt)*cutWeight)<<std::endl;
	hists[1][ipt][1]->Add(hists[0][ipt][1],-1);
        hists[1][ipt][1]->Print();
        hists[1][ipt][1]->SetLineWidth(2);
        hists[1][ipt][1]->SetLineColor(2);
	NormalizeHist(hists[1][ipt][1]);
        leg->AddEntry(hists[1][ipt][1],"MET>10, #Delta #phi(MET,#gamma)<0.5","l");

      hists[1][ipt][0]->SetStats(0);
      if (varTemp=="phoPFChIsoCorr") hists[1][ipt][0]->GetXaxis()->SetTitle("I_{ch}, GeV");
      if (varTemp=="phoSigmaIEtaIEta") hists[1][ipt][0]->GetXaxis()->SetTitle("#sigma_{i #etai #eta}");
      hists[1][ipt][0]->Draw("EP");
      hists[1][ipt][1]->Draw("EP same");
      
      leg->Draw("same");
      
      TString strTitle=strCanvBase+strAffix;
      strTitle.ReplaceAll("cTemplatesVsWMt","");
      strTitle.ReplaceAll("phoSigmaIEtaIEta","");
      strTitle.ReplaceAll("phoPFChIsoCorr","");
      hists[1][ipt][0]->SetTitle(strTitle);

      TString strSave="../WGammaOutput";
      strSave+="/ChannelsMERGED_ZGamma/Plots/QuickChecks/";
      strSave+=canv->GetName();

      canv->SaveAs(strSave+TString(".png"));
      canv->SaveAs(strSave+TString(".pdf"));      
 
  }//end of loop over ipt  
  
}// end of DrawTemplates

void TemplatesVsWMt_data()
{

  TString fName[2]={"../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGammaSIGMC.root",
                    "../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root"};

  TString varTemp[2]={"phoPFChIsoCorr","phoSigmaIEtaIEta"};

  TString strEta[2]={"phoSCEta<1.4442 && phoSCEta>-1.4442","(phoSCEta>-2.5 && phoSCEta<-1.566) || (phoSCEta<2.5 && phoSCEta>1.566)"};
  TString strEtaLabel[2]={"BARREL","ENDCAP"};

  TString strSb[2][2];
  strSb[0][0]="phoSigmaIEtaIEta<0.011";
  strSb[0][1]="phoPFChIsoCorr<1.5";
  strSb[1][0]="phoSigmaIEtaIEta<0.033";
  strSb[1][1]="phoPFChIsoCorr<1.2";

  TTree* tr[2];

  for (int iVarTemp=0; iVarTemp<=1; iVarTemp++){
    for (int ieta=0; ieta<=1; ieta++){
      for (int iZ=0; iZ<=1; iZ++){
        TFile* f = new TFile(fName[iZ]);  
        tr[iZ] = (TTree*)f->Get("selectedEvents");
      }//end of loop over iZ
      TString strCanvBase="cTemplatesVsWMt_ZjetsISR_"+varTemp[iVarTemp]+"_"+strEtaLabel[ieta];
      TCut cutEta(strEta[ieta]); TCut cutSb(strSb[ieta][iVarTemp]);
      TCut cut = cutEta && cutSb;
      DrawTemplates(strCanvBase, tr, varTemp[iVarTemp], cut);
    }//end of loop over ieta
  }//end of loop over iVarTemp

}// end of TemplatesVsWMt_data()

