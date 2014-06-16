#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TInputSample.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TCut.h"
#include "TH1D.h"
#include <vector>
#include <iomanip>
#include <iostream>

struct ParticlePDGID{
  static const int nIds=12;
  int id[nIds];
  TString name[nIds];
//  bool doCount;
  int counts[nIds];
};


void ParticleByPdgId(ParticlePDGID& particle)
{
  particle.id[0]=1; particle.name[0]="d";
  particle.id[1]=2; particle.name[1]="u";
  particle.id[2]=3; particle.name[2]="s";
  particle.id[3]=4; particle.name[3]="c";
//  particle.id[6]=11; particle.name[6]="e";
  particle.id[4]=13; particle.name[4]="mu";
  particle.id[5]=21; particle.name[5]="gluon";
//  particle.id[10]=22; particle.name[10]="gamma";
  particle.id[6]=23; particle.name[6]="Z";
  particle.id[7]=24; particle.name[7]="W";
  particle.id[8]=111; particle.name[8]="pi0";
//  particle.id[10]=113; particle.name[10]="rho0(770)";
  particle.id[9]=221; particle.name[9]="eta";
//  particle.id[10]=223; particle.name[10]="omega(782)";
  particle.id[10]=5; particle.name[10]="b";
  particle.id[11]=15; particle.name[11]="tau";
//  particle.id[11]=6; particle.name[11]="t";
//  particle.id[17]=313; particle.name[17]="K*(892)0";
//  particle.id[18]=323; particle.name[18]="K*(892)+";
//  particle.id[4]=331; particle.name[4]="? 331";
//  particle.id[5]=333; particle.name[5]="? 333";
//  particle.id[6]=999; particle.name[6]="? 999";
//  particle.id[10]=20213; particle.name[10]="a1(1260)";
}

void CountPhotonSources()
{
  ParticlePDGID particle;
  ParticleByPdgId(particle);

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
  vecNames.push_back("ttbarg");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("ttbarjets");
  vecSamples.push_back(config.BKGMC);
  vecNames.push_back("multibosons");

  int nSources = vecSamples.size();
  TCut cutWeight="weight";
  TCut cutTrueGamma="phoGenPID==22";
  TCut cutFakeGamma=!cutTrueGamma;
  TCut cutCS="phoEt>15 && phoEt<600 && lePhoDeltaR>0.7";


  std::cout<<std::setw(15)<<"sample";
  std::cout<<std::setw(8)<<" n";
  std::cout<<std::setw(8)<<" nFakes";
  std::cout<<std::setw(8)<<" nTrues";
  for (int i=0; i<particle.nIds; i++){
    std::cout<<std::setw(8)<<particle.name[i];
  }
  std::cout<<std::endl;
  for (int iSource=0; iSource<nSources; iSource++){

//    TString fileName = config.GetSelectedName(config.VERY_PRELIMINARY, config.MUON, vecSamples[iSource], vecNames[iSource]);

    TString fileName = config.GetSelectedName(config.FULLY, config.MUON, config.UNBLIND, vecSamples[iSource], vecNames[iSource]);

    TFile f(fileName);
    if (!f.IsOpen()){
      std::cout<<"file "<<fileName<<" can't be open"<<std::endl;
      continue;
    }
    TTree* tr = (TTree*)f.Get("selectedEvents");
    TH1D* hist = new TH1D("hist","hist",1,0,10000);

    tr->Draw("weight>>hist",cutCS*cutWeight,"goff");
    int nEntries = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",(cutCS && cutFakeGamma)*cutWeight,"goff");
    int nFakes = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",(cutCS && cutTrueGamma)*cutWeight,"goff");
    int nTrues = hist->GetSumOfWeights();

    std::cout<<std::setw(15)<<vecNames[iSource];
    std::cout<<std::setw(8)<<nEntries;
    std::cout<<std::setw(8)<<nFakes;
    std::cout<<std::setw(8)<<nTrues;

    for (int i=0; i<particle.nIds; i++){
      TString strCut="phoGenMomPID==";
      strCut+=particle.id[i];
      strCut+=" || phoGenMomPID==-";
      strCut+=particle.id[i];
      TCut cut(strCut);
      tr->Draw("weight>>hist",(cutCS && cutTrueGamma && cut)*cutWeight,"goff");
      particle.counts[i] = hist->GetSumOfWeights();
      std::cout<<std::setw(6)<<100*particle.counts[i]/nTrues<<std::setw(2)<<"%";
    }

    std::cout<<std::endl;

  }//end of loop over iSource

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
//  vecSamples.push_back(config.BKGMC);
//  vecNames.push_back("multibosons");
//  vecSamples.push_back(config.BKGMC);
//  vecNames.push_back("ttbarg");
//  vecSamples.push_back(config.BKGMC);
//  vecNames.push_back("ttbarjets");

  int nSources = vecSamples.size();

  TCut cutWeight="weight";
  TCut cutFakeGamma="!(phoGenPID==22)";
  TCut cutTrueGamma="phoGenPID==22";
  TCut cutMomIsMuon="phoGenMomPID==13 || phoGenMomPID==-13";
  TCut cutMomIsW="phoGenMomPID==24 || phoGenMomPID==-24";
  TCut cutMomIsQuark="phoGenMomPID>=-6 && phoGenMomPID<=6 && phoGenMomPID!=0";
  TCut cutGMomIsQuark="phoGenGMomPID>=-6 && phoGenGMomPID<=6 && phoGenGMomPID!=0";
  TCut cutGMomIsW="phoGenGMomPID==24 || phoGenGMomPID==-24";
  TCut cutGMomIsZ="phoGenGMomPID==23";
  TCut cutGMomIsGamma="phoGenGMomPID==22";

  TCut cutISR=cutTrueGamma && cutMomIsQuark;
  TCut cutFSR_W=cutTrueGamma && cutMomIsMuon && cutGMomIsW;
  TCut cutFSR_Z=cutTrueGamma && cutMomIsMuon && cutGMomIsZ;
  TCut cutFSR_Gamma=cutTrueGamma && cutMomIsMuon && cutGMomIsGamma;
  TCut cutTGC=cutTrueGamma && cutMomIsW  && cutGMomIsQuark;

  for (int iSource=0; iSource<nSources; iSource++){
//    if (vecInputs[iSource].sample_==config.DATA)
//      continue;

    TString fileName = config.GetSelectedName(config.VERY_PRELIMINARY, config.MUON, config.UNBLIND, vecSamples[iSource], vecNames[iSource]);

    TFile f(fileName);
    if (!f.IsOpen()){
      std::cout<<"file "<<fileName<<" can't be open"<<std::endl;
      continue;
    }
    TTree* tr = (TTree*)f.Get("selectedEvents");
    TH1D* hist = new TH1D("hist","hist",1,0,10000);

    tr->Draw("weight>>hist",cutWeight,"goff");
    int nEntries = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutFakeGamma*cutWeight,"goff");
    int nFakes = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutTrueGamma*cutWeight,"goff");
    int nTrues = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutISR*cutWeight,"goff");
    int nISR = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutFSR_W*cutWeight,"goff");
    int nFSR_W = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutFSR_Z*cutWeight,"goff");
    int nFSR_Z = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutFSR_Gamma*cutWeight,"goff");
    int nFSR_Gamma = hist->GetSumOfWeights();

    tr->Draw("weight>>hist",cutTGC*cutWeight,"goff");
    int nTGC = hist->GetSumOfWeights();

    float fakeRate = 0;
    if (nEntries!=0) fakeRate = 1.0*nFakes/nEntries;
    float trueRate = 0;
    if (nEntries!=0) trueRate = 1.0*nTrues/nEntries;

    std::cout<<std::setw(15)<<vecNames[iSource];
    std::cout<<": n="<<std::setw(7)<<nEntries;
    std::cout<<", nFakes="<<std::setw(6)<<nFakes;
    std::cout<<", nTrues="<<std::setw(6)<<nTrues;
    std::cout<<", nISR="<<std::setw(6)<<nISR;
    std::cout<<", nFSR(W)="<<std::setw(6)<<nFSR_W;
    std::cout<<", nFSR(Z)="<<std::setw(6)<<nFSR_Z;
    std::cout<<", nFSR(Gamma)="<<std::setw(6)<<nFSR_Gamma;
    std::cout<<", nTGC="<<std::setw(6)<<nTGC<<std::endl;

  }//end of loop over iSource

}

