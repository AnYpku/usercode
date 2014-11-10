#ifndef SkimLeptonPhoton_h
#define SkimLeptonPhoton_h

#include "TInputOutputTree.h"
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>

class SkimLeptonPhoton{
public :
   SkimLeptonPhoton(int channel, int vgamma, int sample, TString inputFileName, bool isDebugMode=0, TString outDir="./", TString nameDir="ggNtuplizer", TString nameTree="EventTree");
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
   int _vgamma;
   int _sample;
   bool _isDebugMode;
   TFile    *_fileOut[2][2]; //output Files
   TTree    *_outputTree[2][2]; //output Trees
   TH1F     *_hskim[2][2];
   TString  _inputFileName; 
   TString  _skimmedFileName[2][2]; 
   TString  _nameDir;
   TString  _nameTree;

};

#endif
