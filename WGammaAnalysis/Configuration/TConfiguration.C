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
  _selectedNameBase[PRELIMINARY_FOR_TEMPLATE_METHOD]="PreliminaryForTemplateMethodSelected/selected";
  _selectedNameBase[PRELIMINARY_FOR_E_TO_GAMMA]="PreliminaryForEtoGammaSelected/selected";
  _selectedNameBase[PRELIMINARY_FOR_UNFOLDING]="PreliminaryForUnfoldingSelected/selected";
  _selectedNameBase[FSR]="FsrSelected/selected";
  _selectedNameBase[FSR_EXCLUDED]="FsrExcludedSelected/selected";
  _selectedNameBase[FULLY]="FullySelected/selected";
}

TConfiguration::~TConfiguration()
{
}

TString TConfiguration::StrChannel(int channel){
  if (channel==MUON) return "MUON";
  if (channel==ELECTRON) return "ELECTRON";
  if (channel==BOTH_CHANNELS) return "ChannelsMERGED";
  return "";
}
 
TString TConfiguration::StrVgType(int vg_type){
  if (vg_type==W_GAMMA) return "WGamma";
  if (vg_type==Z_GAMMA) return "ZGamma";
  if (vg_type==V_GAMMA) return "VGamma";
  return "";
} 

TString TConfiguration::StrSample(int sample){
  if (sample==DATA) return "DATA";
  if (sample==SIGMC) return "SIGMC";
  if (sample==BKGMC) return "BKGMC";
  if (sample==SIGMC_UNSKIMMED) return "SIGMC_UNSKIMMED";
  return "";
}
 
TString TConfiguration::StrBlindType(int blind){
  if (blind==UNBLIND) return "UNblind";
  if (blind==BLIND_PRESCALE) return "blindPRESCALE";
  if (blind==BLIND_COMBINED) return "blindCOMBINED";
  return "";
}

TString TConfiguration::StrEtaBin(int etaBin){
  if (etaBin==BARREL) return "BARREL";
  if (etaBin==ENDCAP) return "ENDCAP";
  if (etaBin==COMMON) return "COMMON";
  return "";
}

TString TConfiguration::StrCsMode(int csMode){
  if (csMode==TOTAL) return "TOTAL"; 
  if (csMode==ONEDI) return "ONEDI"; 
  if (csMode==TWODI) return "TWODI";
  return ""; 
}

TString TConfiguration::StrSelectionStage(int sel){
  if (sel==VERY_PRELIMINARY) return "VERY_PRELIMINARY"; 
  if (sel==PRELIMINARY_FOR_TEMPLATE_METHOD) return "PRELIMINARY_FOR_TEMPLATE_METHOD"; 
  if (sel==PRELIMINARY_FOR_E_TO_GAMMA) return "PRELIMINARY_FOR_E_TO_GAMMA";
  if (sel==PRELIMINARY_FOR_UNFOLDING) return "PRELIMINARY_FOR_UNFOLDING"; 
  if (sel==FSR_EXCLUDED) return "FSR_EXCLUDED";
  if (sel==FSR) return "FSR";
  if (sel==FULLY) return "FULLY";
  return ""; 
}

TString TConfiguration::StrTempl(int templ){
  if (templ==TEMPL_CHISO) return "TEMPL_CHISO";
  if (templ==TEMPL_SIHIH) return "TEMPL_SIHIH";
  return "";
}

TString TConfiguration::StrPlotsType(int plotsType)
{
  if (plotsType==PLOTS_TEMPL_FITS) return "TemplateFits";
  if (plotsType==PLOTS_PREPARE_YIELDS) return "PrepareYields";
  if (plotsType==PLOTS_CONSTANTS) return "Constants";
  if (plotsType==PLOTS_CROSS_SECTION) return "CrossSection";
  if (plotsType==PLOTS_QUICK_CHECKS) return "QuickChecks";
  if (plotsType==PLOTS_TEMPL_FITS_SYST) return "TemplateFitsSyst";
  return "";
}

