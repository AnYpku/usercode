#include "FullChain.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include "../Selection/Selection.h"
#include "../DDBkgTemplateMethod/TTemplatesRandCone.h"
#include "../DDBkgTemplateMethod/AuxTemplatesRandCone.C"
#include "../PrepareYields/TPrepareYields.h"
#include "../PrepareYields/AuxPrepareYields.C"
#include "../AcceptanceAndEfficiency/CalcAccAndEff.h"
//#include "../CrossSection/CalcCrossSection.h"

#include <iostream>

FullChain::FullChain()
{
//  _INPUT=new TAllInputSamples(_channel,"../Configuration/config.txt");
//  _phoPtBinLimits = new float[_config.GetNPhoPtBins()];
//  _config.GetPhoPtBinsLimits(_phoPtBinLimits);
}

FullChain::~FullChain()
{
//  delete _INPUT;
//  delete _phoPtBinLimits;
}

void FullChain::SetDefaultFullChainParameters(FullChainParameters& anPars, TString varKin)
{
  anPars.year=2012;//2012, 2011
  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.vgamma=TConfiguration::W_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.blind=TConfiguration::BLIND_PRESCALE;

  anPars.sampleMode=Selection::ALL;
  anPars.analyzedSamples="";
  anPars.configfile="../Configuration/config.txt";
  anPars.isNoPuReweight=0;
  anPars.isDebugMode=0;
  anPars.doSystTemplateStat=0;
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;//WP_LOOSE,WP_MEDIUM,WP_TIGHT

  anPars.noPreSelection=0;
  anPars.noExtraSelection=0;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=0;
  anPars.noCalcCrossSection=0;


  //for template method:
  anPars.strDDbkgVarFit="phoSCRChIsoCorr";
  anPars.strDDbkgVarSideband="phoSigmaIEtaIEta";
//  anPars.strDDbkgVarFit="phoSigmaIEtaIEta";
//  anPars.strDDbkgVarSideband="phoSCRChIsoCorr";

  if (varKin=="phoEt")
    SetAnalysisKinParameters(anPars);
  else
    SetDiffKinFullChainParameters(anPars,varKin);

}

void FullChain::SetAnalysisKinParameters(FullChainParameters& anPars)
{
    anPars.varKin="phoEt";
//    anPars.nKinBins=26;
//    anPars.nKinBins=2;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    for (int ib=0; ib<anPars.nKinBins+1; ib++)
//      anPars.kinBinLims[ib]=15+2.5*ib;
//    anPars.nKinBins=3;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=15;
//    anPars.kinBinLims[1]=20;
//    anPars.kinBinLims[2]=25;
//    anPars.kinBinLims[3]=30;


  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);

}

void FullChain::SetDiffKinFullChainParameters(FullChainParameters& anPars, TString varKin)
{
  if (varKin=="phoEtNoPhosphor"){
    anPars.varKin="phoEtNoPhosphor";

  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);

//    anPars.nKinBins=26;
//    anPars.nKinBins=2;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    for (int ib=0; ib<anPars.nKinBins+1; ib++)
//      anPars.kinBinLims[ib]=15+2.5*ib;

  }

  if (varKin=="phoSCEta" || varKin=="phoEta"){
    anPars.varKin=varKin;

//    anPars.nKinBins=3;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=-2.5;
//    anPars.kinBinLims[1]=-1.5;
//    anPars.kinBinLims[2]=1.5;
//    anPars.kinBinLims[3]=2.5;


//    anPars.nKinBins=4;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=-2.5;
//    anPars.kinBinLims[1]=-1.5;
//    anPars.kinBinLims[2]=0;
//    anPars.kinBinLims[3]=1.5;
//    anPars.kinBinLims[4]=2.5;

    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.5;
    anPars.kinBinLims[1]=-2.366;
    anPars.kinBinLims[2]=-2.166;
    anPars.kinBinLims[3]=-1.966;
    anPars.kinBinLims[4]=-1.766;
    anPars.kinBinLims[5]=-1.566;
    anPars.kinBinLims[6]=-1.4442;
    anPars.kinBinLims[7]=-1.08315;
    anPars.kinBinLims[8]=-0.7221;
    anPars.kinBinLims[9]=-0.36105;
    anPars.kinBinLims[10]=0;
    anPars.kinBinLims[11]=0.36105;
    anPars.kinBinLims[12]=0.7221;
    anPars.kinBinLims[13]=1.08315;
    anPars.kinBinLims[14]=1.4442;
    anPars.kinBinLims[15]=1.566;
    anPars.kinBinLims[16]=1.766;
    anPars.kinBinLims[17]=1.966;
    anPars.kinBinLims[18]=2.166;
    anPars.kinBinLims[19]=2.366;
    anPars.kinBinLims[20]=2.5;
  }
  if (varKin=="phoPhi"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
  }
  if (varKin=="lepton1Pt" || varKin=="lepton2Pt"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=26+10*ib;
  }
  if (varKin=="lepton1Phi" || varKin=="lepton2Phi"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
  }
  if (varKin=="lepton1Eta" || varKin=="lepton2Eta"){
    anPars.varKin=varKin;
    anPars.nKinBins=16;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.1;
    anPars.kinBinLims[1]=-1.966;
    anPars.kinBinLims[2]=-1.766;
    anPars.kinBinLims[3]=-1.566;
    anPars.kinBinLims[4]=-1.4442;
    anPars.kinBinLims[5]=-1.08315;
    anPars.kinBinLims[6]=-0.7221;
    anPars.kinBinLims[7]=-0.36105;
    anPars.kinBinLims[8]=0;
    anPars.kinBinLims[9]=0.36105;
    anPars.kinBinLims[10]=0.7221;
    anPars.kinBinLims[11]=1.08315;
    anPars.kinBinLims[12]=1.4442;
    anPars.kinBinLims[13]=1.566;
    anPars.kinBinLims[14]=1.766;
    anPars.kinBinLims[15]=1.966;
    anPars.kinBinLims[16]=2.1;
  }
  if (varKin=="WMt"){
    anPars.varKin=varKin;
    anPars.nKinBins=20;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+5*ib;
  }
  if (varKin=="Mleplep"){
    anPars.varKin=varKin;
    anPars.nKinBins=30;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+2*ib;
  }
  if (varKin=="lep1PhoDeltaR" || varKin=="lep2PhoDeltaR"){
    anPars.varKin=varKin;
    anPars.nKinBins=10;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0.7+0.5*ib;
  }
  if (varKin=="pfMET"){
    anPars.varKin="pfMET";
    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+5*ib;
  }
  if (varKin=="pfMETPhi"){
    anPars.varKin="pfMETPhi";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
 }
}

