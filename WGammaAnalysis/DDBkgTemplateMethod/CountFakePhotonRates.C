#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TInputSample.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include <vector>

TString ParticleByPdgId(unsigned int id)
{
  switch (id){
    case     1: return "d";
    case     2: return "u";
    case     3: return "s";
    case     4: return "c";
    case     5: return "b";
    case     6: return "t";
    case    11: return "e";
    case    13: return "mu";
    case    15: return "tau";
    case    21: return "gluon";
    case    22: return "gamma";
    case    23: return "Z0";
    case    24: return "W";
    case   111: return "pi0";
    case   113: return "rho0(770)";
    case   221: return "eta-meson";
    case   223: return "omega(782)";
    case   313: return "K*(892)0";
    case   323: return "K*(892)+";
    case   331: return "? 331";
    case   333: return "? 333";
    case   999: return "? 999";
    case 20213: return "a1(1260)";
    default:
      std::cout<<"id="<<id<<", particle is unknown"<<std::endl;
      return "unknown";
  }
  return "unknown";
}

void CountFakePhotonRates()
{
  TConfiguration config;
  vector <int> vecSamples;
  vector <TString> vecNames;
  vecSamples.push_back(config.SIGMC);
  vecNames.push_back("Wg_to_munu");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("Wg_to_taunu");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("Wjets_to_lnu");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("Zg");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("DYjets_to_ll");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("multibosons");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("ttbarg");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("ttbarjets");

  int nSources = vecSamples.size();

  TCut cutWeight="weight";
  TCut cutFakeGamma="phoGenPID==-1000";
  TCut cutTrueGamma="phoGenPID==22";
  TCut cutMomIsLepton="phoGenMomPID==13 || phoGenMomPID==-13 || phoGenMomPID==11 || phoGenMomPID==-11 || phoGenMomPID==15 || phoGenMomPID==-15";
  TCut cutMomIsGaugeBoson="phoGenMomPID==21 || phoGenMomPID==23 || phoGenMomPID==24 || phoGenMomPID==-24";
  TCut cutMomIsQuark="phoGenMomPID>-6 && phoGenMomPID<6 && phoGenMomPID!=0";
  TCut cutISRorFSR=cutTrueGamma && (cutMomIsLepton || cutMomIsGaugeBoson || cutMomIsQuark);

  for (int iSource=0; iSource<nSources; iSource++){
//    if (vecInputs[iSource].sample_==config.DATA)
//      continue;

    TString fileName = config.GetSelectedName(config.FULLY, config.MUON, vecSamples[iSource], vecNames[iSource]);

    TFile f(fileName);
    if (!f.IsOpen()){
      std::cout<<"file "<<fileName<<" can't be open"<<std::endl;
      continue;
    }
    TTree* tr = (TTree*)f.Get("selectedEvents");
    TH1D* hist = new TH1D("hist","hist",1,tr->GetMinimum("weight"),tr->GetMaximum("weight"));
    tr->Draw("weight>>hist",cutWeight,"goff");
    int nEntries = hist->GetSumOfWeights();
    tr->Draw("weight>>hist",cutFakeGamma*cutWeight,"goff");
    int nFakes = hist->GetSumOfWeights();
    tr->Draw("weight>>hist",cutTrueGamma*cutWeight,"goff");
    int nTrues = hist->GetSumOfWeights();
    tr->Draw("weight>>hist",cutISRorFSR*cutWeight,"goff");
    int nISRorFSR = hist->GetSumOfWeights();
    float fakeRate = 0;
    if (nEntries!=0) fakeRate = 1.0*nFakes/nEntries;
    float trueRate = 0;
    if (nEntries!=0) trueRate = 1.0*nTrues/nEntries;
    std::cout<<vecNames[iSource]<<": "<<"n="<<nEntries<<", nFakes="<<nFakes<<", nTrues="<<nTrues<<", fakeRate="<<fakeRate<<", trueRate="<<trueRate<<", nISRorFSR="<<nISRorFSR<<std::endl;

  }

}
