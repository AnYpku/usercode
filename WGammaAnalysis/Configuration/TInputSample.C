#include "TInputSample.h"
  //this class
#include "TConfiguration.h"
  //this package
#include <iostream>
#include <string>
#include <fstream>
  // standard C++
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
  //ROOT classes

TInputSample::TInputSample()
{
  channel_=-1;
  sample_=-1;
  nFiles_=0;
  sourceName_="";
  sourceLatexLabel_="";
  color_=-1;
  isSharedCS_=0;
  csTotal_=-1;
  lumiTotal_=-1;
  
  for (int i=0; i<nFiles_; i++)
    {
      fileNames_[i] = "";
      lumiEachFile_[i] = -1;
      cs_[i] = -1;
      factor_[i] = 1;
      lumiWeights_[i] = 1;
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
  std::cout<<"channel_="<<channel_<<std::endl;
  std::cout<<"sample_="<<sample_<<std::endl;
  std::cout<<"color_="<<color_<<std::endl;
  std::cout<<"lumiTotal_="<<lumiTotal_<<std::endl;
  std::cout<<"nFiles_="<<nFiles_<<std::endl;
  std::cout<<"isSharedCS_="<<isSharedCS_<<std::endl;
  std::cout<<"csTotal_="<<csTotal_<<std::endl;
  std::cout<<"fileSelected_="<<fileSelected_<<std::endl;
  std::cout<<"fileSelectedDebug_="<<fileSelectedDebug_<<std::endl;


  std::cout<<"lumiEachFile_("<<lumiEachFile_.size()<<")={";
  for (int i=0; i<nFiles_-1; i++) std::cout<<lumiEachFile_[i]<<", ";
  std::cout<<lumiEachFile_[nFiles_-1];
  std::cout<<"}"<<std::endl;

  std::cout<<"cs_("<<cs_.size()<<")={";
  for (int i=0; i<nFiles_-1; i++) std::cout<<cs_[i]<<", ";
  std::cout<<cs_[nFiles_-1];
  std::cout<<"}"<<std::endl;

  std::cout<<"factor_("<<factor_.size()<<")={";
  for (int i=0; i<nFiles_-1; i++) std::cout<<factor_[i]<<", ";
  std::cout<<factor_[nFiles_-1];
  std::cout<<"}"<<std::endl;

  std::cout<<"lumiWeights_("<<lumiWeights_.size()<<")={";
  for (int i=0; i<nFiles_-1; i++) std::cout<<lumiWeights_[i]<<", ";
  std::cout<<lumiWeights_[nFiles_-1];
  std::cout<<"}"<<std::endl;

  std::cout<<"fileNames_("<<fileNames_.size()<<")={";
  for (int i=0; i<nFiles_-1; i++) std::cout<<fileNames_[i]<<", "<<std::endl;
  std::cout<<fileNames_[nFiles_-1];
  std::cout<<"}"<<std::endl;

  std::cout<<std::endl;
  
}

void TInputSample::GetFileSelectedNames()
{
  TConfiguration conf;
  TString name;
  if (sample_==DATA)
    name = conf.GetSelectedNameData(channel_);
  else if (channel_==conf.MUON && sample_==SIGMC)
    name = conf.GetSelectedNameSignalMC(channel_);
  else if (sample_==BKGMC){
    name = conf.GetSelectedNameBkgMC(channel_)+sourceName_;
  }
  fileSelected_ = name + TString(".root");
  fileSelectedDebug_ = name + conf.GetNameDebugMode() + TString(".root"); 
}

void TInputSample::CalcLuminocities()
{
  if (sample_==DATA) return; 

  double nEventsTotal = 0;

  for (int iFile=0; iFile<nFiles_; iFile++)
    {
       TFile f(fileNames_[iFile]);
       if (!f.IsOpen()) {
           std::cout<<"ERROR detected in TInputSample::CalcLuminocities: file "<<fileNames_[iFile]<<" is not open"<<std::endl;
           return;
       }
       f.cd("ggNtuplizer");

       TTree* tr = (TTree*)gDirectory->Get("EventTree");
       double nEvents = (double)tr->GetEntries();
       nEvents = nEvents/factor_[iFile];

       bool isSkimmed = gDirectory->GetListOfKeys()->Contains("hskim");

       if (isSkimmed){
            TH1F* hist = (TH1F*)gDirectory->Get("hskim");
            nEvents = nEvents * hist->GetBinContent(1) / hist->GetBinContent(2);
            hist = 0;
       }
       tr = 0;
       if (!isSharedCS_) lumiEachFile_[iFile]=1.0*nEvents/cs_[iFile];
       else nEventsTotal+=nEvents;
    }
  if (isSharedCS_) lumiTotal_=1.0*nEventsTotal/csTotal_;
}
