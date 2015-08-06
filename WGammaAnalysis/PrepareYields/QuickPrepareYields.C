#include "../Configuration/TConfiguration.h"
#include "../Include/TFullCuts.h"
#include "../Include/TPhotonCuts.h"
#include "../FullChain/FullChain.h"
#include "AuxPrepareYields.C"

void QuickPrepareYields_FSRandISR_Ich(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;

  fch.SetDefaultFullChainParameters(anPars,"phoPFChIsoCorr");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.blind[conf.BOTH_CHANNELS][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.cutAdd="phoEt>15";
  int selStage=conf.FSR_EXCLUDED;
  AuxPrepareYields(anPars,0,selStage);

  anPars.cutAdd="1";
  anPars.nKinBins=6;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
  anPars.kinBinLims[ib]=0+1*ib;
  selStage=conf.FSR;
  AuxPrepareYields(anPars,0,selStage);

}//end of QuickPrepareYields_FSRandISR_Ich

void QuickPrepareYields_FSRandISR(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon;
  TCut cut = photon.RangePhoton(2012, photon.WP_MEDIUM, 1, 0);
  anPars.cutAdd=cut;//"phoEt<40";
//  anPars.cutAdd=photon.RangeSigmaIEtaIEta(2012,photon.WP_MEDIUM,conf.COMMON);

  anPars.blind[conf.BOTH_CHANNELS][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;


  int selStage=conf.VERY_PRELIMINARY;

  fch.SetDefaultFullChainParameters(anPars,"Mpholeplep");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd=cut;
  AuxPrepareYields(anPars,0,selStage); 

  fch.SetDefaultFullChainParameters(anPars,"Mleplep");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd=cut;
  AuxPrepareYields(anPars,0,selStage); 

  fch.SetDefaultFullChainParameters(anPars,"lep1PhoDeltaR");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd=cut;
  AuxPrepareYields(anPars,0,selStage);

  fch.SetDefaultFullChainParameters(anPars,"lep2PhoDeltaR");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd=cut;
  AuxPrepareYields(anPars,0,selStage);

  selStage=conf.FSR;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd="1";
  AuxPrepareYields(anPars,0,selStage);

  selStage=conf.FSR_EXCLUDED;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd="1";
  AuxPrepareYields(anPars,0,selStage);

}//end of QuickPrepareYields_FSRandISR

void QuickPrepareYields_WMt(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon;
  TFullCuts fullcut;
  TCut cut = photon.RangePhoton(2012, photon.WP_MEDIUM);
  cut = cut && fullcut.RangeDeltaR(conf.W_GAMMA);
  TCut cutPt("phoEt>15 && phoEt<45");
  cut = cut && cutPt;

  fch.SetDefaultFullChainParameters(anPars,"WMt");
  anPars.nKinBins=40;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+5*ib;

  anPars.cutAdd=cut;

  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.blind[conf.MUON][conf.W_GAMMA]=conf.UNBLIND;
  // UNBLIND is to open the VERY_PRELIMINARY files, blinding cut is further applied for plotting
  // blinding cut: fullcut.RangeBlinding(conf.BLIND_COMBINED);
  int selStage=conf.VERY_PRELIMINARY;

  anPars.channel=conf.MUON;
  anPars.vgamma=conf.W_GAMMA;
  AuxPrepareYields(anPars,0,selStage); 

  cut = cut && fullcut.RangeZmassWindowCut();
  anPars.cutAdd=cut;
  anPars.channel=conf.ELECTRON;
  anPars.vgamma=conf.W_GAMMA;
  AuxPrepareYields(anPars,0,selStage); 

}//end of QuickPrepareYields_WMt

void QuickPrepareYields_EtoGammaEnriched(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  fch.SetDefaultFullChainParameters(anPars,"Mpholep1");
  anPars.cutAdd="phoEt>15 && phoEt<45";//"phoEt<40";

  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.channel=conf.ELECTRON;
  anPars.vgamma=conf.W_GAMMA;
  anPars.varKin="Mpholep1";
  anPars.nKinBins=20;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=70+2*ib;//70-110 GeV


  int selStage=conf.PRELIMINARY_FOR_E_TO_GAMMA;
  AuxPrepareYields(anPars,0,selStage); 

  anPars.nKinBins=60;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=30+2*ib;//30-150 GeV
  TPhotonCuts photon;
  TFullCuts fullcut;
  TCut cut = photon.RangePhoton(2012, photon.WP_MEDIUM);
  anPars.cutAdd = anPars.cutAdd && cut && fullcut.RangeDeltaR(conf.W_GAMMA) && fullcut.RangeMetRelatedCut(2012,conf.ELECTRON);
  selStage=conf.VERY_PRELIMINARY;
  AuxPrepareYields(anPars,0,selStage);

  anPars.blind[conf.MUON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.channel=conf.MUON; 
  AuxPrepareYields(anPars,0,selStage);

}//end of QuickPrepareYields_Mass_FSRandISR
