#include "../Configuration/TConfiguration.h"
#include "../Include/TEventTree.h"
#include <iostream>
#include "TTree.h"
#include "TH2F.h"
#include "TCanvas.h"

void DrawMigrationMatrix(TTree* tr)
{

  TEventTree evTr;
  evTr.Init(tr);
  
  long nentries = tr->GetEntries();
  //nentries = 100000;

 // TFile fOut("testGenHists.root","RECREATE");
  TConfiguration config;
  float binLimits[config.GetNPhoPtBins()+1];
  vector <float> vecLimits = config.GetPhoPtBinsLimits();
  for (int i=0; i<config.GetNPhoPtBins()+1; i++){
    binLimits[i]=vecLimits[i];
  }
  TH2F* histMigr = new TH2F("hist","migration matrix", config.GetNPhoPtBins(),binLimits,config.GetNPhoPtBins(),binLimits);

  //for (long entry=0; entry<100000; entry++)
  for (long entry=0; entry<nentries; entry++)
 
    {
      evTr.GetEntryNeededBranchesOnly(entry);
      evTr.GetEntryMCSpecific(entry);

      if (evTr.treeLeaf.nMC==0) continue;

      for (int iMC=0; iMC<evTr.treeLeaf.nMC; iMC++)
        {

          /////////////////////////////////////
          // photons check starts
          /////////////////////////////////////

          if (fabs(evTr.treeLeaf.mcPID[iMC])==22){
            for (int ipho=0; ipho<evTr.treeLeaf.nPho; ipho++){
              if (evTr.treeLeaf.mcIndex[iMC]==evTr.treeLeaf.phoGenIndex[ipho]){
                int binx = config.FindPhoPtBinByPhoPt(evTr.treeLeaf.mcEt[iMC])+1;
                int biny = config.FindPhoPtBinByPhoPt(evTr.treeLeaf.phoEt[ipho])+1;
                float binCont = histMigr->GetBinContent(binx,biny)+1;
                //std::cout<<"binx="<<binx<<", biny="<<biny<<", binCont="<<binCont<<std::endl;
                histMigr->SetBinContent(binx,biny,binCont);
             
              } //end of if (mcIndex[iMC]==phoGenIndex[ipho])
            } //end of loop over photons
          } //end of if (fabs(mcPID[iMC])==22)

          /////////////////////////////////////
          // photons check ends
          /////////////////////////////////////


        } //end of loop over iMC    
    } //end of loop over entry 

  for (int iy=1; iy<histMigr->GetNbinsY()+1; iy++){
    float totalGen=0;
    for (int ix=1; ix<histMigr->GetNbinsX()+1; ix++){
      totalGen+=histMigr->GetBinContent(ix,iy);
    }
    for (int ix=1; ix<histMigr->GetNbinsX()+1; ix++){
      float cont = histMigr->GetBinContent(ix,iy)/totalGen;
      histMigr->SetBinContent(ix,iy,cont);
    }
  }

  TCanvas c("matrMigr","matrMigr");
  histMigr->Print();
  for (int ix=1; ix<histMigr->GetNbinsX()+1; ix++){
    for (int iy=1; iy<histMigr->GetNbinsY()+1; iy++){
      std::cout<<histMigr->GetBinContent(ix,iy)<<" ";
    }
    std::cout<<std::endl;
  }
  histMigr->SetStats(0);
  histMigr->GetXaxis()->SetMoreLogLabels(1);
  histMigr->GetYaxis()->SetMoreLogLabels(1);
  histMigr->Draw("COLZ");
  c.SetLogx();
  c.SetLogy();
  c.SaveAs("matrMigr.png");
}