void TConfiguration::Print()
{

  std::cout<<"GetPlotsDirName s: "<<GetPlotsDirName(MUON, W_GAMMA, PLOTS_TEMPL_FITS)<<", "<<GetPlotsDirName(ELECTRON, Z_GAMMA, PLOTS_PREPARE_YIELDS)<<std::endl;
  std::cout<<"GetOutputDirName s: "<<GetOutputDirName(MUON, W_GAMMA)<<", "<<GetOutputDirName(ELECTRON, W_GAMMA)<<std::endl;
  std::cout<<"GetSampleName s: "<<StrSample(DATA)<<", "<<StrSample(SIGMC)<<", "<<StrSample(BKGMC)<<std::endl;
  std::cout<<"GetEtaBinName s: "<<StrEtaBin(BARREL)<<", "<<StrEtaBin(ENDCAP)<<", "<<StrEtaBin(COMMON)<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetSelectedPreliminaryName s: "<<GetSelectedName(VERY_PRELIMINARY,MUON,W_GAMMA,UNBLIND,DATA,"[sourceName]")<<", "<<GetSelectedName(VERY_PRELIMINARY,MUON,Z_GAMMA,UNBLIND,SIGMC,"[sourceName]")<<", "<<GetSelectedName(VERY_PRELIMINARY,MUON,Z_GAMMA,BLIND_PRESCALE,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetYieldsFileName s: "<<GetYieldsFileName(MUON,W_GAMMA,TEMPL_CHISO,"phoEt")<<", "<<GetYieldsFileName(ELECTRON,Z_GAMMA,TEMPL_SIHIH,"WMt")<<std::endl;
  std::cout<<"GetSelectedHistName s: "<<GetYieldsSelectedName(TOTAL,BARREL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(TOTAL,BARREL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BARREL,DATA,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BARREL,SIGMC,"[sourceName]")<<", "<<GetYieldsSelectedName(ONEDI,BARREL,BKGMC,"[sourceName]")<<std::endl;
  std::cout<<std::endl;
  std::cout<<"GetDDTemplateFileName s"<<GetDDTemplateFileName(MUON,W_GAMMA,TEMPL_CHISO,"WMt")<<std::endl;
  std::cout<<"GetDDEtoGammaFileName s"<<GetDDBkgEtoGammaFileName("phoEt")<<std::endl;

}

TString TConfiguration::GetOutputDirName(int channel, int vgamma){
  TString strDir=_outputDir+StrChannel(channel)+TString("_")+StrVgType(vgamma)+TString("/");
  return strDir;
} 

TString TConfiguration::GetPlotsDirName(int channel, int vgamma, int plotsType){
  TString strDir=GetOutputDirName(channel, vgamma)+TString("Plots/")+StrPlotsType(plotsType)+TString("/");
  return strDir;
}

TString TConfiguration::GetSelectedName(int selectionStage, int channel, int vgamma, int blindType, int sample, TString sourceName, bool isDebugMode, bool isNoPuReweight)
{
  TString name = GetOutputDirName(channel,vgamma); 
  name+=_selectedNameBase[selectionStage];
  name+="_";
  name+=StrVgType(vgamma);
  if (sample==DATA){
    name+="_";
    name+=StrBlindType(blindType);
    name+="_";
  }
  name+=StrSample(sample);
  if (sample == BKGMC){
    name+="_";
    name+=sourceName;
  }
  name+=GetSpecialModeName(isDebugMode, isNoPuReweight);
  name+=".root";

  return name;
}

TString TConfiguration::GetSpecialModeName(bool isDebugMode, bool isNoPuReweight){
  TString name="";
  if (isDebugMode) name+=nameDebugMode_;
  if (isNoPuReweight) name+=nameNoPuReweight_;
  return name;
}

TString TConfiguration::GetYieldsFileName(int channel, int vgamma, int templ, TString strKin){
  return GetOutputDirName(channel,vgamma)+_yieldsFileName+TString("_")+StrVgType(vgamma)+TString("_")+StrTempl(templ)+TString("_")+strKin+TString("_.root");
}

TString TConfiguration::GetYieldsMCtruthFileName(int channel, int vgamma, TString strKin){
  return GetOutputDirName(channel,vgamma)+_yieldsFileName+TString("_MCtruth_")+StrVgType(vgamma)+TString("_")+strKin+TString("_.root");
}


TString TConfiguration::GetYieldsSelectedName(int csMode, int etaBin,int sample, TString sourceName){
  TString name = _yieldsSelectedName+TString("_")+StrSample(sample)+TString("_");
  if (sample==BKGMC) name+=sourceName;
  return name+TString("_")+StrCsMode(csMode)+TString("_")+StrEtaBin(etaBin);
}

TString TConfiguration::GetYieldsDDTemplateFakeName(int csMode, int etaBin){
  return _yieldsDDTemplateFakeName+StrCsMode(csMode)+StrEtaBin(etaBin);
}

TString TConfiguration::GetYieldsDDTemplateTrueName(int csMode, int etaBin){
  return _yieldsDDTemplateTrueName+StrCsMode(csMode)+StrEtaBin(etaBin);
}

TString TConfiguration::GetYieldsDDBkgEtoGamma(int csMode, int etaBin){
  return TString("yieldsBkgEtoGamma")+StrCsMode(csMode)+StrEtaBin(etaBin);
}

TString TConfiguration::GetYieldsBkgSubtrDataName(int csMode, int etaBin){
  return TString("yieldsBkgSubtrData_")+StrCsMode(csMode)+TString("_")+StrEtaBin(etaBin);
}

TString TConfiguration::GetSystCHISOvsSIHIHname(int csMode, int etaBin){
  return TString("systCHISOvsSIHIH_")+StrCsMode(csMode)+TString("_")+StrEtaBin(etaBin);
}

TString TConfiguration::GetSystBkgSubtrZgWgtaunu(int csMode, int etaBin){
  return TString("systBkgSubtrZgWgtaunu_")+StrCsMode(csMode)+TString("_")+StrEtaBin(etaBin);
}

TString TConfiguration::GetYieldsSignalMCGenBinsName(int csMode, int etaBin){
  return TString("yieldsSignalMCGenBins_")+StrCsMode(csMode)+TString("_")+StrEtaBin(etaBin);
}

TString TConfiguration::GetDDTemplateFileName(int channel, int vgamma, int templ, TString strKin){
  return GetOutputDirName(channel,vgamma)+_DDTemplateFileName+TString("_")+StrVgType(vgamma)+TString("_")+StrTempl(templ)+TString("_")+strKin+TString("_.root");}

TString TConfiguration::GetDDBkgEtoGammaFileName(TString strKin){
  return GetOutputDirName(ELECTRON,W_GAMMA)+_DDBkgEtoGammaFileName+TString("_")+StrVgType(W_GAMMA)+TString("_")+strKin+TString("_.root");}

TString TConfiguration::GetAccXEffFileName(int channel, int vgamma){
  return GetOutputDirName(channel, vgamma)+_accXeffFileName;}

TString TConfiguration::GetAccXEffName(int csMode){
  return _accXeffName+StrCsMode(csMode);}

TString TConfiguration::GetTheoryCSname(int csMode){
  return TString("csTheory_")+StrCsMode(csMode);}

TString TConfiguration::GetCrossSectionFileName(int channel, int vgamma){
  return  GetOutputDirName(channel, vgamma)+TString("Constants/CS.root");}

TString TConfiguration::GetCSname(int channel, int csMode){
  return TString("csMeasured_")+StrChannel(channel)+TString("_")+StrCsMode(csMode);}

TString TConfiguration::GetSidebandsFileName(int channel, int vgamma, int templ, TString strKin){
  return GetOutputDirName(channel, vgamma)+TString("Constants/sidebands_")+StrTempl(templ)+TString("_")+strKin+TString(".root");
}// end of GetSidebandsFileName

TString TConfiguration::GetSidebandsLowerHistName(int ieta){
  return TString("sidebandsLower_")+StrEtaBin(ieta);}

TString TConfiguration::GetSidebandsUpperHistName(int ieta){
  return TString("sidebandsUpper_")+StrEtaBin(ieta);}

TString TConfiguration::GetMCtruthTrueHistName(int ieta){
  return TString("hTrueMCtruth_")+StrEtaBin(ieta);}

TString TConfiguration::GetFromMCfitTrueHistName(int ieta){
  return TString("hTrueFromMCfit_")+StrEtaBin(ieta);}

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

TString TConfiguration::GetPhotonScaleFactorsFileNamePt15to20GeV(){
  return _certifiedConstantsDir+_photonScaleFactorsFileNamePt15to20GeV;
}

TString TConfiguration::GetPhotonScaleFactorsFileNamePt20toInfGeV(){
  return _certifiedConstantsDir+_photonScaleFactorsFileNamePt20toInfGeV;
}

TString TConfiguration::GetPileupDataFileName(){
  return _certifiedConstantsDir+_pileupDataFileName;
}

TString TConfiguration::GetPhosphorConstantFileName(){
  return (_phosphorConstantsFile);
}

int TConfiguration::GetNPhoPtBins(){
  return _nPhoPtBins;
}

void TConfiguration::GetPhoPtBinsLimits(float* lims){
  for (int i=0; i<_nPhoPtBins+1; i++)
    lims[i]=_phoPtBinsLimits[i];
}

int TConfiguration::FindPhoPtBinByPhoPt(float pt){
  for (int i=0; i<_nPhoPtBins+1; i++)
    if (pt>=_phoPtBinsLimits[i] && pt<_phoPtBinsLimits[i+1])
      return i;
  return -1; 
}

float TConfiguration::GetPhoPtMin(){
  return _phoPtMin;
}

float TConfiguration::GetPhoPtMax(){
  return _phoPtMax;
}

float TConfiguration::GetLePhoDeltaRMin(){
  return _lePhoDeltaRMin;
}

int TConfiguration::GetBlindPrescale(){
  return _blindPrescale;
}

int TConfiguration::GetPhoPtBlindThreshold(){
  return _phoPtBlindThreshold;
}

int TConfiguration::GetNPhoPtUnfBins(bool isOverflowUsed){ 
  std::cout<<"TConfiguration::GetNPhoPtUnfBins:"<<std::endl;
  std::cout<<"isOverflowUsed="<<isOverflowUsed<<std::endl;
  std::cout<<" GetNPhoPtBins()="<< GetNPhoPtBins()<<std::endl;
  if (isOverflowUsed) return GetNPhoPtBins()+1;
  return GetNPhoPtBins()+0;
}

void TConfiguration::GetPhoPtUnfBinsLimits(float* lims, bool isOverflowUsed){
//  lims[0]=10.0;
  std::cout<<"TConfiguration::GetPhoPtUnfBinsLimits:"<<std::endl;
  std::cout<<"isOverflowUsed="<<isOverflowUsed<<std::endl;
  std::cout<<"_nPhoPtBins="<<_nPhoPtBins<<std::endl;
  for (int i=0; i<_nPhoPtBins+1; i++){
    lims[i]=_phoPtBinsLimits[i];
    std::cout<<"lims["<<i<<"]="<<lims[i]<<std::endl;
  }
  if (isOverflowUsed) 
    lims[_nPhoPtBins+1]=_phoPtMax;
}// end of TConfiguration::GetPhoPtUnfBinsLimits

int TConfiguration::FindPhoPtUnfBinByPhoPt(float pt, bool isOverflowUsed){
  if (pt<_phoPtBinsLimits[0]) return 0;
  for (int i=0; i<GetNPhoPtBins()+1; i++)
    if (pt>=_phoPtBinsLimits[i] && pt<_phoPtBinsLimits[i+1])
      return i+1;
  if (isOverflowUsed && pt>=_phoPtBinsLimits[GetNPhoPtBins()]) return GetNPhoPtUnfBins(1)-1;
  return -1; 
}
