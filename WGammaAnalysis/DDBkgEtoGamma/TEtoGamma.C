#include "TEtoGamma.h"
  //this class

#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TFullCuts.h"
#include "../Include/TMathTools.h"
#include "../Include/RooCMSShape.h"

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

#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooFormulaVar.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooExponential.h"
#include "RooNDKeysPdf.h"
#include "RooGaussian.h"
#include "RooPlot.h"
using namespace RooFit;

TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars, int selStage, bool isMCclosure)
{
  
  _isMCclosure=isMCclosure;
  _cutAdd=anPars.cutAdd;
  _selStage=selStage;
  
  _varKin=anPars.varKin;
  TString strFout = _conf.GetDDBkgEtoGammaFileName(anPars.varKin);
  if (selStage==_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_NO_WMT_CUT) 
    strFout.ReplaceAll(".root","_noWMTcut.root");
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
  
  SetYields(_ZJETS_EtoGAMMA_ENR);
  SetYields(_ZJETS_NOM);   
  SetYields(_SIGMC_NOM);  

  SetYields(_SIGMC_ENR);
  SetYields(_WGAMMATAUNU_ENR);
  SetYields(_ZGAMMA_ENR);
  SetYields(_TTJETS_ENR);
  SetYields(_WJETS_ENR);

  SetYields(_DATA_EtoGAMMA_ENR); 
 
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

  //selStage = _conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT
  //selStage = _conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_NO_WMT_CUT
  TString str;
  if (inum==_DATA_EtoGAMMA_ENR) {
        str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
        if (_isMCclosure) str.ReplaceAll(".root","_MCclosure.root");
    std::cout<<"file _DATA_EtoGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_ZJETS_EtoGAMMA_ENR) {
    str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
    std::cout<<"file _ZJETS_EtoGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_SIGMC_ENR) {
    str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.SIGMC);
    std::cout<<"file _SIGMC_ENR: "<<str<<std::endl;
  }
  if (inum==_WGAMMATAUNU_ENR) {
    str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wg_to_taunu");
    std::cout<<"file __WGAMMATAUNU_ENR: "<<str<<std::endl;
  }
  if (inum==_ZGAMMA_ENR) {
    str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Zg");
    std::cout<<"file _ZGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_TTJETS_ENR) {
    str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"ttbarjets");
    std::cout<<"file _TTJETS_ENR: "<<str<<std::endl;
  }
  if (inum==_WJETS_ENR) {
    str=_conf.GetSelectedName(_selStage,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wjets_to_lnu");
    std::cout<<"file _WJETS_ENR: "<<str<<std::endl;
  }
  if (inum==_ZJETS_NOM) {
    str=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
    std::cout<<"file _ZJETS_NOM: "<<str<<std::endl;
  }
  if (inum==_SIGMC_NOM) {
    str=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.SIGMC);
    std::cout<<"file _SIGMC_NOM: "<<str<<std::endl;
  }
  return str;
}//end of TEtoGamma::FName

TString TEtoGamma::HName(int inum, int ieta)
{
  TString name;
  if (inum==_DATA_EtoGAMMA_ENR) name="h_Data_EtoGamma_Enr_";
  if (inum==_ZJETS_EtoGAMMA_ENR) name="h_ZJets_EtoGamma_Enr_";
  if (inum==_ZJETS_NOM)  name="h_ZJets_Nom_";
  if (inum==_SIGMC_NOM)  name="h_SIGMC_Nom_";
  if (inum==_SIGMC_ENR)  name="h_SIGMC_Enr_";
  if (inum==_WGAMMATAUNU_ENR) name="h_Wgtaunu_Enr_";
  if (inum==_ZGAMMA_ENR) name="h_Zg_Enr_";
  if (inum==_TTJETS_ENR) name="h_ttjets_Enr_";
  if (inum==_WJETS_ENR) name="h_Wjets_Enr_";
  return name+_conf.StrEtaBin(ieta);
}//end of TEtoGamma::HName

