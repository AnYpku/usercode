#include "../Include/TFullCuts.h"
#include "../Include/TPhotonCuts.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TInputSample.h"

#include "TFile.h"
#include "TCut.h"

#include <iostream>
#include <vector>

void ExtraSelection(int channel, int wp, TString strPhoIsoBase)
//strPhoBase = "PF" or "SCR"
{
  TFullCuts fullCut;
  TConfiguration config;
  TAllInputSamples INPUT(channel,"../Configuration/config.txt");
  TTree* tr=0;
  TTree* tr1=0;
  TTree* tr2=0;
  TTree* tr3=0;
//  INPUT.Print();
//  INPUT.allInputs_[0].Print();
  for (int i=0; i<INPUT.nSources_; i++){
//    INPUT.allInputs_[i].Print();
//    std::cout<<", INPUT.allInputs_["<<i<<"].nFiles_="<<INPUT.allInputs_[i].nFiles_<<std::endl;
//    std::cout<<", INPUT.allInputs_["<<i<<"].sourceName_="<<INPUT.allInputs_[i].sourceName_<<std::endl;
    TString fInName = config.GetSelectedName(config.VERY_PRELIMINARY,channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);

    TFile fIn(fInName);
    if (!fIn.IsOpen()){
      std::cout<<"file "<<fInName<<" can't be open"<<std::endl;
      continue;
    }
    std::cout<<"file "<<fInName<<" was open"<<std::endl;
    tr = (TTree*)fIn.Get("selectedEvents");

    TPhotonCuts photon;
    int phoWP=photon.GetWP();

    TString fOutName1=config.GetSelectedName(config.PRELIMINARY_FOR_MET_CUT,channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);
    TFile fOut1(fOutName1,"recreate");
    tr1 = tr->CopyTree(fullCut.RangeForMetCut(channel,phoWP, strPhoIsoBase));
    tr1->Write();

    TString fOutName2=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);
    TFile fOut2(fOutName2,"recreate");
    tr2 = tr->CopyTree(fullCut.RangeForTemplateMethodCut(channel,phoWP, strPhoIsoBase));
    tr2->Write();

    TString fOutName3=config.GetSelectedName(config.FULLY,channel,INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_);
    TFile fOut3(fOutName3,"recreate");
    tr3 = tr->CopyTree(fullCut.RangeFullCut(channel,wp,strPhoIsoBase));
    tr3->Write();

  }

}
