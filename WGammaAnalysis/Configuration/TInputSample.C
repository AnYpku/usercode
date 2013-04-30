#include "TInputSample.h"
#include <iostream>

TInputSample::TInputSample()
{
}

TInputSample::TInputSample(int nFilesIn, 
    TString sourceNameIn,
    TString sourceLatexLabelIn,
    int colorIn,
    float lumiDataTotalIn,
    TString* fileNamesIn,
    float* lumiDataIn,
    float* lumiMCIn,
    float* csMCIn,
    bool* isSkimmedIn    )
{

  nFiles_=nFilesIn;
  sourceName_=sourceNameIn;
  sourceLatexLabel_=sourceLatexLabelIn;
  color_=colorIn;
  lumiDataTotal_=lumiDataTotalIn;
  
  for (int i=0; i<nFilesIn; i++)
    {
      fileNames_[i] = fileNamesIn[i];
      lumiData_[i] = lumiDataIn[i];
      lumiMC_[i] = lumiMCIn[i];
      csMC_[i] = csMCIn[i];
      isSkimmed_[i] = isSkimmedIn[i];
    }
}

TInputSample::~TInputSample()
{
}

void TInputSample::Print()
{
  std::cout<<std::endl;
  std::cout<<"sourceName_="<<sourceName_<<std::endl;
  std::cout<<"sourceLatexLabel_="<<sourceLatexLabel_<<std::endl;
  std::cout<<"color_="<<color_<<std::endl;
  std::cout<<"lumiDataTotal_="<<lumiDataTotal_<<std::endl;
  std::cout<<"nFiles_="<<nFiles_<<std::endl;


  std::cout<<"lumiData_={";
  for (int i=0; i<nFiles_; i++) std::cout<<lumiData_[i]<<", ";
  std::cout<<"}"<<std::endl;

  std::cout<<"lumiMC_={";
  for (int i=0; i<nFiles_; i++) std::cout<<lumiMC_[i]<<", ";
  std::cout<<"}"<<std::endl;

  std::cout<<"csMC_={";
  for (int i=0; i<nFiles_; i++) std::cout<<csMC_[i]<<", ";
  std::cout<<"}"<<std::endl;

  std::cout<<"isSkimmed_={";
  for (int i=0; i<nFiles_; i++) std::cout<<isSkimmed_[i]<<", ";
  std::cout<<"}"<<std::endl;

  std::cout<<"fileNames_={";
  for (int i=0; i<nFiles_; i++) std::cout<<fileNames_[i]<<", "<<std::endl;
  std::cout<<"}"<<std::endl;

  std::cout<<std::endl;
  
}
