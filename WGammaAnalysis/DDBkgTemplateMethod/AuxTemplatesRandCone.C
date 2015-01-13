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

void SetParsSigmaIEtaIEtaTempl(TTemplatesRandCone::TemplatesRandConePars &pars, TConfiguration::AnalysisParameters &anPars);

void SetParsChIsoTempl(TTemplatesRandCone::TemplatesRandConePars &pars, TConfiguration::AnalysisParameters &anPars);

void SetLimsChIsoTempl_phoEt_Wg_MUON(TTemplatesRandCone::TemplatesRandConePars &pars);
void SetLimsChIsoTempl_phoEt_Zg_MUON(TTemplatesRandCone::TemplatesRandConePars &pars);
void SetLimsSihihTempl_phoEt_Wg_MUON(TTemplatesRandCone::TemplatesRandConePars &pars);
void SetLimsSihihTempl_phoEt_Zg_MUON(TTemplatesRandCone::TemplatesRandConePars &pars);

void SetValuesToKinEtaArray(int ieta, float vals[250], TTemplatesRandCone::TemplatesRandConePars &pars);

void AuxTemplatesRandCone(TConfiguration::AnalysisParameters &anPars)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

  if (anPars.templFits==TConfiguration::TEMPL_SIHIH) 
    SetParsSigmaIEtaIEtaTempl(pars, anPars);

  if (anPars.templFits==TConfiguration::TEMPL_CHISO) 
    SetParsChIsoTempl(pars, anPars);


  TTemplatesRandCone temp(pars);
  temp.ComputeBackground();
}

void AuxTemplatesRandConeSystSidebandVariation(TConfiguration::AnalysisParameters &anPars)
{
  //this function is called in FullChain

  TTemplatesRandCone::TemplatesRandConePars pars;

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

void SetParsChIsoTempl(TTemplatesRandCone::TemplatesRandConePars &pars, TConfiguration::AnalysisParameters &anPars)
{

  TConfiguration config;
  TPhotonCuts photon;
  pars.varKin=anPars.varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=anPars.nKinBins;// number of analysis bins, max=50 (determined in TTemplatesRandCone.h)
  if (anPars.nKinBins>TTemplatesRandCone::nKinBinsMax){
    std::cout<<"nKinsBins="<<anPars.nKinBins<<", shouldn't exceed "<<TTemplatesRandCone::nKinBinsMax<<std::endl;
    return;
  }
  for (int ikb=0; ikb<=anPars.nKinBins; ikb++){
    pars.kinBinLims[ikb]=anPars.kinBinLims[ikb];// binning 15-20-25-30-35-40-55-75-95-500
    for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
      pars.nFitBins[ikb][ieta]=21;
      pars.minVarFit[ikb][ieta]=-1.0+0.1;
      pars.maxVarFit[ikb][ieta]=20.0+0.1;
      pars.combineTrueTempl[ikb][ieta]=0;
      pars.combineFakeTempl[ikb][ieta]=0;
    }
    pars.sideband[ikb][config.BARREL]=0.011;
    pars.sideband[ikb][config.ENDCAP]=0.033;
    pars.sidebandUp[ikb][config.BARREL]=0.018;
    pars.sidebandUp[ikb][config.ENDCAP]=0.053;
      //for these arrays, nFitBins[ikin][ieta], 
      //maxVarFit[ikin][ieta], sideband[ikin][ieta]
      // ikin=0 stands for total fit (e.g. 15-500)
      // ikin=[1,nKinBins] are for individual bin fits 
      // ieta=0 - BARREL, ieta=1 - ENDCAP
  }
  if (anPars.varKin=="phoEt" && anPars.channel==config.MUON && anPars.vgamma==config.W_GAMMA)
    SetLimsChIsoTempl_phoEt_Wg_MUON(pars);

  if (anPars.varKin=="phoEt" && anPars.channel==config.MUON && anPars.vgamma==config.Z_GAMMA)
    SetLimsChIsoTempl_phoEt_Zg_MUON(pars);

  pars.strFileOutName=config.GetDDTemplateFileName(anPars.channel,anPars.vgamma,config.TEMPL_CHISO,anPars.varKin);
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

 pars.strPlotsDir=config.GetPlotsDirName(anPars.channel, anPars.vgamma, config.PLOTS_TEMPL_FITS);
 pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_CHISO)+TString("_")+config.StrBlindType(anPars.blind)+TString("_");

  std::cout<<std::endl;

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,anPars.blind,config.DATA);
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.DATA);
//  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.DATA);
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  pars.varSideband="phoSigmaIEtaIEta";//TString
  pars.varTrueTempl="phoRandConeChIso04Corr";//"phoRandConeChIso04Corr";//TString
  pars.varFakeTempl="phoSCRChIso04Corr";//"phoSCRChIso04Corr";//TString
  pars.varFit="phoSCRChIso04Corr";//"phoSCRChIso04Corr"; //TString
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  pars.cutNominal=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK) && photon.RangeSigmaIEtaIEta(2012, anPars.phoWP);//TCut; 
  pars.cutNominalExceptSidebandVar=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK);//TCut; 
    //charged isolation cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.cutSidebandVarNominalRange=photon.RangeSigmaIEtaIEta(2012, anPars.phoWP);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
