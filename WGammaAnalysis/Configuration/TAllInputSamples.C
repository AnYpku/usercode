#include "TAllInputSamples.h"

#include "TInputSample.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

TAllInputSamples::TAllInputSamples()
{
}

TAllInputSamples::TAllInputSamples(int channel, string configFile)
{
   ReadConfig(channel,configFile);
   for (int i=0; i<nSources_; i++) {
  //   allInputs_[i].GetFileSelectedNames();
     allInputs_[i].CalcLuminocities();
   }
   CalcLumiWeights();
}

TAllInputSamples::~TAllInputSamples()
{
}

void TAllInputSamples::ReadConfig(int channel, string configFile)
{
  ifstream ifs;
  ifs.open(configFile.c_str());
  assert(ifs.is_open());
  string line;
  nSources_ = 0;
  bool isSampleNeeded = 0;
  while (!ifs.eof())
    {
      getline(ifs,line);
      if (line[0] == '#') continue;
      if (line[0] == '$') 
        isSampleNeeded = ReadSampleGeneralInfo(channel, line);
      if (line[0] == '-' && isSampleNeeded) 
        ReadFileSpecificInfo(line);
    } //while (!ifs.eof())
}

bool TAllInputSamples::ReadSampleGeneralInfo(int channel, string line)
{
   stringstream ss(line);
   string chr;
   ss >> chr;
   string sampleType;
   ss >> sampleType;
   string channelName;
//   if (sampleType=="BKGMC") {
  //   allInputs_.push_back(TInputSample());
    // nSources_++;
//     allInputs_.back().sample_=TInputSample::BKGMC;
 //    allInputs_.back().channel_=ChannelNumber(channelName);
   //} //if (sampleType=="BKGMC")
   //else if (sampleType=="DATA" || sampleType=="SIGMC") {
     ss >> channelName;
     if (ChannelNumber(channelName)==channel){           
       allInputs_.push_back(TInputSample());
       nSources_++;
       allInputs_.back().sample_=SampleNumber(sampleType);
       allInputs_.back().channel_=ChannelNumber(channelName);
     } //if (ChannelNumber(channelName)==channel)
     else return 0;        
   //}// else if (sampleType=="DATA" || sampleType=="SIGMC")
   //else return 0;

   if (sampleType=="DATA"){
     allInputs_.back().sourceName_=sampleType;
     allInputs_.back().sourceLatexLabel_=sampleType+string(", ")+channelName;
   }
   else ss >> allInputs_.back().sourceName_ >> allInputs_.back().sourceLatexLabel_;
   ss >> allInputs_.back().color_;

   string next;
   ss >> next;
   if (next=="lumiTotal") ss >> allInputs_.back().lumiTotal_;
     //should happen for data only
   if (next=="csTotal") {
     ss >> allInputs_.back().csTotal_;
     allInputs_.back().isSharedCS_ = 1;
   } //should happen for MC only
   allInputs_.back().nFiles_=0;
   return 1;
}

void TAllInputSamples::ReadFileSpecificInfo(string line)
{
   stringstream ss(line);
   string chr;
   ss >> chr;
   allInputs_.back().nFiles_++;

   string fileName;
   ss >> fileName;
   allInputs_.back().fileNames_.push_back(fileName);

   if (allInputs_.back().sample_==TInputSample::DATA){
     float luminocity;
     if (ss >> luminocity) 
     allInputs_.back().lumiEachFile_.push_back(luminocity);
     else allInputs_.back().lumiEachFile_.push_back(-1);
     allInputs_.back().cs_.push_back(-1);
     allInputs_.back().factor_.push_back(1);
   }
   else {
     float cs;
     float factor;
     if (ss >> cs ) 
       allInputs_.back().cs_.push_back(cs);
     else allInputs_.back().cs_.push_back(-1);
     if (ss >> factor)
       allInputs_.back().factor_.push_back(factor);
     else allInputs_.back().factor_.push_back(1);
     allInputs_.back().lumiEachFile_.push_back(-1);
   }
}

int TAllInputSamples::ChannelNumber (string channelStr)
{
  if (channelStr=="MUON")  
    return TConfiguration::MUON;
  else if (channelStr=="ELECTRON") 
    return TConfiguration::ELECTRON;
  return -1;
}

int TAllInputSamples::SampleNumber (string sampleStr)
{
  if (sampleStr=="DATA")  
    return TInputSample::DATA;
  else if (sampleStr=="SIGMC") 
    return TInputSample::SIGMC;
  else if (sampleStr=="BKGMC") 
    return TInputSample::BKGMC;
  return -1;
}

void TAllInputSamples::CalcLumiWeights()
{

   float lumiData = 1;
   for (int i=0; i<nSources_; i++) {
     if (allInputs_[i].sample_==TInputSample::DATA) {
       lumiData = allInputs_[i].lumiTotal_;
       for (int j=0; j<allInputs_[i].nFiles_; j++)
         allInputs_[i].lumiWeights_.push_back(1.0);
     }
     break;
   } //search for lumiData ended

   for (int i=0; i<nSources_; i++) {
     if (allInputs_[i].sample_==TInputSample::DATA) 
       continue;
     if (allInputs_[i].isSharedCS_) 
       for (int j=0; j<allInputs_[i].nFiles_; j++)
         allInputs_[i].lumiWeights_.push_back(1.0*lumiData/allInputs_[i].lumiTotal_);
     else 
       for (int j=0; j<allInputs_[i].nFiles_; j++)
         allInputs_[i].lumiWeights_.push_back(1.0*lumiData/allInputs_[i].lumiEachFile_[j]);
   } //setting of lumi MC weights ended

}

void TAllInputSamples::Print()
{
  std::cout<<std::endl;
  std::cout<<"nSources_="<<nSources_<<std::endl;
  std::cout<<"allInputs_.size()="<<allInputs_.size()<<std::endl;

  for (int i=0; i<nSources_; i++)
    allInputs_[i].Print();
}
