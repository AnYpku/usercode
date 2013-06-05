#include "../Configuration/TConfiguration.h"
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
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

void PlotAllVariables(int plotType)
{
  string confFile = "../Configuration/config.txt";
  int nBins=50;
  float binLimits[nBins+1];
   
  int nVars=10;
  string plotVars[nVars];
  float varMin[nVars];
  float varMax[nVars];
   
  plotVars[0]="leptonEta";    varMin[0]=-2.1;  varMax[0]=2.1;
  plotVars[1]="leptonPhi";    varMin[1]=-3.14; varMax[1]=3.14;
  plotVars[2]="leptonPt";     varMin[2]=26;     varMax[2]=150;
  plotVars[3]="phoEta";       varMin[3]=-2.5;    varMax[3]=2.5;
  plotVars[4]="phoPhi";       varMin[4]=-3.14; varMax[4]=3.14;
  plotVars[5]="phoEt";        varMin[5]=0;     varMax[5]=60;
  plotVars[6]="lePhoDeltaR";  varMin[6]=0;     varMax[6]=8;
  plotVars[7]="WMt";          varMin[7]=0;     varMax[7]=150;
  plotVars[8]="pfMETPhi";     varMin[8]=-3.14; varMax[8]=3.14;
  plotVars[9]="pfMET";        varMin[9]=0;     varMax[9]=150;
  for (int i=0; i<nVars; i++){
    TSelectionPlots plotter;
    bool areTrees = plotter.GetTrees(TInputSample::MUON, confFile);
    if (!areTrees) continue;

    for (int j=0; j<nBins+1; j++){
      binLimits[j]=varMin[i]+j*(varMax[i]-varMin[i])/nBins;
    }
    std::cout<<std::endl;
    std::cout<<"plotVars[i]="<<plotVars[i]<<std::endl;
    bool areHists = plotter.SetHistograms(plotVars[i], nBins, binLimits);
    if (!areHists) continue;
 
    TString varStr(plotVars[i].c_str());
    std::cout<<"varStr="<<varStr<<std::endl;
    TString nameForSave=varStr;
    if (plotType==SigVsBkg) nameForSave+="_SigVsBkg.png";
    else if (plotType==DataVsMC) nameForSave+="_DataVsMC.png";
    TString nameCanvas="canv_";
    nameCanvas+=varStr;
    if (plotType==SigVsBkg) plotter.DrawSpectrumSigVsBkg(nameCanvas,nameForSave);
    else if (plotType==DataVsMC)  plotter.DrawSpectrumDataVsMC(nameCanvas,nameForSave);

  }//end of loop for (int i=0; i<nVars; i++)
}
