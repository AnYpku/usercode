#include "TTemplatesRandCone.h"
#include "TTemplatesRandConeSyst.h"
//this package

#include <iostream>
//standard C++

#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TCut.h"
//ROOT

void SetParsRegularCases(TTemplatesRandCone::TemplatesRandConePars &pars, TString varKin, int nKinBins, float* kinBinLims);

void SetParsSpecialCases(TTemplatesRandCone::TemplatesRandConePars &pars);

void AuxTemplatesRandCone(TString varKin, int nKinBins, float* kinBinLims)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  SetParsRegularCases(pars, varKin, nKinBins, kinBinLims);
  SetParsSpecialCases(pars);

  TTemplatesRandCone temp(pars);
  temp.ComputeBackground();
}

void AuxTemplatesRandConeSystSidebandVariation(TString varKin, int nKinBins, float* kinBinLims)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  SetParsRegularCases(pars, varKin, nKinBins, kinBinLims);
  SetParsSpecialCases(pars);

  TTemplatesRandConeSyst temp(pars);
  temp.SidebandVariation();
}

TTree* LoadOneTree(TString strFileWithWhat, TString strFileName, TFile* f)
{
  f = new TFile(strFileName);
  if (!f->IsOpen()){
    std::cout<<"ERROR in AuxTemplatesRandCone, SetParsRegularCases: file with "<<strFileWithWhat<<": "<<strFileName<<" can't be open"<<std::endl;
    return 0;
  }
  else std::cout<<"file with "<<strFileWithWhat<<": "<<strFileName<<std::endl;
  return (TTree*)f->Get("selectedEvents");
}

void SetParsRegularCases(TTemplatesRandCone::TemplatesRandConePars &pars, TString varKin, int nKinBins, float* kinBinLims)
{
  pars.varKin=varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=nKinBins;// number of analysis bins, max=50 (determined in TTemplatesRandCone.h)
  if (nKinBins>TTemplatesRandCone::nKinBinsMax){
    std::cout<<"nKinsBins="<<nKinBins<<", shouldn't exceed "<<TTemplatesRandCone::nKinBinsMax<<std::endl;
    return;
  }
  for (int ikb=0; ikb<=nKinBins; ikb++){
    pars.kinBinLims[ikb]=kinBinLims[ikb];// binning 15-20-25-30-35-40-55-75-95-500
    for (int ieta=0; ieta<=1; ieta++){

      pars.nFitBins[ikb][ieta]=21;
      pars.maxVarFit[ikb][ieta]=-1.0;
      pars.maxVarFit[ikb][ieta]=20.0;
    }
    pars.sideband[ikb][0]=0.011;
    pars.sideband[ikb][1]=0.033;
      //for these arrays, nFitBins[ikin][ieta], 
      //maxVarFit[ikin][ieta], sideband[ikin][ieta]
      // ikin=0 stands for total fit (e.g. 15-500)
      // ikin=[1,nKinBins] are for individual bin fits 
      // ieta=0 - BARREL, ieta=1 - ENDCAP
  }

  pars.strFileOutName="fOut.root";
    //the histograms with extracted yields will be saved here


  for (int ieta=0; ieta<=2; ieta++){
    pars.strTrueYieldsTot[ieta]="yieldsTrueTot"+TString(ieta);
    pars.strTrueYields1D[ieta] ="yieldsTrue1D"+TString(ieta);
    pars.strFakeYieldsTot[ieta]="yieldsFakeTot"+TString(ieta);
    pars.strFakeYields1D[ieta] ="yieldsFake1D"+TString(ieta);
  }
    //names of TH1F histograms with True and Fake extracted yields,
    //for TOTAL and differentian (ONEDI) cross section, 
    //for barrel, endcap and common(barrel+endcap)

  std::cout<<std::endl;

  pars.fData = new TFile("../Hist_job_electron_data.root");
  pars.treeData=(TTree*)pars.fData->Get("miniTree");
  if (!pars.treeData) return;

  pars.fSign = new TFile("../Hist_job_summer12_Wg_skimmed_wg.root");
  pars.treeSign=(TTree*)pars.fSign->Get("miniTree");
  if (!pars.treeSign) return;

  pars.fTrue = new TFile("../Hist_job_electron_data.root");
  pars.treeTrue=(TTree*)pars.fTrue->Get("miniTree");
  if (!pars.treeTrue) return;

  pars.fFake = new TFile("../Hist_job_electron_data.root");
  pars.treeFake=(TTree*)pars.fFake->Get("miniTree");
  if (!pars.treeFake) return;

  pars.varSideband="pho_Sihih";//TString
  pars.varTrueTempl="pho_RandConeChIso";//TString
  pars.varFakeTempl="pho_SCRChIso";//TString
  pars.varFit="pho_SCRChIso"; //TString
  pars.varPhoEta="pho_eta";//TString
  pars.varWeight="evt_weight";//TString

  pars.cutBarrel="pho_eta>-1.4442 && pho_eta<1.4442";//TCut
  pars.cutEndcap="(pho_eta>-2.5 && pho_eta<-1.566) || (pho_eta>1.566 && pho_eta<2.5)";//TCut

  TCut cutChIsoB="pho_ChHadIso<1.5";
  TCut cutChIsoE="pho_ChHadIso<1.2";
  TCut cutSihihB="pho_Sihih<0.011";
  TCut cutSihihE="pho_Sihih<0.033";
  pars.cutChIsolation=(cutChIsoB && pars.cutBarrel) || (cutChIsoE && pars.cutEndcap);//TCut; 
    //charged isolation cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.cutSidebandVarNominalRange=(cutSihihB && pars.cutBarrel) || (cutSihihE && pars.cutEndcap);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.noLeakSubtr=0;
}

void SetParsSpecialCases(TTemplatesRandCone::TemplatesRandConePars &pars)
{
  for (int ikin=0; ikin<=pars.nKinBins; ikin++){
    if (ikin==0) continue;//0 is for Total yield
    if (pars.varKin=="pho_pt"){
      if (pars.kinBinLims[ikin-1]>34){
        pars.sideband[ikin][1] = 0.033;
      }
      if (pars.kinBinLims[ikin-1]>54){
        pars.sideband[ikin][0] = 0.011;
      }
      if (pars.kinBinLims[ikin-1]>74){
        pars.nFitBins[ikin][1] = 10;
        pars.maxVarFit[ikin][1] = 16.0;
        pars.sideband[ikin][1] = 0.030;
      }
      if (pars.kinBinLims[ikin-1]>94){
        pars.nFitBins[ikin][1] = 8;
        pars.maxVarFit[ikin][1] = 16.0;
        pars.sideband[ikin][1] = 0.025;
      }
    }//end of if (pars.varKin=="phoEt")
    if (pars.varKin=="W_MT"){
      if (pars.kinBinLims[ikin-1]>119){
        pars.nFitBins[ikin][0] = 8;
        pars.maxVarFit[ikin][0] = 16.0;
        pars.nFitBins[ikin][1] = 8;
        pars.maxVarFit[ikin][1] = 16.0;
      }     
    }
  }//end of loop over ikin
}
