#include "TTemplates.h"
#include "TTemplatesSyst.h"
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

void SetParsSigmaIEtaIEtaTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars);

void SetParsChIsoTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars, bool isMCclosure);


void AuxTemplates(TConfiguration::AnalysisParameters &anPars, bool isMCclosure)
{
  //this function is called in FullChain
  TConfiguration config;
  TPhotonCuts photon;

  TTemplates::TemplatesPars pars;

  pars.varKin=anPars.varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=anPars.nKinBins;// number of analysis bins, max=50 (determined in TTemplates.h)
  if (anPars.nKinBins>TTemplates::nKinBinsMax){
    std::cout<<"nKinsBins="<<anPars.nKinBins<<", shouldn't exceed "<<TTemplates::nKinBinsMax<<std::endl;
    return;
  }

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

  pars.isMCclosureMode=isMCclosure;
  pars.isRooFit=1;//0;

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut
  pars.cutAdd=anPars.cutAdd;
  pars.noLeakSubtrTrueToFake=0;
  pars.noLeakSubtrFakeToTrue=0;

  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
    pars.cutNominal[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK, ieta) 
                    && photon.RangeSigmaIEtaIEta(2012, anPars.phoWP, ieta);//TCut; 
    pars.cutNominalExceptSidebandVar[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK, ieta);//TCut; 
    //pars.cutNominalExceptSidebandVar[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CH_RandCone, ieta);//TCut;
      //charged isolation cut as applied during selection procedure;
    pars.cutSidebandVarNominalRange[ieta]=photon.RangeSigmaIEtaIEta(2012, anPars.phoWP, ieta);//TCut;
      //phoSigmaIEtaIEta cut as applied during selection procedure;
    std::cout<<"in AuxTemplatesRandomCone"<<std::endl;
    std::cout<<"ieta="<<config.StrEtaBin(ieta)<<std::endl;
    std::cout<<"cutNominal="<<pars.cutNominal[ieta].GetTitle()<<std::endl;
    std::cout<<"cutNominalExceptSidebandVar="<<pars.cutNominalExceptSidebandVar[ieta].GetTitle()<<std::endl;
    std::cout<<"cutSidebandVarNominalRange="<<pars.cutSidebandVarNominalRange[ieta].GetTitle()<<std::endl;
  }

  pars.strPlotsDir=config.GetPlotsDirName(anPars.channel, anPars.vgamma, config.PLOTS_TEMPL_FITS);

  if (anPars.templFits==TConfiguration::TEMPL_SIHIH) 
    SetParsSigmaIEtaIEtaTempl(pars, anPars);

  if (anPars.templFits==TConfiguration::TEMPL_CHISO) 
    SetParsChIsoTempl(pars, anPars, isMCclosure);
  
  TTemplates temp(pars);
  temp.ComputeBackground();
  
}// end of AuxTemplates()


TTree* LoadOneTree(TString strFileWithWhat, TString strFileName, TFile* f)
{
  f = new TFile(strFileName);
  if (!f->IsOpen()){
    std::cout<<"ERROR in AuxTemplates, SetParsRegularCases: file with "<<strFileWithWhat<<": "<<strFileName<<" can't be open"<<std::endl;
    return 0;
  }
  else std::cout<<"file with "<<strFileWithWhat<<": "<<strFileName<<std::endl;
  return (TTree*)f->Get("selectedEvents");
}

void SetParsChIsoTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars, bool isMCclosure)
{

  TConfiguration config;
  TPhotonCuts photon;

  pars.thresholdCombineTrueTemplates=0;//combine all
  pars.thresholdCombineFakeTemplates=10000;//never combine
  for (int ikb=0; ikb<=anPars.nKinBins; ikb++){
    pars.kinBinLims[ikb]=anPars.kinBinLims[ikb];// binning 15-20-25-30-35-45-55-65-75-85-95-120-500
    for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
      pars.nFitBins[ikb][ieta]=21;
      pars.minVarFit[ikb][ieta]=-1.0+0.1;
      pars.maxVarFit[ikb][ieta]=20.0+0.1;
    } 
    pars.sideband[ikb][config.BARREL]=0.000;
    pars.sideband[ikb][config.ENDCAP]=0.000;
    pars.sidebandUp[ikb][config.BARREL]=0.011;//0.018;
    pars.sidebandUp[ikb][config.ENDCAP]=0.033;//0.053;
      //for these arrays, nFitBins[ikin][ieta], 
      //maxVarFit[ikin][ieta], sideband[ikin][ieta]
      // ikin=0 stands for total fit (e.g. 15-500)
      // ikin=[1,nKinBins] are for individual bin fits 
      // ieta=0 - BARREL, ieta=1 - ENDCAP
  }

 pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_CHISO)+TString("_")+config.StrBlindType(anPars.blind[anPars.channel][anPars.vgamma])+TString("_");

  std::cout<<std::endl;

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.DATA);
  if (isMCclosure) strData.ReplaceAll(".root","_MCclosure.root");
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign="../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGammaSIGMC.root";
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strFakeToTrue="../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGammaBKGMC_DYjets_to_ll.root";
  pars.treeFakeToTrue=LoadOneTree("leak_FakeToTrue", strFakeToTrue, pars.fFakeToTrue); 
  if (!pars.treeFakeToTrue) return;

  TString strTrue="../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root";
  if (isMCclosure) strTrue="../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA_MCclosure.root";
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake="../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root";
  if (isMCclosure) strFake="../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA_MCclosure.root";
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  TString bkgFakeMC;
  if (anPars.vgamma==config.Z_GAMMA) bkgFakeMC="DYjets_to_ll";
  if (anPars.vgamma==config.W_GAMMA) bkgFakeMC="Wjets_to_lnu";
  TString strFakeRef=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.BKGMC,bkgFakeMC);
//  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeFakeRef=LoadOneTree("fake-pho ref", strFakeRef, pars.fFakeRef);
  if (!pars.treeFakeRef) return;

  pars.varSideband="phoSigmaIEtaIEta";//TString
//  pars.varTrueTempl="phoRandConeChIsoCorr";//"phoRandConeChIso04Corr";//TString
  pars.varTrueTempl="phoPFChIsoCorr";//"phoRandConeChIso04Corr";//TString
  pars.varFakeTempl="phoPFChIsoCorr";//"phoSCRChIso04Corr";//TString
  pars.varFit="phoPFChIsoCorr";//"phoSCRChIso04Corr"; //TString

//  pars.cutWeight="weight";//TCut; weight for signal MC tree

}// end of SetParsChIsoTempl()



void SetParsSigmaIEtaIEtaTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars)
{

  TConfiguration config;
  TPhotonCuts photon;

  pars.thresholdCombineTrueTemplates=54.9;
  pars.thresholdCombineFakeTemplates=54.9;
  for (int ikb=0; ikb<=anPars.nKinBins; ikb++){
    pars.kinBinLims[ikb]=anPars.kinBinLims[ikb];// binning 15-20-25-30-35-45-55-65-75-85-95-120-500
    pars.nFitBins[ikb][config.BARREL]=32;
    pars.minVarFit[ikb][config.BARREL]=0.005;
    pars.maxVarFit[ikb][config.BARREL]=0.021;
    pars.nFitBins[ikb][config.ENDCAP]=25;
    pars.minVarFit[ikb][config.ENDCAP]=0.019;
    pars.maxVarFit[ikb][config.ENDCAP]=0.069;
    pars.sideband[ikb][config.BARREL]=0.0;
    pars.sideband[ikb][config.ENDCAP]=0.0;
    pars.sidebandUp[ikb][config.BARREL]=1.5;
    pars.sidebandUp[ikb][config.ENDCAP]=1.2;
  }//end

 pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_SIHIH)+TString("_")+config.StrBlindType(anPars.blind[anPars.channel][anPars.vgamma])+TString("_");

  std::cout<<std::endl;

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.DATA);
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue=config.GetSelectedName(config.FSR,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
//  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.SIGMC);
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.DATA);
//  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  TString bkgFakeMC;
  if (anPars.vgamma==config.Z_GAMMA) bkgFakeMC="DYjets_to_ll";
  if (anPars.vgamma==config.W_GAMMA) bkgFakeMC="Wjets_to_lnu";
  TString strFakeRef=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.BKGMC,bkgFakeMC);
//  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeFakeRef=LoadOneTree("fake-pho ref", strFakeRef, pars.fFakeRef);
  if (!pars.treeFakeRef) return;


  pars.varSideband="phoSCRChIsoCorr";//TString
  pars.varTrueTempl="phoSigmaIEtaIEta";//TString
  pars.varFakeTempl="phoSigmaIEtaIEta";//TString
  pars.varFit="phoSigmaIEtaIEta"; //TString

}// end of SetParsSigmaIEtaIEtaTempl()

