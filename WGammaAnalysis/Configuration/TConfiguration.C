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
  std::cout<<"GetSelectedHistName s: "<<GetYieldsSelectedName(TOTAL,DATA,COMMON,"[sourceName]")<<", "<<GetYieldsSelectedName(TOTAL,DATA,BARREL,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,DATA,ENDCAP,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,SIGMC,BARREL,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BKGMC,BARREL,"[sourceName]")<<std::endl;
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

TString TConfiguration::GetValTypeName(int valType)
{
  if (valType==VAL) return "Val";
  else if (valType==ERRSTAT) return "ErrStat";
  else if (valType==ERRSYST) return "ErrSyst";
  return "UNKNOWN_ValType";
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

TString TConfiguration::GetYieldsFileName(int channel)
{
  return GetOutputDirName(channel)+_yieldsFileName;
}


TString TConfiguration::GetYieldsSelectedName(int csMode,  int valType, int sample, TString sourceName)
{
  TString name = _yieldsSelectedName+GetSampleName(sample);
  if (sample==BKGMC) name+=sourceName;
  return name+GetCsModeName(csMode)+GetValTypeName(valType);
}

TString TConfiguration::GetYieldsSelectedSignalMCGenName(int csMode, int valType)
{
  return _yieldsSelectedSignalMCGenName+GetCsModeName(csMode)+GetValTypeName(valType);
}

TString TConfiguration::GetYieldsDDTemplateBkgName(int csMode, int valType)
{
  return _yieldsDDTemplateBkgName+GetCsModeName(csMode)+GetValTypeName(valType);
}

TString TConfiguration::GetYieldsSignalName(int csMode, int valType)
{
  return _yieldsSignalName+GetCsModeName(csMode)+GetValTypeName(valType);
}

TString TConfiguration::GetDDTemplateBkgFileName(int channel)
{
  return GetOutputDirName(channel)+_DDTemplateBkgFileName;
}

/*
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

vector <float> TConfiguration::GetPhoPtBinsLimits()
{
  vector <float> lims;
  for (int i=0; i<_nPhoPtBins+1; i++)
    lims.push_back(_phoPtBinsLimits[i]);
  return lims;   
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

float TConfiguration::GetLePhoDeltaRMin()
{
  return _lePhoDeltaRMin;
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
  for (int i=0; i<_nPhoPtBins+1; i++)
    unfoldLims.push_back(analysLims[i]);
  if (isOverflowUsed) 
    unfoldLims.push_back(_phoPtOverflowBinLimit);
  return unfoldLims;
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
