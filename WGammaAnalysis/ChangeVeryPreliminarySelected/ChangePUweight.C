#include "../Selection/TSelectedEventsTree.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"

#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include "iostream.h"

//coef = 1.05, 0.95

void ChangePUweight(float coef, int channel, int vgamma, int sample, TString sourceName)
{
  TConfiguration conf;
  TFile* f = new TFile(conf.GetSelectedName(conf.VERY_PRELIMINARY, channel, vgamma, conf.UNBLIND, sample, sourceName),"UPDATE");
  if (!f->IsOpen()) {
    std::cout<<"file" <<conf.GetSelectedName(conf.VERY_PRELIMINARY, channel, vgamma, conf.UNBLIND, sample, sourceName)<< "isn't open"<<std::endl;
    return;
  }
  TTree* trInput = (TTree*)f->Get("selectedEvents");
  TTree* trOutput = new TTree("selectedEventsNew","selected events new");
  TSelectedEventsTree selEvTree;
  selEvTree.SetAsInputTree(trInput);
  selEvTree.SetAsOutputTree(trOutput);
  long nEntries = trInput->GetEntries();
  float weight, PUweight;
  for (long ientry=0; ientry<nEntries; ientry++){
        trInput->GetEntry(ientry);
        if ((ientry%50000)==0) std::cout<<"ientry="<<ientry<<std::endl;
        weight = coef*selEvTree._weight;
        PUweight = coef*selEvTree._PUweight;
        selEvTree._weight = weight;
        selEvTree._PUweight = PUweight;
        trOutput->Fill();
  }//end of loop over ientry
  trInput->SetName("selectedEventsOld");
  f->Delete("selectedEventsOld");
  trOutput->SetName("selectedEvents");
  trOutput->Write();
}//end of ChangePUweight

void ChangePUweight_Files(float coef, int channel, int vgamma, std::string configfile)
{
  TConfiguration conf;
  TAllInputSamples INPUT(channel, vgamma, configfile);
  int nSources = INPUT.nSources_; 
  for (int iSource=0; iSource<nSources; iSource++){
       int sample = INPUT.allInputs_[iSource].sample_;
       if (sample==conf.DATA) continue;
       if (sample==conf.SIGMC_UNSKIMMED) continue;
       std::cout<<"channel="<<channel<<"("<<conf.StrChannel(channel)<<")";
       std::cout<<", vgamma="<<vgamma<<"("<<conf.StrVgType(vgamma)<<")"<<std::endl;
       std::cout<<"sample="<<sample<<"("<<conf.StrSample(sample)<<")";
       std::cout<<", sourceName="<<INPUT.allInputs_[iSource].sourceName_<<std::endl;
       ChangePUweight(coef, channel, vgamma, INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
  } //loop over iSource ends
}//end of LoopOverVeryPreliminarySelectedFiles
