#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TH1F.h"
#include "TCanvas.h"

#include <iostream>

void OneVarInBinsOfTheOther(TTree* tr, TString var1, TCut cutNotWeight, TCut cutWeight, TString var2, int nBinsVar2, float* binLimsVar2, int nBinsHist, float histMin, float histMax)
{
  if (nBinsVar2>9) {
    std::cout<<"nBinsVar2="<<nBinsVar2<<", but can't exceed 9; if you want to check more than 9 bins, split into two or more plots"<<std::endl;
    return;
  }
  TH1F* hist[nBinsVar2]; 
  TCanvas* canv = new TCanvas(var1,var1);
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
    hist[ib]->SetLineStyle(ib+1);
    hist[ib]->SetLineColor(ib+1);
    if (ib==0) hist[ib]->DrawNormalized();
    else hist[ib]->DrawNormalized("same");
  }
}
