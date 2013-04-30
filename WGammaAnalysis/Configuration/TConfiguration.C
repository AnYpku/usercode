#include "TConfiguration.h" 
  //this class
#include "TInputSample.h" 
  //this package
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

TInputSample TConfiguration::GetInputSample(int channel, int sample, int iBkg)
{
  TInputSample empty;

  /////////////////////////////////////////
  //input DATA MUON 
  if (channel==MUON && sample==DATA)
    {
      const int nFiles = 8;
      TString dir = "/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/";
      TString fileNames[nFiles]=
        {  dir+"V05-03-07-04_maybe/job_1muon_2012a_Jul13rereco_skim.root",
           dir+"V05-03-07-06/job_muon_2012b_Jul13rereco_skim.root",
           dir+"V05-03-07-04_maybe/job_1muon_2012a_Aug6rereco_skim.root",
           dir+"V05-03-07-06/job_muon_2012c_Aug24rereco_skim.root",
           dir+"V05-03-07-06/job_muon_2012c_Dec11rereco_skim.root",
           dir+"V05-03-07-06/job_muon_2012c_PRv2_skim.root",
           dir+"V05-03-07-06/job_muon_2012c_PRv21_skim.root",
           dir+"V05-03-07-04_maybe/job_1muon_2012d_PRv1_skim.root"
        };
      float lumiData[nFiles] = {-1.0, -1.0, 0.082, 0.495, 0.134, -1.0, -1.0, -1.0};
      float lumiMC[nFiles] = {-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
      float csMC[nFiles] = {-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
      bool skim[nFiles] = {1,1,1,1,1,1,1,1};
      int color = 1; //kBlack
      float lumiDataTotal = 19593.0;
      TInputSample dataMu (nFiles, "data", "data, muons", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
      return dataMu;
    }

  /////////////////////////////////////////
  //input DATA ELECTRON 
  else if (channel==ELECTRON && sample==DATA)
    {
      //const int nFiles = 0;
      //int color = 1; //kBlack
      TInputSample dataEle;
      return dataEle;
    }

  /////////////////////////////////////////
  //input SIGMC MUON 
  else if (channel==MUON && sample==SIGMC)
    {
      const int nFiles = 1;
      TString fileNames[nFiles]= {"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_munu.root"};
      float lumiData[nFiles] = {-1.0};
      float lumiMC[nFiles] = {-1.0};
      float csMC[nFiles] = {461.6*927660/4802358};
      bool skim[nFiles] = {0};
      int color = 634; //kRed+2
      float lumiDataTotal = -1.0;
      TInputSample sigmcMu (nFiles, "Wg_to_munu", "W#gamma{#rightarrow}#mu#nu#gamma", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
      return sigmcMu;
    }

  /////////////////////////////////////////
  //input SIGMC ELECTRON
  else if (channel==ELECTRON && sample==SIGMC)
    {
      const int nFiles = 1;
      TString fileNames[nFiles]= {"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_enu.root"};
      float lumiData[nFiles] = {-1.0};
      float lumiMC[nFiles] = {-1.0};
      float csMC[nFiles] = {461.6*927363/4802358};
      bool skim[nFiles] = {0};
      int color = 921; //kGray+1
      float lumiDataTotal = -1.0;
      TInputSample sigmcEle (nFiles, "Wg_to_enu", "W#gamma{#rightarrow}e#nu#gamma", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
      return sigmcEle;
    }

  /////////////////////////////////////////
  //input BKGMC, the same for both channels
  else if (sample==BKGMC)
    {

      if (iBkg==0)
        {
          TString sourceName="Wjets_to_lnu";
          const int nFiles = 2;
          TString fileNames[nFiles]= {"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_WJetsToLNu1_skim.root","/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_WJetsToLNu2_skim.root"};
          float lumiData[nFiles] = {-1.0,-1.0};
          float lumiMC[nFiles] = {-1.0,-1.0};
          float csMC[nFiles] = {30400./2,30400./2};
          bool skim[nFiles] = {1,1};
          int color = 435; //kCyan+3
          float lumiDataTotal = -1.0;
          TInputSample bkgmc (nFiles, sourceName, "W+jets{#rightarrow}l#nu+jets", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
          return bkgmc;          
        }

      else if (iBkg==1)
        {
          TString sourceName="DYjets_to_ll";
          const int nFiles = 1;
          TString fileNames[nFiles]= {"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_DYJetsToLL.root"};
          float lumiData[nFiles] = {-1.0};
          float lumiMC[nFiles] = {-1.0};
          float csMC[nFiles] = {11050.};
          bool skim[nFiles] = {1};
          int color = 419; //kGreen+3
          float lumiDataTotal = -1.0;
          TInputSample bkgmc (nFiles, sourceName, "DY+jets{#rightarrow}l#bar{l}", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
          return bkgmc;          
        }

      else if (iBkg==2)
        {
          TString sourceName="dibosons";
          const int nFiles = 10;
          TString dir="/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/";
          TString fileNames[nFiles]= 
            {dir+"job_summer12_WWg.root",
             dir+"job_summer12_WW_2l2nu.root",
             dir+"job_summer12_WZ_3lnu.root",
             dir+"job_summer12_WZ_2l2q.root",
             dir+"job_summer12_ZZ_4e.root",
             dir+"job_summer12_ZZ_4mu.root",
             dir+"job_summer12_ZZ_4tau.root",
             dir+"job_summer12_ZZ_2e2mu.root",
             dir+"job_summer12_ZZ_2e2tau.root",
             dir+"job_summer12_ZZ_2mu2tau.root",
            };
          float lumiData[nFiles] = {-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
          float lumiMC[nFiles] = {-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
          float csMC[nFiles] = {0.528,4.7,0.8674,1.755,0.07691,0.07691,0.07691,0.1767,0.1767,0.1767};
          bool skim[nFiles] = {0,0,0,0,0,0,0,0,0,0};
          int color = 799; //kOrange-1
          float lumiDataTotal = -1.0;
          TInputSample bkgmc (nFiles, sourceName, "WW, WZ, ZZ", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
          return bkgmc;          
        }


      else if (iBkg==3)
        {
          TString sourceName="Wg_to_taunu";
          const int nFiles = 1;
          TString fileNames[nFiles]= 
            { "/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_taunu.root" };
          float lumiData[nFiles] = {-1.0};
          float lumiMC[nFiles] = {-1.0};
          float csMC[nFiles] = {461.6*927456/4802358};
          bool skim[nFiles] = {0};
          int color = 882; //kViolet+2
          float lumiDataTotal = -1.0;
          TInputSample bkgmc (nFiles, sourceName, "W#gamma{#rightarrow}#tau#nu#gamma", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
          return bkgmc;          
        }

      else if (iBkg==4)
        {
          TString sourceName="Zg_to_ll";
          const int nFiles = 1;
          TString fileNames[nFiles]= 
            { "/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Zg.root" };
          float lumiData[nFiles] = {-1.0};
          float lumiMC[nFiles] = {-1.0};
          float csMC[nFiles] = {132.6};
          bool skim[nFiles] = {0};
          int color = 402; //kYellow+2
          float lumiDataTotal = -1.0;
          TInputSample bkgmc (nFiles, sourceName, "Z#gamma{#rightarrow}l#bar{l}", color, lumiDataTotal, fileNames, lumiData, lumiMC, csMC, skim);
          return bkgmc;          
        }
      //the last iBkg must be nBkgSources_-1

    } //end of else if (sample==BKGMC)

  return empty;
}


TString TConfiguration::GetSelectedFileName(int channel, int sample, int iBkg, bool isDebugMode)
{
  if ( !CheckSample(sample) || !CheckChannel(channel) ) return "";

  TString fileName=selectedEventsDir_;
  
  if (channel==MUON && sample==DATA) 
    fileName+=selectedEventsNameDataMu_;
  else if (channel==ELECTRON && sample==DATA)  
    fileName+=selectedEventsNameDataEle_;
  else if (channel==MUON && sample==SIGMC) 
    fileName+=selectedEventsNameSignalMCMu_;
  else if (channel==ELECTRON && sample==SIGMC) 
    fileName+=selectedEventsNameSignalMCMu_;
  else if (sample==BKGMC)
    {
      TInputSample input = GetInputSample (channel, sample, iBkg);
      fileName+=selectedEventsNameBkgMC_;
      if (iBkg>=0 && iBkg<nBkgSources_)
        fileName+=input.sourceName_;
      else
        {
           std::cout<<"ERROR detected in TConfiguration::GetSelectedFileName: no "<<iBkg<<" background source exists for muons"<<std::cout;
           return "";
        }
    }

  if (isDebugMode) fileName+=nameDebugMode_;
  fileName+=".root";
  return fileName;
}

TString TConfiguration::GetPhosphorConstantFileName()
{
  return phosphorConstantsFile_;
}

int TConfiguration::GetNSources(int sample)
{
  if (sample==DATA || sample==SIGMC) return 1;
  else if (sample==BKGMC) return nBkgSources_;
  else std::cout<<"ERROR detected in TConfiguration::GetNSources: sample "<<sample<<" is not known"<<std::endl;
  return 0;
}

float TConfiguration::GetLumiWeight(int channel, int sample, int iBkg, int iFile)
{
  if (sample==DATA) return 1.0;

  if (!CheckSample(sample) || !CheckChannel(channel) || !CheckBkgNumber(sample,iBkg)) return -1.0;


  TInputSample data = GetInputSample(channel, DATA);
  float lumiData = data.lumiDataTotal_;
  TInputSample mc = GetInputSample(channel, sample, iBkg);
  if (iFile>=mc.nFiles_) 
    {
      std::cout<<"ERROR detected in TConfiguration::GetLumiWeight: iFile="<<iFile<<" is too big"<<std::endl;
      return -1.0;
    }

  if (mc.lumiMC_[iFile]!=-1.0) return (lumiData / mc.lumiMC_[iFile]);
  TFile f(mc.fileNames_[iFile]);
  if (!f.IsOpen()) 
    {
      std::cout<<"ERROR detected in TConfiguration::GetLumiWeight: file "<<mc.fileNames_[iFile]<<" is not open"<<std::endl;
      return -1.0;
    }
  f.cd("ggNtuplizer");

  TTree* tr = (TTree*)gDirectory->Get("EventTree");
  long nEvents = tr->GetEntries();

  if (mc.isSkimmed_[iFile])
    {

      TH1F* hist = (TH1F*)gDirectory->Get("hskim");
      nEvents = nEvents * hist->GetBinContent(1) / hist->GetBinContent(2);
      hist = 0;

    }

  tr = 0;
  return (lumiData / (nEvents/mc.csMC_[iFile]));

}

bool TConfiguration::CheckSample(int sample)
{
  if (sample==DATA || sample==SIGMC || sample==BKGMC) return 1;
  std::cout<<"ERROR detected in TConfiguration::CheckSample: sample = "<<sample<<" is unknown"<<std::endl;
  return 0;
}

bool TConfiguration::CheckChannel(int channel)
{
  if (channel==MUON || channel==ELECTRON) return 1;
  std::cout<<"ERROR detected in TConfiguration::CheckChannel: channel = "<<channel<<" is unknown"<<std::endl;
  return 0;
}

bool TConfiguration::CheckBkgNumber(int sample, int iBkg)
{
  if (!CheckSample(sample)) return 0;
  if (sample==BKGMC && iBkg>=nBkgSources_) 
    {
      std::cout<<"ERROR detected in TConfiguration::CheckBkgNumber: (iBkg="<<iBkg<<") > (nBkgSources_="<<nBkgSources_<<"). nBkgSources_ can be increased in TConfiguration.h "<<std::endl;
      return 0;
    }
  return 1;
}
