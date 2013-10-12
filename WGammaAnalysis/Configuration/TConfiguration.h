#ifndef TConfiguration_h
#define TConfiguration_h
 
#include "TString.h" 
#include "TColor.h" 
  //ROOT class

class TConfiguration
{
  public:
    TConfiguration();
    virtual ~TConfiguration();

    enum {MUON, ELECTRON};

    TString GetOutputDirName(int channel);

    TString GetSelectedPreliminaryNameData(int channel);
    TString GetSelectedPreliminaryNameSignalMC(int channel);
    TString GetSelectedPreliminaryNameBkgMC(int channel);
    TString GetSelectedFullyNameData(int channel);
    TString GetSelectedFullyNameSignalMC(int channel);
    TString GetSelectedFullyNameBkgMC(int channel);

    TString GetYieldsFileName(int channel);
    TString GetYieldsSelectedHistName(int sample, TString sourceName);
    TString GetYieldsDDTemplateBkgHistName();
    TString GetYieldsSignalName();

    TString GetTemplatePicNameBase(int channel);
    TString GetYieldsDDTemplateBkgFileName(int channel);
    TString GetFractionsDDTemplateBkgHistName();

    TString GetAccEffFileName(int channel);
    TString GetAcc1DName();
    TString GetEff1DName();
    TString GetAccErr1DName();
    TString GetEffErr1DName();
    TString GetAccTotalName();
    TString GetEffTotalName();
    TString GetAccErrTotalName();
    TString GetEffErrTotalName();

    TString GetNameDebugMode();

    TString GetPhotonScaleFactorsFileNamePt15to20GeV();
    TString GetPhotonScaleFactorsFileNamePt20toInfGeV();
    TString GetPileupDataFileName();
    TString GetPhosphorConstantFileName();

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
//general locations
//
static const TString outputDirMu_="../WGammaOutput/MUON/";
static const TString outputDirEle_="../WGammaOutput/ELECTRON/";

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
static const TString selectedPreliminaryEventsNameData_="PreliminarySelected/selectedDATA";//.root
static const TString selectedPreliminaryEventsNameSignalMC_="PreliminarySelected/selectedSIGMC";//.root
static const TString selectedPreliminaryEventsNameBkgMC_="PreliminarySelected/selectedBKGMC_";//[input.sourceName_].root
static const TString selectedFullyEventsNameData_="FullySelected/selectedDATA";//.root
static const TString selectedFullyEventsNameSignalMC_="FullySelected/selectedSIGMC";//.root
static const TString selectedFullyEventsNameBkgMC_="FullySelected/selectedBKGMC_";//[input.sourceName_].root

//////////////////////////////////////////
//yields
//
static const TString yieldsFileName_="yields.root";
static const TString yieldsSelectedHistName_="yieldsSelected";
static const TString yieldsDDTemplateBkgHist_="yieldsDDBkg";
static const TString yieldsSignal_="yieldsSignal";


//////////////////////////////////////////
//data driven background estimation
//
static const TString templatePicNameBase_="TemplateFits/templateFit_";
static const TString yieldsDDTemplateBkgFile_="yieldsDDTemplateBkg.root";
static const TString fractionsDDTemplateBkgHist_="fractionsBkg";

//////////////////////////////////////////
//acceptance and efficiency
//
static const TString acceffFile_="AccEff.root";
static const TString acc1DName_="acc1D"; 
static const TString eff1DName_="eff1D"; 
static const TString accErr1DName_="accErr1D"; 
static const TString effErr1DName_="effErr1D";
static const TString accTotalName_="accTotal"; 
static const TString effTotalName_="effTotal"; 
static const TString accErrTotalName_="accErrTotal"; 
static const TString effErrTotalName_="effErrTotal";

//////////////////////////////////////////
static const TString nameDebugMode_ = "_debugMode";

//////////////////////////////////////////
//certrified constants
//
static const TString certifiedConstantsDir_="../CertifiedConstants/";
static const TString photonScaleFactorsFileNamePt15to20GeV_="PhotonScaleFactors/ScaleFactors_Pt15to20GeV_2012.root";
static const TString photonScaleFactorsFileNamePt20toInfGeV_="PhotonScaleFactors/ScaleFactors_Pt20toInfGeV_2012.root";
static const TString pileupDataFileName_ = "Pileup/TruePU_69300_2012Full.root";
// phosphor corrections file
static const TString phosphorConstantsFile_ = "PHOSPHOR_NUMBERS_EXPFIT_ERRORS.txt";

#endif //#ifndef TConfiguration_h
