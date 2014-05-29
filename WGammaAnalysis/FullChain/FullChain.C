#include "FullChain.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include "../Selection/WGammaSelection.h"
#include "../DDBkgTemplateMethod/TTemplates.h"
#include "../PrepareYields/TPrepareYields.h"
#include "../AcceptanceAndEfficiency/CalcAccAndEff.h"
#include "../CrossSection/CalcCrossSection.h"

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
  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.blind=TConfiguration::BLIND_PRESCALE;

  anPars.sampleMode=WGammaSelection::ALL;
  anPars.analyzedSamples="";
  anPars.configfile="../Configuration/config.txt";
  anPars.isNoPuReweight=0;
  anPars.isDebugMode=0;
  anPars.isMetCutOptimization=0;
  anPars.phoWP=TPhotonCuts::WP_TIGHT;//WP_LOOSE,WP_MEDIUM,WP_TIGHT
  anPars.strPhoIsoBase="SCR";//"SCR","PF"

  anPars.noPreSelection=0;
  anPars.noExtraSelection=0;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=0;
  anPars.noCalcCrossSection=0;

  //for template method:
  anPars.strDDbkgVarFit="phoSCRChIsoCorr";
  anPars.strDDbkgVarSideband="phoSigmaIEtaIEta";
  if (varKin=="phoEt")
    SetAnalysisKinParameters(anPars);
  else
    SetDiffKinFullChainParameters(anPars,varKin);

}

void FullChain::SetAnalysisKinParameters(FullChainParameters& anPars)
{
  anPars.varKin="phoEt";
  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);
  anPars.nBinsLeftBTot=2;
  anPars.maxVarFitBTot=14.0;
  anPars.nBinsLeftETot=2;
  anPars.maxVarFitETot=14.0;
  anPars.nBinsLeftB=new int[anPars.nKinBins];
  anPars.maxVarFitB=new float[anPars.nKinBins];
  anPars.nBinsLeftE=new int[anPars.nKinBins];
  anPars.maxVarFitE=new float[anPars.nKinBins];
  for (int i=0; i<anPars.nKinBins; i++){
    anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
    anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
    anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
    anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    if (anPars.kinBinLims[i]>24){
      anPars.nBinsLeftB[i]=1;
      anPars.nBinsLeftE[i]=1;
    }
    if (anPars.kinBinLims[i]>36){
      anPars.maxVarFitE[i]=10.5;//20 events in last bin
    }
    if (anPars.kinBinLims[i]>61){
      anPars.maxVarFitB[i]=10.5;//16 events in last bin
      anPars.maxVarFitE[i]=8.5;//20 events in last bin
    }
    if (anPars.kinBinLims[i+1]>81){
      anPars.maxVarFitB[i]=12.0;//20 events in last bin
      anPars.maxVarFitE[i]=7.5;//15 events in last bin
    }
    if (anPars.kinBinLims[i]>201){
      anPars.maxVarFitB[i]=4.0;//5 events in last bin; may limit at 2.0
      anPars.maxVarFitE[i]=4.0;//4 events in last bin; may limit at 2.0
    }
  }
}