void TEtoGamma::SetYields(int inum)
{
  _fOut->cd();
  TPhotonCuts photon;
  TFullCuts fullcut;
  _yield[inum].num=inum;
  TString fName=FName(inum);
  std::cout<<"fName="<<fName<<std::endl;
  _yield[inum].tr = GetTree(fName);
  TCut cutW="weight";
  //  TCut cutWMt=fullcut.RangeMetRelatedCut(2012,_conf.ELECTRON);
  
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    
    TString hName=HName(inum,ieta);
    _yield[inum].hist[ieta] = new TH1F(hName,hName,_nKinBins,_kinBinLims);
    _yield[inum].hist[ieta]->Sumw2();
    TCut cutEta;
    if (ieta==_conf.BARREL) cutEta=photon.RangeBarrel();
    if (ieta==_conf.ENDCAP) cutEta=photon.RangeEndcap();
    TCut cut = cutEta && _cutAdd;// && cutWMt;
    TCut cutMatch("pho_genEle_dRMin<0.4");
    
    std::cout<<"cut="<<cut.GetTitle()<<std::endl;
    if (inum==_ZJETS_NOM || inum==_ZJETS_EtoGAMMA_ENR) { cut=cut && cutMatch;}
    _yield[inum].hist[ieta]->Print();
    std::cout<<"trEntries="<<_yield[inum].tr->GetEntries(cut*cutW)<<std::endl;
    _yield[inum].tr->Draw(_varKin+TString(">>")+_yield[inum].hist[ieta]->GetName(),cut*cutW,"goff");
    
    if (inum!=_DATA_EtoGAMMA_ENR) continue;
    for (int ikin=0; ikin<_nKinBins; ikin++){
      
      if (_kinBinLims[ikin]<=55) _nFineEtaBins=4;
      else if (_kinBinLims[ikin]<=85) _nFineEtaBins=2;
      else _nFineEtaBins=1;  
        
      for (int ietaFine=0; ietaFine<_nFineEtaBins; ietaFine++){
        TString strCutKin;
        strCutKin=_varKin+">";
        strCutKin+=_kinBinLims[ikin];
        strCutKin+=" && "+_varKin+"<";
        strCutKin+=_kinBinLims[ikin+1]; 
        TCut cutKin(strCutKin);
        std::cout<<"ikin="<<ikin<<", cutKin="<<cutKin.GetTitle()<<std::endl; 
        TCut cutEtaFine = CutEtaFine(ieta,ietaFine);
        TCut cutWithKin = cutEta && cutEtaFine && _cutAdd && cutKin;
        TString hNameZmass="hZmass_"+hName+"_pt";
        hNameZmass+=(int)_kinBinLims[ikin];
        hNameZmass+="to";
        hNameZmass+=(int)_kinBinLims[ikin+1]; 
        hNameZmass+="_ieta";  
        hNameZmass+=ietaFine;      
        int nB = _ZmassNbins[0];
        // 0: 10-15, 1: 15-20, 2: 20-25, 3: 25-30, 4: 30-35, 5: 35-45, 6: 45-55
        // 7: 55-65, 8: 65-75, 9: 75-85, 10: 85-95, 11: 95-120, 12: 120-500
        if (ikin>=5) nB=_ZmassNbins[1];
        if (ikin>=7) nB=_ZmassNbins[2];
        if (ikin>=9) nB=_ZmassNbins[3];
        if (ikin>=11) nB=_ZmassNbins[4];
        _hZmassData[ieta][ietaFine][ikin] = new TH1F(hNameZmass,hNameZmass,nB,_ZmassMin,_ZmassMax);    
        _yield[inum].tr->Draw("Mpholep1>>"+hNameZmass,cutWithKin*cutW,"goff");

        TString saveas="sa_"+hNameZmass;
        if (_selStage==_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_NO_WMT_CUT) saveas+="_noWMtCut";

	FitMeg(ikin, ieta, ietaFine, _hZmassData[ieta][ietaFine][ikin],(cutEta && cutEtaFine && _cutAdd && cutKin && cutMatch)*cutW,saveas);

      }//end of loop over ietaFine

      float val=0; float err=0;
      for (int ietaFine=0; ietaFine<_nFineEtaBins; ietaFine++){
        val += _NsigVal[ietaFine];
        err += _NsigErr[ietaFine]*_NsigErr[ietaFine];
      }//end of ietaFine 
      err=sqrt(err);
      _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->SetBinContent(ikin+1,val);
      _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->SetBinError(ikin+1,err);
      
    }//end of loop over ikin
    
  }// end of loop over ieta
  
}// end of TEtoGamma::SetYields

