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
     //the main function which is called from outside
   const static int numberOfTrees=5;
     //tree [0] is input tree;
     //trees [1]-[4] are output trees
private :
   TFile    *fileOut_[numberOfTrees]; //output Files
   TTree    *outputTree_[numberOfTrees]; //output Trees
   TString nameWGammaSample_[numberOfTrees]; //names of input [0] and output [1]-[numberOfTrees] files
};

#endif
