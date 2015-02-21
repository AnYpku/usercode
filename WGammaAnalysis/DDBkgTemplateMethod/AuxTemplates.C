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

void SetParsChIsoTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars);

void SetLimsChIsoTempl_phoEt_Wg_MUON(TTemplates::TemplatesPars &pars);
void SetLimsChIsoTempl_phoEt_Zg_MUON(TTemplates::TemplatesPars &pars);
void SetLimsSihihTempl_phoEt_Wg_MUON(TTemplates::TemplatesPars &pars);
void SetLimsSihihTempl_phoEt_Zg_MUON(TTemplates::TemplatesPars &pars);
void SetLimsChIsoTempl_phoEt_Wg_ELECTRON(TTemplates::TemplatesPars &pars);
void SetLimsChIsoTempl_phoEt_Zg_ELECTRON(TTemplates::TemplatesPars &pars);
void SetLimsSihihTempl_phoEt_Wg_ELECTRON(TTemplates::TemplatesPars &pars);
void SetLimsSihihTempl_phoEt_Zg_ELECTRON(TTemplates::TemplatesPars &pars);

void SetValuesToKinEtaArray(int ieta, float vals[250], TTemplates::TemplatesPars &pars);

void AuxTemplates(TConfiguration::AnalysisParameters &anPars)
{
  //this function is called in FullChain

  TTemplates::TemplatesPars pars;


  if (anPars.templFits==TConfiguration::TEMPL_SIHIH) 
    SetParsSigmaIEtaIEtaTempl(pars, anPars);

  if (anPars.templFits==TConfiguration::TEMPL_CHISO) 
    SetParsChIsoTempl(pars, anPars);


  TTemplates temp(pars);
  temp.ComputeBackground();
}

void AuxTemplatesSystSidebandVariation(TConfiguration::AnalysisParameters &anPars)
{
  //this function is called in FullChain
  TConfiguration conf;
  TTemplates::TemplatesPars pars;
  TTemplatesSyst::TemplatesSidebandVariationPars variationPars;

  if (anPars.templFits==TConfiguration::TEMPL_SIHIH) {

    SetParsSigmaIEtaIEtaTempl(pars, anPars);
    
    for (int ieta=conf.BARREL; ieta<=conf.ENDCAP; ieta++){
      variationPars.nPointsLower[ieta]=6+1;//6;//30;
      variationPars.lowerSidebandCutFrom[ieta]=0.1;
      variationPars.lowerSidebandCutTo[ieta]=3.1;
      variationPars.nPointsUpper[ieta]=14+1;//14;//30; 
      variationPars.upperSidebandCutFrom[ieta]=1.1;
      variationPars.upperSidebandCutTo[ieta]=15.1;
    }            
  }

  if (anPars.templFits==TConfiguration::TEMPL_CHISO) {

    SetParsChIsoTempl(pars, anPars);

    variationPars.nPointsLower[conf.BARREL]=10+1;//10;//10;//16;
    variationPars.lowerSidebandCutFrom[conf.BARREL]=0.005;
    variationPars.lowerSidebandCutTo[conf.BARREL]=0.015;
    variationPars.nPointsUpper[conf.BARREL]=9+1;//10;//10;//16;
    variationPars.upperSidebandCutFrom[conf.BARREL]=0.012;
    variationPars.upperSidebandCutTo[conf.BARREL]=0.021;

    variationPars.nPointsLower[conf.ENDCAP]=15+1;//12;//12;//16;
    variationPars.lowerSidebandCutFrom[conf.ENDCAP]=0.019;
    variationPars.lowerSidebandCutTo[conf.ENDCAP]=0.049;
    variationPars.nPointsUpper[conf.ENDCAP]=15+1;//15;//15;//16;
    variationPars.upperSidebandCutFrom[conf.ENDCAP]=0.027;
    variationPars.upperSidebandCutTo[conf.ENDCAP]=0.067;
  }

  pars.strPlotsDir=conf.GetPlotsDirName(anPars.channel, anPars.vgamma, conf.PLOTS_TEMPL_FITS_SYST);
  //pars.strPlotsBaseName=TString("c_")+config.StrTempl(config.TEMPL_CHISO)+TString("_")+config.StrBlindType(anPars.blind[anPars.channel][anPars.vgamma])+TString("_");
  pars.strPlotsBaseName.Replace(0,1,"cSyst_");

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

void SetParsChIsoTempl(TTemplates::TemplatesPars &pars, TConfiguration::AnalysisParameters &anPars)
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

  if (anPars.varKin=="phoEt" && anPars.channel==config.ELECTRON && anPars.vgamma==config.W_GAMMA)
    SetLimsChIsoTempl_phoEt_Wg_ELECTRON(pars);

  if (anPars.varKin=="phoEt" && anPars.channel==config.ELECTRON && anPars.vgamma==config.Z_GAMMA)
    SetLimsChIsoTempl_phoEt_Zg_ELECTRON(pars);

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
  pars.treeData=LoadOneTree("data", strData, pars.fData);
  if (!pars.treeData) return;

  TString strSign=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.SIGMC);
  pars.treeSign=LoadOneTree("signalMC", strSign, pars.fSign); 
  if (!pars.treeSign) return;

  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.DATA);
