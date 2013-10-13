#include "TConfiguration.h" 
  //this class
#include <iostream> 
#include <vector> 
  //standard C++ class
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

TConfiguration::TConfiguration()
{
}

TConfiguration::~TConfiguration()
{
}

TString TConfiguration::GetOutputDirName(int channel)
{
  if (channel==MUON) return outputDirMu_;
  else if (channel==ELECTRON) return outputDirEle_;
  return "0";
} 

TString TConfiguration::GetSelectedPreliminaryName(int channel, TString sourceName)
{
  return (GetOutputDirName(channel)+selectedPreliminaryEventsNameBase_+"_"+sourceName+".root");
}

TString TConfiguration::GetSelectedFullyName(int channel, TString sourceName)
{
  return (GetOutputDirName(channel)+selectedFullyEventsNameBase_+"_"+sourceName+".root");
}

TString TConfiguration::GetNameDebugMode()
{
  return nameDebugMode_;
}

TString TConfiguration::GetYieldsFileName(int channel)
{
  return GetOutputDirName(channel)+yieldsFileName_;
}


TString TConfiguration::GetYieldsSelectedHistName(TString sourceName)
{
  return yieldsSelectedHistName_+sourceName;
}

TString TConfiguration::GetYieldsDDTemplateBkgHistName()
{
  return yieldsDDTemplateBkgHist_;
}

TString TConfiguration::GetYieldsSignalName()
{
  return yieldsSignal_;
}

TString TConfiguration::GetTemplatePicNameBase(int channel)
{
  return GetOutputDirName(channel)+templatePicNameBase_;
}

TString TConfiguration::GetYieldsDDTemplateBkgFileName(int channel)
{
  return GetOutputDirName(channel)+yieldsDDTemplateBkgFile_;
}

TString TConfiguration::GetFractionsDDTemplateBkgHistName()
{
  return fractionsDDTemplateBkgHist_;
}

TString TConfiguration::GetAccEffFileName(int channel)
{
  return GetOutputDirName(channel)+acceffFile_;
}

TString TConfiguration::GetAcc1DName()
{
  return acc1DName_;
}

TString TConfiguration::GetEff1DName()
{
  return eff1DName_;
}

TString TConfiguration::GetAccErr1DName()
{
  return accErr1DName_;
}

TString TConfiguration::GetEffErr1DName()
{
  return effErr1DName_;
}

TString TConfiguration::GetAccTotalName()
{
  return accTotalName_;
}

TString TConfiguration::GetEffTotalName()
{
  return effTotalName_;
}

TString TConfiguration::GetAccErrTotalName()
{
  return accErrTotalName_;
}

TString TConfiguration::GetEffErrTotalName()
{
  return effErrTotalName_;
}

TString TConfiguration::GetPhotonScaleFactorsFileNamePt15to20GeV()
{
  return certifiedConstantsDir_+photonScaleFactorsFileNamePt15to20GeV_;
}

TString TConfiguration::GetPhotonScaleFactorsFileNamePt20toInfGeV()
{
  return certifiedConstantsDir_+photonScaleFactorsFileNamePt20toInfGeV_;
}

TString TConfiguration::GetPileupDataFileName()
{
  return certifiedConstantsDir_+pileupDataFileName_;
}

TString TConfiguration::GetPhosphorConstantFileName()
{
  return (certifiedConstantsDir_+phosphorConstantsFile_);
}

int TConfiguration::GetNPhoPtBins()
{
  return nPhoPtBins_;
}

vector <float> TConfiguration::GetPhoPtBinsLimits()
{
  vector <float> lims;
  for (int i=0; i<nPhoPtBins_+1; i++)
    lims.push_back(phoPtBinsLimits_[i]);
  return lims;   
}

int TConfiguration::FindPhoPtBinByPhoPt(float pt)
{
  for (int i=0; i<nPhoPtBins_+1; i++)
    if (pt>=phoPtBinsLimits_[i] && pt<phoPtBinsLimits_[i+1])
      return i;
  return -1; 
}

float TConfiguration::GetPhoPtMin()
{
  return phoPtMin_;
}

float TConfiguration::GetLePhoDeltaRMin()
{
  return lePhoDeltaRMin_;
}
