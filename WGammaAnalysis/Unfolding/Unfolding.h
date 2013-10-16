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
#include "TMath.h" 
  //ROOT package

class Unfolding
  {
     public:
       Unfolding (int channel, string configfile="../Configuration/config.txt");
       virtual ~Unfolding();


     private:

       TAllInputSamples* INPUT_;
       int channel_;

       TConfiguration config_;
  };

#endif 
  //#ifndef 
