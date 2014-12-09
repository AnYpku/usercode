#include "TTemplatesRandCone.h"
#include "TTemplatesRandConeSyst.h"
#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"
//this package

#include <iostream>
//standard C++

#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TCut.h"
//ROOT

void SetParsRegularCasesClosure(TTemplatesRandCone::TemplatesRandConePars &pars, int channel, 
	int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims);

void SetParsSpecialCasesClosure(TTemplatesRandCone::TemplatesRandConePars &pars, int vgamma);

void AuxTemplatesRandConeClosure(int channel, int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  SetParsRegularCasesClosure(pars, channel, vgamma, blind, phoWP, varKin, nKinBins, kinBinLims);
  SetParsSpecialCasesClosure(pars, vgamma);

  TTemplatesRandCone temp(pars);
  temp.ComputeBackground();
}

void AuxTemplatesRandConeClosureSystSidebandVariation(int channel, int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  SetParsRegularCasesClosure(pars, channel, vgamma, blind, phoWP, varKin, nKinBins, kinBinLims);
  SetParsSpecialCasesClosure(pars, vgamma);

  TTemplatesRandConeSyst temp(pars);
  temp.SidebandVariation();
}

void SetParsRegularCasesClosure(TTemplatesRandCone::TemplatesRandConePars &pars, int channel, 
	int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims)
{
  TConfiguration config;
  TPhotonCuts photon;
  pars.varKin=varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=nKinBins;// number of analysis bins, max=50 (determined in TTemplatesRandCone.h)
  if (nKinBins>TTemplatesRandCone::nKinBinsMax){
    std::cout<<"nKinsBins="<<nKinBins<<", shouldn't exceed "<<TTemplatesRandCone::nKinBinsMax<<std::endl;
    return;
  }
  for (int ikb=0; ikb<=nKinBins; ikb++){
    pars.kinBinLims[ikb]=kinBinLims[ikb];// binning 15-20-25-30-35-40-55-75-95-500
    for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){

      pars.nFitBins[ikb][ieta]=20;
      pars.maxVarFit[ikb][ieta]=20.0;
    }
    pars.sideband[ikb][config.BARREL]=0;//0.012;
    pars.sideband[ikb][config.ENDCAP]=0;//0.034;
      //for these arrays, nFitBins[ikin][ieta], 
      //maxVarFit[ikin][ieta], sideband[ikin][ieta]
      // ikin=0 stands for total fit (e.g. 15-500)
      // ikin=[1,nKinBins] are for individual bin fits 
      // ieta=0 - BARREL, ieta=1 - ENDCAP
  }

  pars.strFileOutName=config.GetDDTemplateFileName(channel,vgamma,varKin);
    //the histograms with extracted yields will be saved here


  for (int ieta=config.BARREL; ieta<=config.COMMON; ieta++){
    pars.strTrueYieldsTot[ieta]=config.GetYieldsDDTemplateTrueName(config.TOTAL,ieta);
    pars.strTrueYields1D[ieta] =config.GetYieldsDDTemplateTrueName(config.ONEDI,ieta);
    pars.strFakeYieldsTot[ieta]=config.GetYieldsDDTemplateFakeName(config.TOTAL,ieta);
    pars.strFakeYields1D[ieta] =config.GetYieldsDDTemplateFakeName(config.ONEDI,ieta);
  }
    //names of TH1F histograms with True and Fake extracted yields,
    //for TOTAL and differentian (ONEDI) cross section, 
    //for barrel, endcap and common(barrel+endcap)

  std::cout<<std::endl;

  enum {F_DATA, F_SIGN, F_TRUE, F_FAKE};
  TString strfiles[4]; // 0 - data, 1 - sigmc, 2 - true, 3 - fake
  if (channel==config.MUON && vgamma==config.W_GAMMA){
    strfiles[F_DATA]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_WGamma_WjetsAndWg.root";
    strfiles[F_SIGN]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root";
    strfiles[F_TRUE]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root";
    strfiles[F_FAKE]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root";
  }
  if (channel==config.MUON && vgamma==config.Z_GAMMA){
    strfiles[F_DATA]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGamma_ZjetsAndZg.root";
    strfiles[F_SIGN]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root";
    strfiles[F_TRUE]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root";
    strfiles[F_FAKE]="../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root";
  }

  pars.fData=new TFile(strfiles[F_DATA]);
  pars.treeData=(TTree*)pars.fData->Get("selectedEvents");
  if(!pars.treeData) return;

  pars.fSign=new TFile(strfiles[F_SIGN]);
  pars.treeSign=(TTree*)pars.fSign->Get("selectedEvents");
  if(!pars.treeSign) return;

  pars.fTrue=new TFile(strfiles[F_TRUE]);
  pars.treeTrue=(TTree*)pars.fTrue->Get("selectedEvents");
  if(!pars.treeTrue) return;

  pars.fFake=new TFile(strfiles[F_FAKE]);
  pars.treeFake=(TTree*)pars.fFake->Get("selectedEvents");
  if(!pars.treeFake) return;

  std::cout<<"files used:"<<std::endl;
  std::cout<<"data: "<<strfiles[F_DATA]<<std::endl;
  std::cout<<"sign: "<<strfiles[F_SIGN]<<std::endl;
  std::cout<<"true: "<<strfiles[F_TRUE]<<std::endl;
  std::cout<<"fake: "<<strfiles[F_FAKE]<<std::endl;

  pars.noLeakSubtr=1;

  pars.varSideband="phoSigmaIEtaIEta";//TString