//  pars.cutWeight="weight";//TCut; weight for signal MC tree

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut
  pars.cutAdd=anPars.cutAdd;
  pars.noLeakSubtr=0;

}// end of SetParsChIsoTempl()



void SetParsSigmaIEtaIEtaTempl(TTemplatesRandCone::TemplatesRandConePars &pars, TConfiguration::AnalysisParameters &anPars)
{

  TConfiguration config;
  TPhotonCuts photon;
  pars.varKin=anPars.varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=anPars.nKinBins;// number of analysis bins, max=50 (determined in TTemplatesRandCone.h)
  if (anPars.nKinBins>TTemplatesRandCone::nKinBinsMax){
    std::cout<<"nKinsBins="<<anPars.nKinBins<<", shouldn't exceed "<<TTemplatesRandCone::nKinBinsMax<<std::endl;
    return;
  }
  for (int ikb=0; ikb<=anPars.nKinBins; ikb++){
    pars.kinBinLims[ikb]=anPars.kinBinLims[ikb];// binning 15-20-25-30-35-40-55-75-95-500
    pars.nFitBins[ikb][config.BARREL]=32;
    pars.minVarFit[ikb][config.BARREL]=0.005;
    pars.maxVarFit[ikb][config.BARREL]=0.021;
    pars.nFitBins[ikb][config.ENDCAP]=25;
    pars.minVarFit[ikb][config.ENDCAP]=0.019;
    pars.maxVarFit[ikb][config.ENDCAP]=0.069;
    pars.sideband[ikb][config.BARREL]=1.5;//2.6;
    pars.sideband[ikb][config.ENDCAP]=1.2;//2.3;
    pars.sidebandUp[ikb][config.BARREL]=5.0;//15.0;
    pars.sidebandUp[ikb][config.ENDCAP]=5.0;//15.0;
    pars.combineTrueTempl[ikb][config.BARREL]=0;
    pars.combineTrueTempl[ikb][config.ENDCAP]=0;
    pars.combineFakeTempl[ikb][config.BARREL]=0;
    pars.combineFakeTempl[ikb][config.ENDCAP]=0;
  }
  if (anPars.varKin=="phoEt" && anPars.channel==config.MUON && anPars.vgamma==config.W_GAMMA)
    SetLimsSihihTempl_phoEt_Wg_MUON(pars);
  if (anPars.varKin=="phoEt" && anPars.channel==config.MUON && anPars.vgamma==config.Z_GAMMA)
    SetLimsSihihTempl_phoEt_Zg_MUON(pars);

  pars.strFileOutName=config.GetDDTemplateFileName(anPars.channel,anPars.vgamma,config.TEMPL_SIHIH,anPars.varKin);
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

 pars.strPlotsDir=config.GetPlotsDirName(anPars.channel, anPars.vgamma, config.PLOTS_TEMPL_FITS);
 pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_SIHIH)+TString("_")+config.StrBlindType(anPars.blind)+TString("_");

  std::cout<<std::endl;

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,anPars.blind,config.DATA);
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue=config.GetSelectedName(config.FSR,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
//  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.DATA);
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  pars.varSideband="phoPFChIsoCorr";//TString
  pars.varTrueTempl="phoSigmaIEtaIEta";//TString
  pars.varFakeTempl="phoSigmaIEtaIEta";//TString
  pars.varFit="phoSigmaIEtaIEta"; //TString
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  pars.cutNominal=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK) && photon.RangeSigmaIEtaIEta(2012, anPars.phoWP);//TCut; 
  pars.cutNominalExceptSidebandVar=photon.RangeSigmaIEtaIEta(2012, anPars.phoWP);//TCut; 
    //charged isolation cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
  pars.cutSidebandVarNominalRange=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    //must include barrel and endcap ((cutB && Barrel) || (cutE && Endcap))
