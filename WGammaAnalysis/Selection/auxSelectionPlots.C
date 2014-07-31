#include "../Configuration/TConfiguration.h"
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TMuonCuts.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TFullCuts.h"
#include "TSelectionPlots.h"
  //this class
#include "TString.h"
  //ROOT class
#include <iostream>

enum {SigVsBkg, DataVsMC};
void PlotAllVariables(int plotType);

void PlotSigVsBkg()
{
  PlotAllVariables(SigVsBkg);
}

void PlotDataVsMC()
{
  PlotAllVariables(DataVsMC);
}

void PlotOneVariable(TSelectionPlots &plotter, TString plotVar, int nBins, float varMin, float varMax, TCut cutWhole, TCut cutCutted, TString nameforSave="")
{
  TCut cutWeight="weight";
  float binLimits[nBins+1];
  for (int j=0; j<nBins+1; j++){
    binLimits[j]=varMin+j*(varMax-varMin)/nBins;
  }
  std::cout<<std::endl;
  std::cout<<"plotVar="<<plotVar<<std::endl;
  bool areHists = plotter.SetHistograms(TConfiguration::BLIND_PRESCALE, plotVar, nBins, binLimits, cutWhole*cutWeight, cutCutted*cutWeight);
  if (!areHists) return;
 
  std::cout<<"varStr="<<plotVar<<std::endl;
  if (nameforSave==""){
    nameforSave=plotVar;
    nameforSave+="_DataVsMC.png";
  }
  TString nameCanvas=nameforSave;
  nameCanvas.ReplaceAll(".png","");
  plotter.DrawSpectrumDataVsMC(nameCanvas,nameforSave);
}

void PlotThreeVariables(TSelectionPlots &plotter, TString plotVar, int nBins, float varMin, float varMax, TCut cutWhole, TCut cutCutted, TString nameforSaveBase)
{
   TString nameforSave;
   nameforSave = nameforSaveBase;
   nameforSave = nameforSaveBase;
   nameforSave+="_";
   nameforSave+=plotVar;
   nameforSave+=".png";
   PlotOneVariable(plotter, plotVar, nBins, varMin, varMax, cutWhole, cutCutted, nameforSave);
   nameforSave = nameforSaveBase;
   nameforSave+="_phoEt.png";
   PlotOneVariable(plotter, "phoEt", 8, 15, 55, cutWhole, cutCutted, nameforSave);
   nameforSave = nameforSaveBase;
   nameforSave+="_WMt.png";
   PlotOneVariable(plotter, "WMt", 15, 0, 150, cutWhole, cutCutted, nameforSave);
}


