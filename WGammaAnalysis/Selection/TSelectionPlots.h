#ifndef TSelectionPlots_h
#define TSelectionPlots_h

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TLegend.h>
#include "../Configuration/TConfiguration.h"
#include <string>
#include <vector>

class TSelectionPlots {

public:
  TSelectionPlots();
  virtual ~TSelectionPlots();
  bool GetTrees(int channel, string confFile, string strSources="ALL");
  bool SetHistograms(TString plotVar, int nBins, float* binLimits, TString cut="1");
  void ScaleHistogramsToData();
  void DrawSpectrumDataVsMC(TString nameCanvas, TString nameForSave, bool isNoData=0);
  void DrawSpectrumSigVsBkg(TString nameCanvas, TString nameForSave);
  bool CheckSizesOfAllVectors();
  void PrintWeightedNumberOfEvents(TString cut);

private:
  vector <TFile*> file_;
  vector <TTree*> tree_;
  vector <TH1F*> hist_;
  vector <TString> sourceLabel_;
  vector <bool> hasHist_;
  vector <bool> isData_;
  vector <bool> isSigMC_;
  vector <int> colors_;
  unsigned int nSources_;
  TLegend* legend_;
  TConfiguration config_;
};

#endif //#ifndef TSelectionPlots_h
