#ifndef CalcCrossSection_h
#define CalcCrossSection_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include <vector>
  //C++

class CalcCrossSection
  {
     public:
       CalcCrossSection (int channel, string configfile="../Configuration/config.txt");

       virtual ~CalcCrossSection();
       void    GetSignalYields(string cut);
       void    ApplyAccAndEff();
       void    DivideOverLumi();
       void    DivideOverBinWidth();
       void    SaveOutput();

     private:

       TAllInputSamples* INPUT_;
       TConfiguration config_;
       vector <float> signalYields_;
       vector <float> signalYieldsErr_;
       float lumi_;

       vector <float> vecPhoPtLimits_;
  };

#endif 
  //#ifndef 
