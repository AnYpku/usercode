#ifndef TConfiguration_h
#define TConfiguration_h
 
#include "TString.h" 
#include "TColor.h" 
#include "TCut.h" 
  //ROOT class
#include <vector>
#include <string>

class TConfiguration
{
  public:
    TConfiguration();
    virtual ~TConfiguration();

    enum {MUON, ELECTRON, BOTH_CHANNELS};//channel
    enum {W_GAMMA, Z_GAMMA, V_GAMMA};//vg_type
    enum {DATA=0, SIGMC=1, BKGMC=2, SIGMC_UNSKIMMED=5};//sample
    enum {UNBLIND, BLIND_PRESCALE, BLIND_COMBINED};//blinding type
    enum {BARREL=0, ENDCAP=1, COMMON=2};//eta bin, must match enum in TTemplateRandCone.h
    enum {TOTAL, ONEDI, TWODI};//csMode
    enum {VERY_PRELIMINARY,
          PRELIMINARY_FOR_TEMPLATE_METHOD,
          PRELIMINARY_FOR_E_TO_GAMMA,
          PRELIMINARY_FOR_UNFOLDING,
          FSR,
          FSR_EXCLUDED,
          FULLY};//selection stage
    enum {TEMPL_CHISO=0, TEMPL_SIHIH=1, TEMPL_OVERLAY=2};
      // template fits: from I_ch (with random cone isolation method)
      // or with sigmaIEtaIEta
    enum {PLOTS_TEMPL_FITS, PLOTS_PREPARE_YIELDS, PLOTS_CONSTANTS, 
          PLOTS_CROSS_SECTION, PLOTS_QUICK_CHECKS, PLOTS_TEMPL_FITS_SYST};

    struct AnalysisParameters{
      int year;
      int channel;
      int vgamma;
      int templFits;
      TString varKin;
      int nKinBins;
      float* kinBinLims;
      int sampleMode;
      string analyzedSamples;
      string configfile;
      bool isNoPuReweight;
      bool isDebugMode;
      int phoWP;
      TCut cutAdd;

      int blind[3][2];//channel, vgamma

      bool noPreSelection[2][2];//channel, vgamma
      bool noExtraSelection[2][2];//channel, vgamma
      bool noDDBkgComputation[2][2][2];//channel, vgamma, different templates
      bool noDDBkgEtoGamma;
      bool noPrepareYields[2][2];//channel, vgamma
      bool noSubtractBackground[2][2][3];//channel, vgamma, different templates
      bool noCalcAccAndEff[2][2];//channel, vgamma
      bool noCalcCrossSection[2][2];//channel, vgamma

      bool noSystDDBkgSidebandVariation[2][2][2];//channel, vgamma, different templates
    };

    TString StrChannel(int channel); 
    TString StrVgType(int vg_type); 
    TString StrSample(int sample); 
    TString StrBlindType(int blind);
    TString StrEtaBin(int etaBin);
    TString StrCsMode(int csMode);
    TString StrSelectionStage(int sel); 
    TString StrTempl(int templ);
    TString StrPlotsType(int plotsType);


    void Print();

    TString GetOutputDirName(int channel, int vgamma);
    TString GetPlotsDirName(int channel, int vgamma, int plotsType);

//    TString GetSampleName(int sample);
//    TString GetEtaBinName(int etaBin);
//    TString GetCsModeName(int csMode);
//    TString GetBlindModeName(int sample, int blindMode);
//    TString GetVgTypeName(int vgamma);

    TString GetSelectedName(int selectionStage, int channel, int vgamma, int blindType, int sample, TString sourceName="", bool isDebugMode=0, bool isNoPuReweight=0);
    //strSelectionStage
    TString GetSpecialModeName(bool isDebugMode, bool isNoPuReweight);

    TString GetYieldsFileName(int channel, int vgamma, int templ, TString strKin);
    TString GetYieldsMCtruthFileName(int channel, int vgamma, TString strKin);

    TString GetYieldsSelectedName(int csMode, int etaBin, int sample, TString sourceName="");
    TString GetYieldsDDTemplateFakeName(int csMode, int etaBin);
    TString GetYieldsDDTemplateTrueName(int csMode, int etaBin);
    TString GetYieldsDDBkgEtoGamma(int csMode, int etaBin);
    TString GetYieldsBkgSubtrDataName(int csMode, int etaBin=COMMON);
    TString GetYieldsSignalMCGenBinsName(int csMode, int etaBin=COMMON);

    TString GetDDTemplateFileName(int channel, int vgamma, int templ, TString strKin);
    TString GetDDBkgEtoGammaFileName(TString strKin);

    TString GetSidebandsFileName(int channel, int vgamma, int templ, TString strKin);
    TString GetSidebandsLowerHistName(int ieta);
    TString GetSidebandsUpperHistName(int ieta);
    TString GetMCtruthTrueHistName(int ieta);
    TString GetFromMCfitTrueHistName(int ieta);

