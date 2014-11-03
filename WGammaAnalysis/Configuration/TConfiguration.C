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

TString TConfiguration::StrChannel(int channel){
  if (channel==MUON) return "MUON";
  if (channel==ELECTRON) return "ELECTRON";
  if (channel==BOTH) return "BOTH";
}
 
TString TConfiguration::StrVgType(int vg_type){
  if (vg_type==W_GAMMA) return "W_GAMMA";
  if (vg_type==Z_GAMMA) return "Z_GAMMA";
  if (vg_type==V_GAMMA) return "V_GAMMA";
} 

TString TConfiguration::StrSample(int sample){
  if (sample==DATA) return "DATA";
  if (sample==SIGMC) return "SIGMC";
  if (sample==BKGMC) return "BKGMC";
}
 
TString TConfiguration::StrBlindType(int blind){
  if (blind==UNBLIND) return "UNBLIND";
  if (blind==BLIND_PRESCALE) return "BLIND_PRESCALE";
  if (blind==BLIND_DECRACC) return "BLIND_DECRACC";
}

TString TConfiguration::StrEtaBin(int etaBin){
  if (etaBin==BARREL) return "BARREL";
  if (etaBin==ENDCAP) return "ENDCAP";
  if (etaBin==COMMON) return "COMMON";
}

TString TConfiguration::StrCsMode(int csMode){
  if (csMode==TOTAL) return "TOTAL"; 
  if (csMode==ONEDI) return "ONEDI"; 
  if (csMode==TWODI) return "TWODI"; 
}

TString TConfiguration::StrSelectionStage(int sel){
  if (sel==VERY_PRELIMINARY) return "VERY_PRELIMINARY"; 
  if (sel==PRELIMINARY_FOR_MET_CUT) return "PRELIMINARY_FOR_MET_CUT"; 
  if (sel==PRELIMINARY_FOR_TEMPLATE_METHOD) return "PRELIMINARY_FOR_TEMPLATE_METHOD"; 
  if (sel==PRELIMINARY_FOR_UNFOLDING) return "PRELIMINARY_FOR_UNFOLDING"; 
  if (sel==FULLY) return "FULLY"; 
}

