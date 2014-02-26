#ifndef Unfolding_h
#define Unfolding_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
#include "../Include/TPuReweight.h"
#include "../RooUnfold/src/RooUnfold.h"
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

       enum {LOCAL_INVERSION=0, LOCAL_DAGOSTINI, LOCAL_DAGOSTINI_NOITERATIONS, RooUnfold_INVERSION, RooUnfold_DAGOSTINI};

       bool PrepareConstants();

       bool ApplyUnfolding(TH1D* inputYields, TH1D* unfoldedYields, int method);
       bool TestDifferentMethods();

       bool PrepareMigrationMatrix();

       bool PrepareUnfoldingMatrix();
       float CalculateMatrixSpectralNorm(TMatrixD &matrix);
       bool CalculateInvertedMatrixErrors(TMatrixD &T, TMatrixD &TErrPos, TMatrixD &TErrNeg, TMatrixD &TinvErr);
       bool PseudoMatrixInversion(TMatrixD& mOriginal, TMatrixD& mInverted);

       bool PlotAndStore();

       bool ApplyUnfoldingInversion(TH1D* inputYields, TH1D* unfoldedYields);
       bool ApplyUnfoldingDAgostiniNoIterations(TH1D* inputYields, TH1D* unfoldedYields);
       bool ApplyUnfoldingDAgostini(TH1D* inputYields, TH1D* unfoldedYields);
       bool DoIterations(TH2D* histRespMatrix, TH1D* histGenYields, TH2D* histUnfoBayes, TH1D* histInputYields);
       double DoOneIteration(TH2D* histRespMatrix, TH2D* histUnfoBayes, TH1D* histInputYields, TH1D* nOld, TH1D* nNew, TH1D* pOld, TH1D* pNew);
         //return chi2/ndf between nOld and nNew
       bool ApplyRooUnfold(TH1D* inputYields, TH1D* unfoldedYields,RooUnfold::Algorithm alg);

       bool MatrixTimesVector(TH2D* mat, TH1D* vec, TH1D* resultVec, bool noPrint=0, bool hasReference=0, TH1D* referenceVec=0);


     private:

       float _unfolded[10][20];
       float _unfoldedErrStat[10][20];
       float _unfoldedErrSyst[10][20];
         //yield; err propagated through unfolding; 
         //err coming from finite MC; igenBin 

       TAllInputSamples* INPUT_;
       int channel_;

       TConfiguration config_;

       vector <float> vecPhoPtLimitsRec_;
       vector <float> vecPhoPtLimitsGen_;
       float* binLimsRec_;
       float* binLimsGen_;
       
       int nBinsRec_;
       int nBinsGen_;

       TFile* fIn_;
       TFile* fOut_;

       TH2D* histEventCountMigrMatrix_;
       TH2D* histMigrMatrixNotNormalized_;

       TH2D* histResponseMatrix_;

       TH2D* histUnfoMatrixDAg_;

       TH2D* histUnfoMatrix_;
       TH2D* histUnfoMatrixInv_;
       
       TH2D* histRooResponse_;

       TH1D* histYieldsRec_;
       TH1D* histYieldsRecSmeared_;
       TH1D* histYieldsGen_;

       TTree* tr_;

       bool isOverflowRec_;
       bool isOverflowGen_;
  };

#endif 
  //#ifndef 
