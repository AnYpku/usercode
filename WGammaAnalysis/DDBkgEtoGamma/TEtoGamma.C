#include "TEtoGamma.h"
  //this class

#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TFullCuts.h"
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
  SetYields(_DATA_EtoGAMMA_ENR); SetYields(_ZJETS_EtoGAMMA_ENR);
  SetYields(_ZJETS_NOM_ELE);   SetYields(_ZJETS_NOM_MUO);
  SetYields(_WJETS_NOM_ELE);   SetYields(_WJETS_NOM_MUO);
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    // data driven e->gamma estimate as to be used for background subtraction:
    TString strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _yieldDDEtoGamma[ieta]->Sumw2();
  }//end of loop over ieta
  ComputeBkg();
  WriteToFile();
}// end of TEtoGamma::EstimateBkg()

TTree* TEtoGamma::GetTree(TString strFileName){
  TFile* f = new TFile(strFileName);
  TTree* tr=(TTree*)f->Get("selectedEvents");
}// end of GetTree

TString TEtoGamma::FName(int inum)
{
  if (inum==_DATA_EtoGAMMA_ENR) 
    return _conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
  if (inum==_ZJETS_EtoGAMMA_ENR) 
    return _conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
  if (inum==_ZJETS_NOM_ELE) 
    return _conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
  if (inum==_ZJETS_NOM_MUO) 
    return _conf.GetSelectedName(_conf.FULLY,_conf.MUON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
  if (inum==_WJETS_NOM_ELE) 
    return _conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wjets_to_lnu");
  if (inum==_WJETS_NOM_MUO) 
    return _conf.GetSelectedName(_conf.FULLY,_conf.MUON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wjets_to_lnu");
}//end of TEtoGamma::FName

TString TEtoGamma::HName(int inum, int ieta)
{
  TString name;
  if (inum==_DATA_EtoGAMMA_ENR) name="h_Data_EtoGamma_Enr_";
  if (inum==_ZJETS_EtoGAMMA_ENR) name="h_ZJets_EtoGamma_Enr_";
  if (inum==_ZJETS_NOM_ELE)  name="h_ZJets_Nom_Ele_";
  if (inum==_ZJETS_NOM_MUO)  name="h_ZJets_Nom_Muo_";
  if (inum==_WJETS_NOM_ELE)  name="h_WJets_Nom_Ele_";
  if (inum==_WJETS_NOM_MUO)  name="h_WJets_Nom_Muo_";
  return name+_conf.StrEtaBin(ieta);
}//end of TEtoGamma::HName

void TEtoGamma::SetYields(int inum)
{
  _fOut->cd();
  TPhotonCuts photon;
  TFullCuts fullcut;
  _yield[inum].num=inum;
  TString fName=FName(inum);
  _yield[inum].tr = GetTree(fName);
  TCut cutW="weight";
  TCut cutWMt=fullcut.RangeMetRelatedCut(2012,_conf.ELECTRON);
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    TString hName=HName(inum,ieta);
    _yield[inum].hist[ieta] = new TH1F(hName,hName,_nKinBins,_kinBinLims);
    _yield[inum].hist[ieta]->Sumw2();
    TCut cutEta;
    if (ieta==_conf.BARREL) cutEta=photon.RangeBarrel();
    if (ieta==_conf.ENDCAP) cutEta=photon.RangeEndcap();
    _yield[inum].tr->Draw(_varKin+TString(">>")+_yield[inum].hist[ieta]->GetName(),(cutEta && _cutAdd && cutWMt)*cutW,"goff");
  }// end of loop over ieta
  
}// end of TEtoGamma::SetYields

void TEtoGamma::ComputeBkg()
{
  _fOut->cd();
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    TH1F* hZj_jg_nom_ele = (TH1F*)_yield[_WJETS_NOM_ELE].hist[ieta]->Clone(TString("hZj_jg_nom_ele")+_conf.StrEtaBin(ieta));
    hZj_jg_nom_ele->Multiply(_yield[_ZJETS_NOM_MUO].hist[ieta]);
    hZj_jg_nom_ele->Divide(_yield[_WJETS_NOM_MUO].hist[ieta]);
    TString strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = (TH1F*)_yield[_ZJETS_NOM_ELE].hist[ieta]->Clone(strName);
    _yieldDDEtoGamma[ieta]->Add(hZj_jg_nom_ele,-1);
    _yieldDDEtoGamma[ieta]->Multiply(_yield[_DATA_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Divide(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Print();
  }// end of loop over ieta

}// end of TEtoGamma::ComputeBkg()

void TEtoGamma::WriteToFile()
{
  _fOut->cd();
  TH1F* histTot[3];
    for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
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
