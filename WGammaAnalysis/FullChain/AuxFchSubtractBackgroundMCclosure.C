#include "../Configuration/TConfiguration.h"
#include "../FullChain/FullChain.h"
#include "../PrepareYields/AuxPrepareYields.C"
#include "TBenchmark.h" // ROOT
#include <iostream> //C++

void AuxFchSubtractBackgroundMCclosure(TString strChannel, TString strVGamma)
{
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");

  TConfiguration conf;
  int channel;
  int vgamma;
  if (strChannel=="MUON") channel=conf.MUON;
  if (strChannel=="ELECTRON") channel=conf.ELECTRON;
  if (strVGamma=="WGamma") vgamma=conf.W_GAMMA;
  if (strVGamma=="ZGamma") vgamma=conf.Z_GAMMA;

  anPars.templFits=conf.TEMPL_CHISO;
  AuxSubtractBackground(anPars,1); 

  anPars.templFits=conf.TEMPL_SIHIH;
  AuxSubtractBackground(anPars,1); 

  anPars.templFits=conf.TEMPL_OVERLAY;
  AuxSubtractBackground(anPars,1); 

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
