#include "TConfiguration.h" 
  //this class
#include <iostream> 
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
