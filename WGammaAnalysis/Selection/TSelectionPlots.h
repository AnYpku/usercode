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
  void GetTrees(int channel, string confFile);
  void SetHistograms(string plotVar, int nBins, float* binLimits);
  void ScaleHistogramsToData();
  void DrawSpectrumDataVsMC();
  void DrawSpectrumSigVsBkg(TString nameCanvas, TString nameForSave);

private:
  vector <TFile*> file_;
  vector <TTree*> tree_;
  vector <TH1F*> hist_;
  vector <string> sourceLabel_;
  vector <bool> hasHist_;
  vector <bool> isData_;
  vector <bool> isSigMC_;
  vector <int> colors_;
  int nSources_;
  TLegend* legend_;
  TConfiguration config_;
};

#endif //#ifndef TSelectionPlots_h
