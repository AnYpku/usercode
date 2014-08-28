#ifndef SplitVDecayMC_h
#define SplitVDecayMC_h

#include "TInputOutputTree.h"
#include <TString.h>
#include <TFile.h>

class SplitVDecayMC{
public :
   SplitVDecayMC(TString nameInput, TString nameEle, TString nameMuo, TString nameTau, TString nameDir, TString nameTree, bool isDebugMode=0);
   virtual ~SplitVDecayMC();
   void LoopOverInputTree();
     //the main function which is called from outside
   enum {ID_ELECTRON=2, ID_MUON=3, ID_TAU=4};
private :

   TInputOutputTree _TREE;
   bool _isDebugMode;

   TFile    *_fileOutEle; //output Files
   TFile    *_fileOutMuo; //output Files
   TFile    *_fileOutTau; //output Files
   TTree    *_outputTreeEle; //output Trees
   TTree    *_outputTreeMuo; //output Trees
   TTree    *_outputTreeTau; //output Trees
   TString _nameInput;
   TString _nameEle; 
   TString _nameMuo; 
   TString _nameTau; 
   TString _nameDir;
   TString _nameTree;

};

#endif
