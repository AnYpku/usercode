#include "../Include/TFullCuts.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"

#include "TFile.h"
#include "TCut.h"

#include <iostream>

void ExtraSelection(int channel)
{
  TFullCuts fullCut;
  TConfiguration config;
  TAllInputSamples INPUT(channel,"../Configuration/config.txt");
  TTree* tr=0;
  TTree* tr1=0;
  TTree* tr2=0;
  for (int i=0; i<INPUT.nSources_; i++){
    TString fInName = config.GetSelectedVeryPreliminaryName(channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);
    TFile fIn(fInName);
    if (!fIn.IsOpen()){
      std::cout<<"file "<<fInName<<" can't be open"<<std::endl;
      continue;
    }
    std::cout<<"file "<<fInName<<" was open"<<std::endl;
    tr = (TTree*)fIn.Get("selectedEvents");
    TString fOutName1=config.GetSelectedPreliminaryName(channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);
    TFile fOut1(fOutName1,"recreate");
    tr1 = tr->CopyTree(fullCut.RangeMetRelatedCut());
    tr1->Write();
    TString fOutName2=config.GetSelectedFullyName(channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);
    TFile fOut2(fOutName2,"recreate");
    tr2 = tr->CopyTree(fullCut.RangeExtraCut());
    tr2->Write();
  }

}
