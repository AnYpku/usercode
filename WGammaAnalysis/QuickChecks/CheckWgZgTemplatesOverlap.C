#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include <math.h>

TCut CutPt(float lowPt, float upPt)
{
  TString strPt = "phoEt>";
  strPt+=lowPt;
  strPt+= "&& phoEt<=";
  strPt+=upPt;
  TCut cut(strPt);
  return cut;  
}// end of CutPt

void CompareTwoSelectedTrees(TTree* tr1, TTree* tr2)
{

  const int nPtBins = 7;
  const double lims[nPtBins+1]={15,20,25,30,35,45,55,500};

  int ev1, ev2, ipho1, ipho2, ilep11, ilep12;
  float phoEt1, phoEt2, phoEta1, phoEta2, phoSigmaIEtaIEta1, phoSigmaIEtaIEta2;
  TBranch *b_ev1, *b_ev2, *b_ipho1, *b_ipho2, *b_ilep11, *b_ilep12;
  TBranch *b_phoEt1, *b_phoEt2, *b_phoEta1, *b_phoEta2, *b_phoSigmaIEtaIEta1, *b_phoSigmaIEtaIEta2;

  tr1->SetBranchAddress("event", &ev1, &b_ev1);
  tr1->SetBranchAddress("ipho", &ipho1, &b_ipho1);
  tr1->SetBranchAddress("ilep1", &ilep11, &b_ilep11);

  tr1->SetBranchAddress("phoEt", &phoEt1, &b_phoEt1);
  tr1->SetBranchAddress("phoEta", &phoEta1, &b_phoEta1);
  tr1->SetBranchAddress("phoSigmaIEtaIEta", &phoSigmaIEtaIEta1, &b_phoSigmaIEtaIEta1);

  tr2->SetBranchAddress("event", &ev2, &b_ev2);
  tr2->SetBranchAddress("ipho", &ipho2, &b_ipho2);
  tr2->SetBranchAddress("ilep1", &ilep12, &b_ilep12);

  tr2->SetBranchAddress("phoEt", &phoEt2, &b_phoEt2);
  tr2->SetBranchAddress("phoEta", &phoEta2, &b_phoEta2);
  tr2->SetBranchAddress("phoSigmaIEtaIEta", &phoSigmaIEtaIEta2, &b_phoSigmaIEtaIEta2);

  long nentries1 = tr1->GetEntries();
  long nentries2 = tr2->GetEntries();

  std::cout<<"nentries1="<<nentries1<<", nentries2="<<nentries2<<std::endl;

  nentries1=10000;
  nentries2=10000;

  std::cout<<"nentries1="<<nentries1<<", nentries2="<<nentries2<<std::endl;

  int n1[nPtBins][2];
  int n2[nPtBins][2];
  int nOverlap[nPtBins][2];

  for (int ipt=0; ipt<nPtBins; ipt++){
    n1[ipt][0]=0;
    n1[ipt][1]=0;
    n2[ipt][0]=0;
    n2[ipt][1]=0;
    nOverlap[ipt][0]=0;
    nOverlap[ipt][1]=0;
  }//end of loop over ipt

  for (long ient1=0; ient1<nentries1; ient1++){

    b_phoEta1->GetEntry(ient1);
    b_phoSigmaIEtaIEta1->GetEntry(ient1);
    
    int etaBin1; int ptBin1;

    if (fabs(phoEta1)<1.4442 && phoSigmaIEtaIEta1>0.011 && phoSigmaIEtaIEta1<0.014)
      etaBin1=0;
    else if (fabs(phoEta1)<2.5 && fabs(phoEta1)>1.566 && phoSigmaIEtaIEta1>0.033 && phoSigmaIEtaIEta1<0.044)
      etaBin1=1;
    else continue;

    b_phoEt1->GetEntry(ient1);
    if (phoEt1<15 || phoEt1>500) continue;
    for (int ipt=0; ipt<nPtBins; ipt++){
      if ( phoEt1>lims[ipt] && phoEt1<lims[ipt+1] ) 
        {ptBin1=ipt; continue;}     
    }//end of loop over ipt

    n1[ptBin1][etaBin1]++;

    b_ev1->GetEntry(ient1);
    b_ipho1->GetEntry(ient1);
    b_ilep11->GetEntry(ient1);   

    for (long ient2=0; ient2<nentries2; ient2++){

      int etaBin2; int ptBin2;

//      if (ient1==0){
        b_phoEta2->GetEntry(ient2);
        b_phoSigmaIEtaIEta2->GetEntry(ient2);

        if (fabs(phoEta2)<1.4442 && phoSigmaIEtaIEta2>0.011 && phoSigmaIEtaIEta2<0.014)
          etaBin2=0;
        else if (fabs(phoEta2)<2.5 && fabs(phoEta2)>1.566 && phoSigmaIEtaIEta2>0.033 && phoSigmaIEtaIEta2<0.044)
          etaBin2=1;
        else continue;

        b_phoEt2->GetEntry(ient2);
        if (phoEt2<15 || phoEt2>500) continue;
        for (int ipt=0; ipt<nPtBins; ipt++){
          if ( phoEt2>lims[ipt] && phoEt2<lims[ipt+1] ) 
            {ptBin2=ipt; continue;}     
        }//end of loop over ipt

        if (ient1==0) n2[ptBin2][etaBin2]++;
//      }// end of if (ieta1==0)

      b_ev2->GetEntry(ient2);
      b_ipho2->GetEntry(ient2);
      b_ilep12->GetEntry(ient2);

      if (ev1==ev2 && ipho1==ipho2 && ilep11==ilep12) { 
        b_phoEt2->GetEntry(ient2);
        b_phoEta2->GetEntry(ient2);  
        if (fabs(phoEta1-phoEta2)<0.01 || fabs(phoEt1-phoEt2)<0.01 || ptBin1!=ptBin2 || etaBin1!=etaBin2)
          {std::cout<<"ev="<<ev1<<", phoEta1="<<phoEta1<<", phoEta2=";
           std::cout<<phoEta2<<", phoEt1="<<phoEt1<<", phoEt2="<<phoEt2;
           std::cout<<", ptBin1="<<ptBin1<<", ptBin2="<<ptBin2<<", etaBin1="<<etaBin1<<", etaBin2="<<etaBin2<<std::endl;}
        else
          {nOverlap[ptBin1][etaBin1]++;} 
      }

    }//end of loop over ient2

  }//end of loop over ient1

  for (int ipt=0; ipt<nPtBins; ipt++){
    for (int ieta=0; ieta<=1; ieta++){
      std::cout<<"pt "<<lims[ipt]<<"-"<<lims[ipt+1]<<", ";
      std::cout<<"ieta="<<ieta<<", ";
      std::cout<<"n1="<<n1[ipt][ieta]<<", n2="<<n2[ipt][ieta]<<", nOverlap="<<nOverlap[ipt][ieta];
      std::cout<<std::endl;
    }
  }//end of loop over ipt

}//end of compare two selected trees


