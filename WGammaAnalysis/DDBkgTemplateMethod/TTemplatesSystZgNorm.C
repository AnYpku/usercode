#include "TTemplatesSystZgNorm.h"

#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "TVectorD.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TPaletteAxis.h"

#include <iostream>
#include <iomanip>

TTemplatesSystZgNorm::TTemplatesSystZgNorm()
{
}

TTemplatesSystZgNorm::TTemplatesSystZgNorm(TemplatesPars pars)
{
  SetPars(pars);
  _pars.fOutForSave->Close();
  if (_pars.isMCclosureMode) _pars.strFileOutName.ReplaceAll(".root","_SystZgNorm_MCclosure.root");
  else _pars.strFileOutName.ReplaceAll(".root","_SystZgNorm.root");
  _pars.fOutForSave = new TFile(_pars.strFileOutName,"recreate");
//  _randPars=randPars;
}

TTemplatesSystZgNorm::~TTemplatesSystZgNorm()
{
  
}

void TTemplatesSystZgNorm::SystDueToZgNorm()
{

  for (int ikin=1; ikin<=_pars.nKinBins; ikin++){
 //  for (int ikin=1; ikin<=1; ikin++){
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
//    for (int ieta=_BARREL; ieta<=_BARREL; ieta++){
      FitWithDifferentNorms(ikin, ieta);
      TString strCanv="c_"; strCanv+=ikin; strCanv+="_"; strCanv+=ieta;
      TCanvas* c = new TCanvas(strCanv,strCanv);
      _hFakeDiff[ikin][ieta][2]->SetLineWidth(2);
      _hFakeDiff[ikin][ieta][2]->SetLineColor(4);
      _hFakeDiff[ikin][ieta][2]->Draw("EP");
      _hFakeDiff[ikin][ieta][0]->SetLineWidth(2);
      _hFakeDiff[ikin][ieta][0]->SetLineColor(1);
      _hFakeDiff[ikin][ieta][0]->Draw("EP same");
      _hFakeDiff[ikin][ieta][1]->SetLineWidth(2);
      _hFakeDiff[ikin][ieta][1]->SetLineColor(2);
      _hFakeDiff[ikin][ieta][1]->Draw("EP same");
    }//end of loop over ieta
  }// end of loop over ikin

  _pars.fOutForSave->cd();
  TH1F* hTotTrueYield[3];
  TH1F* h1DTrueYield[3];
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){

    TString strTot=_pars.strTrueYieldsTot[ieta];

    TString str1D=_pars.strTrueYields1D[ieta];
    
    if (ieta==_COMMON){
      h1DTrueYield[_COMMON] = (TH1F*)h1DTrueYield[_BARREL]->Clone(str1D); h1DTrueYield[_COMMON]->SetTitle(str1D);
      h1DTrueYield[_COMMON]->Add(h1DTrueYield[_ENDCAP]);
      hTotTrueYield[_COMMON] = (TH1F*)hTotTrueYield[_BARREL]->Clone(strTot); hTotTrueYield[_COMMON]->SetTitle(strTot);
      hTotTrueYield[_COMMON]->Add(hTotTrueYield[_ENDCAP]);
      h1DTrueYield[_COMMON]->Write(str1D);
      hTotTrueYield[_COMMON]->Write(strTot);
      continue; 
    }// end of if (ieta==_COMMON)

   hTotTrueYield[ieta] = new TH1F(strTot,strTot,1,_pars.kinBinLims[0],_pars.kinBinLims[_pars.nKinBins]);
   h1DTrueYield[ieta] = new TH1F(str1D,str1D,_pars.nKinBins,_pars.kinBinLims);

   float contTot=0;
   float errTot=0;

    
 
   for (int ib=1; ib<_pars.nKinBins+1; ib++){  
      float cont = _nTrueYieldsValRef[ib][ieta];
      h1DTrueYield[ieta]->SetBinContent(ib,cont);
      float err = 0.0; 
      for (int i=0; i<5; i++){
       for (int j=0; j<i; j++){
         float diff = fabs(_nTrueYieldsValDiff[ib][ieta][i]-_nTrueYieldsValDiff[ib][ieta][j]);
         if (err<diff) err=diff;
       }// end of loop over j
      }// end of loop over i
      std::cout<<StrLabelEta(ieta)<<" "<<StrLabelKin(ib)<<std::endl;
      std::cout<<"nTrue = ";
      for (int i=0; i<5; i++){
        std::cout<<_nTrueYieldsValDiff[ib][ieta][i]<<" ";
      }// end of loop over i
      std::cout<<"err = "<<err;
      std::cout<<std::endl;

      h1DTrueYield[ieta]->SetBinError(ib,err);
      if (ib==1) continue;
      contTot+=cont;
      //errTot+=err*err;
   }//end of loop over ib
   //errTot=sqrt(errTot);
   //hTotTrueYield[ieta]->SetBinContent(1,contTot);
   //hTotTrueYield[ieta]->SetBinError(1,errTot);
   h1DTrueYield[ieta]->Write(str1D);
   //hTotTrueYield[ieta]->Write(strTot);


   float nTot[5];
   for (int i=0; i<5; i++){
     for (int ib=1; ib<_pars.nKinBins+1; ib++){ 
       nTot[i]+=_nTrueYieldsValDiff[ib][ieta][i];
     }//end of for (int ib=1; ib<_pars.nKinBins+1; ib++)
     for (int j=0; j<i; j++){ 
       float diff = fabs(nTot[i]-nTot[j]);
       if (errTot<diff) errTot=diff;
     }//(int j=0; j<5; j++)
   }//end of for (int i=0; i<5; i++)
     std::cout<<StrLabelEta(ieta)<<" TOTAL PT"<<std::endl;
    
   hTotTrueYield[ieta]->SetBinContent(1,contTot);
   hTotTrueYield[ieta]->SetBinError(1,errTot);
   hTotTrueYield[ieta]->Write(strTot);

  }//end of loop over ieta

}// end of SystDueToZgNorm()

