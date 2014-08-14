#include "../Configuration/TConfiguration.h"
#include "../Selection/TSelectedEventsTree.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

void ComputeNumberOfCandidatesPerEvent(TString fInName)
{
  TFile fIn(fInName);
  if (!fIn.IsOpen()){
    std::cout<<"file "<<fInName<<" can't be open"<<std::endl;
    return;
  }
  std::cout<<"file "<<fInName<<" was open"<<std::endl;
  TTree* tr = (TTree*)fIn.Get("selectedEvents");
  TSelectedEventsTree selEvTree;
  selEvTree.SetAsInputTree(tr);
  long nEntries = tr->GetEntries();
  int nOfCandsMax=10;
  long nEventsWithGivenNofCands[nOfCandsMax];
  long nEvents=0;
  for (int in=0; in<nOfCandsMax; in++){
    nEventsWithGivenNofCands[in]=0;
  }
  int inc=0;
//  nEntries=10;
  for (long ientry=0; ientry<nEntries; ientry+=inc){
    inc=0;
    long eventCurr=0;
    long eventPrev=-1;
//    std::cout<<"before \"while\": ientry="<<ientry<<", eventCurr="<<eventCurr<<", eventPrev="<<eventPrev<<std::endl;
    int nCands=1;
    while (1){
      eventPrev=eventCurr;
      tr->GetEntry(ientry+inc);
      eventCurr = selEvTree._event;
//      std::cout<<"ientry+inc="<<ientry+inc<<", eventCurr="<<eventCurr<<", eventPrev="<<eventPrev<<std::endl;
      if (eventCurr==eventPrev){
        nCands++;
      }
      if (inc>0 && eventCurr!=eventPrev){
//        std::cout<<"event="<<eventPrev<<", nCands="<<nCands<<std::endl;
        if (nCands<nOfCandsMax) nEventsWithGivenNofCands[nCands]++;
        inc--;
        break;
      }
      if (ientry+inc>=nEntries) break;
      inc++;
    }//end of while loop  
    nEvents++;
    inc++;
    if (ientry+inc>=nEntries) break;  
  }//end of loop over ientry
  std::cout<<"nEntries="<<nEntries<<", nEvents="<<nEvents<<std::endl;
  for (int in=0; in<nOfCandsMax; in++){
    std::cout<<"N of events with "<<in<<" cands: "<<nEventsWithGivenNofCands[in]<<std::endl;
  }
}
