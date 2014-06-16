#include "SearchForOverlap.h" 
  //this class
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "TMath.h" 
#include "TCanvas.h" 
  // ROOT class
#include <iostream> 
#include <iomanip>
#include <string>
#include <sstream>  
  //standard C++ class

SearchForOverlap::SearchForOverlap(int channel, bool isDebugMode, string analyzedSampleNames, string configFile, bool isNoPuReweight)
{

  _INPUT = new TAllInputSamples(channel, configFile);

  _channel=channel;
  _isNoPuReweight=isNoPuReweight;
  _isDebugMode=isDebugMode;

  std::cout<<"_isDegubMode: "<<_isDebugMode<<std::endl;  

  stringstream ss(analyzedSampleNames);
  vector <string> names;
  string name;
  while (ss >> name) 
    names.push_back(name);
  int nNames = names.size();

  std::cout<<"1 - will do, 0 - will not do"<<std::endl;
  for (int i=0; i<_INPUT->nSources_; i++)
    {
      _doAnalizeSample.push_back(0);  
      for (int j=0; j<nNames; j++)
        {
          if (names[j]==_INPUT->allInputs_[i].sourceName_)
            _doAnalizeSample.back()=1; 
        }
      std::cout<<_INPUT->allInputs_[i].sourceName_<<": "<<_doAnalizeSample[i]<<std::endl;
    }  
  if ((int)_doAnalizeSample.size()!=_INPUT->nSources_)
    std::cout<<"ERROR in SearchForOverlap::SearchForOverlap: wrong _doAnalizeSample.size()"<<std::endl;

  _fOut = new TFile("fOut.root","recreate");

}

SearchForOverlap::~SearchForOverlap()
{
   _eventTree.fChain = 0;
     //field of TEventTree 
}

