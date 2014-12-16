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

void SetParsSigmaIEtaIEtaTempl(TTemplatesRandCone::TemplatesRandConePars &pars, int channel, 
	int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims);

void SetParsRegularCases(TTemplatesRandCone::TemplatesRandConePars &pars, int channel, 
	int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims);

void SetParsSpecialCases(TTemplatesRandCone::TemplatesRandConePars &pars, int vgamma);

void AuxTemplatesRandCone(int channel, int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  SetParsSigmaIEtaIEtaTempl(pars, channel, vgamma, blind, phoWP, varKin, nKinBins, kinBinLims);

//  SetParsRegularCases(pars, channel, vgamma, blind, phoWP, varKin, nKinBins, kinBinLims);
//  SetParsSpecialCases(pars, vgamma);

  TTemplatesRandCone temp(pars);
  temp.ComputeBackground();
}

void AuxTemplatesRandConeSystSidebandVariation(int channel, int vgamma, int blind, int phoWP, TString varKin, int nKinBins, float* kinBinLims)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  SetParsRegularCases(pars, channel, vgamma, blind, phoWP, varKin, nKinBins, kinBinLims);
  SetParsSpecialCases(pars, vgamma);

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

void SetParsRegularCases(TTemplatesRandCone::TemplatesRandConePars &pars, int channel, 
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
      pars.nFitBins[ikb][ieta]=21;
      pars.minVarFit[ikb][ieta]=-1.0+0.1;
      pars.maxVarFit[ikb][ieta]=20.0+0.1;
    }
    pars.sideband[ikb][config.BARREL]=0.012;
    pars.sideband[ikb][config.ENDCAP]=0.034;
    pars.sidebandUp[ikb][config.BARREL]=0.018;
    pars.sidebandUp[ikb][config.ENDCAP]=0.053;
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

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,blind,config.DATA);
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,config.UNBLIND,config.DATA);
//  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,config.UNBLIND,config.DATA);
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  pars.varSideband="phoSigmaIEtaIEta";//TString
  pars.varTrueTempl="phoRandConeChIso04Corr";//TString
  pars.varFakeTempl="phoSCRChIso04Corr";//TString
  pars.varFit="phoSCRChIso04Corr"; //TString
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  pars.cutNominal=photon.RangeOneIsolation(2012,phoWP,photon.ISO_CHorTRK) && photon.RangeSigmaIEtaIEta(2012, phoWP);//TCut; 
  pars.cutNominalExceptSidebandVar=photon.RangeOneIsolation(2012,phoWP,photon.ISO_CHorTRK);//TCut; 
    //charged isolation cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.cutSidebandVarNominalRange=photon.RangeSigmaIEtaIEta(2012, phoWP);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
//  pars.cutWeight="weight";//TCut; weight for signal MC tree

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut

  pars.noLeakSubtr=0;
}

void SetParsSpecialCases(TTemplatesRandCone::TemplatesRandConePars &pars, int vgamma)
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

void SetParsSigmaIEtaIEtaTempl(TTemplatesRandCone::TemplatesRandConePars &pars, int channel, 
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
    pars.nFitBins[ikb][config.BARREL]=32;
    pars.minVarFit[ikb][config.BARREL]=0.005;
    pars.maxVarFit[ikb][config.BARREL]=0.021;
    pars.nFitBins[ikb][config.ENDCAP]=25;
    pars.minVarFit[ikb][config.ENDCAP]=0.019;
    pars.maxVarFit[ikb][config.ENDCAP]=0.069;
    if (varKin=="phoEt" && pars.kinBinLims[ikb-1]>24){
    pars.nFitBins[ikb][config.BARREL]=14;
    pars.minVarFit[ikb][config.BARREL]=0.006;
    pars.maxVarFit[ikb][config.BARREL]=0.018;
    }
    if (varKin=="phoEt" && pars.kinBinLims[ikb-1]>24){
      pars.nFitBins[ikb][config.ENDCAP]=15;
      pars.minVarFit[ikb][config.ENDCAP]=0.020;
      pars.maxVarFit[ikb][config.ENDCAP]=0.065;
    }

    pars.sideband[ikb][config.BARREL]=1.5;//2.6;
    pars.sideband[ikb][config.ENDCAP]=1.2;//2.3;
    pars.sidebandUp[ikb][config.BARREL]=5.0;//15.0;
    pars.sidebandUp[ikb][config.ENDCAP]=5.0;//15.0;
      //for these arrays, nFitBins[ikin][ieta], 
      //maxVarFit[ikin][ieta], sideband[ikin][ieta]
      // ikin=0 stands for total fit (e.g. 15-500)
      // ikin=[1,nKinBins] are for individual bin fits 
      // ieta=0 - BARREL, ieta=1 - ENDCAP
    if (vgamma==config.Z_GAMMA && varKin=="phoEt"){
      pars.nFitBins[ikb][config.ENDCAP]=15;
      pars.minVarFit[ikb][config.ENDCAP]=0.020;
      pars.maxVarFit[ikb][config.ENDCAP]=0.065;
      if (pars.kinBinLims[ikb-1]>24){
        pars.nFitBins[ikb][config.BARREL]=10;
        pars.minVarFit[ikb][config.BARREL]=0.006;
        pars.maxVarFit[ikb][config.BARREL]=0.018;
      }
      if (pars.kinBinLims[ikb-1]>24){
        pars.nFitBins[ikb][config.ENDCAP]=10;
        pars.minVarFit[ikb][config.ENDCAP]=0.020;
        pars.maxVarFit[ikb][config.ENDCAP]=0.060;
      }
    }
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

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,blind,config.DATA);
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue=config.GetSelectedName(config.FSR,channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
//  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,config.UNBLIND,config.DATA);
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  pars.varSideband="phoPFChIsoCorr";//TString
  pars.varTrueTempl="phoSigmaIEtaIEta";//TString
  pars.varFakeTempl="phoSigmaIEtaIEta";//TString
  pars.varFit="phoSigmaIEtaIEta"; //TString
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  pars.cutNominal=photon.RangeOneIsolation(2012,phoWP,photon.ISO_CHorTRK) && photon.RangeSigmaIEtaIEta(2012, phoWP);//TCut; 
  pars.cutNominalExceptSidebandVar=photon.RangeSigmaIEtaIEta(2012, phoWP);//TCut; 
    //charged isolation cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.cutSidebandVarNominalRange=photon.RangeOneIsolation(2012,phoWP,photon.ISO_CHorTRK);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
//  pars.cutWeight="weight";//TCut; weight for signal MC tree

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut

  pars.noLeakSubtr=0;
}