float TEtoGamma::EtaMin(int ieta, int ietaFine){
  if (_nFineEtaBins==4){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return 0.00;}
      if (ietaFine==1) {return 0.10;}
      if (ietaFine==2) {return 0.50;}
      if (ietaFine==3) {return 1.00;} 
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return 1.56;}
      if (ietaFine==1) {return 2.10;}
      if (ietaFine==2) {return 2.20;}
      if (ietaFine==3) {return 2.40;} 
    }//end of  if (ieta==1)
  }//end of _nFineEtaBins==4
  if (_nFineEtaBins==2){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return 0.00;}
      if (ietaFine==1) {return 0.50;}
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return 1.56;}
      if (ietaFine==1) {return 2.20;}
    }//end of  if (ieta==1)
  }//end of _nFineEtaBins==2
  if (_nFineEtaBins==1){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return 0.00;}
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return 1.56;}
    }//end of  if (ieta==1)
  }//end of _nFineEtaBins==1
}//end of EtaMin

float TEtoGamma::EtaMax(int ieta, int ietaFine){
  if (_nFineEtaBins==4){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return 0.10;}
      if (ietaFine==1) {return 0.50;}
      if (ietaFine==2) {return 1.00;}
      if (ietaFine==3) {return 1.45;} 
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return 2.10;}
      if (ietaFine==1) {return 2.20;}
      if (ietaFine==2) {return 2.40;}
      if (ietaFine==3) {return 2.51;} 
    }//end of  if (ieta==1)
  }//end of _nFineEtaBins==4
  if (_nFineEtaBins==2){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return 0.50;}
      if (ietaFine==1) {return 1.45;} 
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return 2.20;}
      if (ietaFine==1) {return 2.51;} 
    }//end of  if (ieta==1)
  }//end of _nFineEtaBins==2
  if (_nFineEtaBins==1){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return 1.45;} 
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return 2.51;} 
    }//end of  if (ieta==1)
  }//end of _nFineEtaBins==1
}//end of EtaMax

TString TEtoGamma::StrEtaFine(int ieta, int ietaFine){
  if (_nFineEtaBins==4){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return "0.00-0.10";}
      if (ietaFine==1) {return "0.10-0.50";}
      if (ietaFine==2) {return "0.50-1.00";}
      if (ietaFine==3) {return "1.00-1.44";} 
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return "1.56-2.10";}
      if (ietaFine==1) {return "2.10-2.20";}
      if (ietaFine==2) {return "2.20-2.40";}
      if (ietaFine==3) {return "2.40-2.50";} 
    }//end of  if (ieta==1)
  }//end of _nEtaBins==4
  if (_nFineEtaBins==2){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return "0.00-0.50";}
      if (ietaFine==1) {return "0.50-1.44";}
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return "1.56-2.20";}
      if (ietaFine==1) {return "2.20-2.50";}
    }//end of  if (ieta==1)
  }//end of _nEtaBins==2
  if (_nFineEtaBins==1){
    if (ieta==0){//BARREL
      if (ietaFine==0) {return "0.00-1.44";}
    }//end of  if (ieta==0)
    if (ieta==1){//ENDCAP
      if (ietaFine==0) {return "1.56-2.50";}
    }//end of  if (ieta==1)
  }//end of _nEtaBins==1
}//end of EtaMin

TCut TEtoGamma::CutEtaFine(int ieta, int ietaFine){
  float etaMin=EtaMin(ieta, ietaFine); float etaMax=EtaMax(ieta, ietaFine);
  TString strCut;
  strCut="(phoSCEta>=";
  strCut+=etaMin;
  strCut+=" && ";
  strCut+="phoSCEta<"; 
  strCut+=etaMax;
  strCut+=") || (phoSCEta<="; 
  strCut+=-etaMin;
  strCut+=" && ";
  strCut+="phoSCEta>"; 
  strCut+=-etaMax;
  strCut+=")";
  TCut cut(strCut);
  std::cout<<"ieta="<<ieta<<", ietaFine="<<ietaFine<<", cut="<<cut.GetTitle()<<std::endl;
  std::cout<<"etaMin="<<etaMin<<", etaMax="<<etaMax<<std::endl;
  std::cout<<"nFineEtaBins="<<_nFineEtaBins<<", EtaMin("<<ieta<<", "<<ietaFine<<")="<<EtaMin(ieta, ietaFine)<<", EtaMax("<<ieta<<", "<<ietaFine<<")="<<EtaMax(ieta, ietaFine)<<std::endl;
  return cut;
}//end of  CutEtaFine