void GetList(int sample, TString sourceName, int eventNeeded)
{

  TConfiguration config;
  TString fileName = config.GetSelectedName(config.FULLY, config.MUON, config.UNBLIND, sample, sourceName);

  TFile f(fileName);
  if (!f.IsOpen()){
    std::cout<<"file "<<fileName<<" can't be open"<<std::endl;
    return;
  }

  TTree* tr = (TTree*)f.Get("selectedEvents");
  Int_t           event;
  Int_t           nMC;
  vector<int>     *mcPID;
  vector<int>     *mcMomPID;
  vector<int>     *mcGMomPID;
  mcPID=0;
  mcMomPID=0;
  mcGMomPID=0;
  TBranch        *b_event;   //!
  TBranch        *b_nMC;   //!
  TBranch        *b_mcPID;   //!
  TBranch        *b_mcMomPID;   //!
  TBranch        *b_mcGMomPID;   //!
  tr->SetBranchAddress("event", &event, &b_event);
  tr->SetBranchAddress("nMC", &nMC, &b_nMC);
  tr->SetBranchAddress("mcPID", &mcPID, &b_mcPID);
  tr->SetBranchAddress("mcMomPID", &mcMomPID, &b_mcMomPID);
  tr->SetBranchAddress("mcGMomPID", &mcGMomPID, &b_mcGMomPID);

  long nEntries = tr->GetEntries();

  for (long ie=0; ie<nEntries; ie++){
//    b_event->GetEntry(ie);
//    b_nMC->GetEntry(ie);
//    b_mcPID->GetEntry(ie);
//    b_mcMomPID->GetEntry(ie);
//    b_mcGMomPID->GetEntry(ie);
    tr->GetEntry(ie);

    if (event!=eventNeeded) continue;
    std::cout<<std::endl;
    std::cout<<"source="<<sourceName<<"; event="<<event<<"; nMC="<<nMC<<std::endl;
    std::cout<<std::setw(10)<<"mcPID";
    for(int i=0; i<nMC; i++)
      std::cout<<std::setw(7)<<mcPID->at(i);
    std::cout<<std::endl;
    std::cout<<std::setw(10)<<"mcMomPID";
    for(int i=0; i<nMC; i++)
      std::cout<<std::setw(7)<<mcMomPID->at(i);
    std::cout<<std::endl;
    std::cout<<std::setw(10)<<"mcGMomPID";
    for(int i=0; i<nMC; i++)
      std::cout<<std::setw(7)<<mcGMomPID->at(i);
    std::cout<<std::endl;
    std::cout<<std::endl;

  }

}