//  TString strTrue=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeTrue=LoadOneTree("true-pho template", strTrue, pars.fTrue);
  if (!pars.treeTrue) return;

//  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.DATA);
  pars.treeFake=LoadOneTree("fake-pho template", strFake, pars.fFake);
  if (!pars.treeFake) return;

  TString bkgFakeMC;
  if (anPars.vgamma==config.Z_GAMMA) bkgFakeMC="DYjets_to_ll";
  if (anPars.vgamma==config.W_GAMMA) bkgFakeMC="Wjets_to_lnu";
  TString strFakeRef=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,anPars.vgamma,config.UNBLIND,config.BKGMC,bkgFakeMC);
//  TString strFake=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,anPars.channel,config.Z_GAMMA,config.UNBLIND,config.DATA);
  pars.treeFakeRef=LoadOneTree("fake-pho ref", strFakeRef, pars.fFakeRef);
  if (!pars.treeFakeRef) return;

  pars.showTreeRef=1;
  pars.sumOverHist=0;

  pars.varSideband="phoSigmaIEtaIEta";//TString
  pars.varTrueTempl="phoRandConeChIso04Corr";//"phoRandConeChIso04Corr";//TString
  pars.varFakeTempl="phoSCRChIso04Corr";//"phoSCRChIso04Corr";//TString
  pars.varFit="phoSCRChIso04Corr";//"phoSCRChIso04Corr"; //TString
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
    pars.combineTrueTempl[ikb][config.BARREL]=0;
    pars.combineTrueTempl[ikb][config.ENDCAP]=0;
    pars.combineFakeTempl[ikb][config.BARREL]=0;
    pars.combineFakeTempl[ikb][config.ENDCAP]=0;
  }
  if (anPars.varKin=="phoEt" && anPars.channel==config.MUON && anPars.vgamma==config.W_GAMMA)
    SetLimsSihihTempl_phoEt_Wg_MUON(pars);
  if (anPars.varKin=="phoEt" && anPars.channel==config.MUON && anPars.vgamma==config.Z_GAMMA)
    SetLimsSihihTempl_phoEt_Zg_MUON(pars);
  if (anPars.varKin=="phoEt" && anPars.channel==config.ELECTRON && anPars.vgamma==config.W_GAMMA)
    SetLimsSihihTempl_phoEt_Wg_ELECTRON(pars);
  if (anPars.varKin=="phoEt" && anPars.channel==config.ELECTRON && anPars.vgamma==config.Z_GAMMA)
    SetLimsSihihTempl_phoEt_Zg_ELECTRON(pars);

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

  pars.showTreeRef=1;
  pars.sumOverHist=0;

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

}

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

