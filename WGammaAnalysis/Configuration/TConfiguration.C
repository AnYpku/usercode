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
