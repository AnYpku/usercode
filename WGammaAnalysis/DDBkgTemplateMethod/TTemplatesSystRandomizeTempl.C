#include "TTemplatesSystRandomizeTempl.h"

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

TTemplatesSystRandomizeTempl::TTemplatesSystRandomizeTempl()
{
}

TTemplatesSystRandomizeTempl::TTemplatesSystRandomizeTempl(TemplatesPars pars)
{
  SetPars(pars);
  _pars.fOutForSave->Close();
  if (_pars.isMCclosureMode) _pars.strFileOutName.ReplaceAll(".root","_SystRand_MCclosure.root");
  else _pars.strFileOutName.ReplaceAll(".root","_SystRand.root");
  _pars.fOutForSave = new TFile(_pars.strFileOutName,"recreate");
//  _randPars=randPars;
}

TTemplatesSystRandomizeTempl::~TTemplatesSystRandomizeTempl()
{
  
}

void TTemplatesSystRandomizeTempl::RandomizeTempl()
{

  for (int ikin=1; ikin<=_pars.nKinBins; ikin++){
 //  for (int ikin=3; ikin<=3; ikin++){
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
//    for (int ieta=_ENDCAP; ieta<=_ENDCAP; ieta++){
      FitWithRandTemplatesKinEtaBin(ikin, ieta);
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
      float err = sqrt(_rmsTrue[ib][ieta]*_rmsTrue[ib][ieta]+_rmsFake[ib][ieta]*_rmsFake[ib][ieta]);
      h1DTrueYield[ieta]->SetBinError(ib,err);
      if (ib==1) continue;
      contTot+=cont;
      errTot+=err*err;
   }//end of loop over ib
   errTot=sqrt(errTot);
   hTotTrueYield[ieta]->SetBinContent(1,contTot);
   hTotTrueYield[ieta]->SetBinError(1,errTot);
   h1DTrueYield[ieta]->Write(str1D);
   hTotTrueYield[ieta]->Write(strTot);

  }//end of loop over ieta

  for (int ikin=1; ikin<=_pars.nKinBins; ikin++){
//   for (int ikin=7; ikin<=7; ikin++){
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
//      PlotOneKinOneEtaBin(ikin, ieta);
          std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<std::endl;
      std::cout<<"_nTrueYieldsValRef="<<_nTrueYieldsValRef[ikin][ieta]<<", _nFakeYieldsValRef="<<_nFakeYieldsValRef[ikin][ieta]<<std::endl;
      std::cout<<"Fake templ variation: "<<"_rms[ikin][ieta]="<<_rmsFake[ikin][ieta]<<", sumSuqares="<<_sumSquaresFake[ikin][ieta]<<", nValues="<<_nValuesFake[ikin][ieta]<<std::endl;
      std::cout<<"True templ variation: "<<"_rms="<<_rmsTrue[ikin][ieta]<<", sumSuqares="<<_sumSquaresTrue[ikin][ieta]<<", nValues="<<_nValuesTrue[ikin][ieta]<<std::endl;
    }//end of loop over ieta
  }// end of loop over ikin

}// end of RandomizeTempl()



void TTemplatesSystRandomizeTempl::PlotOneKinOneEtaBin(int ikin, int ieta)
{
    _pars.fOutForSave->cd();

    TString canvNameT=_pars.strPlotsBaseName+TString("_RandTempl_True_");
    canvNameT+=StrLabelKin(ikin);
    canvNameT+=StrLabelEta(ieta);
    TCanvas* cT = new TCanvas(canvNameT,canvNameT,600,600);
    _hYieldsTrue[ikin][ieta]->Draw();
    TString saveNameT=_pars.strPlotsDir+canvNameT;
    if (_pars.isMCclosureMode) saveNameT+="_MCclosure.png";
    else saveNameT+=".png";
    cT->SaveAs(saveNameT);
    saveNameT.ReplaceAll(".png",".pdf");
    cT->SaveAs(saveNameT);
    saveNameT.ReplaceAll(".pdf",".root");
    cT->SaveAs(saveNameT);

    TString canvNameF=_pars.strPlotsBaseName+TString("_RandTempl_Fake_");
    canvNameF+=StrLabelKin(ikin);
    canvNameF+=StrLabelEta(ieta);
    TCanvas* cF = new TCanvas(canvNameF,canvNameF,600,600);
    _hYieldsFake[ikin][ieta]->Draw();
    TString saveNameF=_pars.strPlotsDir+canvNameF;
    if (_pars.isMCclosureMode) saveNameF+="_MCclosure.png";
    else saveNameF+=".png";
    cF->SaveAs(saveNameF);
    saveNameT.ReplaceAll(".png",".pdf");
    cF->SaveAs(saveNameF);
    saveNameT.ReplaceAll(".pdf",".root");
    cF->SaveAs(saveNameF);

}// end of PlotOneKinOneEtaBin()

