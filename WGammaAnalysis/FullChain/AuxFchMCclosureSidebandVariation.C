#include "TString.h"
#include "../Configuration/TConfiguration.h"
#include "../FullChain/FullChain.h"
#include "../DDBkgTemplateMethod/AuxTemplates.C"
#include "TBenchmark.h" // ROOT
#include <iostream> //C++

void AuxFchMCclosureSidebandVariation(TString strChannel, TString strVGamma)
{

  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  std::cout<<"loaded ../Include/rootlogon.C"<<std::endl;

  TConfiguration conf;
  int channel;
  int vgamma;
  if (strChannel=="MUON") channel=conf.MUON;
  if (strChannel=="ELECTRON") channel=conf.ELECTRON;
  if (strVGamma=="WGamma") vgamma=conf.W_GAMMA;
  if (strVGamma=="ZGamma") vgamma=conf.Z_GAMMA;

  bool noClosure[2]; //templ
  noClosure[conf.TEMPL_SIHIH]=1;
  noClosure[conf.TEMPL_CHISO]=0;

  TConfiguration::AnalysisParameters anPars;
  FullChain fch; 
  fch.SetDefaultFullChainParameters(anPars, "phoEt");

  anPars.blind[channel][vgamma]=TConfiguration::UNBLIND;

  for (int tm=0; tm<=1; tm++){
    if (!noClosure[tm]){
      anPars.channel=channel;//MUON, ELECTRON
      anPars.vgamma=vgamma;//W_GAMMA, Z_GAMMA
      anPars.templFits=tm;
      AuxTemplatesSystSidebandVariation(anPars,1);// 1 - isMCclosure
    }//end of if (!noClosure[ch][vg][tm])
  }// end of loop over tm

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

}//end of AuxFchMCclosureSidebandVariation
