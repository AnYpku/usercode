#ifndef SplitWGammaMC_h
#define SplitWGammaMC_h

#include "TInputOutputTree.h"
#include <TString.h>
#include <TFile.h>

class SplitWGammaMC{
public :
   SplitWGammaMC(TString nameWGammaInput, TString nameWGammaEle, TString nameWGammaMuo, TString nameWGammaTau, TString nameDir, TString nameTree);
   virtual ~SplitWGammaMC();
   void LoopOverInputTree();
     //the main function which is called from outside
   enum {ID_ELECTRON=2, ID_MUON=3, ID_TAU=4};
private :

   TInputOutputTree TREE_;

   TFile    *fileOutEle_; //output Files
   TFile    *fileOutMuo_; //output Files
   TFile    *fileOutTau_; //output Files
   TTree    *outputTreeEle_; //output Trees
   TTree    *outputTreeMuo_; //output Trees
   TTree    *outputTreeTau_; //output Trees
   TString nameWGammaInput_;
   TString nameWGammaEle_; 
   TString nameWGammaMuo_; 
   TString nameWGammaTau_; 
   TString nameDir_;
   TString nameTree_;

};

#endif
