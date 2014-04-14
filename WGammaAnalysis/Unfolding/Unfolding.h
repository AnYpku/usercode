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

       bool TestDifferentMethods();

       bool PrepareMigrationMatrix();

       bool ApplyRooUnfold(TH1D* inputYields, TH1D* unfoldedYields,RooUnfold::Algorithm alg,TMatrixD& errCovStat, TVectorD& errStatV, TVectorD& errSystV, TVectorD& errCovStatV);
       bool ComputeSystErrors(TH1D* histInputYields, float* errSyst,RooUnfold::Algorithm alg, int NSmears);
       bool ComputeStatErrors(TH1D* histInputYields, float* errStat, RooUnfold::Algorithm alg, int NSmears);

       bool PlotAndStore();

       bool FillBinLimits(int nBins, vector<float> vecLims, float* lims);




     private:

       TAllInputSamples* INPUT_;
       int _channel;

       TConfiguration config_;

       vector <float> vecPhoPtLimitsRec_;
       vector <float> vecPhoPtLimitsGen_;
       
       int nBinsRec_;
       int nBinsGen_;

       TFile* fIn_;
       TFile* fOut_;

       TH2D* histEventCountMigrMatrix_;
       TH2D* histMigrMatrixNotNormalized_;

       TH1D* histYieldsRec_;
       TH1D* histYieldsRecSmeared_;
       TH1D* histYieldsGen_;

       TTree* tr_;

       bool isOverflowRec_;
       bool isOverflowGen_;
  };

#endif 
  //#ifndef 
