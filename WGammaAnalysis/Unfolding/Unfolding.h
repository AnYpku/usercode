#ifndef Unfolding_h
#define Unfolding_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
#include "../Include/TPuReweight.h"
#include "../RooUnfold/src/RooUnfold.h"
  //this package
//#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
#include "TTree.h" 
#include "TMatrixD.h" 
#include "TMath.h" 
#include "TH2D.h"
  //ROOT package

class Unfolding
  {
     public:
       Unfolding (int channel,int vgamma);
       virtual ~Unfolding();

       bool TestDifferentMethods();

       bool PrepareMigrationMatrix();

       bool ApplyRooUnfold(TH1D* inputYields, TH1D* unfoldedYields,RooUnfold::Algorithm alg=RooUnfold::kBayes);
       bool ApplyRooUnfold(TH1D* inputYields, TH1D* unfoldedYields,RooUnfold::Algorithm alg, TMatrixD& errCovStat, TVectorD& errStatV, TVectorD& errSystV, TVectorD& errCovStatV);
       bool ComputeSystErrors(TH1D* histInputYields, float* errSyst,RooUnfold::Algorithm alg, int NSmears);
       bool ComputeStatErrors(TH1D* histInputYields, float* errStat, RooUnfold::Algorithm alg, int NSmears);

       bool PlotAndStore();


     private:

//       TAllInputSamples* _INPUT;
       int _channel;
       int _vgamma;

       TConfiguration _config;

       float* _phoPtLimitsRec;
       float* _phoPtLimitsGen;
      // vector <float> vecPhoPtLimitsRec_;
      // vector <float> vecPhoPtLimitsGen_;
       
       int _nBinsRec;
       int _nBinsGen;

       TFile* _fIn;
       TFile* _fOut;

       TH2D* _histEventCountMigrMatrix;
       TH2D* _histMigrMatrixNotNormalized;

       TH1D* _histYieldsRec;
       TH1D* _histYieldsRecSmeared;
       TH1D* _histYieldsGen;

       TTree* _tr;

       bool _isOverflowRec;
       bool _isOverflowGen;
  };

#endif 
  //#ifndef 