//  pars.varTrueTempl="phoRandConeChIso04Corr";//TString
  pars.varTrueTempl="phoSCRChIso04Corr";//TString
  pars.varFakeTempl="phoSCRChIso04Corr";//TString
  pars.varFit="phoSCRChIso04Corr"; //TString
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  pars.cutChIsolation=photon.RangeOneIsolation(2012,phoWP,photon.ISO_CHorTRK);//TCut; 
    //charged isolation cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.cutSidebandVarNominalRange=photon.RangeSigmaIEtaIEta(2012, phoWP);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
//  pars.cutWeight="weight";//TCut; weight for signal MC tree

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut

  pars.noLeakSubtr=1;
}

void SetParsSpecialCasesClosure(TTemplatesRandCone::TemplatesRandConePars &pars, int vgamma)
{
  TConfiguration config;

  // for 
  for (int ikin=0; ikin<=pars.nKinBins; ikin++){
    if (ikin==0) continue;//0 is for Total yield
    if (pars.varKin=="phoEt"){
      if (pars.kinBinLims[ikin-1]>34){
        pars.sideband[ikin][config.ENDCAP] = 0.033;
      }
      if (pars.kinBinLims[ikin-1]>54){
        pars.sideband[ikin][config.BARREL] = 0.011;
      }
      if (pars.kinBinLims[ikin-1]>74){
        pars.nFitBins[ikin][config.ENDCAP] = 10;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
        pars.sideband[ikin][config.ENDCAP] = 0.030;
      }
      if (pars.kinBinLims[ikin-1]>94){
        pars.nFitBins[ikin][config.ENDCAP] = 8;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
        pars.sideband[ikin][config.ENDCAP] = 0.025;
      }
    }//end of if (pars.varKin=="phoEt")
    if (pars.varKin=="WMt"){
      if (pars.kinBinLims[ikin-1]>119){
        pars.nFitBins[ikin][config.BARREL] = 8;
        pars.maxVarFit[ikin][config.BARREL] = 16.0;
        pars.nFitBins[ikin][config.ENDCAP] = 8;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
      }     
    }
  }//end of loop over ikin
  if (vgamma == config.W_GAMMA) return;
  // for Z_GAMMA only; make less bins and lower maxVarFit 
  // for high phoEt bins otherwise fit doesn't work
  for (int ikin=0; ikin<=pars.nKinBins; ikin++){
    if (ikin==0) continue;//0 is for Total yield
    if (pars.varKin=="phoEt" && vgamma==config.Z_GAMMA){ 
      if (pars.kinBinLims[ikin-1]>24){
        pars.nFitBins[ikin][config.BARREL] = 8;
        pars.maxVarFit[ikin][config.BARREL] = 16.0;
        pars.nFitBins[ikin][config.ENDCAP] = 16;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
      }
      if (pars.kinBinLims[ikin-1]>29){
        pars.nFitBins[ikin][config.BARREL] = 10;
        pars.maxVarFit[ikin][config.BARREL] = 16.0;
        pars.nFitBins[ikin][config.ENDCAP] = 10;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
      }
      if (pars.kinBinLims[ikin-1]>34){
        pars.nFitBins[ikin][config.BARREL] = 8;
        pars.maxVarFit[ikin][config.BARREL] = 16.0;
        pars.nFitBins[ikin][config.ENDCAP] = 10;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
      }
      if (pars.kinBinLims[ikin-1]>54){
        pars.nFitBins[ikin][config.BARREL] = 10;
        pars.maxVarFit[ikin][config.BARREL] = 16.0;
        pars.nFitBins[ikin][config.ENDCAP] = 5;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
      }
      if (pars.kinBinLims[ikin-1]>74){
        pars.nFitBins[ikin][config.BARREL] = 8;
        pars.maxVarFit[ikin][config.BARREL] = 16.0;
        pars.nFitBins[ikin][config.ENDCAP] = 5;
        pars.maxVarFit[ikin][config.ENDCAP] = 16.0;
      }
      if (pars.kinBinLims[ikin-1]>94){
        pars.nFitBins[ikin][config.BARREL] = 3;
        pars.maxVarFit[ikin][config.BARREL] = 12.0;
        pars.nFitBins[ikin][config.ENDCAP] = 3;
        pars.maxVarFit[ikin][config.ENDCAP] = 12.0;
      }
    }//end of if (varKin=="phoEt")
  }//end of ikin loop
}
