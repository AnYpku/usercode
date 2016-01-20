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

  anPars.cutAdd="1";
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

void QuickPrepareYields_FSRandISR_TemplatesIch(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;

  fch.SetDefaultFullChainParameters(anPars,"phoPFChIsoCorr");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.blind[conf.BOTH_CHANNELS][conf.Z_GAMMA]=conf.UNBLIND;

  TCut cutSihihNomB="phoSigmaIEtaIEta<0.011 && phoSCEta>-1.4442 && phoSCEta<1.4442";
  TCut cutSihihNomE="phoSigmaIEtaIEta<0.033 && ((phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566))";
  TCut cutSihihNom = cutSihihNomB || cutSihihNomE;

  anPars.cutAdd=cutSihihNom && TCut("phoEt>10 && phoEt<15");
  int selStage=conf.FSR_EXCLUDED;
  AuxPrepareYields(anPars,0,selStage,"_pt10to15_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>15 && phoEt<20");
  AuxPrepareYields(anPars,0,selStage,"_pt15to20_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>20 && phoEt<25");
  AuxPrepareYields(anPars,0,selStage,"_pt20to25_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>25 && phoEt<30");
  AuxPrepareYields(anPars,0,selStage,"_pt25to30_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>30 && phoEt<35");
  AuxPrepareYields(anPars,0,selStage,"_pt30to35_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>35 && phoEt<45");
  AuxPrepareYields(anPars,0,selStage,"_pt35to45_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>45 && phoEt<55");
  AuxPrepareYields(anPars,0,selStage,"_pt45to55_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>55 && phoEt<65");
  AuxPrepareYields(anPars,0,selStage,"_pt55to65_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>65 && phoEt<75");
  AuxPrepareYields(anPars,0,selStage,"_pt65to75_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>75 && phoEt<85");
  AuxPrepareYields(anPars,0,selStage,"_pt75to85_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>85 && phoEt<95");
  AuxPrepareYields(anPars,0,selStage,"_pt85to95_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>95 && phoEt<120");
  AuxPrepareYields(anPars,0,selStage,"_pt95to120_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>120 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,"_pt120to500_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>10 && phoEt<500");
  anPars.nKinBins=6;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
  anPars.kinBinLims[ib]=0+1*ib;
  selStage=conf.FSR;
  AuxPrepareYields(anPars,0,selStage,"_pt10to500_");

}//end of QuickPrepareYields_FSRandISR_TemplatesIch

void QuickPrepareYields_FSRandISR_TemplatesSihih(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;

  TCut cutIsoNomB="phoPFChIsoCorr<1.5 && phoSCEta>-1.4442 && phoSCEta<1.4442";
  TCut cutIsoNomE="phoPFChIsoCorr<1.2 && ((phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566))";
  TCut cutIsoNom = cutIsoNomB || cutIsoNomE;

  fch.SetDefaultFullChainParameters(anPars,"phoSigmaIEtaIEta");
  anPars.channel=conf.BOTH_CHANNELS;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.blind[conf.BOTH_CHANNELS][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.cutAdd=cutIsoNom && TCut("phoEt>10 && phoEt<15");
  int selStage=conf.FSR_EXCLUDED;
  AuxPrepareYields(anPars,0,selStage,"_pt10to15_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>15 && phoEt<20");
  AuxPrepareYields(anPars,0,selStage,"_pt15to20_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>20 && phoEt<25");
  AuxPrepareYields(anPars,0,selStage,"_pt20to25_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>25 && phoEt<30");
  AuxPrepareYields(anPars,0,selStage,"_pt25to30_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>30 && phoEt<35");
  AuxPrepareYields(anPars,0,selStage,"_pt30to35_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>35 && phoEt<45");
  AuxPrepareYields(anPars,0,selStage,"_pt35to45_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>45 && phoEt<55");
  AuxPrepareYields(anPars,0,selStage,"_pt45to55_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>55 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,"_pt55to500_");


  selStage=conf.FSR;

  anPars.cutAdd=cutIsoNom && TCut("phoEt>10 && phoEt<15");
  AuxPrepareYields(anPars,0,selStage,"_pt10to15_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>15 && phoEt<20");
  AuxPrepareYields(anPars,0,selStage,"_pt15to20_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>20 && phoEt<25");
  AuxPrepareYields(anPars,0,selStage,"_pt20to25_");

   anPars.cutAdd=cutIsoNom && TCut("phoEt>25 && phoEt<30");
   AuxPrepareYields(anPars,0,selStage,"_pt25to30_");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>30 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,"_pt30to500_");


}//end of QuickPrepareYields_FSRandISR_TemplatesSihih

void QuickPrepareYields_FSRandISR(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon;
  TCut cut = photon.RangePhoton(conf.MUON, conf.Z_GAMMA, 2012, photon.WP_MEDIUM, 1, 0, 1, 1, 1, 1);
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

void QuickPrepareYields_WMt(int channel){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon;
  TFullCuts fullcut;
  TCut cut = photon.RangePhoton(channel, conf.W_GAMMA, 2012, photon.WP_MEDIUM, 1, 1, 1, 1, 1, 1);
  cut = cut && fullcut.RangeDeltaR(conf.W_GAMMA);
  TCut cutPt("phoEt>15 && phoEt<45");
  cut = cut && cutPt;
  if (channel==conf.ELECTRON) cut = cut && fullcut.RangeZmassWindowCut();

  fch.SetDefaultFullChainParameters(anPars,"WMt");
  anPars.nKinBins=40;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+5*ib;

  anPars.cutAdd=cut;

  anPars.blind[channel][conf.W_GAMMA]=conf.UNBLIND;
  // UNBLIND is to open the VERY_PRELIMINARY files, blinding cut is further applied for plotting
  // blinding cut: fullcut.RangeBlinding(conf.BLIND_COMBINED);
  int selStage=conf.VERY_PRELIMINARY;

  anPars.channel=channel;
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
  //  AuxPrepareYields(anPars,0,selStage); 

  anPars.nKinBins=75;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+2*ib;//0-150 GeV
  TPhotonCuts photon;
  TFullCuts fullcut;
  TCut cut = photon.RangePhoton(conf.ELECTRON, conf.W_GAMMA,2012, photon.WP_MEDIUM, 1, 1, 1, 1, 1, 1);
  anPars.cutAdd = anPars.cutAdd && cut && fullcut.RangeDeltaR(conf.W_GAMMA) && fullcut.RangeMetRelatedCut(2012,conf.ELECTRON);
  selStage=conf.VERY_PRELIMINARY;
  AuxPrepareYields(anPars,0,selStage);

  //  anPars.blind[conf.MUON][conf.W_GAMMA]=conf.UNBLIND;
  //  anPars.channel=conf.MUON;
  //    TCut cut = photon.RangePhoton(conf.MUON, conf.W_GAMMA,2012, photon.WP_MEDIUM, 1, 1, 1, 1, 1, 1);
  //    anPars.cutAdd = anPars.cutAdd && cut && fullcut.RangeDeltaR(conf.W_GAMMA) && fullcut.RangeMetRelatedCut(2012,conf.MUON);
  //    selStage=conf.VERY_PRELIMINARY;
  //    AuxPrepareYields(anPars,0,selStage);

}//end of QuickPrepareYields_Mass_FSRandISR