//  pars.cutWeight="weight";//TCut; weight for signal MC tree

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut
  pars.cutAdd=anPars.cutAdd;
  pars.noLeakSubtr=0;

}

void SetValuesToKinEtaArray(int ieta, float vals[250], TTemplatesRandCone::TemplatesRandConePars &pars)
{
  TConfiguration config;
  for (int ik=0; ik<=pars.nKinBins; ik++){
    pars.nFitBins[ik][ieta]=(int)vals[0+7*ik];
    pars.minVarFit[ik][ieta]=    vals[1+7*ik];
    pars.maxVarFit[ik][ieta]=    vals[2+7*ik];
    pars.sideband[ik][ieta]=     vals[3+7*ik];
    pars.sidebandUp[ik][ieta]=   vals[4+7*ik];
    pars.combineTrueTempl[ik][ieta]=vals[5+7*ik];
    pars.combineFakeTempl[ik][ieta]=vals[6+7*ik];
    std::cout<<"ik="<<ik<<", ieta="<<ieta<<", nFitBins="<<pars.nFitBins[ik][ieta]<<", varFit: "<<pars.minVarFit[ik][ieta]<<"-"<<pars.maxVarFit[ik][ieta]<<", sideband: "<<pars.sideband[ik][ieta]<<"-"<<pars.sidebandUp[ik][ieta]<<"; combTrue="<<pars.combineTrueTempl[ik][ieta]<<"; combineFake="<<pars.combineFakeTempl[ik][ieta]<<std::endl;
  }
}// end of SetValuesToKinEtaArray

void SetLimsSihihTempl_phoEt_Wg_MUON( TTemplatesRandCone::TemplatesRandConePars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp, combTrue, combFake

    float valsB[250]={32, 0.005, 0.021, 1.5,  5.0, 0, 0,//Total
                      32, 0.005, 0.021, 1.5,  5.0, 0, 0,//15-20 GeV
                      32, 0.005, 0.021, 1.5,  5.0, 0, 0,//20-25 GeV
                      24, 0.006, 0.018, 1.5,  5.0, 0, 0,//25-30 GeV
                      12, 0.006, 0.018, 1.5,  5.0, 0, 0,//30-35 GeV
                      13, 0.005, 0.018, 1.5,  5.0, 0, 0,//35-40 GeV
                      13, 0.005, 0.018, 1.7,  5.0, 0, 0,//40-55 GeV
                       5, 0.005, 0.015, 2.5,  6.0, 1, 0,//55-75 GeV
                       5, 0.005, 0.015, 2.5,  6.0, 1, 1,//75-95 GeV
                       5, 0.005, 0.015, 2.5, 16.0, 1, 1};//95-500 GeV

    float valsE[250]={25, 0.019, 0.069, 1.1,  5.0, 0, 0,//Total
                      25, 0.019, 0.069, 1.1,  5.0, 0, 0,//15-20 GeV
                      25, 0.019, 0.069, 1.1,  5.0, 0, 0,//20-25 GeV
                      18, 0.019, 0.055, 1.2,  5.0, 0, 0,//25-30 GeV
                      18, 0.019, 0.055, 1.2,  5.0, 0, 0,//30-35 GeV
                      18, 0.019, 0.055, 1.2,  5.0, 0, 0,//35-40 GeV
                      13, 0.019, 0.045, 1.2,  5.0, 0, 0,//40-55 GeV
                       8, 0.021, 0.045, 1.8,  6.0, 1, 0,//55-75 GeV
                       6, 0.021, 0.045, 1.8,  6.0, 1, 1,//75-95 GeV
                       5, 0.018, 0.043, 1.8, 16.0, 1, 1};//95-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
}//end  of SetLimsSihihTempl_phoEt_Wg_MUON