void SetLimsSihihTempl_phoEt_Wg_MUON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp, combTrue, combFake

    float valsB[250]={32, 0.005, 0.021, 0.1, 1.1, 0, 0,//Total
                      32, 0.005, 0.021, 0.1, 1.1, 0, 0,//15-20 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 0, 0,//20-25 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 0, 0,//25-30 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 0, 0,//30-35 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 0, 0,//35-45 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 0, 0,//45-55 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1,//55-65 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1,//65-75 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1,//75-85 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1,//85-95 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1,//95-120 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1};//120-500 GeV

    float valsE[250]={48, 0.019, 0.067, 0.1, 1.1, 0, 0,//Total
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//15-20 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//20-25 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//25-30 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 0, 0,//30-35 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//35-45 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//45-55 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 1, 1,//55-65 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 1, 1,//65-75 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 1, 1,//75-85 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 1, 1,//85-95 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 1, 1,//95-120 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 1, 1};//120-500 GeV 

                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
}//end  of SetLimsSihihTempl_phoEt_Wg_MUON

void SetLimsSihihTempl_phoEt_Zg_MUON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={32, 0.005, 0.021, 2.6, 3.1, 0, 0,//Total
                      32, 0.005, 0.021, 2.6, 3.1, 0, 0,//15-20 GeV
                      32, 0.005, 0.021, 1.1, 7.1, 0, 0,//20-25 GeV
                      32, 0.005, 0.021, 2.1, 5.1, 0, 0,//25-30 GeV
                      32, 0.005, 0.021, 1.1, 2.1, 0, 0,//30-35 GeV
                      32, 0.005, 0.021, 1.1, 2.1, 0, 0,//35-45 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 0, 0,//45-55 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 1, 1,//55-65 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 1, 1,//65-75 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 1, 1,//75-85 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 1, 1,//85-95 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 1, 1,//95-120 GeV
                      32, 0.005, 0.021, 0.6, 1.1, 1, 1};//120-500 GeV

    float valsE[250]={48, 0.019, 0.067, 0.1, 1.1, 0, 0,//Total
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//15-20 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//20-25 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//25-30 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//30-35 GeV
                      48, 0.019, 0.067, 2.1, 3.1, 0, 0,//35-45 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 0, 0,//45-55 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1,//55-65 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 1, 1,//65-75 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 1, 1,//75-85 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 1, 1,//85-95 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 1, 1,//95-120 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 1, 1};//120-500 GeV 
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
}//end  of SetLimsSihihTempl_phoEt_Zg_MUON


void SetLimsChIsoTempl_phoEt_Wg_MUON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={21, -1.0+0.1, 20.0+0.1, 0.009, 0.014, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.009, 0.014, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.009, 0.019, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.009, 0.019, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.020, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.009, 0.020, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.019, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.009, 0.012, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.013, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.013, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.013, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.013, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.013, 1, 1};//120-500 GeV

    float valsE[250]={21, -1.0+0.1, 20.0+0.1, 0.027, 0.035, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.027, 0.035, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.021, 0.043, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.019, 0.046, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.021, 0.051, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.021, 0.059, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.023, 0.046, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.023, 0.046, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.046, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.046, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.046, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.046, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.046, 1, 1};//120-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
                 
}//end  of SetLimsChIsoTempl_phoEt_Wg_MUON

void SetLimsChIsoTempl_phoEt_Zg_MUON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={21, -1.0+0.1, 20.0+0.1, 0.009, 0.014, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.009, 0.014, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.014, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.012, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.014, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.019, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.017, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.021, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.012, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.012, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.012, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.012, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.010, 0.012, 1, 1};//120-500 GeV

    float valsE[250]={21, -1.0+0.1, 20.0+0.1, 0.019, 0.040, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.019, 0.040, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.048, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.023, 0.067, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.031, 0.064, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.030, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.019, 0.027, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.019, 0.027, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.056, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.056, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.056, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.056, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.056, 1, 1};//120-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
     
}//end  of SetLimsChIsoTempl_phoEt_Zg_MUON

void SetLimsSihihTempl_phoEt_Wg_ELECTRON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp, combTrue, combFake

    float valsB[250]={32, 0.005, 0.021, 0.1, 2.1, 0, 0,//Total
                      32, 0.005, 0.021, 0.1, 2.1, 0, 0,//15-20 GeV
                      32, 0.005, 0.021, 0.1, 1.1, 0, 0,//20-25 GeV
                      32, 0.005, 0.021, 1.1, 2.1, 0, 0,//25-30 GeV
                      32, 0.005, 0.021, 1.1, 3.1, 0, 0,//30-35 GeV
                      32, 0.005, 0.021, 0.1, 5.1, 0, 0,//35-45 GeV
                      32, 0.005, 0.021, 0.1, 7.1, 0, 0,//45-55 GeV
                      32, 0.005, 0.021, 0.1, 8.1, 1, 1,//55-65 GeV
                      32, 0.005, 0.021, 1.6, 4.1, 1, 1,//65-75 GeV
                      32, 0.005, 0.021, 1.6, 4.1, 1, 1,//75-85 GeV
                      32, 0.005, 0.021, 1.6, 4.1, 1, 1,//85-95 GeV
                      32, 0.005, 0.021, 1.6, 4.1, 1, 1,//95-120 GeV
                      32, 0.005, 0.021, 1.6, 4.1, 1, 1};//120-500 GeV

    float valsE[250]={48, 0.019, 0.067, 0.1, 8.1, 0, 0,//Total
                      48, 0.019, 0.067, 0.1, 8.1, 0, 0,//15-20 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 0, 0,//20-25 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//25-30 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//30-35 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//35-45 GeV
                      48, 0.019, 0.067, 2.1, 4.1, 0, 0,//45-55 GeV
                      48, 0.019, 0.067, 1.6, 2.1, 1, 1,//55-65 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1,//65-75 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1,//75-85 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1,//85-95 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1,//95-120 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1};//120-500 GeV 

                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
}//end  of SetLimsSihihTempl_phoEt_Wg_ELECTRON

