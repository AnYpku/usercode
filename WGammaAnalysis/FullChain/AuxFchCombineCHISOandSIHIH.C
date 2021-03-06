#include "../Configuration/TConfiguration.h"
#include "../FullChain/FullChain.h"
#include "../PrepareYields/CombineCHISOandSIHIH.C"
#include "TBenchmark.h" // ROOT
#include <iostream> //C++

void AuxFchCombineCHISOandSIHIH(TString strChannel, TString strVGamma)
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
  if (strChannel=="MUON") anPars.channel=conf.MUON;
  if (strChannel=="ELECTRON") anPars.channel=conf.ELECTRON;
  if (strVGamma=="WGamma") anPars.vgamma=conf.W_GAMMA;
  if (strVGamma=="ZGamma") anPars.vgamma=conf.Z_GAMMA;

  CombineCHISOandSIHIH(anPars.channel, anPars.vgamma);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