void SearchForOverlap::LoopOverInputFiles()
{
  int nSources = _INPUT->nSources_; 
  //if (isDebugMode_ && nSources>3)
  //  nSources = 3;

  _hasWGamma=0;
  _hasWJets=0;
  _hasZGamma=0;
  _hasZJets=0;
  for (_iSource=0; _iSource<nSources; _iSource++)
    {
       std::cout<<"iSource="<<_iSource<<": "<<_INPUT->allInputs_[_iSource].sourceName_<<std::endl;
       _sample = _INPUT->allInputs_[_iSource].sample_;
       if (_sample==TConfiguration::DATA){
         _lumiData=_INPUT->allInputs_[_iSource].lumiTotal_;
       }

       if (!_doAnalizeSample[_iSource])
         continue;

//       _selectedTreeFileName=_config.GetSelectedName(_config.VERY_PRELIMINARY,_channel,_INPUT->allInputs_[_iSource].sample_,_INPUT->allInputs_[_iSource].sourceName_,_isDebugMode,_isNoPuReweight);

       if(_INPUT->allInputs_[_iSource].sourceName_=="Wg"){ 
          _hasWGamma=0;
          _idWGamma=_iSource;
       }
       else if(_INPUT->allInputs_[_iSource].sourceName_=="Wjets_to_lnu"){  
         _hasWJets=1;
         _idWJets=_iSource;
       }
       else if(_INPUT->allInputs_[_iSource].sourceName_=="Zg"){
         _hasZGamma=1;
         _idZGamma=_iSource;
       }
       else if(_INPUT->allInputs_[_iSource].sourceName_=="DYjets_to_ll"){
          _hasZJets=1;
          _idZJets=_iSource;
       }
       
      std::cout<<"_idZGamma="<<_idZGamma<<", _idZJets="<<_idZJets<<std::endl;
      std::cout<<"_hasZGamma="<<_hasZGamma<<", _hasZJets="<<_hasZJets<<std::endl;


       TTree* tree;
       int inputFileNMax = _INPUT->allInputs_[_iSource].nFiles_;
       if (inputFileNMax>1) return;

       TFile* f = new TFile(_INPUT->allInputs_[_iSource].fileNames_[0]);
        if (f->IsOpen()) 
          std::cout<<std::endl<<"processing file "<<_INPUT->allInputs_[_iSource].fileNames_[0]<<std::endl;
         else{
           std::cout<<"ERROR detected in SearchForOverlap::LoopOverInputFiles: file "<<_INPUT->allInputs_[_iSource].fileNames_[0]<<" was not found"<<std::endl;
           return;
         } 
         f->cd("ggNtuplizer");
         tree = (TTree*)gDirectory->Get("EventTree");
         if (tree) 
           _eventTree.Init(tree);
         else{
           std::cout<<"ERROR detected in SearchForOverlap::LoopOverInputFiles: tree in the file "<<_INPUT->allInputs_[_iSource].fileNames_[0]<<" does not exist"<<std::endl;
           return;
         } 
         TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
         _nentries = (double)hEvents->GetBinContent(1);

         float csMC=_INPUT->allInputs_[_iSource].cs_[0];
         _lumiWeight=_lumiData*csMC/_nentries;

         _puWeight=new TPuReweight(_config.GetPileupDataFileName(),_INPUT->allInputs_[_iSource].fileNames_[0]);

         _fOut->cd();
         TString histNameB="h";
         histNameB+=_INPUT->allInputs_[_iSource].sourceName_;
         TString histName;

         histName=histNameB+"_weight";
         _histWeight[_iSource]=new TH1F(histName,histName,1000,0,100);

         histName=histNameB+"_PhoPt";
         _histPhoPt[_iSource]=new TH1F(histName,histName,60,0,60);
         histName=histNameB+"_PhoEta";
         _histPhoEta[_iSource]=new TH1F(histName,histName,10,-2.5,2.5);
         histName=histNameB+"_PhoPhi";
         _histPhoPhi[_iSource]=new TH1F(histName,histName,18,-1.125*TMath::Pi(),1.125*TMath::Pi());

         histName=histNameB+"_Lep1Pt";
         _histLep1Pt[_iSource]=new TH1F(histName,histName,20,0,100);
         histName=histNameB+"_Lep1Eta";
         _histLep1Eta[_iSource]=new TH1F(histName,histName,10,-2.5,2.5);
         histName=histNameB+"_Lep1Phi";
         _histLep1Phi[_iSource]=new TH1F(histName,histName,18,-1.125*TMath::Pi(),1.125*TMath::Pi());

         histName=histNameB+"_Lep2Pt";
         _histLep2Pt[_iSource]=new TH1F(histName,histName,20,0,100);
         histName=histNameB+"_Lep2Eta";
         _histLep2Eta[_iSource]=new TH1F(histName,histName,10,-2.5,2.5);
         histName=histNameB+"_Lep2Phi";
         _histLep2Phi[_iSource]=new TH1F(histName,histName,18,-1.125*TMath::Pi(),1.125*TMath::Pi());

         histName=histNameB+"_Lep1PhodR";
         _histLep1PhodR[_iSource]=new TH1F(histName,histName,30,0,1.5);
         histName=histNameB+"_Lep2PhodR";
         _histLep2PhodR[_iSource]=new TH1F(histName,histName,30,0,1.5);
         histName=histNameB+"_LepLepdR";
         _histLepLepdR[_iSource]=new TH1F(histName,histName,40,0,20);
         histName=histNameB+"_LepLepMassTr";
         _histLepLepMass[_iSource]=new TH1F(histName,histName,12,0,120);
           
         LoopOverTreeEvents();
             //method of this class (SearchForOverlap)
         _eventTree.fChain=0;
             //field of TEventTree
 

         delete _puWeight;

    } //loop over iSource ends

    if (_hasZGamma && _hasZJets){
      PlotTwoHistograms("cZPhoPt", _histPhoPt[_idZGamma], _histPhoPt[_idZJets]);
      PlotTwoHistograms("cZPhoEta", _histPhoEta[_idZGamma], _histPhoEta[_idZJets]);
      PlotTwoHistograms("cZPhoPhi", _histPhoPhi[_idZGamma], _histPhoPhi[_idZJets]);
      PlotTwoHistograms("cZLep1Pt", _histLep1Pt[_idZGamma], _histLep1Pt[_idZJets]);
      PlotTwoHistograms("cZLep1Eta", _histLep1Eta[_idZGamma], _histLep1Eta[_idZJets]);
      PlotTwoHistograms("cZLep1Phi", _histLep1Phi[_idZGamma], _histLep1Phi[_idZJets]);
      PlotTwoHistograms("cZLep2Pt", _histLep2Pt[_idZGamma], _histLep2Pt[_idZJets]);
      PlotTwoHistograms("cZLep2Eta", _histLep2Eta[_idZGamma], _histLep2Eta[_idZJets]);
      PlotTwoHistograms("cZLep2Phi", _histLep2Phi[_idZGamma], _histLep2Phi[_idZJets]);
      PlotTwoHistograms("cZLep1PhodR", _histLep1PhodR[_idZGamma], _histLep1PhodR[_idZJets]);
      PlotTwoHistograms("cZLep2PhodR", _histLep2PhodR[_idZGamma], _histLep2PhodR[_idZJets]);
      PlotTwoHistograms("cZLepLepdR", _histLepLepdR[_idZGamma], _histLepLepdR[_idZJets]);
      PlotTwoHistograms("cZLepLepMassT", _histLepLepMass[_idZGamma], _histLepLepMass[_idZJets]);
      PlotTwoHistograms("cWeight", _histWeight[_idZGamma], _histWeight[_idZJets]);
    }

}

