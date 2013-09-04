#include "TConfiguration.h" 
  //this class
#include <iostream> 
#include <vector> 
  //standard C++ class
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

TConfiguration::TConfiguration()
{
}

TConfiguration::~TConfiguration()
{
}

string TConfiguration::GetPhosphorConstantFileName()
{
  return phosphorConstantsFile_;
}

string TConfiguration::GetSelectedEventsDir()
{
  return selectedEventsDir_;
}

string TConfiguration::GetSelectedNameDataMu()
{
  return selectedEventsNameDataMu_;
}

string TConfiguration::GetSelectedNameDataEle()
{
  return selectedEventsNameDataEle_;
}

string TConfiguration::GetSelectedNameSignalMCMu()
{
  return selectedEventsNameSignalMCMu_;
}

string TConfiguration::GetSelectedNameSignalMCEle()
{
  return selectedEventsNameSignalMCEle_;
}

string TConfiguration::GetSelectedNameBkgMC()
{
  return selectedEventsNameBkgMC_;
}

string TConfiguration::GetNameDebugMode()
{
  return nameDebugMode_;
}


string TConfiguration::GetNameReleasedCuts()
{
  return nameReleasedCuts_;
}

string TConfiguration::GetPileupDataFileName()
{
  return pileupDataFileName_;
}

string TConfiguration::GetSignalTemplateNameMu()
{
  return signalTemplateNameMu_;
}

string TConfiguration::GetSignalTemplateNameEle()
{
  return signalTemplateNameEle_;
}


string TConfiguration::GetAccEffDirName()
{
  return acceffDir_;
}

string TConfiguration::GetAccEffFileName()
{
  return acceffFile_;
}

string TConfiguration::GetAcc1DName()
{
  return acc1DName_;
}

string TConfiguration::GetEff1DName()
{
  return eff1DName_;
}

string TConfiguration::GetAccErr1DName()
{
  return accErr1DName_;
}

string TConfiguration::GetEffErr1DName()
{
  return effErr1DName_;
}

string TConfiguration::GetAccTotalName()
{
  return accTotalName_;
}

string TConfiguration::GetEffTotalName()
{
  return effTotalName_;
}

string TConfiguration::GetAccErrTotalName()
{
  return accErrTotalName_;
}

string TConfiguration::GetEffErrTotalName()
{
  return effErrTotalName_;
}

string TConfiguration::GetPhotonScaleFactorsFileNamePt15to20GeV()
{
  return photonScaleFactorsFileNamePt15to20GeV_;
}

string TConfiguration::GetPhotonScaleFactorsFileNamePt20toInfGeV()
{
  return photonScaleFactorsFileNamePt20toInfGeV_;
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
