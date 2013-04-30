#ifndef TConfiguration_h
#define TConfiguration_h

#include "TInputSample.h" 
  //this package
#include "TString.h" 
#include "TColor.h" 
  //ROOT class

class TConfiguration
{
  public:
    TConfiguration();
    virtual ~TConfiguration();
    TInputSample GetInputSample( int channel, int sample, int iBkg=0);
    TString GetSelectedFileName(int channel, int sample, int iBkg=0, bool isDebugMode=0);
    TString GetPhosphorConstantFileName();
    int GetNSources(int sample);
    float GetLumiWeight(int channel, int sample, int iBkg=0, int iFile=0);

    bool CheckChannel(int channel);
    bool CheckSample(int sample);
    bool CheckBkgNumber(int sample, int iBkg);

    enum {MUON, ELECTRON};//channel
    enum {DATA, SIGMC, BKGMC};//sample

    static const int nBkgSources_ = 5;
};

/////////////////////////////////////////
//photon Pt binning
static const int nPhoPtBins_ = 9;
static const float phoPtBinsLimits_[nPhoPtBins_+1]={15.,20.,25.,30.,35.,40.,60.,80.,200.,600.};

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
static const TString selectedEventsDir_="../WGammaOutput/";
static const TString selectedEventsNameDataMu_="selected_MUON_DATA";//.root
static const TString selectedEventsNameDataEle_="selected_ELECTRON_DATA";//.root
static const TString selectedEventsNameSignalMCMu_="selected_MUON_SIGMC";//.root
static const TString selectedEventsNameSignalMCEle_="selected_ELECTRON_SIGMC";//.root
static const TString selectedEventsNameBkgMC_="selected_BKGMC_";//[input.sourceName_].root
static const TString nameDebugMode_ = "_debugMode";

/////////////////////////////////////////
// phosphor corrections file
static const TString phosphorConstantsFile_ = "../CertifiedConstants/PHOSPHOR_NUMBERS_EXPFIT_ERRORS.txt";

#endif //#ifndef TConfiguration_h
