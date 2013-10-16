#include "Unfolding.h" 
  //this class
#include "../Include/TMathTools.h"
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TMath.h" 
#include "TH1F.h" 
#include "TVectorF.h" 
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

Unfolding::Unfolding(int channel, string configFile)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  for (int i=0; i<config_.GetNPhoPtBins(); i++){

  }

  vecPhoPtLimits_ = config_.GetPhoPtBinsLimits();
}

Unfolding::~Unfolding()
{
   fChain = 0;
     //field of TEventTree 
}

