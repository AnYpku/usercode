#ifndef SplitWGammaMC_h
#define SplitWGammaMC_h

#include "TOutputTree.h"
#include <TString.h>
#include <TFile.h>

class SplitWGammaMC: public TOutputTree{
public :
   SplitWGammaMC(TString* nameWGammaSample, TString nameDir, TString nameTree);
   virtual ~SplitWGammaMC();
   void LoopOverInputTree();
   const static int numberOfTrees=5;
private :
   TFile    *fileOut_[numberOfTrees];
   TTree    *outputTree_[numberOfTrees];
   TString nameWGammaSample_[numberOfTrees]; 
};

#endif
