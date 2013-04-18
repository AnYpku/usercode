//////////////////////////////////////////////////////////
// Output tree
//////////////////////////////////////////////////////////

#ifndef TOutputTree_h
#define TOutputTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TInputTree.h"
class TOutputTree: public TInputTree{
public : 
   TString nameFile_;
   TString nameDir_;
   TString nameTree_; 
   TOutputTree();
   virtual ~TOutputTree();
   void     InitOutputTree  (TTree* outputTree);
   //void FillOutputTree(int iTree);
};

#endif
