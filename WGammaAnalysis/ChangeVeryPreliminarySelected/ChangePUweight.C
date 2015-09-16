#include "../Selection/TSelectedEventsTree.h"
#include "../Configuration/TConfiguration.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

//coef = 1.05, 0.95

void ChangePUweight(float coef, int channel, int vgamma, int sample, TString sourceName)
{
  TConfiguration conf;
  TFile* f = new TFile(conf.GetSelectedName(conf.VERY_PRELIMINARY, channel, vgamma, conf.UNBLIND, sample, sourceName),"UPDATE");
  TTree* trInput = (TTree*)f->Get("selectedEvents");
  TTree* trOutput = (TTree*)f->Get("selectedEvents");
  TSelectedEventsTree selEvTree;
  selEvTree.SetAsInputTree(trInput);
  selEvTree.SetAsOutputTree(trOutput);
  long nEntries = trInput->GetEntries();
  float weight;
  for (long ientry=0; ientry<nEntries; ientry++){
        trInput->GetEntry(ientry);
        weight = coef*selEvTree._weight;
        PUweight = coef*selEvTree._PUweight;
        selEvTree._weight = weight;
        selEvTree._PUweight = PUweight;
        trOutput->Fill();
  }//end of loop over ientry
  trOutput->Write();
}//end of ChangePUweight

void LoopOverVeryPreliminarySelectedFiles()
{
  int nSources = _INPUT->nSources_; 
  for (int iSource=0; iSource<nSources; iSource++)
    {

       std::cout<<"_channel="<<_channel<<"("<<_config.StrChannel(_channel)<<")";
       std::cout<<", _vgamma="<<_vgamma<<"("<<_config.StrVgType(_vgamma)<<")"<<std::endl;
       std::cout<<"_sample="<<_sample<<"("<<_config.StrSample(_sample)<<")";
       std::cout<<", sourceName="<<_INPUT->allInputs_[iSource].sourceName_<<std::endl;
       TTree* tree;
       int inputFileNMax = _INPUT->allInputs_[iSource].nFiles_;

       ChangePUweight(coef, _channel, _vgamma, _INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_);

    } //loop over iSource ends
}//end of LoopOverVeryPreliminarySelectedFiles