void TTemplatesSystZgNorm::FitWithDifferentNorms(int ikin, int ieta)
{

  _pars.noRebinTemplates=0;

  std::cout<<"WILL DO FIT: NOMINAL"<<std::endl;

  bool isOk = ComputeBackgroundOne(ikin,ieta,0);
  TString strFakeDiff="hFakeDiff"; strFakeDiff+="_"; strFakeDiff+=ikin; strFakeDiff+="_"; strFakeDiff+=ieta;
  _hFakeDiff[ikin][ieta][0]=(TH1D*)_hFake[ikin][ieta]->Clone(strFakeDiff+"0");

  std::cout<<"DONE FIT: NOMINAL"<<std::endl;
  _nTrueYieldsValRef[ikin][ieta]=_nTrueYieldsVal[ikin][ieta];
  _nFakeYieldsValRef[ikin][ieta]=_nFakeYieldsVal[ikin][ieta];

  _nTrueYieldsValDiff[ikin][ieta][0]=_nTrueYieldsVal[ikin][ieta];

  _pars.noRebinTemplates=1;

  std::cout<<"_hFake[ikin][ieta]:"<<std::endl;
  _hFake[ikin][ieta]->Print();
  ChangeTemplatesDiffNorm(_hFake[ikin][ieta],_hLeakTrueToFake[ikin][ieta], 1);
  std::cout<<"_hFake[ikin][ieta]:"<<std::endl;
  _hFake[ikin][ieta]->Print();
  std::cout<<"WILL DO FIT: Zg +20%"<<std::endl;

  if (_pars.isRooFit) FitOneRooFit(ikin, ieta, 0);
  else FitOneROOT(ikin, ieta, 0);
  ComputeYieldOneKinBin(ikin,ieta,0);
  _hFakeDiff[ikin][ieta][1]=(TH1D*)_hFake[ikin][ieta]->Clone(strFakeDiff+"1");
  ChangeTemplatesDiffNorm(_hFake[ikin][ieta],_hLeakTrueToFake[ikin][ieta], -1);

  std::cout<<"DONE FIT: Zg +20%"<<std::endl;
  _nTrueYieldsValDiff[ikin][ieta][1]=_nTrueYieldsVal[ikin][ieta];
  ChangeTemplatesDiffNorm(_hFake[ikin][ieta],_hLeakTrueToFake[ikin][ieta], -1);
  std::cout<<"WILL DO FIT: Zg -20%"<<std::endl;

  if (_pars.isRooFit) FitOneRooFit(ikin, ieta, 0);
  else FitOneROOT(ikin, ieta, 0);
  ComputeYieldOneKinBin(ikin,ieta,0);
  _hFakeDiff[ikin][ieta][2]=(TH1D*)_hFake[ikin][ieta]->Clone(strFakeDiff+"2");
  ChangeTemplatesDiffNorm(_hFake[ikin][ieta],_hLeakTrueToFake[ikin][ieta], 1);

  std::cout<<"DONE FIT: Zg -20%"<<std::endl;
  _nTrueYieldsValDiff[ikin][ieta][2]=_nTrueYieldsVal[ikin][ieta];

  ChangeTemplatesDiffNorm(_hTrue[ikin][ieta],_hLeakFakeToTrue[ikin][ieta], 1);
  std::cout<<"WILL DO FIT: DYjets +20%"<<std::endl;

  if (_pars.isRooFit) FitOneRooFit(ikin, ieta, 0);
  else FitOneROOT(ikin, ieta, 0);
  ComputeYieldOneKinBin(ikin,ieta,0);
  ChangeTemplatesDiffNorm(_hTrue[ikin][ieta],_hLeakFakeToTrue[ikin][ieta], -1);

  std::cout<<"DONE FIT: DYjets +20%"<<std::endl;
  _nTrueYieldsValDiff[ikin][ieta][3]=_nTrueYieldsVal[ikin][ieta];

  ChangeTemplatesDiffNorm(_hTrue[ikin][ieta],_hLeakFakeToTrue[ikin][ieta], -1);
  std::cout<<"WILL DO FIT: DYjets -20%"<<std::endl;

  if (_pars.isRooFit) FitOneRooFit(ikin, ieta, 0);
  else FitOneROOT(ikin, ieta, 0);
  ComputeYieldOneKinBin(ikin,ieta,0);
  ChangeTemplatesDiffNorm(_hTrue[ikin][ieta],_hLeakFakeToTrue[ikin][ieta], 1);

  std::cout<<"DONE FIT: DYjets -20%"<<std::endl;
  _nTrueYieldsValDiff[ikin][ieta][4]=_nTrueYieldsVal[ikin][ieta];

}// end of FitWithDifferentNorms(int ikin, int ieta)

void TTemplatesSystZgNorm::ChangeTemplatesDiffNorm(TH1D* hTemp, TH1D* hLeak, int k)
// k = 0, +1, -1
{
  int nBins=hTemp->GetNbinsX();
  std::cout<<"ChangeTemplatesDiffNorm:"<<std::endl;
  std::cout<<"hTemp:"<<std::endl;
  hTemp->Print();
  for (int ib=1; ib<=nBins; ib++){
    float frac = 0.046*k*hLeak->GetBinContent(ib);
    //float frac = 0.2*k*hLeak->GetBinContent(ib);
    float cont = hTemp->GetBinContent(ib);
    hTemp->SetBinContent(ib,cont-frac);
    std::cout<<"ib="<<ib<<", cont="<<cont<<", frac="<<frac<<", leak="<<hLeak->GetBinContent(ib)<<"="<<int(100*hLeak->GetBinContent(ib)/(cont+hLeak->GetBinContent(ib)))<<"%"<<std::endl;
  }//end of loop over ib
  std::cout<<"hTemp:"<<std::endl;
  hTemp->Print();
}//end of ChangeTemplatesDiffNorm
