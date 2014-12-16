#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <iostream>

bool IsCharDigit(char c)
{
  if (c=='0' || c=='1' || c=='2' || c=='3' || 
      c=='4' || c=='5' || c=='6' || c=='7' || 
      c=='8' || c=='9')
    return 1;
  return 0;
}

void OneVarInBinsOfTheOther(TString canvName, TString axName, TTree* tr, TString var1, TCut cutNotWeight, TCut cutWeight, TString var2, int nBinsVar2, float* binLimsVar2, int nBinsHist, float histMin, float histMax)
{
  if (nBinsVar2>9) {
    std::cout<<"nBinsVar2="<<nBinsVar2<<", but can't exceed 9; if you want to check more than 9 bins, split into two or more plots"<<std::endl;
    return;
  }
  TH1F* hist[nBinsVar2]; 
  TCanvas* canv = new TCanvas(canvName,canvName);
  TLegend* leg = new TLegend(0.55,0.65,1.00,1.00);
  float max=0;
  for (int ib=0; ib<nBinsVar2; ib++){
    TString strCutBin = var2+TString(">=");
    strCutBin+=binLimsVar2[ib];
    strCutBin+=TString(" && ")+var2+TString("<=");
    strCutBin+=binLimsVar2[ib+1];
    TCut cutBin(strCutBin);
    TString histName = TString("h")+var1;
    histName+=ib;
    hist[ib] = new TH1F(histName,histName,nBinsHist,histMin,histMax);
    tr->Draw(var1+TString(">>")+histName,(cutNotWeight+cutBin)*cutWeight,"goff");
    hist[ib]->SetLineWidth(2);
    //hist[ib]->SetLineStyle(ib+1);
    //hist[ib]->SetLineStyle(1);
    hist[ib]->SetLineColor(ib+1);
    float sum = 0;
    for (int ibh=1; ibh<=hist[ib]->GetNbinsX();ibh++)
      sum+=hist[ib]->GetBinContent(ibh);

    for (int ibh=1; ibh<=hist[ib]->GetNbinsX();ibh++){
      float cont=hist[ib]->GetBinContent(ibh);
      float err=hist[ib]->GetBinError(ibh);
      hist[ib]->SetBinContent(ibh,1.0*cont/sum);
      hist[ib]->SetBinError(ibh,1.0*err/sum);
    }
    if (max<hist[ib]->GetMaximum()) max=hist[ib]->GetMaximum();
    TString name=cutBin.GetTitle();
    int indDot=name.Index(".");
    int length=name.Length();
    int dAfterDot=0;
    for (int ind=1; ind<length; ind++){
      if (name(ind-1)=='.') dAfterDot=1;
      else if (dAfterDot && IsCharDigit(name(ind))){
        dAfterDot++;
        if (dAfterDot>3){ 
        name.Replace(ind,1,"");
        ind=ind-1;
        }
      }
      else dAfterDot=0;
    }
    name.ReplaceAll("Corr","");
    name.ReplaceAll("pho","");    
    leg->AddEntry(hist[ib],name,"l");
  }

  for (int ib=0; ib<nBinsVar2; ib++){
    if (ib==0){
      hist[ib]->SetStats(0);
      hist[ib]->SetTitle(canvName);
      hist[ib]->GetXaxis()->SetTitle(axName);
      hist[ib]->GetYaxis()->SetRangeUser(0,1.1*max);
      hist[ib]->Draw("EP");
    }
    else hist[ib]->Draw("EP same");
  }
  leg->Draw("same");
  canv->SaveAs(canvName+TString(".png"));
}