void SetLimsSihihTempl_phoEt_Zg_MUON( TTemplatesRandCone::TemplatesRandConePars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={32, 0.005, 0.021, 1.5, 15.0, 0, 0,//Total
                      32, 0.005, 0.021, 1.5, 15.0, 0, 0,//15-20 GeV
                      32, 0.005, 0.021, 1.5, 15.0, 0, 0,//20-25 GeV
                      24, 0.006, 0.018, 1.5, 15.0, 0, 0,//25-30 GeV
                      12, 0.006, 0.018, 1.5, 15.0, 0, 0,//30-35 GeV
                      13, 0.005, 0.018, 1.5, 15.0, 0, 0,//35-40 GeV
                       6, 0.005, 0.017, 1.7, 15.0, 0, 0,//40-55 GeV
                       4, 0.005, 0.017, 2.5, 15.0, 1, 1,//55-75 GeV
                       3, 0.005, 0.014, 1.5, 15.0, 1, 1,//75-95 GeV
                       3, 0.005, 0.014, 1.5, 15.0, 1, 1};//95-500 GeV

    float valsE[250]={25, 0.019, 0.069, 1.2, 15.0, 0, 0,//Total
                      25, 0.019, 0.069, 1.2, 15.0, 0, 0,//15-20 GeV
                      18, 0.019, 0.055, 1.2, 15.0, 0, 0,//20-25 GeV
                      18, 0.019, 0.055, 1.2, 15.0, 0, 0,//25-30 GeV
                       9, 0.021, 0.039, 1.2, 15.0, 0, 0,//30-35 GeV
                      10, 0.019, 0.039, 1.2, 15.0, 0, 0,//35-40 GeV
                      13, 0.019, 0.045, 1.2, 15.0, 0, 0,//40-55 GeV
                       4, 0.023, 0.043, 1.8, 15.0, 1, 1,//55-75 GeV
                       4, 0.023, 0.043, 1.2, 15.0, 1, 1,//75-95 GeV
                       4, 0.023, 0.043, 1.2, 15.0, 1, 1};//95-500 GeV 
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
}//end  of SetLimsSihihTempl_phoEt_Zg_MUON


void SetLimsChIsoTempl_phoEt_Wg_MUON( TTemplatesRandCone::TemplatesRandConePars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//35-40 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//40-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.018, 0, 0,//55-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//75-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0};//95-500 GeV

    float valsE[250]={21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//35-40 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//40-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//55-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0,//75-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.030, 0.053, 0, 0};//95-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars); 
                 
}//end  of SetLimsChIsoTempl_phoEt_Wg_MUON

void SetLimsChIsoTempl_phoEt_Zg_MUON( TTemplatesRandCone::TemplatesRandConePars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//35-40 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//40-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 0,//55-75 GeV
                      11, -2.0+0.1, 20.0+0.1, 0.011, 0.018, 0, 1,//75-95 GeV
                      10, -2.0+0.1, 18.0+0.1, 0.011, 0.018, 0, 1};//95-500 GeV

    float valsE[250]={21, -1.0+0.1, 20.0+0.1, 0.032, 0.053, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.032, 0.053, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.032, 0.053, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.032, 0.053, 0, 0,//25-30 GeV
                      15, -1.0+0.1, 14.0+0.1, 0.032, 0.053, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.032, 0.053, 0, 0,//35-40 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.032, 0.053, 0, 0,//40-55 GeV
                       9, -2.0+0.1, 16.0+0.1, 0.033, 0.053, 1, 0,//55-75 GeV
                       9, -2.0+0.1, 16.0+0.1, 0.033, 0.053, 1, 1,//75-95 GeV
                       5, -3.0+0.1, 12.0+0.1, 0.033, 0.053, 1, 1};//95-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
     
}//end  of SetLimsChIsoTempl_phoEt_Zg_MUON
