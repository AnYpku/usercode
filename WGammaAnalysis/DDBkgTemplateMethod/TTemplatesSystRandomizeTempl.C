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

  //for (int ikin=1; ikin<=_pars.nKinBins; ikin++){
   for (int ikin=7; ikin<=7; ikin++){
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
      FitWithRandTemplatesKinEtaBin(ikin, ieta);
    }//end of loop over ieta
  }// end of loop over ikin

//  for (int ikin=1; ikin<=_pars.nKinBins; ikin++){
   for (int ikin=7; ikin<=7; ikin++){
    for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
      PlotOneKinOneEtaBin(ikin, ieta);
      std::cout<<"True: _nTrueYieldsVal="<<_nTrueYieldsVal[ikin][ieta]<<", _rms="<<_rmsTrue[ikin][ieta]<<", sumSuqares="<<_sumSquaresTrue[ikin][ieta]<<", nValues="<<_nValuesTrue[ikin][ieta]<<std::endl;
      std::cout<<"Fake: _nFakeYieldsVal="<<_nFakeYieldsVal[ikin][ieta]<<", _rms[ikin][ieta]="<<_rmsFake[ikin][ieta]<<", sumSuqares="<<_sumSquaresFake[ikin][ieta]<<", nValues="<<_nValuesFake[ikin][ieta]<<std::endl;
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
    float sigma = _hTrueReference[ikin][ieta]->GetBinError(ib);
    float newmean=_random.Gaus(mean,sigma);
    _hTrue[ikin][ieta]->SetBinContent(ib,newmean);
    _hTrue[ikin][ieta]->SetBinError(ib,sigma);
    _hFake[ikin][ieta]->SetBinContent(ib,_hFakeReference[ikin][ieta]->GetBinContent(ib));
    std::cout<<"ib="<<ib<<", mean="<<mean<<", sigma="<<sigma<<", newmean="<<newmean<<std::endl;
  }// end of loop over ib
}//end of RandomizeTrueTemplates

void TTemplatesSystRandomizeTempl::RandomizeFakeTemplates(int ikin, int ieta)
{
    std::cout<<"Randomize fake templates"<<std::endl;
  for (int ib=1; ib<_hFakeReference[ikin][ieta]->GetNbinsX(); ib++){
    float mean = _hFakeReference[ikin][ieta]->GetBinContent(ib);
    float sigma = _hFakeReference[ikin][ieta]->GetBinError(ib);
    float newmean=_random.Gaus(mean,sigma);
    _hFake[ikin][ieta]->SetBinContent(ib,newmean);
    _hFake[ikin][ieta]->SetBinError(ib,sigma);
    _hTrue[ikin][ieta]->SetBinContent(ib,_hTrueReference[ikin][ieta]->GetBinContent(ib));
    std::cout<<"ib="<<ib<<", mean="<<mean<<", sigma="<<sigma<<", newmean="<<newmean<<std::endl;
  }// end of loop over ib
}//end of RandomizeFakeTemplates

void TTemplatesSystRandomizeTempl::FitWithRandTemplatesKinEtaBin(int ikin, int ieta)
{
  bool isOk = ComputeBackgroundOne(ikin,ieta,0);
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

  for (int ip=0; ip<nRandPointsMax; ip++){
    std::cout<<"FITS WITH RANDOMIZED TEMPLATES, rand point = "<<ip<<std::endl;
    std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<", ";

    // True gamma templates
    RandomizeTrueTemplates(ikin, ieta);
    bool isOk = ComputeBackgroundOne(ikin,ieta,0);
    if (_nTrueYieldsErr[ikin][ieta]>0.5*_nTrueYieldsVal[ikin][ieta]) isOk=0;
    if (isOk){
      _hYieldsTrue[ikin][ieta]->Fill(_nTrueYieldsVal[ikin][ieta]);
      _nValuesTrue[ikin][ieta]++;
      _sumSquaresTrue[ikin][ieta]+=_nTrueYieldsVal[ikin][ieta]*_nTrueYieldsVal[ikin][ieta];
    }

    // Fake gamma templates
    RandomizeFakeTemplates(ikin, ieta);
    isOk = ComputeBackgroundOne(ikin,ieta,0);
    if (_nFakeYieldsErr[ikin][ieta]>0.5*_nFakeYieldsVal[ikin][ieta]) isOk=0;
    if (isOk){
      _hYieldsFake[ikin][ieta]->Fill(_nFakeYieldsVal[ikin][ieta]);
      _nValuesFake[ikin][ieta]++;
      _sumSquaresFake[ikin][ieta]+=_nFakeYieldsVal[ikin][ieta]*_nFakeYieldsVal[ikin][ieta];
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
