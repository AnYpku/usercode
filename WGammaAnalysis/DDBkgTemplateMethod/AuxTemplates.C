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

//void SetValuesToKinEtaArray(int ieta, float vals[250], TTemplates::TemplatesPars &pars);

void AuxTemplates(TConfiguration::AnalysisParameters &anPars, bool isMCclosure)
{
  //this function is called in FullChain

  TTemplates::TemplatesPars pars;


  if (anPars.templFits==TConfiguration::TEMPL_SIHIH) 
    SetParsSigmaIEtaIEtaTempl(pars, anPars);

  if (anPars.templFits==TConfiguration::TEMPL_CHISO) 
    SetParsChIsoTempl(pars, anPars, isMCclosure);

  TConfiguration conf;
  TFile* fSbs = new TFile(conf.GetSidebandsFileName(anPars.channel, anPars.vgamma, anPars.templFits, anPars.varKin));
  TH1F* hSbL[2];
  TH1F* hSbU[2];

  for (int ieta=conf.BARREL; ieta<=conf.ENDCAP; ieta++){
    
    hSbL[ieta] = (TH1F*)fSbs->Get(conf.GetSidebandsLowerHistName(ieta));
    hSbU[ieta] = (TH1F*)fSbs->Get(conf.GetSidebandsUpperHistName(ieta));
    for (int ikin=1; ikin<=anPars.nKinBins; ikin++){
        pars.sideband[ikin][ieta]=hSbL[ieta]->GetBinContent(ikin);
        pars.sidebandUp[ikin][ieta]=hSbU[ieta]->GetBinContent(ikin);
    }//end of loop over ikin
    
  }// end of loop over ieta

  pars.isMCclosureMode=isMCclosure;
  pars.isRooFit=1;//0;
  
  TTemplates temp(pars);
  temp.ComputeBackground();
  
}// end of AuxTemplates()

