#include "TEtoGamma.h"
  //this class

#include "../Configuration/TConfiguration.h"

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
  _fOut=new TFile(_conf.GetDDBkgEtoGammaFileName(anPars.varKin),"recreate");
    for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
      TString strName;

      // data sample enriched by e->gamma:
      strName=TString("yieldDataEnriched_")+_conf.StrEtaBin(ieta);
      _yieldDataEnr[ieta] = new TH1F(strName,strName,anPars.nKinBins,anPars.kinBinLims);

      // DYjets MC sample enriched by e->gamma:
      strName=TString("yieldDYjetsEnriched_")+_conf.StrEtaBin(ieta);
      _yieldDYjetsEnr[ieta] = new TH1F(strName,strName,anPars.nKinBins,anPars.kinBinLims);

      // data/DYjetsMC ratio:
      strName=TString("hRatio_")+_conf.StrEtaBin(ieta);
      _hRatio[ieta] = new TH1F(strName,strName,anPars.nKinBins,anPars.kinBinLims);

      // DYjets MC sample in nominal selection conditions:
      strName=TString("yieldDYjets_")+_conf.StrEtaBin(ieta);
      _yieldDYjets[ieta] = new TH1F(strName,strName,anPars.nKinBins,anPars.kinBinLims);

      // data driven e->gamma estimate as to be used for background subtraction:
      strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
      _yieldDDEtoGamma[ieta] = new TH1F(strName,strName,anPars.nKinBins,anPars.kinBinLims);
    }//end of loop over ieta
}// end of   TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars) 

TEtoGamma::~TEtoGamma()
{
}

void TEtoGamma::ComputePlotSave()
{
  ComputeBkg();
  WriteToFile();
}// end of TEtoGamma::EstimateBkg()

TTree* TEtoGamma::GetTree(TString strFileName){
  TFile* f = new TFile(strFileName);
  TTree* tr=(TTree*)f->Get("selectedEvents");
}// end of GetTree

void TEtoGamma::ComputeBkg()
{
  std::cout<<"ComputeBkg"<<std::endl;
//   TString strDataEnr=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
//   TString strDYjetsEnr=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
  TString strDataEnr=_conf.GetSelectedName(_conf.VERY_PRELIMINARY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
   TString strDYjetsEnr=_conf.GetSelectedName(_conf.VERY_PRELIMINARY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
   TString strDYjetsNom=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");

   TTree* trDataEnr = GetTree(strDataEnr); 
   TTree* trDYjetsEnr = GetTree(strDYjetsEnr); 
   TTree* trDYjetsNom = GetTree(strDYjetsNom); 
  
}// end of TEtoGamma::EstimateBkg()

void TEtoGamma::WriteToFile()
{
  _fOut->cd();
    for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
      _yieldDataEnr[ieta]->Write();
      _yieldDYjetsEnr[ieta]->Write();
      _hRatio[ieta]->Write();
      _yieldDYjets[ieta]->Write();
      _yieldDDEtoGamma[ieta]->Write();
    }//end of loop over ieta
}
