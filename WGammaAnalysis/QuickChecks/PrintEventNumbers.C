#include "TFile.h"
#include "TTree.h"
#include "TBranch.h" 
#include <iostream>

void PrintEventNumbers()
{
  TFile* f = new TFile("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGamma_blindPRESCALE_DATA.root");

  TTree* tr = (TTree*)f->Get("selectedEvents");

  int event, run;
  TBranch *b_event, *b_run;

  tr->SetBranchAddress("event", &event, &b_event);
  tr->SetBranchAddress("run", &run, &b_run);


  long nentries = tr->GetEntries();

  for (long ient=0; ient<nentries; ient++){
    b_event->GetEntry(ient);
    b_run->GetEntry(ient);
    std::cout<<run<<" "<<event<<std::endl;
  }//end of loop over ient1
}
