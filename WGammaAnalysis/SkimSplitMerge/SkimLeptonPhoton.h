#ifndef SkimLeptonPhoton_h
#define SkimLeptonPhoton_h

#include "TInputOutputTree.h"
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>

class SkimLeptonPhoton{
public :
   SkimLeptonPhoton(TString inputFileName, TString nameDir, TString nameTree, TString outDir, int channel);
   virtual ~SkimLeptonPhoton();
   void LoopOverInputTree();
     //the main function which is called from outside
   const static int numberOfTrees=5;
     //tree [0] is input tree;
     //trees [1]-[4] are output trees
   enum {MUON, ELECTRON, BOTH};
private :
   TInputOutputTree TREE_;
   int channel_;
   TFile    *fileOutMu_; //output Files
   TFile    *fileOutEle_; //output Files
   TTree    *outputTreeMu_; //output Trees
   TTree    *outputTreeEle_; //output Trees
   TH1F     *hskimMu_;
   TH1F     *hskimEle_;
   TString  inputFileName_; 
   TString  skimmedFileNameMu_; 
   TString  skimmedFileNameEle_;
   TString  nameDir_;
   TString  nameTree_;

};

#endif
