#ifndef SkimNEvents_h
#define SkimNEvents_h

#include "TInputOutputTree.h"
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>

class SkimNEvents{
public :
   SkimNEvents(long N, int channel, int sample, TString inputFileName, TString nameDir, TString nameTree, TString outDir, bool isDebugMode=0);
   virtual ~SkimNEvents();
   void LoopOverInputTree();
     //the main function which is called from outside
   //const static int numberOfTrees=5;
     //tree [0] is input tree;
     //trees [1]-[4] are output trees
private :
   TInputOutputTree _TREE;
   TConfiguration _config;
   long _N;
   int _channel;
   int _sample;
   bool _isDebugMode;
   TFile    *_fileOut; //output Files
   TTree    *_outputTree; //output Trees
   TH1F     *_hskim;
   TString  _inputFileName; 
   TString  _skimmedFileName; 
   TString  _nameDir;
   TString  _nameTree;

};

#endif
