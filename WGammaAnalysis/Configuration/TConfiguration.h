#ifndef TConfiguration_h
#define TConfiguration_h

#include <string>
 
#include "TString.h" 
#include "TColor.h" 
  //ROOT class

class TConfiguration
{
  public:
    TConfiguration();
    virtual ~TConfiguration();
    string GetPhosphorConstantFileName();

    string GetSelectedEventsDir();
    string GetSelectedNameDataMu();
    string GetSelectedNameDataEle();
    string GetSelectedNameSignalMCMu();
    string GetSelectedNameSignalMCEle();
    string GetSelectedNameBkgMC();

    string GetSignalTemplateNameMu();
    string GetSignalTemplateNameEle();
    string GetBkgTemplateRawNameMu();
    string GetBkgTemplateRawNameEle();
    string GetTemplatePicNameBase();
//    string GetBkgTemplateNameMu();
//    string GetBkgTemplateNameEle();

    string GetAccEffDirName();
    string GetAccEffFileName();
    string GetAcc1DName();
    string GetEff1DName();
    string GetAccErr1DName();
    string GetEffErr1DName();
    string GetAccTotalName();
    string GetEffTotalName();
    string GetAccErrTotalName();
    string GetEffErrTotalName();

    string GetPhotonScaleFactorsFileNamePt15to20GeV();
    string GetPhotonScaleFactorsFileNamePt20toInfGeV();

    string GetNameDebugMode();
    string GetNameReleasedCuts();
    string GetPileupDataFileName();
    int GetNPhoPtBins();
    vector <float> GetPhoPtBinsLimits();
    int FindPhoPtBinByPhoPt(float pt);
    float GetPhoPtMin();
    float GetLePhoDeltaRMin();

};

/////////////////////////////////////////
//photon Pt binning
static const int nPhoPtBins_ = 9;
static const float phoPtBinsLimits_[nPhoPtBins_+1]={15.,20.,25.,30.,35.,40.,60.,80.,200.,600.};
static const float phoPtMin_=15.;
  //minimum value for total CS
static const float lePhoDeltaRMin_=0.7;
  //minimum value for total and differential cross section; 
  //cut value may be bigger or equal to this value

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
static const string selectedEventsDir_="../WGammaOutput/";
static const string selectedEventsNameDataMu_="selected_MUON_DATA";//.root
static const string selectedEventsNameDataEle_="selected_ELECTRON_DATA";//.root
static const string selectedEventsNameSignalMCMu_="selected_MUON_SIGMC";//.root
static const string selectedEventsNameSignalMCEle_="selected_ELECTRON_SIGMC";//.root
static const string selectedEventsNameBkgMC_="selected_BKGMC_";//[input.sourceName_].root

//////////////////////////////////////////
//data driven background estimation
//
static const string signalTemplateNameMu_="../WGammaOutput/templateSignal_MUON.root";
static const string signalTemplateNameEle_="../WGammaOutput/templateSignal_ELECTRON.root";
static const string bkgTemplateRawNameMu_="../WGammaOutput/templateRawBkg_MUON.root";
static const string bkgTemplateRawNameEle_="../WGammaOutput/templateRawBkg_ELECTRON.root";
static const string templatePicNameBase_="../WGammaOutput/templateFits/templateFit_";
//static const string bkgTemplateNameMu_="../WGammaOutput/bkgTemplate_MUON.root";
//static const string bkgTemplateNameEle_="../WGammaOutput/bkgTemplate_ELECTRON.root";

//////////////////////////////////////////
//acceptance and efficiency
//
static const string acceffDir_="../WGammaOutput/";
static const string acceffFile_="AccEff_MUON.root";
static const string acc1DName_="acc1D"; 
static const string eff1DName_="eff1D"; 
static const string accErr1DName_="accErr1D"; 
static const string effErr1DName_="effErr1D";
static const string accTotalName_="accTotal"; 
static const string effTotalName_="effTotal"; 
static const string accErrTotalName_="accErrTotal"; 
static const string effErrTotalName_="effErrTotal";

//////////////////////////////////////////
//scale factors
//
static const string photonScaleFactorsFileNamePt15to20GeV_="../CertifiedConstants/PhotonScaleFactors/ScaleFactors_Pt15to20GeV_2012.root";
static const string photonScaleFactorsFileNamePt20toInfGeV_="../CertifiedConstants/PhotonScaleFactors/ScaleFactors_Pt20toInfGeV_2012.root";

//////////////////////////////////////////
static const string nameDebugMode_ = "_debugMode";
static const string nameReleasedCuts_ = "_releasedCuts";
static const string pileupDataFileName_ = "../CertifiedConstants/Pileup/TruePU_69300_2012Full.root";

/////////////////////////////////////////
// phosphor corrections file
static const string phosphorConstantsFile_ = "../CertifiedConstants/PHOSPHOR_NUMBERS_EXPFIT_ERRORS.txt";

#endif //#ifndef TConfiguration_h
