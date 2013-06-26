#ifndef CalcCrossSection_h
#define CalcCrossSection_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include <vector>
#include <string>
  //C++

class CalcCrossSection
  {
     public:
       CalcCrossSection (int channel, string configfile="../Configuration/config.txt");

       virtual ~CalcCrossSection();
       void    Calc(string cut);
       void    GetSignalYields(string cut);
       void    ApplyAccAndEff();
       void    DivideOverLumi();
       void    DivideOverBinWidth();
       void    PlotAndSaveOutput();

     private:

       int channel_;
       TAllInputSamples* INPUT_;
       TConfiguration config_;
       float signalYieldTotal_;
       float signalYieldErrTotal_;
       vector <float> signalYields1D_;
       vector <float> signalYieldsErr1D_;
       float lumi_;

       vector <float> vecPhoPtLimits_;
  };

#endif 
  //#ifndef 
