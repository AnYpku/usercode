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
    string GetNameDebugMode();
    string GetNameReleasedCuts();
    int GetNPhoPtBins();
    vector <float> GetPhoPtBinsLimits();

};

/////////////////////////////////////////
//photon Pt binning
static const int nPhoPtBins_ = 9;
static const float phoPtBinsLimits_[nPhoPtBins_+1]={15.,20.,25.,30.,35.,40.,60.,80.,200.,600.};

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
static const string selectedEventsDir_="../WGammaOutput/";
static const string selectedEventsNameDataMu_="selected_MUON_DATA";//.root
static const string selectedEventsNameDataEle_="selected_ELECTRON_DATA";//.root
static const string selectedEventsNameSignalMCMu_="selected_MUON_SIGMC";//.root
static const string selectedEventsNameSignalMCEle_="selected_ELECTRON_SIGMC";//.root
static const string selectedEventsNameBkgMC_="selected_BKGMC_";//[input.sourceName_].root
static const string nameDebugMode_ = "_debugMode";
static const string nameReleasedCuts_ = "_releasedCuts";

/////////////////////////////////////////
// phosphor corrections file
static const string phosphorConstantsFile_ = "../CertifiedConstants/PHOSPHOR_NUMBERS_EXPFIT_ERRORS.txt";

#endif //#ifndef TConfiguration_h
