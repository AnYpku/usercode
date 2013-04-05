#include "TConfiguration.h" //this class
#include <iostream> //standard C++ class

TConfiguration::TConfiguration(int channel, int sample)
{
  channel_=channel;
  sample_=sample;
}

TConfiguration::~TConfiguration()
{
}

void TConfiguration::SetChannelAndSample(int channel, int sample)
{
  channel_=channel;
  sample_=sample;
}

void TConfiguration::SetInputFileNames()
{
  if (nDataMuFiles_>nInputFilesMax_ || nDataEleFiles_>nInputFilesMax_
     || nSigMCMuFiles_>nInputFilesMax_ || nSigMCEleFiles_>nInputFilesMax_
     || nBkgMCMuFiles_>nInputFilesMax_ || nBkgMCEleFiles_>nInputFilesMax_)
    {
      std::cout<<"Error in TConfiguration: One of these values nDataMuFiles_, nDataEleFiles_, nSigMCMuFiles_, nSigMCEleFiles_,  nBkgMCMuFiles_, nBkgMCEleFiles_ is greater than nInputFilesMax_; Please, go to TConfiguration.h and increase nInputFilesMax_; NO input values will be set up now!"<<std::endl;
      nInputFiles=0;
      return;
    }
  if (channel_==MUON)
    {
      if (sample_==DATA)
        {
          nInputFiles=nDataMuFiles_;
          for (int i=0; i<nInputFiles; i++)
            inputFileNames[i]=inputDataMuFileNames_[i];
        }
      else if (sample_==SIGMC)
        {
          nInputFiles=nSigMCMuFiles_;
          for (int i=0; i<nInputFiles; i++)
            inputFileNames[i]=inputSigMCMuFileNames_[i];
        }
      else if (sample_==BKGMC)
        {
          nInputFiles=nBkgMCMuFiles_;
          for (int i=0; i<nInputFiles; i++)
            inputFileNames[i]=inputBkgMCMuFileNames_[i];
        }
      else
        {
          nInputFiles=0;
          std::cout<<"Error in TConfiguration::SetInputFileNames(): sample must be DATA, SIGMC or BKGMC. Please, make a change in the appropriate place"<<std::endl;
        }
    }//closing brace for if (channel_==MUON)
  else if (channel_==ELECTRON)
    {
      if (sample_==DATA)
        {
          nInputFiles=nDataEleFiles_;
          for (int i=0; i<nInputFiles; i++)
            inputFileNames[i]=inputDataEleFileNames_[i];
        }
      else if (sample_==SIGMC)
        {
          nInputFiles=nSigMCEleFiles_;
          for (int i=0; i<nInputFiles; i++)
            inputFileNames[i]=inputSigMCEleFileNames_[i];
        }
      else if (sample_==BKGMC)
        {
          nInputFiles=nBkgMCEleFiles_;
          for (int i=0; i<nInputFiles; i++)
            inputFileNames[i]=inputBkgMCEleFileNames_[i];
        }
      else
        {
          nInputFiles=0;
          std::cout<<"Error in TConfiguration::SetInputFileNames(): sample must be DATA, SIGMC or BKGMC. Please, make a change in the appropriate place"<<std::endl;
        }
    }//closing brace for else if (channel_==ELECTRON)
  else
    {
      nInputFiles=0;
      std::cout<<"Error in TConfiguration::SetInputFileNames(): channel must be MUON or ELECTRON. Please, make a change in the appropriate place"<<std::endl;
    }//closing brace for else
}

void TConfiguration::SetSelectedFileName()
{
  
  selectedFileName=selectedEventsDir_+selectedEventsNameBase_;

  if (channel_==MUON)
    selectedFileName+="_MUON";
  else if (channel_==ELECTRON)
    selectedFileName+="_ELECTRON";
  else
    {
      std::cout<<"Error in TConfiguration::SetSelectedFileName(): channel must be MUON or ELECTRON. Please, make a change in the appropriate place"<<std::endl;
      selectedFileName="";
      return;
    }

  if (sample_==DATA)
    selectedFileName+="_DATA";
  else if (sample_==SIGMC)
    selectedFileName+="_SIGMC";
  else if (sample_==BKGMC)
    selectedFileName+="_BKGMC";
  else
    {
      std::cout<<"Error in TConfiguration::SetSelectedFileName(): sample must be DATA, SIGMC or BKGMC. Please, make a change in the appropriate place"<<std::endl;
      selectedFileName="";
      return;
     }
  selectedFileName+=".root";
}

TString TConfiguration::GetPhosphorConstantFileName()
{
  return phosphorConstantsFile_;
}
