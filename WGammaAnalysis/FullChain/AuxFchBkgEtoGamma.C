#include "../Configuration/TConfiguration.h"
#include "../FullChain/FullChain.h"
#include "TBenchmark.h" // ROOT
#include <iostream> //C++

void AuxFchBkgEtoGamma()
{
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");

  anPars.noDDBkgEtoGamma=0;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}
