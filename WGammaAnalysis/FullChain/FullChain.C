#include "FullChain.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include "../Selection/WGammaSelection.h"
#include "../Selection/ExtraSelection.C"
#include "../DDBkgTemplateMethod/TTemplates.h"
#include "../PrepareYields/TPrepareYields.h"
#include "../AcceptanceAndEfficiency/CalcAccAndEff.h"

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

void FullChain::SetDefaultFullChainParameters(FullChainParameters& anPars)
{
  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;//WP_LOOSE,WP_MEDIUM,WP_TIGHT
  anPars.strPhoIsoBase="SCR";//"SCR","PF"
  anPars.noPreSelection=0;
  anPars.noExtraSelection=0;
  anPars.noDDBkgComputation=0;
  anPars.noPrepareYields=0;
  anPars.noCalcAccAndEff=0;
}

void FullChain::RunAnalysis(FullChainParameters anPars)
{

  if (!anPars.noPreSelection){
    //very preliminary selection
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Preliminary Selection"<<std::endl;
    WGammaSelection selection(anPars.channel, WGammaSelection::ALL);
    selection.LoopOverInputFiles();
    std::cout<<"%_%  DONE Preliminary Selection"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noExtraSelection){
    //extra selection
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Extra Selection"<<std::endl;
    ExtraSelection(anPars.channel, anPars.phoWP, anPars.strPhoIsoBase);
    std::cout<<"%_%  DONE Extra Selection"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noDDBkgComputation){
    //compute fake-gamma background by data-driven template method
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO DataDriven bkg"<<std::endl;
    TTemplates temp(anPars.channel);
    temp.ComputeBackground();
    std::cout<<"%_%  DONE Extra DataDriven bkg"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noPrepareYields){
    //prepare yields and subtract background
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Prepare Yields"<<std::endl;
    TPrepareYields yields(anPars.channel);
    yields.PrepareYields();
    std::cout<<"%_%  DONE Prepare Yields"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }

  if (!anPars.noCalcAccAndEff){
    //compute acceptance and efficiency constants
    std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
    std::cout<<"%^%  WILL DO Compute Acceptance and Efficiency"<<std::endl;
    CalcAccAndEff accAndEff(anPars.channel, anPars.phoWP, anPars.strPhoIsoBase, "../Configuration/config.txt");
    accAndEff.LoopOverInputFiles();
    std::cout<<"%_%  DONE Compute Acceptance and Efficiency"<<std::endl;
    std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
  }
}