    TString GetAccXEffFileName(int channel, int vgamma);
    TString GetAccXEffName(int csMode);
    TString GetTheoryCSname(int csMode);

    TString GetCrossSectionFileName(int channel, int vgamma);
    TString GetCSname(int channel, int csMode);

    TString GetSystCHISOvsSIHIHname(int csMode, int etaBin);

//    TString GetUnfoldingFileName(int channel);
//    TString GetMatrUnfo1DName();
//    TString GetMatrMigr1DName();
//    TString GetYieldsRec1DName();
//    TString GetYieldsGen1DName();

    TString GetPhotonScaleFactorsFileNamePt15to20GeV();
    TString GetPhotonScaleFactorsFileNamePt20toInfGeV();
    TString GetPileupDataFileName();
    TString GetPhosphorConstantFileName();

    int GetNPhoPtBins();
    void GetPhoPtBinsLimits(float* lims);
    int FindPhoPtBinByPhoPt(float pt);

    float GetPhoPtMin();
    float GetPhoPtMax();
    float GetLePhoDeltaRMin();

    int GetBlindPrescale();
    int GetPhoPtBlindThreshold();

    int GetNPhoPtUnfBins(bool isOverflowUsed);
    void GetPhoPtUnfBinsLimits(float* lims,bool isOverflowUsed);
    int FindPhoPtUnfBinByPhoPt(float pt, bool isOverflowUsed);

};

/////////////////////////////////////////
//photon Pt binning
//and other analysis constants
//static const int _nPhoPtBins = 1;
//static const float _phoPtBinsLimits[_nPhoPtBins+1]={15.,20.};
//static const int _nPhoPtBins = 5;
//static const float _phoPtBinsLimits[_nPhoPtBins+1]={15.,20.,25.,30.,35.,40.};
//static const int _nPhoPtBins = 2;
//static const float _phoPtBinsLimits[_nPhoPtBins+1]={15.,20.,25.};
static const int _nPhoPtBins = 13;
static const float _phoPtBinsLimits[_nPhoPtBins+1]={10.,15.,20.,25.,30.,35.,45.,55.,65.,75.,85.,95.,120.,500.};
static const float _phoPtMin=10.;
static const float _phoPtMax=2000.;
  //minimum value for total CS
static const float _lePhoDeltaRMin=0.7;
  //minimum value for total and differential cross section; 
  //cut value may be bigger or equal to this value
static const int _blindPrescale=20;//13;
static const float _phoPtBlindThreshold=45.;

/////////////////////////////////////////
//general locations
//
static const TString _outputDir="../WGammaOutput/";
//all other pathes are relative to one of these two

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
//names are listed in the constructor for now
TString _selectedNameBase[7];

//////////////////////////////////////////
//yields
//
static const TString _yieldsFileName="YieldsAndBackground/yields";
static const TString _yieldsSelectedName="yieldsSelected";
static const TString _yieldsDDTemplateFakeName="yieldsDDFake";
static const TString _yieldsDDTemplateTrueName="yieldsDDTrue";

//////////////////////////////////////////
//data driven background estimation
//
static const TString _DDTemplateFileName="YieldsAndBackground/DDTemplate";
static const TString _DDBkgEtoGammaFileName="YieldsAndBackground/DDEtoGamma";


//////////////////////////////////////////
//acceptance and efficiency
//
static const TString _accXeffFileName="Constants/AccXEff.root";
static const TString _accXeffName="accXeff"; 

//////////////////////////////////////////
//unfolding
//
static const TString unfoldingFileName_="Constants/Unfolding.root";
static const TString matrUnfo1DName_="matrUnfo1D";
static const TString matrMigr1DName_="matrMigr1D";
static const TString yieldsRec1DName_="yieldsRec1D";
static const TString yieldsGen1DName_="yieldsGen1D";

//////////////////////////////////////////
static const TString nameDebugMode_ = "_debugMode";
static const TString nameNoPuReweight_="_noPuReweight";

//////////////////////////////////////////
//certrified constants
//
static const TString _certifiedConstantsDir="../CertifiedConstants/";
static const TString _photonScaleFactorsFileNamePt15to20GeV="PhotonScaleFactors/ScaleFactors_Pt15to20GeV_2012.root";
static const TString _photonScaleFactorsFileNamePt20toInfGeV="PhotonScaleFactors/ScaleFactors_Pt20toInfGeV_2012.root";
static const TString _pileupDataFileName = "Pileup/TruePU_69300_2012Full.root";
// phosphor corrections file
static const TString _phosphorConstantsFile = "../PHOSPHOR_CORRECTION/PHOSPHOR_NUMBERS_EXPFIT_ERRORS_2012ABCD_HggRegression_NoIsolationCut.txt";

#endif //#ifndef TConfiguration_h