void FullChain::SetDiffKinFullChainParameters(FullChainParameters& anPars, TString varKin)
{
  if (varKin=="phoEta"){
    anPars.varKin="phoEta";
    anPars.nKinBins=4;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.5;
    anPars.kinBinLims[1]=-1.5;
    anPars.kinBinLims[2]=0;
    anPars.kinBinLims[3]=1.5;
    anPars.kinBinLims[4]=2.5;

    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }
  if (varKin=="phoPhi"){
    anPars.varKin="phoPhi";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;


    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }

  if (varKin=="leptonPt"){
    anPars.varKin="leptonPt";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=26+10*ib;


    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }

  if (varKin=="leptonPhi"){
    anPars.varKin="leptonPhi";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;


    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }

  if (varKin=="leptonEta"){
    anPars.varKin="leptonEta";
    anPars.nKinBins=4;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.1;
    anPars.kinBinLims[1]=-1.5;
    anPars.kinBinLims[2]=0;
    anPars.kinBinLims[3]=1.5;
    anPars.kinBinLims[4]=2.1;

    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }

  if (varKin=="WMt"){
    anPars.isMetCutOptimization=1;
    anPars.varKin="WMt";
    anPars.nKinBins=12;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0+10*ib;

    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      if (anPars.kinBinLims[i]>9 && anPars.kinBinLims[i+1]<111){
        anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
        anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
        anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
        anPars.maxVarFitE[i]=anPars.maxVarFitETot;
      }
      else{
        anPars.nBinsLeftB[i]=2;
        anPars.maxVarFitB[i]=5.0;
        anPars.nBinsLeftE[i]=2;
        anPars.maxVarFitE[i]=5.0;
      }
    }
  }

  if (varKin=="lePhoDeltaR"){
    anPars.varKin="lePhoDeltaR";
    anPars.nKinBins=10;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0.7+0.5*ib;

    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }

  if (varKin=="pfMET"){
    anPars.varKin="pfMET";
    anPars.nKinBins=9;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=20+10*ib;

    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }

  if (varKin=="pfMETPhi"){
    anPars.varKin="pfMETPhi";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;

    anPars.nBinsLeftB=new int[anPars.nKinBins];
    anPars.maxVarFitB=new float[anPars.nKinBins];
    anPars.nBinsLeftE=new int[anPars.nKinBins];
    anPars.maxVarFitE=new float[anPars.nKinBins];
    for (int i=0; i<anPars.nKinBins; i++){
      anPars.nBinsLeftB[i]=anPars.nBinsLeftBTot;
      anPars.maxVarFitB[i]=anPars.maxVarFitBTot;
      anPars.nBinsLeftE[i]=anPars.nBinsLeftETot;
      anPars.maxVarFitE[i]=anPars.maxVarFitETot;
    }
  }
}

void FullChain::RunAnalysis(FullChainParameters anPars)
{

  if (!anPars.noPreSelection){
    //very preliminary selection
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Preliminary Selection"<<std::endl;
    if (anPars.sampleMode!=WGammaSelection::NOTSPECIFIED){
      WGammaSelection selection(anPars.channel,anPars.sampleMode,anPars.configfile,anPars.isNoPuReweight,anPars.isDebugMode);
      selection.LoopOverInputFiles();
    }
    else{
      WGammaSelection selection(anPars.channel,anPars.analyzedSamples,anPars.configfile,anPars.isNoPuReweight,anPars.isDebugMode);
      selection.LoopOverInputFiles();
    }
    std::cout<<"%_%  DONE Preliminary Selection"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noExtraSelection){
    //extra selection
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Extra Selection"<<std::endl;
    WGammaSelection selection;
    selection.ExtraSelection(anPars.channel, anPars.sampleMode,anPars.phoWP, anPars.strPhoIsoBase);
    std::cout<<"%_%  DONE Extra Selection"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noDDBkgComputation){
    //compute fake-gamma background by data-driven template method
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO DataDriven bkg"<<std::endl;
    TTemplates temp(anPars.channel,anPars.blind, anPars.phoWP,anPars.strDDbkgVarFit,anPars.strDDbkgVarSideband, anPars.varKin, anPars.nKinBins, anPars.kinBinLims, anPars.nBinsLeftB, anPars.maxVarFitB, anPars. nBinsLeftE,anPars.maxVarFitE, anPars.nBinsLeftBTot, anPars.maxVarFitBTot, anPars. nBinsLeftETot, anPars.maxVarFitETot, anPars.isMetCutOptimization);
   temp.ComputeBackground();
    std::cout<<"%_%  DONE Extra DataDriven bkg"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noPrepareYields){
    //prepare yields and subtract background
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Prepare Yields"<<std::endl;
    TPrepareYields yields(anPars.channel, anPars.blind, anPars.varKin, anPars.nKinBins, anPars.kinBinLims, anPars.isMetCutOptimization, anPars.phoWP, anPars.strPhoIsoBase);
    yields.PrepareYields();
    std::cout<<"%_%  DONE Prepare Yields"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noCalcAccAndEff){
    //compute acceptance and efficiency constants
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Compute Acceptance and Efficiency"<<std::endl;
    CalcAccAndEff accAndEff(anPars.channel, anPars.phoWP, anPars.strPhoIsoBase, anPars.configfile, anPars.isNoPuReweight, anPars.isDebugMode);
    accAndEff.LoopOverInputFiles();
    std::cout<<"%_%  DONE Compute Acceptance and Efficiency"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noCalcCrossSection){
    //compute acceptance and efficiency constants
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Compute Cross Section"<<std::endl;
    CalcCrossSection calcCS(anPars.channel, anPars.blind, anPars.configfile);
    calcCS.Calc();
    std::cout<<"%_%  DONE Compute Cross Section"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }
}
