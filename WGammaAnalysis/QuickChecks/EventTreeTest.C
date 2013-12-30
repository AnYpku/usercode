#include "../Include/TEventTree.h"
#include "TFile.h"
#include "TTree.h"

void EventTreeTest()
{
  TFile f("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg.root");
  f.cd("ggNtuplizer");
  TTree* tree = (TTree *)gDirectory->Get("EventTree");
  TEventTree eventTree(tree);
  for (int i=0; i<10; i++){
    eventTree.fChain->GetEntry(i);
  }
}
