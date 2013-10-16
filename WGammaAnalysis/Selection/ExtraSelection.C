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
  for (int i=0; i<INPUT.nSources_; i++){
    TFile fIn(config.GetSelectedPreliminaryName(channel,INPUT.allInputs_[i].sourceName_));
    tr = (TTree*)fIn.Get("selectedEvents");
    TString fOutName=config.GetSelectedFullyName(channel,INPUT.allInputs_[i].sourceName_);
    TFile fOut(fOutName,"recreate");

    tr1 = tr->CopyTree(fullCut.ExtraCut("phoSigmaIEtaIEta","phoPFChIsoCorr","phoEta"));
    tr1->Write();

  }

}
