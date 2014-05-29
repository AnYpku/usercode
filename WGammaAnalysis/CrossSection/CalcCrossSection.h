#ifndef CalcCrossSection_h
#define CalcCrossSection_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include <vector>
#include <string>
  //C++
#include "TH1F.h"
#include "TFile.h"

class CalcCrossSection
  {
     public:
       CalcCrossSection (int channel, int blind, string configfile="../Configuration/config.txt");

       virtual ~CalcCrossSection();
       void    Calc();
       void    GetSignalYields();
       void    ApplyEfficiency();
       void    ApplyUnfolding();
       void    ApplyAcceptance();
       void    DivideOverLumi();
       void    DivideOverBinWidth();
       void    Plot();
       void    Print(TString strYields);



     private:

       int _channel;
       TAllInputSamples* _INPUT;
       TConfiguration _config;
       TFile* _fOut;
       TH1F* _signalYieldTotal;
       TH1F* _signalYields1D;
       float _lumi;
  };

#endif 
  //#ifndef 