void TConfiguration::Print()
{

  std::cout<<"GetOutputDirName s: "<<GetOutputDirName(MUON)<<", "<<GetOutputDirName(ELECTRON)<<std::endl;
  std::cout<<"GetSampleName s: "<<GetSampleName(DATA)<<", "<<GetSampleName(SIGMC)<<", "<<GetSampleName(BKGMC)<<std::endl;
  std::cout<<"GetEtaBinName s: "<<GetEtaBinName(BARREL)<<", "<<GetEtaBinName(ENDCAP)<<", "<<GetEtaBinName(COMMON)<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetSelectedPreliminaryName s: "<<GetSelectedName(VERY_PRELIMINARY,MUON,W_GAMMA,UNBLIND,DATA,"[sourceName]")<<", "<<GetSelectedName(VERY_PRELIMINARY,MUON,Z_GAMMA,UNBLIND,SIGMC,"[sourceName]")<<", "<<GetSelectedName(VERY_PRELIMINARY,MUON,Z_GAMMA,BLIND_PRESCALE,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetYieldsFileName s: "<<GetYieldsFileName(MUON,W_GAMMA,"phoEt")<<", "<<GetYieldsFileName(ELECTRON,W_GAMMA,"WMt")<<std::endl;
  std::cout<<"GetSelectedHistName s: "<<GetYieldsSelectedName(TOTAL,BARREL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(TOTAL,BARREL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BARREL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BARREL,SIGMC,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BARREL,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetDDTemplateFileName s"<<GetDDTemplateFileName(MUON,W_GAMMA,"WMt")<<std::endl;

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

TString TConfiguration::GetBlindModeName(int sample, int blindMode)
{
  if (sample!=DATA) return "";
  if (blindMode==UNBLIND) return "UNblind";
  else if (blindMode==BLIND_PRESCALE) return "blindPRESCALE";
  else if (blindMode==BLIND_DECRACC) return "blindDECRACC";
  return "UNKNOWN_BLIND_MODE";
}

TString TConfiguration::GetVgTypeName(int vgamma)
{
  if (vgamma==W_GAMMA) return "WGamma";
  else if (vgamma==Z_GAMMA) return "ZGamma";
  else if (vgamma==V_GAMMA) return "VGamma";
  return "UNKNOWN_VGAMMA";
}

TString TConfiguration::GetSelectedName(int selectionStage, int channel, int vgamma, int blindType, int sample, TString sourceName, bool isDebugMode, bool isNoPuReweight)
{
  TString name = GetOutputDirName(channel); 
  name+=_selectedNameBase[selectionStage];
  name+="_";
  name+=GetVgTypeName(vgamma);
  if (sample==DATA){
    name+="_";
    name+=GetBlindModeName(sample,blindType);
    name+="_";
  }
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

TString TConfiguration::GetYieldsFileName(int channel, int vgamma, TString strKin)
{
  return GetOutputDirName(channel)+_yieldsFileName+TString("_")+GetVgTypeName(vgamma)+TString("_")+strKin+TString("_.root");
}


TString TConfiguration::GetYieldsSelectedName(int csMode, int etaBin,int sample, TString sourceName)
{
  TString name = _yieldsSelectedName+GetSampleName(sample);
  if (sample==BKGMC) name+=sourceName;
  return name+GetCsModeName(csMode)+GetEtaBinName(etaBin);
}

TString TConfiguration::GetYieldsSelectedSignalMCGenName(int csMode, int etaBin)
{
  return _yieldsSelectedSignalMCGenName+GetCsModeName(csMode)+GetEtaBinName(etaBin);
}

TString TConfiguration::GetYieldsDDTemplateFakeName(int csMode, int etaBin)
{
  return _yieldsDDTemplateFakeName+GetCsModeName(csMode)+GetEtaBinName(etaBin);
}

TString TConfiguration::GetYieldsDDTemplateTrueName(int csMode, int etaBin)
{
  return _yieldsDDTemplateTrueName+GetCsModeName(csMode)+GetEtaBinName(etaBin);
}

TString TConfiguration::GetYieldsSignalName(int csMode, int etaBin)
{
  return _yieldsSignalName+GetCsModeName(csMode)+GetEtaBinName(etaBin);
}

TString TConfiguration::GetDDTemplateFileName(int channel, int vgamma, TString strKin)
{
  return GetOutputDirName(channel)+_DDTemplateFileName+TString("_")+GetVgTypeName(vgamma)+TString("_")+strKin+TString("_.root");
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



//TString TConfiguration::GetUnfoldingFileName(int channel)
//{
//  return GetOutputDirName(channel)+unfoldingFileName_;
//}

//TString TConfiguration::GetMatrUnfo1DName()
//{
//  return matrUnfo1DName_;
//}

//TString TConfiguration::GetMatrMigr1DName()
//{
//  return matrMigr1DName_;
//}

//TString TConfiguration::GetYieldsRec1DName()
//{
//  return yieldsRec1DName_;
//}

//TString TConfiguration::GetYieldsGen1DName()
//{
//  return yieldsGen1DName_;
//}

TString TConfiguration::GetPhotonScaleFactorsFileNamePt15to20GeV()
{
  return _certifiedConstantsDir+_photonScaleFactorsFileNamePt15to20GeV;
}

TString TConfiguration::GetPhotonScaleFactorsFileNamePt20toInfGeV()
{
  return _certifiedConstantsDir+_photonScaleFactorsFileNamePt20toInfGeV;
}

TString TConfiguration::GetPileupDataFileName()
{
  return _certifiedConstantsDir+_pileupDataFileName;
}

TString TConfiguration::GetPhosphorConstantFileName()
{
  return (_phosphorConstantsFile);
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

int TConfiguration::GetBlindPrescale()
{
  return _blindPrescale;
}

int TConfiguration::GetPhoPtBlindThreshold()
{
  return _phoPtBlindThreshold;
}

int TConfiguration::GetNPhoPtUnfBins(bool isOverflowUsed)
{ 
  if (isOverflowUsed) return GetNPhoPtBins()+2;
  return GetNPhoPtBins()+1;
}

void TConfiguration::GetPhoPtUnfBinsLimits(float* lims, bool isOverflowUsed)
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
