#include "TConfiguration.h" 
  //this class
#include <iostream> 
#include <vector> 
  //standard C++ class
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

TConfiguration::TConfiguration()
{
}

TConfiguration::~TConfiguration()
{
}

void TConfiguration::Print()
{

  std::cout<<"GetOutputDirName s: "<<GetOutputDirName(MUON)<<", "<<GetOutputDirName(ELECTRON)<<std::endl;
  std::cout<<"GetSampleName s: "<<GetSampleName(DATA)<<", "<<GetSampleName(SIGMC)<<", "<<GetSampleName(BKGMC)<<std::endl;
  std::cout<<"GetEtaBinName s: "<<GetEtaBinName(BARREL)<<", "<<GetEtaBinName(ENDCAP)<<", "<<GetEtaBinName(COMMON)<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetSelectedPreliminaryName s: "<<GetSelectedName(PRELIMINARY,MUON,DATA,"[sourceName]")<<", "<<GetSelectedName(PRELIMINARY,MUON,SIGMC,"[sourceName]")<<", "<<GetSelectedName(PRELIMINARY,MUON,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetYieldsFileName s: "<<GetYieldsFileName(MUON)<<", "<<GetYieldsFileName(ELECTRON)<<std::endl;
  std::cout<<"GetSelectedHistName s: "<<GetYieldsSelectedHistName(DATA,COMMON,"[sourceName]")<<", "<<GetYieldsSelectedHistName(DATA,BARREL,"[sourceName]")<<", "<<GetYieldsSelectedHistName(DATA,ENDCAP,"[sourceName]")<<", "<<GetYieldsSelectedHistName(SIGMC,BARREL,"[sourceName]")<<", "<<GetYieldsSelectedHistName(BKGMC,BARREL,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetFractionsDDTemplateBkgFileName s"<<GetFractionsDDTemplateBkgFileName(MUON)<<std::endl;

}

TString TConfiguration::GetOutputDirName(int channel)
{
  if (channel==MUON) return outputDirMu_;
  else if (channel==ELECTRON) return outputDirEle_;
  return "0";
} 

TString TConfiguration::GetSampleName(int sample)
{
  if (sample==DATA) return "DATA";
  else if (sample==SIGMC) return "SIGMC";
  else if (sample==BKGMC) return "BKGMC";
  return "UNKNOWN_SAMPLE";
}

TString TConfiguration::GetEtaBinName(int etaBin)
{
  if (etaBin==BARREL) return "B";
  else if (etaBin==ENDCAP) return "E";
  else if (etaBin==COMMON) return "";
  return "UNKNOWN_ETABIN";
}

TString TConfiguration::GetSelectedName(int selectionStage, int channel, int sample, TString sourceName, bool isDebugMode, bool isNoPuReweight, bool isVeryLooseSelectionMode)
{
  TString name = GetOutputDirName(channel); 
  if (selectionStage==VERY_PRELIMINARY) 
    name+=selectedVeryPreliminaryEventsNameBase_;
  else if (selectionStage==PRELIMINARY)
    name+=selectedPreliminaryEventsNameBase_;
  else if (selectionStage==FULLY)
    name+=selectedFullyEventsNameBase_;
  name+=GetSampleName(sample);
  if (sample == BKGMC){
    name+="_";
    name+=sourceName;
  }
  name+=GetSpecialModeName(isDebugMode, isNoPuReweight, isVeryLooseSelectionMode);
  name+=".root";

  return name;


}

TString TConfiguration::GetSpecialModeName(bool isDebugMode, bool isNoPuReweight, bool isVeryLooseSelectionMode)
{
  TString name="";
  if (isDebugMode) name+=nameDebugMode_;
  if (isNoPuReweight) name+=nameNoPuReweight_;
  if (isVeryLooseSelectionMode) name+=nameVeryLooseSelectionMode_;
  return name;
}

TString TConfiguration::GetNameDebugMode()
{
  return nameDebugMode_;
}

TString TConfiguration::GetNameNoPuReweight()
{
  return nameNoPuReweight_;
}

TString TConfiguration::GetNameVeryLooseSelectionMode()
{
  return nameVeryLooseSelectionMode_;
}

TString TConfiguration::GetYieldsFileName(int channel)
{
  return GetOutputDirName(channel)+yieldsFileName_;
}


TString TConfiguration::GetYieldsSelectedHistName(int sample, int etaBin, TString sourceName)
{
  TString name = yieldsSelectedHistName_+GetSampleName(sample);
  if (sample==BKGMC) return name+sourceName+GetEtaBinName(etaBin);
  return name+GetEtaBinName(etaBin);
}

TString TConfiguration::GetYieldsSelectedSignalMCGenHistName()
{
  return yieldsSelectedSignalMCGenHistName_;
}

TString TConfiguration::GetYieldsDDTemplateBkgHistName(int etaBin)
{
  return yieldsDDTemplateBkgHist_+GetEtaBinName(etaBin);
}

TString TConfiguration::GetYieldsSignalName(int etaBin)
{
  return yieldsSignal_+GetEtaBinName(etaBin);
}

TString TConfiguration::GetFractionsDDTemplateBkgFileName(int channel)
{
  return GetOutputDirName(channel)+fractionsDDTemplateBkgFile_;
}

TString TConfiguration::GetFractionsDDTemplateBkgHistName(int etaBin)
{
  return fractionsDDTemplateBkgHist_+GetEtaBinName(etaBin);
}

TString TConfiguration::GetTemplatePicNameBase(int ptBin, int etaBin)
{
  return templatePicNameBase_+"_"+TString(ptBin)+"_"+GetEtaBinName(etaBin);
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

TString TConfiguration::GetAccEff1DName()
{
  return acceff1DName_;
}

TString TConfiguration::GetAccTotalName()
{
  return accTotalName_;
}

TString TConfiguration::GetEffTotalName()
{
  return effTotalName_;
}

TString TConfiguration::GetAccEffTotalName()
{
  return acceffTotalName_;
}

TString TConfiguration::GetUnfoldingFileName(int channel)
{
  return GetOutputDirName(channel)+unfoldingFileName_;
}

TString TConfiguration::GetMatrUnfo1DName()
{
  return matrUnfo1DName_;
}

TString TConfiguration::GetMatrMigr1DName()
{
  return matrMigr1DName_;
}

TString TConfiguration::GetYieldsRec1DName()
{
  return yieldsRec1DName_;
}

TString TConfiguration::GetYieldsGen1DName()
{
  return yieldsGen1DName_;
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

int TConfiguration::GetNPhoPtUnfBins(bool isOverflowUsed)
{ 
  if (isOverflowUsed) return GetNPhoPtBins()+2;
  return GetNPhoPtBins()+1;
}

vector <float> TConfiguration::GetPhoPtUnfBinsLimits(bool isOverflowUsed)
{
  vector <float> analysLims;
  vector <float> unfoldLims;
  analysLims = GetPhoPtBinsLimits();
  unfoldLims.push_back(0.0);
  for (int i=0; i<nPhoPtBins_+1; i++)
    unfoldLims.push_back(analysLims[i]);
  if (isOverflowUsed) 
    unfoldLims.push_back(phoPtOverflowBinLimit_);
  return unfoldLims;
}

int TConfiguration::FindPhoPtUnfBinByPhoPt(float pt, bool isOverflowUsed)
{
  if (pt<phoPtBinsLimits_[0]) return 0;
  for (int i=0; i<GetNPhoPtBins()+1; i++)
    if (pt>=phoPtBinsLimits_[i] && pt<phoPtBinsLimits_[i+1])
      return i+1;
  if (isOverflowUsed && pt>=phoPtBinsLimits_[GetNPhoPtBins()]) return GetNPhoPtUnfBins(1)-1;
  return -1; 
}