void PlotWithConsequetiveCuts()
{
  TMuonCuts muon;
  TPhotonCuts photon;
  TFullCuts fullCuts;
  string confFile = "../Configuration/config.txt";
  TSelectionPlots plotter;
  bool areTrees = plotter.GetTrees(TConfiguration::MUON, TConfiguration::BLIND_PRESCALE, confFile);
  if (!areTrees) return;

  int nCuts=13;
  TCut cutExtra[nCuts];
  TString strDescr[nCuts];

  int year=2012; int wp=photon.WP_TIGHT;   TString strIsoBase="PF";
  TCut cut="phoEt>15";
  cutExtra[0]="!hasMoreLeptons";         strDescr[0]="veto on extra muons";
  cutExtra[1]=muon.RangeTriggerMatch();  strDescr[1]="muon trigger match";
  cutExtra[2]=muon.RangeId(year);        strDescr[2]="muon ID";
  cutExtra[3]=muon.RangeIsolation(year); strDescr[3]="muon isolation";
  if (year==2011) cutExtra[4]="leptonPt>35"; else cutExtra[4]="1";

  cutExtra[5]=photon.RangeSigmaIEtaIEta(year, wp);

  cutExtra[6]=photon.RangeHoverE(year);
  cutExtra[7]=photon.RangePhoEleVeto();
  if (year==2011) cutExtra[8]=photon.RangePhoHasPixelSeed(); else cutExtra[8]="1";

  TString strIso;
  if (year==2012) strIso="pho"+strIsoBase+"ChIsoCorr";
  else if (year==2011) strIso="phoTrkIsoHollowDR04Corr";
  cutExtra[9]=photon.RangeOneIsolation(year,wp,photon.ISO_CHorTRK,strIso);

  if (year==2012) strIso="pho"+strIsoBase+"NeuIsoCorr";
  else if (year==2011) strIso="phoHcalIsoDR04Corr";
  cutExtra[10]=photon.RangeOneIsolation(year,wp,photon.ISO_NEUorHCAL,strIso);

  if (year==2012) strIso="pho"+strIsoBase+"PhoIsoCorr";
  else if (year==2011) strIso="phoEcalIsoDR04Corr";
  cutExtra[11]=photon.RangeOneIsolation(year,wp,photon.ISO_PHOorECAL,strIso);

  cutExtra[12]=fullCuts.RangeMetRelatedCut(year);

  TCut cutWhole="phoEt>15";
  TCut cutBarrel = photon.RangeBarrel();
  TCut cutEndcap = photon.RangeEndcap();

  cutWhole = cutWhole &&  cutExtra[0] && cutExtra[1] && cutExtra[2] && cutExtra[3] && cutExtra[4] && cutExtra[5] && cutExtra[6] && cutExtra[7] && cutExtra[8] && cutExtra[9] && cutExtra[10] && cutExtra[11];
  TCut cutCutted = cutWhole &&  cutExtra[0] && cutExtra[1] && cutExtra[2] && cutExtra[3] && cutExtra[4] && cutExtra[5] && cutExtra[6] && cutExtra[7] && cutExtra[8] && cutExtra[9] && cutExtra[10] && cutExtra[11] && cutExtra[12];
  PlotThreeVariables(plotter,"WMt", 15, 0, 150, cutWhole, cutCutted, "c_fullcuts2012_MEDIUM_");

/*
  TMuonCuts muon;
  cutWhole=cutCutted;
  cutCutted=cutWhole && muon.RangeTriggerMatch();
  PlotThreeVariables("HLT_IsoMu24_v", 2, -0.5, 1.5, cutWhole, cutCutted, "c_cuts20XX_1021");

  TCut cutWhole2011=cutCutted;
  TCut cutWhole2012=cutCutted;
  TCut cutCutted2011=cutWhole2011 && muon.RangeId(2011);
  TCut cutCutted2012=cutWhole2012 && muon.RangeId(2012);
  PlotThreeVariables("leptonId2011", 2, -0.5, 1.5, cutWhole2011, cutCutted2011, "c_cuts2011_102122");
  PlotThreeVariables("leptonId2012", 2, -0.5, 1.5, cutWhole2012, cutCutted2012, "c_cuts2012_102122");

  cutWhole2011=cutCutted2011;
  cutWhole2012=cutCutted2012;
  cutCutted2011=cutWhole2011 && muon.RangeIsolation(2011);
  cutCutted2012=cutWhole2012 && muon.RangeIsolation(2012);
  PlotThreeVariables("leptonIsolation2011", 20, -0.25, 0.15, cutWhole2011, cutCutted2011, "c_cuts2011_10212223");
  PlotThreeVariables("leptonIsolation2012", 20,  0.00, 0.40, cutWhole2012, cutCutted2012, "c_cuts2012_10212223");
*/
}


void PlotAllVariables(int plotType)
{
  string confFile = "../Configuration/config.txt";
   
  int nVars=10;
  string plotVars[nVars];
  float varMin[nVars];
  float varMax[nVars];
  float cutMin[nVars];
  float cutMax[nVars];
  TCut  cutWhole[nVars];
  TCut  cutCutted[nVars];
  int nBins[nVars];
  TCut cutWeight="weight";
  for (int i=0; i<nVars; i++){
     cutWhole[i]="phoEt>15";
     cutCutted[i]="phoEt>15";
  }
  float binLimits[101];
 
  plotVars[0]="leptonEta";    varMin[0]=-2.1;  varMax[0]=2.1;   nBins[0]=10;
  plotVars[1]="leptonPhi";    varMin[1]=-3.14; varMax[1]=3.14;  nBins[1]=10;
  plotVars[2]="leptonPt";     varMin[2]=26;    varMax[2]=150;   nBins[2]=30;  
  plotVars[3]="phoEta";       varMin[3]=-2.5;  varMax[3]=2.5;   nBins[3]=10;    
  plotVars[4]="phoPhi";       varMin[4]=-3.14; varMax[4]=3.14;  nBins[4]=10;  
  plotVars[5]="phoEt";        varMin[5]=0;     varMax[5]=60;    nBins[5]=30;  
  plotVars[6]="lePhoDeltaR";  varMin[6]=0;     varMax[6]=8;     nBins[6]=20;  
  plotVars[7]="WMt";          varMin[7]=0;     varMax[7]=150;   nBins[7]=15;   cutCutted[7]=cutCutted[7] && TCut("WMt>50");
  plotVars[8]="pfMETPhi";     varMin[8]=-3.14; varMax[8]=3.14;  nBins[8]=10;   cutCutted[8]=cutCutted[8] && TCut("WMt>50");
  plotVars[9]="pfMET";        varMin[9]=0;     varMax[9]=150;   nBins[9]=20; 

  TSelectionPlots plotter;
  bool areTrees = plotter.GetTrees(TConfiguration::MUON, TConfiguration::BLIND_PRESCALE, confFile);
  if (!areTrees) return;

//  for (int i=0; i<nVars; i++){

  for (int i=7; i<9; i++){
    PlotOneVariable(plotter, plotVars[i], nBins[i], varMin[i], varMax[i], cutWhole[i], cutCutted[i]);
  }//end of loop for (int i=0; i<nVars; i++)
}