void FullChain::RunAnalysis(FullChainParameters anPars)
{

  if (!anPars.noPreSelection){
    //very preliminary selection
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Preliminary Selection"<<std::endl;
    if (anPars.sampleMode!=Selection::NOTSPECIFIED){
      Selection selection(anPars.channel,anPars.vgamma,anPars.sampleMode,anPars.blind,anPars.configfile,anPars.isNoPuReweight,anPars.isDebugMode);
      selection.LoopOverInputFiles();
    }
    else{
      Selection selection(anPars.channel,anPars.vgamma,anPars.analyzedSamples,anPars.blind,anPars.configfile,anPars.isNoPuReweight,anPars.isDebugMode);
      selection.LoopOverInputFiles();
    }
    std::cout<<"%_%  DONE Preliminary Selection"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noExtraSelection){
    //extra selection
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Extra Selection"<<std::endl;
    Selection selection;
    selection.ExtraSelection(anPars.year, anPars.channel,anPars.vgamma, anPars.sampleMode,anPars.blind,anPars.phoWP);
    std::cout<<"%_%  DONE Extra Selection"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noDDBkgComputation){
    //compute fake-gamma background by data-driven template method
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO DataDriven bkg"<<std::endl;
   //TTemplatesRandCone temp(anPars.channel, anPars.vgamma, anPars.phoWP, anPars.varKin, anPars.nKinBins, anPars.kinBinLims);
   //temp.ComputeBackground();
    AuxTemplatesRandCone(anPars.channel, anPars.vgamma, anPars.phoWP, anPars.varKin, anPars.nKinBins, anPars.kinBinLims);
    std::cout<<"%_%  DONE DataDriven bkg"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noSystDDBkgSidebandVariation){
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Syst DDBkg Sideband Variation "<<std::endl;
    AuxTemplatesRandConeSystSidebandVariation(anPars.channel, anPars.vgamma, anPars.phoWP, anPars.varKin, anPars.nKinBins, anPars.kinBinLims);
    std::cout<<"%_%  DONE Syst DDBkg Sideband Variation"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noPrepareYields){
    //prepare yields and subtract background
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Prepare Yields"<<std::endl;
    if (anPars.noDDBkgComputation) 
      AuxPrepareYields(anPars.channel, anPars.vgamma, anPars.blind, anPars.varKin, anPars.nKinBins, anPars.kinBinLims);
    std::cout<<"%_%  DONE Prepare Yields"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noCalcAccAndEff){
    //compute acceptance and efficiency constants
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Compute Acceptance and Efficiency"<<std::endl;
    CalcAccAndEff accAndEff(anPars.year, anPars.channel, anPars.vgamma, anPars.phoWP, anPars.configfile, anPars.isNoPuReweight, anPars.isDebugMode);
    accAndEff.LoopOverInputFiles();
    std::cout<<"%_%  DONE Compute Acceptance and Efficiency"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }
/*
  if (!anPars.noCalcCrossSection){
    //compute acceptance and efficiency constants
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Compute Cross Section"<<std::endl;
    CalcCrossSection calcCS(anPars.channel, anPars.blind, anPars.configfile);
    calcCS.Calc();
    std::cout<<"%_%  DONE Compute Cross Section"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }
*/
}

