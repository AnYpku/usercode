#ifndef Unfolding_h
#define Unfolding_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
#include "../Include/TPuReweight.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
#include "TTree.h" 
#include "TMatrixD.h" 
#include "TMath.h" 
#include "TH2D.h"
  //ROOT package

class Unfolding
  {
     public:
       Unfolding (int channel, string configfile="../Configuration/config.txt");
       virtual ~Unfolding();

       void PrepareMigrationMatrix();
       void PrepareUnfoldingMatrix();
       float CalculateMatrixSpectralNorm(TMatrixD &matrix);
       void CalculateInvertedMatrixErrors(TMatrixD &T, TMatrixD &TErrPos, TMatrixD &TErrNeg, TMatrixD &TinvErr);

       void ClosureTest();

       void Store();


     private:

       TAllInputSamples* INPUT_;
       int channel_;

       TConfiguration config_;

       vector <float> vecPhoPtLimits_;

       TFile* fOut_;

       TH2D* histMigrMatrix_;
       TH2D* histUnfoMatrix_;

       TH1F* histYieldsRec_;
       TH1F* histYieldsGen_;

       bool isOverflowUsed_;
  };

#endif 
  //#ifndef 
