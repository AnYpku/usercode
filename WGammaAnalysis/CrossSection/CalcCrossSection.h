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

       enum{ERR_STAT,
            ERR_SYST_CHISOvsSIHIH,
            ERR_SYST_TemplStat,
            ERR_SYST_etogStat,
            ERR_SYST_accXeff_MCstat,
            ERR_SYST_LUMI,
            ERR_NONE};
      // FromYieldToCS _yCSsyst_CHISOvsSIHIH;
      // FromYieldToCS _yCSsyst_TemplStat;
      // FromYieldToCS _yCSsyst_etogStat;
      // FromYieldToCS _yCSsyst_accXeff_MCstat;

       struct FromYieldToCS{

         int errType;

         TString title;
         TString name;
         TString strUp;
         TString strDown;

         TH1F* yieldTOT_bkgSubtr;
         TH1F* yields1D_bkgSubtr;

         TH1F* yieldTOT_unfolded;
         TH1F* yields1D_unfolded;
         TH2D* covMatrix;

         TH1F* yieldTOT_accXeffCorr;
         TH1F* yields1D_accXeffCorr;
         TH2D* covMatrix_accXeffCorr;

         TH1F* yieldTOT_overLumi;
         TH1F* yields1D_overLumi;
         TH2D* covMatrix_overLumi;

         TH1F* crossSectionTOT;
         TH1F* crossSection1D;
//         TH2D* covMatrix_overBinWidth;

       };

       virtual ~CalcCrossSection();
       void    Calc();

       void    GetSignalYields();
       void    GetYieldsSyst(FromYieldToCS& yCS, TString strFile, TString str1D, TString strTOT);

       void    ApplyUnfolding(FromYieldToCS& yCS);
       void    ApplyAccXEff(FromYieldToCS& yCS);
       void    DivideOverLumi(FromYieldToCS& yCS);
       void    DivideOverBinWidth(FromYieldToCS& yCS);
       void    Plot(FromYieldToCS& yCS);
       void    Print(TString strYields, TH1F* hTot, TH1F* h1D);

       void    PrintLatexAll_ErrInPercent();
       void    PrintLatexAll_MeasVsMCbased();



     private:

       int _channel;
       int _vgamma;
       int _blind;
       TAllInputSamples* _INPUT;
       TConfiguration _config;
       TFile* _fOut;

       static const int Nerrs=10;
       FromYieldToCS _yCSarray[Nerrs];

//       FromYieldToCS _yCSstat;
//       FromYieldToCS _yCSsyst_CHISOvsSIHIH;
//       FromYieldToCS _yCSsyst_TemplStat;
//       FromYieldToCS _yCSsyst_etogStat;
//      FromYieldToCS _yCSsyst_accXeff_MCstat;

//       FromYieldToCS _yCSstatPLUSsyst;


       float _lumi;
       static const float _lumiErr=0.026;//2.6%
  };

#endif 
  //#ifndef 