void CheckWgZgTemplatesOverlap()
{
  TFile fWgMuo("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  TFile fZgMuo("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root");
  TFile fWgEle("../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  TFile fZgEle("../WGammaOutput/ELECTRON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root");

  TTree* trWgMuo = (TTree*)fWgMuo.Get("selectedEvents");
  TTree* trZgMuo = (TTree*)fZgMuo.Get("selectedEvents");
  TTree* trWgEle = (TTree*)fWgEle.Get("selectedEvents");
  TTree* trZgEle = (TTree*)fZgEle.Get("selectedEvents");

  TCut cutSihihTrue[2];
  TCut cutSihihFake[2];
  cutSihihTrue[0]="phoSigmaIEtaIEta<0.011";
  cutSihihTrue[1]="phoSigmaIEtaIEta<0.033";
  cutSihihFake[0]="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.014";
  cutSihihFake[1]="phoSigmaIEtaIEta>0.033 && phoSigmaIEtaIEta<0.044";


  TCut cutEta[2];
  cutEta[0]="phoSCEta>-1.4442 && phoSCEta<1.4442";
  cutEta[1]="(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)";

  std::cout<<"MUON: Wg, Zg, overlap"<<std::endl;
  CompareTwoSelectedTrees(trWgMuo, trZgMuo);
  std::cout<<"ELECTRON: Wg, Zg, overlap"<<std::endl;
  CompareTwoSelectedTrees(trWgEle, trZgEle);



}// end of CheckMuonVsElectronTemplates

