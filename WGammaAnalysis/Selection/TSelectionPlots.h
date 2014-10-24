#ifndef TSelectionPlots_h
#define TSelectionPlots_h

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TCut.h>
#include "../Configuration/TConfiguration.h"
#include <string>
#include <vector>

class TSelectionPlots {

public:
  TSelectionPlots();
  virtual ~TSelectionPlots();
  bool GetTrees(int channel, int vgamma, int blind, string confFile, string strSources="ALL");
  void SelectionEfficiencyInStages(int year, int vgamma, int wp, TString strIsoBase);
  void SelectionEfficiencyOneStage(int nSources, int* nTot, int* nPass, float* eff, TCut cut, TCut cutExtra, TString strCutDescription="");
  bool SetHistograms(int blind, TString plotVar, int nBins, float* binLimits, TCut cutWhole="weight", TCut cutCutted="weight");
  void SetSourceIndex(int iHist);
  TString StrHistName(TString plotVar, int iHist);
  void SetHistProperties(TH1F* hist, int iHist, bool isCutted);
  void HistSubtractions(int ind1, int ind2);
  void ScaleHistogramsToData();
  void DrawSpectrumDataVsMC(TString nameCanvas, TString nameForSave, bool isNoData=0);
  void DrawSpectrumSigVsBkg(TString nameCanvas, TString nameForSave);
  bool CheckSizesOfAllVectors();
  void PrintWeightedNumberOfEvents(TString cut);

private:
  vector <TFile*> _file;
  vector <bool> _hasHist;
  vector <TTree*> _tree;
  vector <int> _colors;
  vector <bool> _isData;
  vector <bool> _isSigMC;
  vector <TString> _sourceLabel;
  vector <TString> _sourceName;
  vector <TH1F*> _hist;
  vector <TH1F*> _histCutted;
  int _dataIndex;
  int _sigmcIndex;
  int _taunuIndex;
  int _wjetsIndex;
  int _zgIndex;
  int _dyjetsIndex;
  int _ttgIndex;
  int _ttjetsIndex;
  int _multibosonsIndex;

  unsigned int _nSources;
  TLegend* _legend;
  TConfiguration _config;
};

#endif //#ifndef TSelectionPlots_h