void SearchForOverlap::PlotTwoHistograms(TString canvName, TH1F* hGamma, TH1F* hJets)
{
   TCanvas* canv = new TCanvas(canvName,canvName,1200,600);
   canv->Divide(2,1);
   hJets->SetLineWidth(2);
   hGamma->SetLineWidth(2);
   hJets->SetLineColor(1);
   hGamma->SetLineColor(2);
   canv->cd(1);
   hJets->Draw();
   hGamma->Draw("same");
   canv->cd(2);
   hJets->DrawNormalized();
   hGamma->DrawNormalized("same");
   canvName+=".png";
   canv->SaveAs(canvName);
}

void SearchForOverlap::LoopOverTreeEvents()
{

   if (_eventTree.fChain == 0) return;
   Long64_t nentries = _eventTree.fChain->GetEntries();
   if (_isDebugMode && nentries>_debugModeNEntries) 
     nentries=_debugModeNEntries;

   if (_sample==_config.DATA){
     std::cout<<"ERROR in SearchForOverlap::LoopOverTreeEvents: search for overlap is impossible for DATA"<<std::endl;
     return;
   }

   _nEvents=0;
   _nEventsWithPhotons=0;
   _nPhotons=0;

   for (int ib=Z_BOSON; ib<=W_BOSON; ib++){
     _nTGC[ib]=0;//( W,Z -> gamma)
     for (int il=0; il<=TAU_LEPTON; il++)
       _nFSR[ib][il]=0;//(W -> (mu,e,tau) -> gamma)
     for (int iq=0; iq<=GLUON; iq++)      
      _nISR[ib][iq]=0;   //(d,u,s,c,b,t) -> gamma, 
   }

   for (int npho=0; npho<91; npho++){
     _nEventsWithGivenNOfPhotons[npho]=0;
     _nEventsWithSomeScheme[npho]=0;
   }


   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {

       bool isZMuonFSR=0;
       bool isZMuonTGC=0;

        _eventTree.GetEntryMCSpecific(entry);

        _totalWeight = 1;
        _totalWeight = _lumiWeight;
        _totalWeight*= _puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
        _nEvents+=_totalWeight;
        _histWeight[_iSource]->Fill(_totalWeight);
        int nPhoInEvent=0;
        int nSomeScheme=0;

        for (int imc=0; imc<_eventTree.treeLeaf.nMC; imc++){

          if (_eventTree.treeLeaf.mcPID->at(imc)!=22) continue;
          _nPhotons+=_totalWeight;
          if (!nPhoInEvent){
            _nEventsWithPhotons+=_totalWeight;
          }
          nPhoInEvent++;

          //FSR
          if (fabs(_eventTree.treeLeaf.mcGMomPID->at(imc))==Z_BOSON || fabs(_eventTree.treeLeaf.mcGMomPID->at(imc))==W_BOSON){ 
            unsigned int ib = fabs(_eventTree.treeLeaf.mcGMomPID->at(imc));
            if (fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==E_LEPTON ||
                fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==MU_LEPTON ||
                fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==TAU_LEPTON){
              _nFSR[ib][ANY_LEPTON]+=_totalWeight;
              unsigned int il = fabs(_eventTree.treeLeaf.mcMomPID->at(imc));
              _nFSR[ib][il]+=_totalWeight;
              nSomeScheme++;
              
              if(((fabs(_eventTree.treeLeaf.mcGMomPID->at(imc))==Z_BOSON) &&
                 (fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==MU_LEPTON)))
                isZMuonFSR=1;            
  
            }  //end of  if (fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==E_LEPTON      
          }//end of if (fabs(_eventTree.treeLeaf.mcGMomPID->at(imc))==Z_BOSON

          //TGC
          if (fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==Z_BOSON || fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==W_BOSON){
            unsigned int ib = fabs(_eventTree.treeLeaf.mcMomPID->at(imc));
            if ((fabs(_eventTree.treeLeaf.mcGMomPID->at(imc))>=D_QUARK &&
                fabs(_eventTree.treeLeaf.mcGMomPID->at(imc))<=T_QUARK) ||
                _eventTree.treeLeaf.mcGMomPID->at(imc)==GLUON){
                  _nTGC[ib]+=_totalWeight;
                  nSomeScheme++;
            }   
            for (int imc2=0; imc2<_eventTree.treeLeaf.nMC; imc2++){
              if (fabs(_eventTree.treeLeaf.mcPID->at(imc2))==MU_LEPTON &&
                  fabs(_eventTree.treeLeaf.mcMomPID->at(imc2))==Z_BOSON &&
                  fabs(_eventTree.treeLeaf.mcMomPID->at(imc))==Z_BOSON)
                isZMuonTGC=1;
            }
          }
 
          //ISR
          if ((fabs(_eventTree.treeLeaf.mcMomPID->at(imc))>=D_QUARK &&
              fabs(_eventTree.treeLeaf.mcMomPID->at(imc))<=T_QUARK) ||
              _eventTree.treeLeaf.mcMomPID->at(imc)==GLUON){
            unsigned int iq=fabs(_eventTree.treeLeaf.mcMomPID->at(imc));
            bool hasBosonFromTheSameQ=0;
            unsigned int ib = 0;
            for (int imc2=0; imc2<_eventTree.treeLeaf.nMC; imc2++){
              if (fabs(_eventTree.treeLeaf.mcPID->at(imc2))==Z_BOSON || fabs(_eventTree.treeLeaf.mcPID->at(imc2))==W_BOSON){
                ib = fabs(_eventTree.treeLeaf.mcPID->at(imc2));
                if (fabs(_eventTree.treeLeaf.mcMomPID->at(imc2))==iq){
                  hasBosonFromTheSameQ=1;
                  break;
                }
              }
            }//end of loop over imc2
            if (hasBosonFromTheSameQ){
              _nISR[ib][ANY_QUARK]+=_totalWeight;
              _nISR[ib][iq]+=_totalWeight;
              nSomeScheme++;
            }
          }//end of if (#smth#>=D_QUARK <...>

          if (!isZMuonFSR && !isZMuonTGC) continue;

          //fill histograms
          int ilep1=-1;
          int ilep2=-1;
          for (int imc2=0; imc2<_eventTree.treeLeaf.nMC; imc2++){
            if (fabs(_eventTree.treeLeaf.mcPID->at(imc2))==MU_LEPTON)
              if (fabs(_eventTree.treeLeaf.mcMomPID->at(imc2))==Z_BOSON){
                if (ilep1==-1) ilep1=imc2;
                else if (_eventTree.treeLeaf.mcPt->at(imc2)<_eventTree.treeLeaf.mcPt->at(ilep1)){
                  ilep2=imc2;
                  continue;
                }    
                else{
                  ilep2=ilep1;
                  ilep1=imc2;
                  continue;
                }
              }
          }//end of loop over imc2

          _histPhoPt[_iSource]->Fill(_eventTree.treeLeaf.mcEt->at(imc),_totalWeight);
          _histPhoEta[_iSource]->Fill(_eventTree.treeLeaf.mcEta->at(imc),_totalWeight);
          _histPhoPhi[_iSource]->Fill(_eventTree.treeLeaf.mcPhi->at(imc),_totalWeight);
          float dphi, deta, dR; 
          if (ilep1>-1){
            _histLep1Pt[_iSource]->Fill(_eventTree.treeLeaf.mcPt->at(ilep1),_totalWeight); 
            _histLep1Eta[_iSource]->Fill(_eventTree.treeLeaf.mcEta->at(ilep1),_totalWeight); 
            _histLep1Phi[_iSource]->Fill(_eventTree.treeLeaf.mcPhi->at(ilep1),_totalWeight); 
          // Lep1 Pho dR
            dphi=fabs(_eventTree.treeLeaf.mcPhi->at(ilep1)-_eventTree.treeLeaf.mcPhi->at(imc));  
            if (dphi>2*TMath::Pi())
              dphi=dphi-2*TMath::Pi();  
            deta=  _eventTree.treeLeaf.mcEta->at(ilep1)-_eventTree.treeLeaf.mcEta->at(imc);
            dR=dphi*dphi+deta*deta;  
            _histLep1PhodR[_iSource]->Fill(dR,_totalWeight);
          }
          if (ilep2>-1){  
            _histLep2Pt[_iSource]->Fill(_eventTree.treeLeaf.mcPt->at(ilep2),_totalWeight);
            _histLep2Eta[_iSource]->Fill(_eventTree.treeLeaf.mcEta->at(ilep2),_totalWeight); 
            _histLep2Phi[_iSource]->Fill(_eventTree.treeLeaf.mcPhi->at(ilep2),_totalWeight); 
          // Lep2 Pho dR 
            dphi=fabs(_eventTree.treeLeaf.mcPhi->at(ilep2)-_eventTree.treeLeaf.mcPhi->at(imc));  
            if (dphi>2*TMath::Pi())
              dphi=dphi-2*TMath::Pi();  
            deta=  _eventTree.treeLeaf.mcEta->at(ilep2)-_eventTree.treeLeaf.mcEta->at(imc);
            dR=dphi*dphi+deta*deta;  
            _histLep2PhodR[_iSource]->Fill(dR,_totalWeight);   
          }

          if (ilep1>-1 && ilep2>-1){ 
          // Lep Lep dR  
            dphi=fabs(_eventTree.treeLeaf.mcPhi->at(ilep2)-_eventTree.treeLeaf.mcPhi->at(ilep1));  
            if (dphi>2*TMath::Pi())
              dphi=dphi-2*TMath::Pi();  
            deta=  _eventTree.treeLeaf.mcEta->at(ilep2)-_eventTree.treeLeaf.mcEta->at(ilep1);
            dR=dphi*dphi+deta*deta;  
            _histLepLepdR[_iSource]->Fill(dR,_totalWeight); 
            // Lep Lep Mass Transversed
            float massT = sqrt(2*_eventTree.treeLeaf.mcEt->at(ilep1)*_eventTree.treeLeaf.mcEt->at(ilep2)*(1-TMath::Cos(dphi))); 
            _histLepLepMass[_iSource]->Fill(massT,_totalWeight); 
          }
         
        }    //end of loop over imc 

        if (nPhoInEvent<91)
          _nEventsWithGivenNOfPhotons[nPhoInEvent]+=_totalWeight; 
        if (nSomeScheme<91)
          _nEventsWithSomeScheme[nSomeScheme]+=_totalWeight; 

        if ((nPhoInEvent==0 && _nEventsWithGivenNOfPhotons[0]<0) ||
             (nSomeScheme==0 && _nEventsWithSomeScheme[0]<0)){
          std::cout<<"event "<<entry<<" has no photons or no schemes;"<<std::endl;
          std::cout<<"mcPIDs:    ";
          for (int imc3=0; imc3<_eventTree.treeLeaf.nMC; imc3++)
            std::cout<<std::setw(2)<<_eventTree.treeLeaf.mcPID->at(imc3)<<", ";
          std::cout<<std::endl;
          std::cout<<"mcMomPIDs: ";
          for (int imc3=0; imc3<_eventTree.treeLeaf.nMC; imc3++)
            std::cout<<std::setw(2)<<_eventTree.treeLeaf.mcMomPID->at(imc3)<<", ";
          std::cout<<std::endl;
          std::cout<<"mcGMomPIDs:";
          for (int imc3=0; imc3<_eventTree.treeLeaf.nMC; imc3++)
            std::cout<<std::setw(2)<<_eventTree.treeLeaf.mcGMomPID->at(imc3)<<", ";
          std::cout<<std::endl;

        }

     } //end of loop over events in the tree

     std::cout<<"weighted number of events:"<<std::endl;
     std::cout<<"nTot="<<_nEvents<<", nWithPhotons="<<_nEventsWithPhotons<<", nPhotons="<<_nPhotons<<std::endl;
/*
     std::cout<<"n of events with diff n of photons:"<<std::endl;
     for (int nphos=0; nphos<91; nphos++){
       if (_nEventsWithGivenNOfPhotons[nphos]!=0)
         std::cout<<nphos<<": "<<_nEventsWithGivenNOfPhotons[nphos]<<"; ";
     }
     std::cout<<std::endl;
     std::cout<<"n of events with diff n of schemes:"<<std::endl;
     for (int nphos=0; nphos<91; nphos++){
       if (_nEventsWithSomeScheme[nphos]!=0)
         std::cout<<nphos<<": "<<_nEventsWithSomeScheme[nphos]<<"; ";
     }
     std::cout<<std::endl;
     std::cout<<"nTGC(W)="<<_nTGC[W_BOSON]<<std::endl;
     std::cout<<"nFSR(W)="<<_nFSR[W_BOSON][ANY_LEPTON]<<", e="<<_nFSR[W_BOSON][E_LEPTON]<<", mu="<<_nFSR[W_BOSON][MU_LEPTON]<<", tau="<<_nFSR[W_BOSON][TAU_LEPTON]<<std::endl;
     std::cout<<"nISR(W)="<<_nISR[W_BOSON][ANY_QUARK]<<", d="<<_nISR[W_BOSON][D_QUARK]<<", u="<<_nISR[W_BOSON][U_QUARK]<<", s="<<_nISR[W_BOSON][S_QUARK]<<", c="<<_nISR[W_BOSON][C_QUARK]<<", b="<<_nISR[W_BOSON][B_QUARK]<<", t="<<_nISR[W_BOSON][T_QUARK]<<", gluon="<<_nISR[W_BOSON][GLUON]<<std::endl;
     std::cout<<"fractions:"<<std::endl;
     std::cout<<"nTGC(W)="<<100*_nTGC[W_BOSON]/_nPhotons<<"%"<<std::endl;
     std::cout<<"nFSR(W)="<<100*_nFSR[W_BOSON][ANY_LEPTON]/_nPhotons<<"%, e="<<100*_nFSR[W_BOSON][E_LEPTON]/_nPhotons<<"%, mu="<<100*_nFSR[W_BOSON][MU_LEPTON]/_nPhotons<<"%, tau="<<100*_nFSR[W_BOSON][TAU_LEPTON]/_nPhotons<<"%"<<std::endl;
     std::cout<<"nISR(W)="<<100*_nISR[W_BOSON][ANY_QUARK]/_nPhotons<<"%, d="<<100*_nISR[W_BOSON][D_QUARK]/_nPhotons<<"%, u="<<100*_nISR[W_BOSON][U_QUARK]/_nPhotons<<"%, s="<<100*_nISR[W_BOSON][S_QUARK]/_nPhotons<<"%, c="<<100*_nISR[W_BOSON][C_QUARK]/_nPhotons<<"%, b="<<100*_nISR[W_BOSON][B_QUARK]/_nPhotons<<"%, t="<<100*_nISR[W_BOSON][T_QUARK]/_nPhotons<<"%, gluon="<<100*_nISR[W_BOSON][GLUON]/_nPhotons<<"%"<<std::endl;
*/
     std::cout<<std::endl;
     std::cout<<"nTGC(Z)="<<_nTGC[Z_BOSON]<<std::endl;
     std::cout<<"nFSR(Z)="<<_nFSR[Z_BOSON][ANY_LEPTON]<<", e="<<_nFSR[Z_BOSON][E_LEPTON]<<", mu="<<_nFSR[Z_BOSON][MU_LEPTON]<<", tau="<<_nFSR[Z_BOSON][TAU_LEPTON]<<std::endl;

//     std::cout<<"nISR(Z)="<<_nISR[Z_BOSON][ANY_QUARK]<<", d="<<_nISR[Z_BOSON][D_QUARK]<<", u="<<_nISR[Z_BOSON][U_QUARK]<<", s="<<_nISR[Z_BOSON][S_QUARK]<<", c="<<_nISR[Z_BOSON][C_QUARK]<<", b="<<_nISR[Z_BOSON][B_QUARK]<<", t="<<_nISR[Z_BOSON][T_QUARK]<<", gluon="<<_nISR[Z_BOSON][GLUON]<<std::endl;
     std::cout<<"fractions:"<<std::endl;
     std::cout<<"nTGC(Z)="<<100*_nTGC[Z_BOSON]/_nPhotons<<"%"<<std::endl;
     std::cout<<"nFSR(Z)="<<100*_nFSR[Z_BOSON][ANY_LEPTON]/_nPhotons<<"%, e="<<100*_nFSR[Z_BOSON][E_LEPTON]/_nPhotons<<"%, mu="<<100*_nFSR[Z_BOSON][MU_LEPTON]/_nPhotons<<"%, tau="<<100*_nFSR[Z_BOSON][TAU_LEPTON]/_nPhotons<<"%"<<std::endl;
//     std::cout<<"nISR(Z)="<<100*_nISR[Z_BOSON][ANY_QUARK]/_nPhotons<<"%, d="<<100*_nISR[Z_BOSON][D_QUARK]/_nPhotons<<"%, u="<<100*_nISR[Z_BOSON][U_QUARK]/_nPhotons<<"%, s="<<100*_nISR[Z_BOSON][S_QUARK]/_nPhotons<<"%, c="<<100*_nISR[Z_BOSON][C_QUARK]/_nPhotons<<"%, b="<<100*_nISR[Z_BOSON][B_QUARK]/_nPhotons<<"%, t="<<100*_nISR[Z_BOSON][T_QUARK]/_nPhotons<<"%, gluon="<<100*_nISR[Z_BOSON][GLUON]/_nPhotons<<"%"<<std::endl;


}