void TEtoGamma::ComputeBkg()
{
  _fOut->cd();
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    TString strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = (TH1F*)_yield[_ZJETS_NOM].hist[ieta]->Clone(strName);
    _yieldDDEtoGamma[ieta]->Multiply(_yield[_DATA_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Divide(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Print();

    // underflow bin: take MC value
    _yieldDDEtoGamma[ieta]->SetBinContent(1,_yield[_ZJETS_NOM].hist[ieta]->GetBinContent(1));
    _yieldDDEtoGamma[ieta]->SetBinError(1,_yield[_ZJETS_NOM].hist[ieta]->GetBinError(1));

    strName="hScale_ONEDI_";
    if (ieta==0) strName+="BARREL"; if (ieta==1) strName+="ENDCAP";
    _hScale[ieta]=(TH1F*)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->Clone(strName);
    _hScale[ieta]->SetTitle(strName);
    _hScale[ieta]->Divide(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
    _hScale[ieta]->Write();
   
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;

  std::cout<<"||||========== Table with err in %"<<std::endl;


    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{$e\\rightarrow\\gamma$ background }";
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|}"<<std::endl;
    std::cout<<" bin  & DYjets    & Data                      & DYjets & scale & $e\\rightarrow\\gamma$ & sigmc\\\\ "<<std::endl;
    std::cout<<" lims & nom. sel. & $e\\rightarrow\\gamma$ enr. & $e\\rightarrow\\gamma$ enr. & & yield & (W\\gamma\\rightarrow e\\nu\\gamma)\\\\ \\hline"<<std::endl;

    for (int ib=1; ib<=_yield[_ZJETS_NOM].hist[ieta]->GetNbinsX(); ib++){
      std::cout<<_yield[_ZJETS_NOM].hist[ieta]->GetBinLowEdge(ib)<<"-"<<_yield[_ZJETS_NOM].hist[ieta]->GetBinLowEdge(ib)+_yield[_ZJETS_NOM].hist[ieta]->GetBinWidth(ib)<<" & ";
      std::cout<<(int)_yield[_ZJETS_NOM].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_ZJETS_NOM].hist[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib)<<" & ";
      float scale = _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)/_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib);
      TMathTools math;
      float scaleErr = math.ErrOfTwoIndependent("x1/x2", _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib), _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib),_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib), _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib) );
      std::cout<<std::setprecision(3)<<scale<<"$\\pm$"<<scaleErr<<"& ";
      std::cout<<(int)_yieldDDEtoGamma[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yieldDDEtoGamma[ieta]->GetBinError(ib)<<"& ";
      std::cout<<(int)_yield[_SIGMC_NOM].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_SIGMC_NOM].hist[ieta]->GetBinError(ib);
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

