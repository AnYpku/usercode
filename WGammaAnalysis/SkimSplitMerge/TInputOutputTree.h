//////////////////////////////////////////////////////////
// Input and Output tree (mostly output here)
//////////////////////////////////////////////////////////

#ifndef TInputOutputTree_h
#define TInputOutputTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "../Include/TEventTree.h"
class TInputOutputTree: public TEventTree{
public : 
   TString nameFile_;
   TString nameDir_;
   TString nameTree_; 
   TInputOutputTree();
   virtual ~TInputOutputTree();
   void     InitOutputTree  (TTree* outputTree);
   //void FillOutputTree(int iTree);
};

#endif
