#include "CalcCrossSection.h" 
  //this class
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
  //currently in this package
#include <string>
#include <iostream>
  //standard C++ class

CalcCrossSection::CalcCrossSection(int channel, string configFile)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;

  vecPhoPtLimits_ = config_.GetPhoPtBinsLimits();

}

CalcCrossSection::~CalcCrossSection()
{

}

void CalcCrossSection::Calc(string cut)
{
  GetSignalYields(cut);
  ApplyAccAndEff();
  DivideOverLumi();
  DivideOverBinWidth();
  SaveOutput();
}

void CalcCrossSection::GetSignalYields(string cut)
{
  int nSources = INPUT_->nSources_;
  for (int iSource=0; iSource<nSources; iSource++)
    {
       int sample = INPUT_->allInputs_[iSource].sample_;
       if (sample==TInputSample::SIGMC) continue;
       TString fileName = (TString)(INPUT.allInputs_[i].fileSelected_).c_str();
            

    } //loop over iSource ends
}

void CalcCrossSection::ApplyAccAndEff(string cut)
{

}

void CalcCrossSection::DivideOverLumi()
{

}

void CalcCrossSection::DivideOverBinWidth()
{

}

void CalcCrossSection::SaveOutput()
{

}