void AuxTemplatesSystSidebandVariation(TConfiguration::AnalysisParameters &anPars, bool isMCclosure)
{
  //this function is called in FullChain
  TConfiguration conf;
  TTemplates::TemplatesPars pars;
  TTemplatesSyst::TemplatesSidebandVariationPars variationPars;

  if (anPars.templFits==TConfiguration::TEMPL_SIHIH) {

    SetParsSigmaIEtaIEtaTempl(pars, anPars);

    // sideband range can not overlap with nominal range
    // nominal for Barrel is <1.5; for Endcap <1.2
    
    for (int ieta=conf.BARREL; ieta<=conf.ENDCAP; ieta++){
      variationPars.nPointsLower[ieta]=6+1;//6;//30;
      variationPars.lowerSidebandCutFrom[ieta]=1.5;
      variationPars.lowerSidebandCutTo[ieta]=3.5;
      variationPars.nPointsUpper[ieta]=13+1;//14;//30; 
      variationPars.upperSidebandCutFrom[ieta]=2.0;
      variationPars.upperSidebandCutTo[ieta]=15.0;
    }            
  }

  if (anPars.templFits==TConfiguration::TEMPL_CHISO) {

    SetParsChIsoTempl(pars, anPars, isMCclosure);

    // sideband range can not overlap with nominal range
    // nominal for Barrel is <0.011; for Endcap <0.033

    variationPars.nPointsLower[conf.BARREL]=4+1;//10+1;
    variationPars.lowerSidebandCutFrom[conf.BARREL]=0.011;
    variationPars.lowerSidebandCutTo[conf.BARREL]=0.015;
    variationPars.nPointsUpper[conf.BARREL]=9+1;//9+1;
    variationPars.upperSidebandCutFrom[conf.BARREL]=0.012;
    variationPars.upperSidebandCutTo[conf.BARREL]=0.021;

    variationPars.nPointsLower[conf.ENDCAP]=8+1;//15+1;
    variationPars.lowerSidebandCutFrom[conf.ENDCAP]=0.033;
    variationPars.lowerSidebandCutTo[conf.ENDCAP]=0.049;
    variationPars.nPointsUpper[conf.ENDCAP]=15+1;//20+1;
    variationPars.upperSidebandCutFrom[conf.ENDCAP]=0.035;
    variationPars.upperSidebandCutTo[conf.ENDCAP]=0.065;
  }

  pars.strPlotsDir=conf.GetPlotsDirName(anPars.channel, anPars.vgamma, conf.PLOTS_TEMPL_FITS_SYST);
  //pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_CHISO)+TString("_")+config.StrBlindType(anPars.blind[anPars.channel][anPars.vgamma])+TString("_");
  pars.strPlotsBaseName.Replace(0,1,"cSyst_");

  pars.isMCclosureMode=isMCclosure;
  pars.isRooFit=1;//0;

  TTemplatesSyst temp(pars,variationPars);
  temp.SidebandVariation();
}

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
  pars.varKin=anPars.varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=anPars.nKinBins;// number of analysis bins, max=50 (determined in TTemplates.h)
  if (anPars.nKinBins>TTemplates::nKinBinsMax){
    std::cout<<"nKinsBins="<<anPars.nKinBins<<", shouldn't exceed "<<TTemplates::nKinBinsMax<<std::endl;
    return;
  }
  pars.thresholdCombineTrueTemplates=29.9;
  pars.thresholdCombineFakeTemplates=54.9;
  for (int ikb=0; ikb<=anPars.nKinBins; ikb++){
    pars.kinBinLims[ikb]=anPars.kinBinLims[ikb];// binning 15-20-25-30-35-45-55-65-75-85-95-120-500
    for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
      pars.nFitBins[ikb][ieta]=21;
      pars.minVarFit[ikb][ieta]=-1.0+0.1;
      pars.maxVarFit[ikb][ieta]=20.0+0.1;
 //     pars.combineTrueTempl[ikb][ieta]=0;
 //     pars.combineFakeTempl[ikb][ieta]=0;
//      if (pars.kinBinLims[ikb]>29) pars.combineTrueTempl[ikb][ieta]=1;
//      if (pars.kinBinLims[ikb]>54) pars.combineFakeTempl[ikb][ieta]=1;
    } 
    pars.sideband[ikb][config.BARREL]=0.011;
    pars.sideband[ikb][config.ENDCAP]=0.033;
    pars.sidebandUp[ikb][config.BARREL]=0.014;//0.018;
    pars.sidebandUp[ikb][config.ENDCAP]=0.044;//0.053;
      //for these arrays, nFitBins[ikin][ieta], 
      //maxVarFit[ikin][ieta], sideband[ikin][ieta]
      // ikin=0 stands for total fit (e.g. 15-500)
      // ikin=[1,nKinBins] are for individual bin fits 
      // ieta=0 - BARREL, ieta=1 - ENDCAP
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

 pars.strPlotsDir=config.GetPlotsDirName(anPars.channel, anPars.vgamma, config.PLOTS_TEMPL_FITS);
 pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_CHISO)+TString("_")+config.StrBlindType(anPars.blind[anPars.channel][anPars.vgamma])+TString("_");

  std::cout<<std::endl;

  TString strData=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.DATA);
  if (isMCclosure) strData.ReplaceAll(".root","_MCclosure.root");
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue="../WGammaOutput/MERGED/selected_WGamma_ForTemplates.root";
  if (isMCclosure) strTrue="../WGammaOutput/MERGED/selected_WGamma_ForTemplates_MCclosure.root";
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

  TString strFake="../WGammaOutput/MERGED/selected_WGamma_ForTemplates.root";
  if (isMCclosure) strFake="../WGammaOutput/MERGED/selected_WGamma_ForTemplates_MCclosure.root";
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
  pars.varTrueTempl="phoRandConeChIsoCorr";//"phoRandConeChIso04Corr";//TString
  pars.varFakeTempl="phoSCRChIsoCorr";//"phoSCRChIso04Corr";//TString
  pars.varFit="phoSCRChIsoCorr";//"phoSCRChIso04Corr"; //TString
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
    pars.cutNominal[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK, ieta) 
                    && photon.RangeSigmaIEtaIEta(2012, anPars.phoWP, ieta);//TCut; 
    pars.cutNominalExceptSidebandVar[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK, ieta);//TCut; 
      //charged isolation cut as applied during selection procedure;
    pars.cutSidebandVarNominalRange[ieta]=photon.RangeSigmaIEtaIEta(2012, anPars.phoWP, ieta);//TCut;
      //phoSigmaIEtaIEta cut as applied during selection procedure;
    std::cout<<"in AuxTemplatesRandomCone"<<std::endl;
    std::cout<<"ieta="<<config.StrEtaBin(ieta)<<std::endl;
    std::cout<<"cutNominal="<<pars.cutNominal[ieta].GetTitle()<<std::endl;
    std::cout<<"cutNominalExceptSidebandVar="<<pars.cutNominalExceptSidebandVar[ieta].GetTitle()<<std::endl;
    std::cout<<"cutSidebandVarNominalRange="<<pars.cutSidebandVarNominalRange[ieta].GetTitle()<<std::endl;
  }
