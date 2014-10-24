#ifndef SkimLeptonPhoton_h
#define SkimLeptonPhoton_h

#include "TInputOutputTree.h"
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>

class SkimLeptonPhoton{
public :
   SkimLeptonPhoton(int channel, int sample, TString inputFileName, bool isDebugMode=0, TString outDir="./", TString nameDir="ggNtuplizer", TString nameTree="EventTree");
   virtual ~SkimLeptonPhoton();
   void LoopOverInputTree();
     //the main function which is called from outside
   //const static int numberOfTrees=5;
     //tree [0] is input tree;
     //trees [1]-[4] are output trees
private :
   TInputOutputTree _TREE;
   TConfiguration _config;
   int _channel;
   int _sample;
   bool _isDebugMode;
   TFile    *_fileOutMu; //output Files
   TFile    *_fileOutEle; //output Files
   TTree    *_outputTreeMu; //output Trees
   TTree    *_outputTreeEle; //output Trees
   TH1F     *_hskimMu;
   TH1F     *_hskimEle;
   TString  _inputFileName; 
   TString  _skimmedFileNameMu; 
   TString  _skimmedFileNameEle;
   TString  _nameDir;
   TString  _nameTree;

};

#endif
