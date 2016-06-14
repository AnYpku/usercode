#include "../Configuration/TConfiguration.h"
#include "../FullChain/FullChain.h"
#include "../DDBkgEtoGamma/TEtoGamma.h"
#include "TBenchmark.h" // ROOT
#include <iostream> //C++

void AuxFchBkgEtoGammaNoWMtCut()
{
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
 
  TConfiguration conf;
  FullChain fch;
  TConfiguration::AnalysisParameters anPars;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");

  anPars.channel=conf.ELECTRON;
  anPars.vgamma=conf.W_GAMMA;
  TEtoGamma etg(anPars,conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_NO_WMT_CUT,0); 
  etg.ComputePlotSave();

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
