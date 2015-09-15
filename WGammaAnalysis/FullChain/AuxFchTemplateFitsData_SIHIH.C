#include "../Configuration/TConfiguration.h"
#include "../FullChain/FullChain.h"
#include "TBenchmark.h" // ROOT
#include <iostream> //C++

void AuxFchTemplateFitsData_SIHIH(TString strChannel, TString strVGamma)
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

  anPars.noDDBkgComputation[channel][vgamma][conf.TEMPL_SIHIH]=0;

  fch.RunAnalysis(anPars);

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  
}