void TTemplatesSystRandomizeTempl::RandomizeTrueTemplates(int ikin, int ieta)
{
    std::cout<<"Randomize true templates"<<std::endl;
  for (int ib=1; ib<_hTrueReference[ikin][ieta]->GetNbinsX(); ib++){
    float mean = _hTrueReference[ikin][ieta]->GetBinContent(ib);
    float err1 = _hTrueReference[ikin][ieta]->GetBinError(ib);
//    float err2 = 0.2*_hLeakFakeToTrue[ikin][ieta]->GetBinContent(ib);
//    float sigma = sqrt(err1*err1+err2*err2);
    float newmean=_random.Gaus(mean,err1);
    if (newmean<0) newmean=0;
    _hTrue[ikin][ieta]->SetBinContent(ib,newmean);
    _hTrue[ikin][ieta]->SetBinError(ib,err1);
    _hFake[ikin][ieta]->SetBinContent(ib,_hFakeReference[ikin][ieta]->GetBinContent(ib));
    std::cout<<"ib="<<ib<<", mean="<<mean<<", err1="<<err1<<", newmean="<<newmean<<std::endl;
  }// end of loop over ib
}//end of RandomizeTrueTemplates

void TTemplatesSystRandomizeTempl::RandomizeFakeTemplates(int ikin, int ieta)
{
    std::cout<<"Randomize fake templates"<<std::endl;
  for (int ib=1; ib<_hFakeReference[ikin][ieta]->GetNbinsX(); ib++){
    float mean = _hFakeReference[ikin][ieta]->GetBinContent(ib);
    float err1 = _hFakeReference[ikin][ieta]->GetBinError(ib);
//    float err2 = 0.2*_hLeakTrueToFake[ikin][ieta]->GetBinContent(ib);
//    float sigma = sqrt(err1*err1+err2*err2);
    float newmean=_random.Gaus(mean,err1);
    if (newmean<0) newmean=0;
    _hFake[ikin][ieta]->SetBinContent(ib,newmean);
    _hFake[ikin][ieta]->SetBinError(ib,err1);
    _hTrue[ikin][ieta]->SetBinContent(ib,_hTrueReference[ikin][ieta]->GetBinContent(ib));
    std::cout<<"ib="<<ib<<", mean="<<mean<<", err1="<<err1<<", newmean="<<newmean<<std::endl;
  }// end of loop over ib
}//end of RandomizeFakeTemplates

