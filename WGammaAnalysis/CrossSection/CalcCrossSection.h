#ifndef CalcCrossSection_h
#define CalcCrossSection_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include <vector>
#include <string>
  //C++
#include "TH1F.h"
#include "TH2D.h"
#include "TFile.h"

class CalcCrossSection
  {
     public:
    CalcCrossSection (int channel, int vgamma, int blind, string configfile="../Configuration/config.txt");

       virtual ~CalcCrossSection();
       void    Calc();
       void    GetSignalYields();
       void    ApplyUnfolding();
       void    ApplyAccXEff();
       void    DivideOverLumi();
       void    DivideOverBinWidth();
       void    Plot();
       void    Print(TString strYields, TH1F* hTot, TH1F* h1D);



     private:

       int _channel;
       int _vgamma;
       int _blind;
       TAllInputSamples* _INPUT;
       TConfiguration _config;
       TFile* _fOut;

       TH1F* _yieldTOT_bkgSubtr;
       TH1F* _yields1D_bkgSubtr;

       TH1F* _yieldTOT_unfolded;
       TH1F* _yields1D_unfolded;
       TH2D* _covMatrix;

       TH1F* _yieldTOT_accXeffCorr;
       TH1F* _yields1D_accXeffCorr;
       TH2D* _covMatrix_accXeffCorr;

       TH1F* _yieldTOT_overLumi;
       TH1F* _yields1D_overLumi;
       TH2D* _covMatrix_overLumi;

       TH1F* _crossSectionTOT;
       TH1F* _crossSection1D;
       TH2D* _covMatrix_overBinWidth;


       float _lumi;
  };

#endif 
  //#ifndef 