void  TEtoGamma::FitMeg(int ikin, int ieta, int ietaFine, TH1F* Z_mass, TCut cut, TString saveas)
{
  
  double hmin = _ZmassMin;//Z_mass->GetXaxis()->GetXmin();
  double hmax = _ZmassMax;//Z_mass->GetXaxis()->GetXmax();
  double nMax = Z_mass->Integral();
  
  // Declare observable x
  RooRealVar xtmp("xtmp","xtmp",_ZmassMin,_ZmassMax) ;
  RooRealVar x("Mpholep1", "Mpholep1 from tree", _ZmassMin,_ZmassMax);//("x","x",hmin,hmax) ;
  RooDataHist dh("dh","dh",x,Import(*Z_mass)) ;
  RooDataHist x1("x1","x1",xtmp,Import(*Z_mass)) ;

    _start_CMS_alpha=60;
    _start_CMS_beta=0.050;
    _start_CMS_gamma=0.020;
    _start_CMS_peak=80;
    _start_mean_gau=0;
    _start_sigma_gau=1.0;

    if (ikin==0 && ieta==0){//10-15 GeV, barrel
      _start_CMS_alpha=20;
      _start_CMS_beta=0.005;
      _start_CMS_gamma=0.055;
      _start_CMS_peak=75;
      _start_mean_gau=0.9;
      _start_sigma_gau=1.34;
    }

    if (ikin==0 && ieta==1){//10-15 GeV, endcap
      _start_CMS_alpha=20;
      _start_CMS_beta=0.005;
      _start_CMS_gamma=0.020;
      _start_CMS_peak=80;
      _start_mean_gau=-1.97;
      _start_sigma_gau=0.1;
    }

    if (ikin==1 && ieta==0){//15-20 GeV, barrel
      _start_CMS_alpha=41;
      _start_CMS_beta=0.2;
      _start_CMS_gamma=0.047;
      _start_CMS_peak=80;
      _start_mean_gau=1.3;
      _start_sigma_gau=5.0;
    }

    if (ikin==1 && ieta==1){//15-20 GeV, endcap
      _start_CMS_alpha=43;
      _start_CMS_beta=0.09;
      _start_CMS_gamma=0.018;
      _start_CMS_peak=80;
      _start_mean_gau=0.6;
      _start_sigma_gau=1.7;
    }

    if (ikin==2 && ieta==1){//20-25 GeV, endcap
      _start_CMS_alpha=50;
      _start_CMS_beta=0.06;
      _start_CMS_gamma=0.023;
      _start_CMS_peak=90;
      _start_mean_gau=2.4;
      _start_sigma_gau=0.1;
    }


  // RooNDKeysPdf x Gaussian
  
    RooRealVar mean_gau("mean_gau", "Gaussian Z mass, mean", _start_mean_gau, -5, 5);
    RooRealVar sigma_gau("sigma_gau", "Gaussian Z mass, sigma", _start_sigma_gau, 0.05, 6.00);
    RooGaussian gauPDF("gauPDF","Signal shape gaussian",x, mean_gau, sigma_gau);
    TFile* fTemp = new TFile("fTemp.toor","RECREATE");
    TTree* tr = (TTree*)_yield[_ZJETS_EtoGAMMA_ENR].tr->CopyTree(cut);
    std::cout<<"tr entries = "<<_yield[_ZJETS_EtoGAMMA_ENR].tr->GetEntries()<<std::endl;
    std::cout<<"after  cut = "<<tr->GetEntries()<<std::endl;
    RooDataSet sig_data("sig_data", "e->g data, DYjets", tr, x);
    RooNDKeysPdf sig_templ( "sig_templ", "sig_template", x, sig_data);
    RooFFTConvPdf SigPDF("SigPDF", "FFT Conv sig templ x Gau", x, sig_templ, gauPDF);
  
  

  //Now define the background P.D.F

    //RooCMSShape
     RooRealVar CMS_alpha("CMS_alpha","CMS_alpha",_start_CMS_alpha,20,120);
     RooRealVar CMS_beta("CMS_beta","CMS_beta",_start_CMS_beta,0.005,0.300);
     RooRealVar CMS_gamma("CMS_gamma","CMS_gamma",_start_CMS_gamma,0.005,0.070);
     RooRealVar CMS_peak("CMS_peak","CMS_peak",_start_CMS_peak,70,90);
     RooCMSShape BkgPDF("BkgPDF","RooCMSShape",x,CMS_alpha,CMS_beta, CMS_gamma,CMS_peak);
    
       	

  //Now construct the total PDF. We need to define the number of signal and background events in the model
  RooRealVar Nsig("Nsig","Number of signal events",0.5*nMax,0.,nMax);
  RooRealVar Nbkg("Nbkg","Number of background events",0.2*nMax,0.,nMax);
  RooAddPdf PDFtot("PDFtot","PDFtot",RooArgList(SigPDF,BkgPDF),RooArgList(Nsig,Nbkg));
  std::cout<<"I will fit PDFtot  "<<std::endl;  
  PDFtot.fitTo(dh);//ML fit is default
  std::cout<<"I fitted PDFtot  "<<std::endl; 
  
  //Now plot the data and the fitted PDF
  
  RooPlot* frame = x.frame() ;

  TString strKin="Pt: ";
  strKin+=(int)_yield[_DATA_EtoGAMMA_ENR].hist[0]->GetBinLowEdge(ikin+1);
  strKin+="-";
  strKin+=(int)_yield[_DATA_EtoGAMMA_ENR].hist[0]->GetBinLowEdge(ikin+1)+(int)_yield[_DATA_EtoGAMMA_ENR].hist[0]->GetBinWidth(ikin+1);
  strKin+=" GeV, ";
  TString strEta="eta: "+StrEtaFine(ieta, ietaFine);
  TString strTitle=strKin+strEta;
  
  frame->SetTitle(strTitle);
  
  //  x1.plotOn(frame);//MarkerColor(2),MarkerSize(0.9),MarkerStyle(21)); //this will show histogram data points on canvas
  dh.plotOn(frame);//MarkerColor(2),MarkerSize(0.9),MarkerStyle(21)); //this will show histogram data points on canvas
  //dh.statOn(frame); //this will display hist stat on canvas
  PDFtot.plotOn(frame,LineColor(4));//this will show fit overlay on canvas
  PDFtot.paramOn(frame,Layout(0.70,0.98,0.8)); //this will display the fit parameters on canvas
  //One can also plot the single components of the total PDF, like the background component
  PDFtot.plotOn(frame,Components(BkgPDF),LineStyle(kDashed),LineColor(kRed));
  //Actually plot the result
  
  TCanvas* c1 = new TCanvas(saveas,saveas);
  c1->cd();
  //c1->SetLogy();
  
  frame->Draw();
  // Prepare and plot THStack
  
  THStack* mcHists = new THStack("mcHistsTot","DATA vs MC and FIT");
  
  _yield[_SIGMC_ENR].hist[ieta]->SetFillColor(634);_yield[_SIGMC_ENR].hist[ieta]->SetLineColor(634);
  mcHists->Add(_yield[_SIGMC_ENR].hist[ieta]);
  _yield[_WGAMMATAUNU_ENR].hist[ieta]->SetFillColor(401);_yield[_WGAMMATAUNU_ENR].hist[ieta]->SetLineColor(401);
  mcHists->Add(_yield[_WGAMMATAUNU_ENR].hist[ieta]);
  _yield[_ZGAMMA_ENR].hist[ieta]->SetFillColor(879);_yield[_ZGAMMA_ENR].hist[ieta]->SetLineColor(879);
  mcHists->Add(_yield[_ZGAMMA_ENR].hist[ieta]);
  _yield[_TTJETS_ENR].hist[ieta]->SetFillColor(631);_yield[_TTJETS_ENR].hist[ieta]->SetLineColor(631);
  mcHists->Add(_yield[_TTJETS_ENR].hist[ieta]);
  _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->SetFillColor(418);_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->SetLineColor(418);
  mcHists->Add(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
  _yield[_WJETS_ENR].hist[ieta]->SetFillColor(433);_yield[_WJETS_ENR].hist[ieta]->SetLineColor(433);
  mcHists->Add(_yield[_WJETS_ENR].hist[ieta]);

  //  mcHists->Draw("HIST same");
  frame->Draw("same");
  
  c1->Draw();
  //double chi2 = RooChi2Var("chi2","chi2",PDFtot,dh);
  double chi2 = frame->chiSquare('PDFtot');
  cout<<"chi2="<<chi2<<endl;
  if (_isMCclosure) saveas+="_MCclosure";
  c1->SaveAs("../WGammaOutput/ELECTRON_WGamma/Plots/EtoGammaFits/"+saveas+".png");
  c1->SaveAs("../WGammaOutput/ELECTRON_WGamma/Plots/EtoGammaFits/"+saveas+".pdf");
  cout<<"Nsig.getVal()="<<Nsig.getVal()<<endl;
  _NsigVal[ietaFine]=Nsig.getVal();
  _NsigErr[ietaFine]=Nsig.getError();
  
  return;
  
}//end of FitMeg
