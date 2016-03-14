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

TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars, bool isMCclosure)
{

  _isMCclosure=isMCclosure;
  _cutAdd=anPars.cutAdd;
  _varKin=anPars.varKin;
  TString strFout = _conf.GetDDBkgEtoGammaFileName(anPars.varKin);
  if (isMCclosure) strFout.ReplaceAll(".root","_MCclosure.root");
  _fOut=new TFile(strFout,"recreate");
  _nKinBins=anPars.nKinBins;
  std::cout<<"_fOut="<<strFout<<std::endl;
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
  SetYields(_DATA_EtoGAMMA_ENR); 
  SetYields(_ZJETS_EtoGAMMA_ENR);
  SetYields(_ZJETS_NOM_ELE);   
//  SetYields(_ZJETS_NOM_MUO);
//  SetYields(_WJETS_NOM_ELE);   
//  SetYields(_WJETS_NOM_MUO);
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
  TString str;
  if (inum==_DATA_EtoGAMMA_ENR) {
        str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
        if (_isMCclosure) str.ReplaceAll(".root","_MCclosure.root");
    std::cout<<"file _DATA_EtoGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_ZJETS_EtoGAMMA_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
    std::cout<<"file _ZJETS_EtoGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_ZJETS_NOM_ELE) {
    str=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
    std::cout<<"file _ZJETS_NOM_ELE: "<<str<<std::endl;
  }
//  if (inum==_ZJETS_NOM_MUO) 
//    str=_conf.GetSelectedName(_conf.FULLY,_conf.MUON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
//  if (inum==_WJETS_NOM_ELE) 
//    str=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wjets_to_lnu");
//  if (inum==_WJETS_NOM_MUO) 
//    str=_conf.GetSelectedName(_conf.FULLY,_conf.MUON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wjets_to_lnu");
  return str;
}//end of TEtoGamma::FName

TString TEtoGamma::HName(int inum, int ieta)
{
  TString name;
  if (inum==_DATA_EtoGAMMA_ENR) name="h_Data_EtoGamma_Enr_";
  if (inum==_ZJETS_EtoGAMMA_ENR) name="h_ZJets_EtoGamma_Enr_";
  if (inum==_ZJETS_NOM_ELE)  name="h_ZJets_Nom_Ele_";
//  if (inum==_ZJETS_NOM_MUO)  name="h_ZJets_Nom_Muo_";
//  if (inum==_WJETS_NOM_ELE)  name="h_WJets_Nom_Ele_";
//  if (inum==_WJETS_NOM_MUO)  name="h_WJets_Nom_Muo_";
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
    TCut cut = cutEta && _cutAdd && cutWMt;
    if (inum==_ZJETS_NOM_ELE) {TCut cutMatch("pho_genEle_dRMin<0.4"); cut=cut && cutMatch;}
    _yield[inum].tr->Draw(_varKin+TString(">>")+_yield[inum].hist[ieta]->GetName(),cut*cutW,"goff");
  }// end of loop over ieta
  
}// end of TEtoGamma::SetYields

void TEtoGamma::ComputeBkg()
{
  _fOut->cd();
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
//    TH1F* hZj_jg_nom_ele = (TH1F*)_yield[_WJETS_NOM_ELE].hist[ieta]->Clone(TString("hZj_jg_nom_ele")+_conf.StrEtaBin(ieta));
//    hZj_jg_nom_ele->Multiply(_yield[_ZJETS_NOM_MUO].hist[ieta]);
//    hZj_jg_nom_ele->Divide(_yield[_WJETS_NOM_MUO].hist[ieta]);
    TString strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = (TH1F*)_yield[_ZJETS_NOM_ELE].hist[ieta]->Clone(strName);
//    _yieldDDEtoGamma[ieta]->Add(hZj_jg_nom_ele,-1);
    _yieldDDEtoGamma[ieta]->Multiply(_yield[_DATA_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Divide(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Print();
   
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;

  std::cout<<"||||========== Table with err in %"<<std::endl;


    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{$e\\rightarrow\\gamma$ background }";
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|}"<<std::endl;
    std::cout<<" bin  & DYjets    & Data                      & DYjets & scale & $e\\rightarrow\\gamma$ \\\\ "<<std::endl;
    std::cout<<" lims & nom. sel. & $e\\rightarrow\\gamma$ enr. & $e\\rightarrow\\gamma$ enr. & & yield     \\\\ \\hline"<<std::endl;

    for (int ib=1; ib<=_yield[_ZJETS_NOM_ELE].hist[ieta]->GetNbinsX(); ib++){
      std::cout<<_yield[_ZJETS_NOM_ELE].hist[ieta]->GetBinLowEdge(ib)<<"-"<<_yield[_ZJETS_NOM_ELE].hist[ieta]->GetBinLowEdge(ib)+_yield[_ZJETS_NOM_ELE].hist[ieta]->GetBinWidth(ib)<<" & ";
      std::cout<<(int)_yield[_ZJETS_NOM_ELE].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_ZJETS_NOM_ELE].hist[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib)<<" & ";
      float scale = _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)/_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib);
      TMathTools math;
      float scaleErr = math.ErrOfTwoIndependent("x1/x2", _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib), _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib),_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib), _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib) );
      std::cout<<std::setprecision(3)<<scale<<"$\\pm$"<<scaleErr<<"& ";
      std::cout<<(int)_yieldDDEtoGamma[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yieldDDEtoGamma[ieta]->GetBinError(ib);
      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:EtoGAMMA_"<<ieta<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;

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
      std::cout<<"ieta="<<ieta<<", total = "<<(int)cont<<"+-"<<(int)err<<std::endl;
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