void SetLimsSihihTempl_phoEt_Zg_ELECTRON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={32, 0.005, 0.021, 0.6, 2.1, 0, 0,//Total
                      32, 0.005, 0.021, 0.6, 2.1, 0, 0,//15-20 GeV
                      32, 0.005, 0.021, 0.1, 9.1, 0, 0,//20-25 GeV
                      32, 0.005, 0.021, 0.1, 3.1, 0, 0,//25-30 GeV
                      32, 0.005, 0.021, 1.1, 8.1, 0, 0,//30-35 GeV
                      32, 0.005, 0.021, 3.1, 15.1, 0, 0,//35-45 GeV
                      32, 0.005, 0.021, 2.1, 6.1, 0, 0,//45-55 GeV
                      32, 0.005, 0.021, 1.6, 3.1, 1, 1,//55-65 GeV
                      32, 0.005, 0.021, 0.1, 2.1, 1, 1,//65-75 GeV
                      32, 0.005, 0.021, 0.1, 2.1, 1, 1,//75-85 GeV
                      32, 0.005, 0.021, 0.1, 2.1, 1, 1,//85-95 GeV
                      32, 0.005, 0.021, 0.1, 2.1, 1, 1,//95-120 GeV
                      32, 0.005, 0.021, 0.1, 2.1, 1, 1};//120-500 GeV

    float valsE[250]={48, 0.019, 0.067, 1.6, 3.1, 0, 0,//Total
                      48, 0.019, 0.067, 1.6, 3.1, 0, 0,//15-20 GeV
                      48, 0.019, 0.067, 0.1, 1.1, 0, 0,//20-25 GeV
                      48, 0.019, 0.067, 0.1, 2.1, 0, 0,//25-30 GeV
                      48, 0.019, 0.067, 0.1, 3.1, 0, 0,//30-35 GeV
                      48, 0.019, 0.067, 0.6, 7.1, 0, 0,//35-45 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 0, 0,//45-55 GeV
                      48, 0.019, 0.067, 0.6, 1.1, 1, 1,//55-65 GeV
                      48, 0.019, 0.067, 1.1, 2.1, 1, 1,//65-75 GeV
                      48, 0.019, 0.067, 1.1, 2.1, 1, 1,//75-85 GeV
                      48, 0.019, 0.067, 1.1, 2.1, 1, 1,//85-95 GeV
                      48, 0.019, 0.067, 1.1, 2.1, 1, 1,//95-120 GeV
                      48, 0.019, 0.067, 1.1, 2.1, 1, 1};//120-500 GeV 
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
}//end  of SetLimsSihihTempl_phoEt_Zg_ELECTRON


void SetLimsChIsoTempl_phoEt_Wg_ELECTRON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.015, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.013, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.014, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.012, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.015, 0.017, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.012, 0.014, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.018, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.018, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.018, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.018, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.018, 1, 1};//120-500 GeV

    float valsE[250]={21, -1.0+0.1, 20.0+0.1, 0.023, 0.027, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.025, 0.030, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.025, 0.062, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.038, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.021, 0.051, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.021, 0.040, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.031, 0.035, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.021, 0.059, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.051, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.051, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.051, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.051, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.029, 0.051, 1, 1};//120-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
                 
}//end  of SetLimsChIsoTempl_phoEt_Wg_ELECTRON

void SetLimsChIsoTempl_phoEt_Zg_ELECTRON( TTemplates::TemplatesPars &pars)
{
    TConfiguration config;
    // nFitBins, minVarFit, maxVarFit, sideband, sidebandUp,  combTrue, combFake

    float valsB[250]={21, -1.0+0.1, 20.0+0.1, 0.014, 0.015, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.015, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.015, 0.016, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.015, 0.016, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.014, 0.016, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.015, 0.018, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.012, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.011, 0.012, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.013, 0.014, 1, 1};//120-500 GeV

    float valsE[250]={21, -1.0+0.1, 20.0+0.1, 0.043, 0.051, 0, 0,//Total
                      21, -1.0+0.1, 20.0+0.1, 0.043, 0.051, 0, 0,//15-20 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.043, 0.046, 0, 0,//20-25 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.043, 0.046, 0, 0,//25-30 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.031, 0.048, 0, 0,//30-35 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.048, 1, 0,//35-45 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.045, 0.048, 1, 0,//45-55 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.037, 0.038, 1, 0,//55-65 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.054, 1, 1,//65-75 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.054, 1, 1,//75-85 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.054, 1, 1,//85-95 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.054, 1, 1,//95-120 GeV
                      21, -1.0+0.1, 20.0+0.1, 0.049, 0.054, 1, 1};//120-500 GeV  
                 
    SetValuesToKinEtaArray(config.BARREL, valsB, pars);
    SetValuesToKinEtaArray(config.ENDCAP, valsE, pars);
     
}//end  of SetLimsChIsoTempl_phoEt_Zg_ELECTRON