//  pars.cutWeight="weight";//TCut; weight for signal MC tree

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut
  pars.cutAdd=anPars.cutAdd;
  pars.noLeakSubtr=0;

}// end of SetParsChIsoTempl()



void SetParsSigmaIEtaIEtaTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars)
{

  TConfiguration config;
  TPhotonCuts photon;
  pars.varKin=anPars.varKin;// usually phoEt, could be any other kinematic variable availiable in treeData and treeSign
  pars.nKinBins=anPars.nKinBins;// number of analysis bins, max=50 (determined in TTemplates.h)
  if (anPars.nKinBins>TTemplates::nKinBinsMax){
    std::cout<<"nKinsBins="<<anPars.nKinBins<<", shouldn't exceed "<<TTemplates::nKinBinsMax<<std::endl;
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
  }//end

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
  pars.varPhoEta="phoSCEta";//TString
  pars.varWeight="weight";//TString

  for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
    pars.cutNominal[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK,ieta) 
                    && photon.RangeSigmaIEtaIEta(2012, anPars.phoWP, ieta);//TCut; 
    pars.cutNominalExceptSidebandVar[ieta]=photon.RangeSigmaIEtaIEta(2012, anPars.phoWP, ieta);//TCut; 
    //charged isolation cut as applied during selection procedure;
    pars.cutSidebandVarNominalRange[ieta]=photon.RangeOneIsolation(2012,anPars.phoWP,photon.ISO_CHorTRK, ieta);//TCut;
    //phoSigmaIEtaIEta cut as applied during selection procedure;
    std::cout<<"in AuxTemplatesRandomCone"<<std::endl;
    std::cout<<"ieta="<<config.StrEtaBin(ieta)<<std::endl;
    std::cout<<"cutNominal="<<pars.cutNominal[ieta].GetTitle()<<std::endl;
    std::cout<<"cutNominalExceptSidebandVar="<<pars.cutNominalExceptSidebandVar[ieta].GetTitle()<<std::endl;
    std::cout<<"cutSidebandVarNominalRange="<<pars.cutSidebandVarNominalRange[ieta].GetTitle()<<std::endl;
  }

  pars.cutBarrel=photon.RangeBarrel();//TCut
  pars.cutEndcap=photon.RangeEndcap();//TCut
  pars.cutAdd=anPars.cutAdd;
  pars.noLeakSubtr=0;

}// end of SetParsSigmaIEtaIEtaTempl()
/*
void SetValuesToKinEtaArray(int ieta, float vals[250], TTemplates::TemplatesPars &pars)
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
*/
