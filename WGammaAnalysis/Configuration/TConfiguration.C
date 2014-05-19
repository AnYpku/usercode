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
  _selectedNameBase[VERY_PRELIMINARY]="VeryPreliminarySelected/selected";
  _selectedNameBase[PRELIMINARY_FOR_MET_CUT]="PreliminaryForMEtCutSelected/selected";
  _selectedNameBase[PRELIMINARY_FOR_TEMPLATE_METHOD]="PreliminaryForTemplateMethodSelected/selected";
  _selectedNameBase[PRELIMINARY_FOR_UNFOLDING]="PreliminaryForUnfoldingSelected/selected";
  _selectedNameBase[FULLY]="FullySelected/selected";
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
  std::cout<<"GetSelectedPreliminaryName s: "<<GetSelectedName(VERY_PRELIMINARY,MUON,DATA,"[sourceName]")<<", "<<GetSelectedName(VERY_PRELIMINARY,MUON,SIGMC,"[sourceName]")<<", "<<GetSelectedName(VERY_PRELIMINARY,MUON,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetYieldsFileName s: "<<GetYieldsFileName(MUON)<<", "<<GetYieldsFileName(ELECTRON)<<std::endl;
  std::cout<<"GetSelectedHistName s: "<<GetYieldsSelectedName(TOTAL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(TOTAL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,SIGMC,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetFractionsDDTemplateBkgFileName s"<<GetDDTemplateBkgFileName(MUON)<<std::endl;

}

TString TConfiguration::GetOutputDirName(int channel)
{
  if (channel==MUON) return _outputDirMu;
  else if (channel==ELECTRON) return _outputDirEle;
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

TString TConfiguration::GetCsModeName(int csMode)
{
  if (csMode==TOTAL) return "Total";
  else if (csMode==ONEDI) return "1D";
  else if (csMode==TWODI) return "2D";
  return "UNKNOWN_CSMODE";
}

TString TConfiguration::GetSelectedName(int selectionStage, int channel, int sample, TString sourceName, bool isDebugMode, bool isNoPuReweight)
{
  TString name = GetOutputDirName(channel); 
  name+=_selectedNameBase[selectionStage];
  name+=GetSampleName(sample);
  if (sample == BKGMC){
    name+="_";
    name+=sourceName;
  }
  name+=GetSpecialModeName(isDebugMode, isNoPuReweight);
  name+=".root";

  return name;
}

TString TConfiguration::GetSpecialModeName(bool isDebugMode, bool isNoPuReweight)
{
  TString name="";
  if (isDebugMode) name+=nameDebugMode_;
  if (isNoPuReweight) name+=nameNoPuReweight_;
  return name;
}

TString TConfiguration::GetYieldsFileName(int channel)
{
  return GetOutputDirName(channel)+_yieldsFileName;
}


TString TConfiguration::GetYieldsSelectedName(int csMode,int sample, TString sourceName)
{
  TString name = _yieldsSelectedName+GetSampleName(sample);
  if (sample==BKGMC) name+=sourceName;
  return name+GetCsModeName(csMode);
}

TString TConfiguration::GetYieldsSelectedSignalMCGenName(int csMode)
{
  return _yieldsSelectedSignalMCGenName+GetCsModeName(csMode);
}

TString TConfiguration::GetYieldsDDTemplateBkgName(int csMode)
{
  return _yieldsDDTemplateBkgName+GetCsModeName(csMode);
}

TString TConfiguration::GetYieldsSignalName(int csMode)
{
  return _yieldsSignalName+GetCsModeName(csMode);
}

TString TConfiguration::GetDDTemplateBkgFileName(int channel)
{
  return GetOutputDirName(channel)+_DDTemplateBkgFileName;
}


TString TConfiguration::GetAccFileName(int channel)
{
  return GetOutputDirName(channel)+_accFileName;
}

TString TConfiguration::GetAccName(int csMode)
{
  return _accName+GetCsModeName(csMode);
}

TString TConfiguration::GetEffFileName(int channel)
{
  return GetOutputDirName(channel)+_effFileName;
}

TString TConfiguration::GetEffName(int csMode)
{
  return _effName+GetCsModeName(csMode);
}

/*

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
*/
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
  return _nPhoPtBins;
}

void TConfiguration::GetPhoPtBinsLimits(float* lims)
{
  for (int i=0; i<_nPhoPtBins+1; i++)
    lims[i]=_phoPtBinsLimits[i];
 
}

int TConfiguration::FindPhoPtBinByPhoPt(float pt)
{
  for (int i=0; i<_nPhoPtBins+1; i++)
    if (pt>=_phoPtBinsLimits[i] && pt<_phoPtBinsLimits[i+1])
      return i;
  return -1; 
}

float TConfiguration::GetPhoPtMin()
{
  return _phoPtMin;
}

float TConfiguration::GetPhoPtMax()
{
  return _phoPtMax;
}

float TConfiguration::GetLePhoDeltaRMin()
{
  return _lePhoDeltaRMin;
}

int TConfiguration::GetNPhoPtUnfBins(bool isOverflowUsed)
{ 
  if (isOverflowUsed) return GetNPhoPtBins()+2;
  return GetNPhoPtBins()+1;
}

void TConfiguration::GetPhoPtUnfBinsLimits(float* lims,bool isOverflowUsed)
{
  lims[0]=0.0;
  for (int i=0; i<_nPhoPtBins+2; i++)
    lims[i+1]=_phoPtBinsLimits[i];
  if (isOverflowUsed) 
    lims[_nPhoPtBins+2]=_phoPtMax;
}

int TConfiguration::FindPhoPtUnfBinByPhoPt(float pt, bool isOverflowUsed)
{
  if (pt<_phoPtBinsLimits[0]) return 0;
  for (int i=0; i<GetNPhoPtBins()+1; i++)
    if (pt>=_phoPtBinsLimits[i] && pt<_phoPtBinsLimits[i+1])
      return i+1;
  if (isOverflowUsed && pt>=_phoPtBinsLimits[GetNPhoPtBins()]) return GetNPhoPtUnfBins(1)-1;
  return -1; 
}
