#include "TEtoGamma.h"
  //this class

#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TMathTools.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVectorD.h"
#include "TLine.h"
#include "TLatex.h"

TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars)
{

  _cutAdd=anPars.cutAdd;
  _varKin=anPars.varKin;
  _fOut=new TFile(_conf.GetDDBkgEtoGammaFileName(anPars.varKin),"recreate");
  _nKinBins=anPars.nKinBins;
  std::cout<<"_fOut="<<_conf.GetDDBkgEtoGammaFileName(anPars.varKin)<<std::endl;
  std::cout<<"_varKin="<<_varKin<<std::endl;
  std::cout<<"_nKinBins="<<_nKinBins<<", _kinBinLims: ";
  for (int ik=0; ik<=_nKinBins; ik++){
    _kinBinLims[ik]=anPars.kinBinLims[ik];
    std::cout<<_kinBinLims[ik];
    if (ik<_nKinBins) std::cout<<", ";
  }// end of loop over ik
  std::cout<<std::endl;



}// end of   TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars) 

TEtoGamma::~TEtoGamma()
{
}

void TEtoGamma::ComputePlotSave()
{
  SetHists();
  ComputeBkg();
  WriteToFile();
}// end of TEtoGamma::EstimateBkg()

TTree* TEtoGamma::GetTree(TString strFileName){
  TFile* f = new TFile(strFileName);
  TTree* tr=(TTree*)f->Get("selectedEvents");
}// end of GetTree