void TTemplatesSystRandomizeTempl::FitWithRandTemplatesKinEtaBin(int ikin, int ieta)
{

  _pars.noRebinTemplates=0;

  bool isOk = ComputeBackgroundOne(ikin,ieta,0);
  _nTrueYieldsValRef[ikin][ieta]=_nTrueYieldsVal[ikin][ieta];
  _nFakeYieldsValRef[ikin][ieta]=_nFakeYieldsVal[ikin][ieta];
  TString hNameT=TString("hTrue_Syst")+StrLabelKin(ikin)+StrLabelEta(ieta);
  TString hNameF=TString("hFake_Syst")+StrLabelKin(ikin)+StrLabelEta(ieta);
  _hYieldsTrue[ikin][ieta] = new TH1F(hNameT,hNameT,50,0,3*_nTrueYieldsVal[ikin][ieta]);
  _hYieldsFake[ikin][ieta] = new TH1F(hNameF,hNameF,50,0,3*_nFakeYieldsVal[ikin][ieta]);

  _nValuesTrue[ikin][ieta]=0;
  _nValuesFake[ikin][ieta]=0;
  _sumSquaresTrue[ikin][ieta]=0;
  _sumSquaresFake[ikin][ieta]=0;
  _rmsTrue[ikin][ieta]=0;
  _rmsFake[ikin][ieta]=0;

  _pars.noRebinTemplates=1;

    // True gamma templates


  for (int ip=0; ip<nRandPointsTrue; ip++){
    std::cout<<"FITS WITH RANDOMIZED TRUE TEMPLATES, rand point = "<<ip<<std::endl;
    std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<", ";

    RandomizeTrueTemplates(ikin, ieta);

    if (_pars.isRooFit) FitOneRooFit(ikin, ieta);
    else FitOneROOT(ikin, ieta);
    ComputeYieldOneKinBin(ikin,ieta);

    if (_nTrueYieldsErr[ikin][ieta]>_nTrueYieldsVal[ikin][ieta] &&
        _nFakeYieldsErr[ikin][ieta]>_nFakeYieldsVal[ikin][ieta]) isOk=0;
    if (isOk){
      _hYieldsTrue[ikin][ieta]->Fill(_nTrueYieldsVal[ikin][ieta]);
      _nValuesTrue[ikin][ieta]++;
      float diff = (_nTrueYieldsVal[ikin][ieta]-_nTrueYieldsValRef[ikin][ieta]);
      _sumSquaresTrue[ikin][ieta]+=diff*diff;
    }

  }//end of loop over ip
    // Fake gamma templates

  for (int ip=0; ip<nRandPointsFake; ip++){
    std::cout<<"FITS WITH RANDOMIZED FAKE TEMPLATES, rand point = "<<ip<<std::endl;
    std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<", ";
    RandomizeFakeTemplates(ikin, ieta);

    if (_pars.isRooFit) FitOneRooFit(ikin, ieta);
    else FitOneROOT(ikin, ieta);
    ComputeYieldOneKinBin(ikin,ieta);

    if (_nTrueYieldsErr[ikin][ieta]>_nTrueYieldsVal[ikin][ieta] &&
        _nFakeYieldsErr[ikin][ieta]>_nFakeYieldsVal[ikin][ieta]) isOk=0;
    if (isOk){
      _hYieldsFake[ikin][ieta]->Fill(_nTrueYieldsVal[ikin][ieta]);
      _nValuesFake[ikin][ieta]++;
      float diff = (_nTrueYieldsVal[ikin][ieta]-_nTrueYieldsValRef[ikin][ieta]);
      _sumSquaresFake[ikin][ieta]+=diff*diff;
     }

  }//end of loop over ip

  if (_nValuesTrue[ikin][ieta]>0)
    _rmsTrue[ikin][ieta]=sqrt(_sumSquaresTrue[ikin][ieta]/_nValuesTrue[ikin][ieta]);
  if (_nValuesFake[ikin][ieta]>0)
    _rmsFake[ikin][ieta]=sqrt(_sumSquaresFake[ikin][ieta]/_nValuesFake[ikin][ieta]);
}// end of VarySidebandKinEtaBin(int ikin, int ieta)

/*
void TTemplatesSystRandomizeTempl::PrintOutKinEtaBin(int ikin, int ieta)
{

    std::cout<<std::endl;
    std::cout<<"PRINT TRUE AND FAKE YIELDS FOR GIVEN BIN"<<std::endl;
    std::cout<<"ikin="<<StrLabelKin(ikin);
    std::cout<<", ieta="<<StrLabelEta(ieta)<<std::endl;
    for (int isL=0; isL<_variationPars.nPointsLower[ieta]; isL++){
      for (int isU=0; isU<_variationPars.nPointsUpper[ieta]; isU++){
      std::cout<<std::setprecision(3)<<std::endl;
      std::cout<<"isL="<<isL<<", isU="<<isU<<", sideband="<<_sidebandLowerVal[isL]<<"-"<<_sidebandUpperVal[isU]<<std::endl;
      std::cout<<"true yield : ";
      std::cout<<std::setprecision(0)<<" "<<_yieldsTrueVal[isL][isU]<<"+-"<<_yieldsTrueErr[isL][isU]<<", ";
      std::cout<<"fake yield : ";
      std::cout<<std::setprecision(0)<<" "<<_yieldsFakeVal[isL][isU]<<"+-"<<_yieldsFakeErr[isL][isU];
      std::cout<<std::endl;
      }//end of loop over isU
    }//end of loop over isL

}// end of PrintOutKinEtaBin
*/
