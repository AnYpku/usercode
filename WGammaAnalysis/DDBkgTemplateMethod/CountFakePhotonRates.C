#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TInputSample.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include <vector>

void CountFakePhotonRates()
{
  TConfiguration config;
  vector <int> vecSamples;
  vector <TString> vecNames;
  vecSamples.push_back(config.SIGMC);
  vecNames.push_back("Wg_to_munu");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("Wjets_to_lnu");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("DYjets_to_ll");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("Zg");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("multibosons");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("top");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("ttbarjets");

  int nSources = vecSamples.size();
  for (int iSource=0; iSource<nSources; iSource++){
//    if (vecInputs[iSource].sample_==config.DATA)
//      continue;

    TString fileName = config.GetSelectedFullyName(config.MUON, vecSamples[iSource], vecNames[iSource]);

    TFile f(fileName);
    if (!f.IsOpen()){
      std::cout<<"file "<<fileName<<" can't be open"<<std::endl;
      continue;
    }
    TTree* tr = (TTree*)f.Get("selectedEvents");
    int nEntries = tr->GetEntries("(1)*weight");
    int nFakes = tr->GetEntries("(phoGenPID==-1000)*weight");
    int nTrues = tr->GetEntries("(phoGenPID==22)*weight");
    float fakeRate = 0;
    if (nEntries!=0) fakeRate = 1.0*nFakes/nEntries;
    float trueRate = 0;
    if (nEntries!=0) trueRate = 1.0*nTrues/nEntries;
    std::cout<<vecNames[iSource]<<": "<<"n="<<nEntries<<", nFakes="<<nFakes<<", nTrues="<<nTrues<<", fakeRate="<<fakeRate<<", trueRate="<<trueRate<<std::endl;

  }

}