void TEtoGamma::SetHists()
{
  _fOut->cd();
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){

    TString strName;

    // data sample enriched by e->gamma:
    strName=TString("yieldDataEnriched_")+_conf.StrEtaBin(ieta);
    _yieldDataEnr[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _yieldDataEnr[ieta]->Sumw2();

    // DYjets MC sample enriched by e->gamma:
    strName=TString("yieldDYjetsEnriched_")+_conf.StrEtaBin(ieta);
    _yieldDYjetsEnr[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _yieldDYjetsEnr[ieta]->Sumw2();

    // data/DYjetsMC ratio:
    strName=TString("hRatio_")+_conf.StrEtaBin(ieta);
    _hRatio[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _hRatio[ieta]->Sumw2();

    // DYjets MC sample in nominal selection conditions:
    strName=TString("yieldDYjets_")+_conf.StrEtaBin(ieta);
    _yieldDYjets[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _yieldDYjets[ieta]->Sumw2();

    // data driven e->gamma estimate as to be used for background subtraction:
    strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _yieldDDEtoGamma[ieta]->Sumw2();

  }//end of loop over ieta
}// end of SetHists

void TEtoGamma::ComputeBkg()
{



  std::cout<<"ComputeBkg"<<std::endl;
   TString strDataEnr=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
   TString strDYjetsEnr=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
   TString strDYjetsNom=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");

   TTree* trDataEnr = GetTree(strDataEnr); 
   std::cout<<"trDataEnr from file "<<strDataEnr<<std::endl;
   TTree* trDYjetsEnr = GetTree(strDYjetsEnr); 
   std::cout<<"trDYjetsEnr from file "<<strDYjetsEnr<<std::endl;
   TTree* trDYjetsNom = GetTree(strDYjetsNom); 
   std::cout<<"trDYjetsNom from file "<<strDYjetsNom<<std::endl;

   TCut cutW="weight";
   TCut cutEta;

   for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
      _fOut->cd();
      TPhotonCuts photon;
      if (ieta==_conf.BARREL) cutEta=photon.RangeBarrel();
      if (ieta==_conf.ENDCAP) cutEta=photon.RangeEndcap();

      trDataEnr->Draw(_varKin+TString(">>")+_yieldDataEnr[ieta]->GetName(),(cutEta && _cutAdd)*cutW,"goff");
      trDYjetsEnr->Draw(_varKin+TString(">>")+_yieldDYjetsEnr[ieta]->GetName(),(cutEta && _cutAdd)*cutW,"goff");
      trDYjetsNom->Draw(_varKin+TString(">>")+_yieldDYjets[ieta]->GetName(),(cutEta && _cutAdd)*cutW,"goff");

      std::cout<<"ib contDYjetsNom contDataEnr contDYjetsEnr cont"<<std::endl;
      for (int ib=1; ib<=_yieldDataEnr[ieta]->GetNbinsX(); ib++){
         float contDataEnr=_yieldDataEnr[ieta]->GetBinContent(ib);
         float errDataEnr=_yieldDataEnr[ieta]->GetBinError(ib);
         float contDYjetsEnr=_yieldDYjetsEnr[ieta]->GetBinContent(ib);
         float errDYjetsEnr=_yieldDYjetsEnr[ieta]->GetBinError(ib);
         float contDYjetsNom=_yieldDYjets[ieta]->GetBinContent(ib);
         float errDYjetsNom=_yieldDYjets[ieta]->GetBinError(ib);
         float cont, err;
         if (contDYjetsEnr==0) {cont=0; err=0.1;}
         else{
           cont = contDYjetsNom*contDataEnr/contDYjetsEnr;
           TMathTools math;
           err=math.ErrOfThreeIndependent("(x1*x2)/x3",contDYjetsNom,contDataEnr,contDYjetsEnr,errDYjetsNom,errDataEnr,errDYjetsEnr);
         }
         _yieldDDEtoGamma[ieta]->SetBinContent(ib,cont);
         _yieldDDEtoGamma[ieta]->SetBinError(ib,err);
         TString str_contDYjetsNom = "";str_contDYjetsNom+=(int)contDYjetsNom;
         TString str_errDYjetsNom = "";str_errDYjetsNom+=(int)errDYjetsNom;
         TString str_contDYjetsEnr = "";str_contDYjetsEnr+=(int)contDYjetsEnr;
         TString str_errDYjetsEnr = "";str_errDYjetsEnr+=(int)errDYjetsEnr;
         TString str_contDataEnr = "";str_contDataEnr+=(int)contDataEnr;
         TString str_errDataEnr = "";str_errDataEnr+=(int)errDataEnr;
         TString str_cont = "";str_cont+=(int)cont;
         TString str_err = "";str_err+=(int)err;
         std::cout<<ib<<" "<<str_contDYjetsNom<<"+-"<<str_errDYjetsNom;
         std::cout<<" "<<str_contDataEnr<<"+-"<<str_errDataEnr<<" "<<str_contDYjetsEnr<<"+-"<<str_errDYjetsEnr;
         std::cout<<" "<<str_cont<<"+-"<<str_err<<std::endl;
      }//end of loop over ib

      std::cout<<std::endl;
      std::cout<<"cutEta="<<cutEta.GetTitle()<<std::endl;
      std::cout<<"cut=(cutEta && _cutAdd)*cutW="<<((cutEta && _cutAdd)*cutW).GetTitle()<<std::endl;
      std::cout<<"varDraw="<<_varKin+TString(">>")+_yieldDataEnr[ieta]->GetName()<<std::endl;
      std::cout<<"trDataEnr->GetEntries((cutEta && _cutAdd)*cutW)="<<trDataEnr->GetEntries((cutEta && _cutAdd)*cutW)<<std::endl;
      std::cout<<"_yieldDataEnr[ieta]->GetSumOfWeights()="<<_yieldDataEnr[ieta]->GetSumOfWeights()<<std::endl;
      std::cout<<"trDYjetsEnr->GetEntries((cutEta && _cutAdd)*cutW)="<<trDYjetsEnr->GetEntries((cutEta && _cutAdd)*cutW)<<std::endl;
      std::cout<<"_yieldDYjetsEnr[ieta]->GetSumOfWeights()="<<_yieldDYjetsEnr[ieta]->GetSumOfWeights()<<std::endl;
      std::cout<<"trDYjetsNom->GetEntries((cutEta && _cutAdd)*cutW)="<<trDYjetsNom->GetEntries((cutEta && _cutAdd)*cutW)<<std::endl;
      std::cout<<"_yieldDYjets[ieta]->GetSumOfWeights()="<<_yieldDYjets[ieta]->GetSumOfWeights()<<std::endl;
      std::cout<<"_yieldDDEtoGamma[ieta]->GetSumOfWeights()="<<_yieldDDEtoGamma[ieta]->GetSumOfWeights()<<std::endl;
      std::cout<<std::endl;
      _yieldDataEnr[ieta]->Print();
      _yieldDYjetsEnr[ieta]->Print();
      _yieldDYjets[ieta]->Print();
      _yieldDDEtoGamma[ieta]->Print();
      std::cout<<std::endl;

   }//end of loop over ieta

}// end of TEtoGamma::ComputeBkg()

void TEtoGamma::WriteToFile()
{
  _fOut->cd();
  TH1F* histTot[3];
    for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
      _yieldDataEnr[ieta]->Write();
      _yieldDYjetsEnr[ieta]->Write();
      _yieldDYjets[ieta]->Write();
      _yieldDDEtoGamma[ieta]->Write();

      // TOTAL
      TString strNameTot=_conf.GetYieldsDDBkgEtoGamma(_conf.TOTAL, ieta);
      histTot[ieta] = new TH1F(strNameTot,strNameTot,1,_kinBinLims[0],_kinBinLims[_nKinBins]);
      histTot[ieta]->SetBinContent(1,0.0); histTot[ieta]->SetBinError(1,0.0);
      float cont=0.0; float err=0.0;      
      for (int ib=1; ib<=_yieldDDEtoGamma[ieta]->GetNbinsX(); ib++){     
        cont+=_yieldDDEtoGamma[ieta]->GetBinContent(ib);
        err+=_yieldDDEtoGamma[ieta]->GetBinError(ib)*_yieldDDEtoGamma[ieta]->GetBinError(ib);
      }
      err = sqrt(err);
      std::cout<<"ieta="<<ieta<<", total = "<<cont<<"+-"<<err<<std::endl;
      histTot[ieta]->SetBinContent(1,cont); histTot[ieta]->SetBinError(1,err);
      histTot[ieta]->Write();
    }//end of loop over ieta

    // _conf.ETA_COMMON
    TString str =_conf.GetYieldsDDBkgEtoGamma(_conf.TOTAL, _conf.COMMON);
    histTot[_conf.COMMON] = (TH1F*)histTot[_conf.BARREL]->Clone(str);
    histTot[_conf.COMMON]->Add(histTot[_conf.ENDCAP]);
    histTot[_conf.COMMON]->SetTitle(str);
    histTot[_conf.COMMON]->Write();
    str =_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, _conf.COMMON);
    _yieldDDEtoGamma[_conf.COMMON] = (TH1F*)_yieldDDEtoGamma[_conf.BARREL]->Clone(str);
    _yieldDDEtoGamma[_conf.COMMON]->Add(_yieldDDEtoGamma[_conf.ENDCAP]);
    _yieldDDEtoGamma[_conf.COMMON]->SetTitle(str);
    _yieldDDEtoGamma[_conf.COMMON]->Write(str);
}// end of WriteToFile()
