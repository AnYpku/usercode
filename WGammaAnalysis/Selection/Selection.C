#include "Selection.h" 
  //this class
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
//#include "../Include/PhosphorCorrectorFunctor.hh"
#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
  //taken from https://github.com/cmorgoth/PHOSPHOR_CORRECTION
  //currently in this package
#include "TMath.h"
#include "TRandom.h"  
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

Selection::Selection()
{
}

Selection::Selection(int channel, int vgamma, int sampleMode, int blind, string configFile, bool isNoPuReweight, bool isDebugMode)
{

  _INPUT = new TAllInputSamples(channel, vgamma,configFile);
  _vgamma=vgamma;
  _channel=channel;
  _blind=blind; 
   std::cout<<"Selection constructor: channel="<<_config.StrChannel(channel)<<", vgamma="<<_config.StrVgType(vgamma)<<", blind="<<_config.StrBlindType(blind)<<", sampleMode="<<StrSampleMode(sampleMode)<<std::endl;
  _isNoPuReweight=isNoPuReweight;
  _isDebugMode=isDebugMode;
  _photonCorrector = new zgamma::PhosphorCorrectionFunctor(_config.GetPhosphorConstantFileName(),1);
    //field of this class, Selection
    //in PhosphorCorrectionFunctor class: PhosphorCorrectionFunctor(const char* filename, bool R9Cat);//Bool just used to overload constructor and allow R9 categories inplementation

  _sampleMode=sampleMode;
  if (_sampleMode==NOTSPECIFIED){
    std::cout<<"ERROR in Selection::Selection: _sampleMode is NOTSPECIFIED"<<std::endl;
    return;
  }
  else if (_sampleMode>NOTSPECIFIED){
    std::cout<<"ERROR in Selection::Selection: wrong _sampleMode "<<std::endl;
    return;    
  }
  for (int i=0; i<_INPUT->nSources_; i++)
    {
      _doAnalizeSample.push_back(1);      
    } 
}

Selection::Selection(int channel, int vgamma, string analyzedSampleNames, int blind, string configFile, bool isNoPuReweight, bool isDebugMode)
{
  _INPUT = new TAllInputSamples(channel, vgamma, configFile);
  _channel=channel;
  _vgamma=vgamma;
  _blind=blind;
   std::cout<<"Selection constructor: channel="<<_config.StrChannel(channel)<<", vgamma="<<_config.StrVgType(vgamma)<<", blind="<<_config.StrBlindType(blind)<<std::endl;
  _isNoPuReweight=isNoPuReweight;
  _isDebugMode=isDebugMode;
  _photonCorrector = new zgamma::PhosphorCorrectionFunctor(_config.GetPhosphorConstantFileName());
    //field of this class, Selection
  _sampleMode=NOTSPECIFIED;
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
    std::cout<<"ERROR in Selection::Selection: wrong _doAnalizeSample.size()"<<std::endl;
}

Selection::~Selection()
{
   _eventTree.fChain = 0;
     //field of TEventTree 
   //delete _photonCorrector;
}

TString Selection::StrSampleMode(int sampleMode)
{
  if (sampleMode==DATA) return "DATA";
  if (sampleMode==SIGMC) return "SIGMC";
  if (sampleMode==BKGMC) return "BKGMC";
  if (sampleMode==MC) return "MC";
  if (sampleMode==NOBKG) return "NOBKG";
  if (sampleMode==ALL) return "ALL";
  if (sampleMode==NOTSPECIFIED) return "NOTSPECIFIED";
  return "sampleMode="+TString(sampleMode)+"is unknown";
}

void Selection::LoopOverInputFiles()
{
  int nSources = _INPUT->nSources_; 
  for (int iSource=0; iSource<nSources; iSource++)
    {
       //sources are data, sigmc, DYjets, multibosons etc.
       //each source may consist of several input files
       if (_INPUT->allInputs_[iSource].sourceName_=="DYjets_to_ll" ||
              _INPUT->allInputs_[iSource].sourceName_=="Wjets_to_lnu")
         _isVJets=1;
       else _isVJets=0;
       if (!_doAnalizeSample[iSource])
         continue;
       _sample = _INPUT->allInputs_[iSource].sample_;
       if (_sample==_config.DATA){
           _lumiData=_INPUT->allInputs_[iSource].lumiTotal_;
           if (_sampleMode==ALL || _sampleMode==DATA || _sampleMode==NOBKG || _sampleMode==NOTSPECIFIED);
           else continue;
       }
       if (_sample==_config.SIGMC){
           if (_sampleMode==ALL || _sampleMode==SIGMC || _sampleMode==MC || _sampleMode==NOBKG || _sampleMode==NOTSPECIFIED);
           else continue;
       }
       if (_sample==_config.BKGMC){
           if (_sampleMode==ALL || _sampleMode==BKGMC || _sampleMode==MC || _sampleMode==NOTSPECIFIED);
           else continue;
       }
       std::cout<<"_channel="<<_channel<<"("<<_config.StrChannel(_channel)<<")";
       std::cout<<", _vgamma="<<_vgamma<<"("<<_config.StrVgType(_vgamma)<<")"<<std::endl;
       std::cout<<"_sample="<<_sample<<"("<<_config.StrSample(_sample)<<")";
       std::cout<<", _sampleMode="<<_sampleMode<<"("<<StrSampleMode(_sampleMode)<<")"<<std::endl;
       std::cout<<"sourceName="<<_INPUT->allInputs_[iSource].sourceName_<<", _isVJets="<<_isVJets<<std::endl;
       TTree* tree;
       int inputFileNMax = _INPUT->allInputs_[iSource].nFiles_;
       TString fileOutName;
       if (_sample==TConfiguration::DATA) 
         fileOutName=_config.GetSelectedName(_config.VERY_PRELIMINARY,_channel,_vgamma,_blind,_INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_,_isDebugMode,_isNoPuReweight);
       else
         fileOutName=_config.GetSelectedName(_config.VERY_PRELIMINARY,_channel,_vgamma,_config.UNBLIND,_INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_,_isDebugMode,_isNoPuReweight);
       _fileOut = new TFile(fileOutName,"recreate");
       _outTree = new TTree("selectedEvents","selected Events");
       _selEvTree.SetAsOutputTree(_outTree);

       _passed.metFiltersPassed=0;
       _passed.triggerPassed=0;
       _passed.goodVertexPassed=0;
       _passed.leptonPtPassed=0;
       _passed.leptonEtaPassed=0;
       _passed.evAfterKinCuts=0;
       _passed.vgvjOverlapPassed=0;
       _passed.phoPtPassed=0;
       _passed.phoEtaPassed=0;
       _passed.dRPassed=0;

       for (_inputFileN=0; _inputFileN< inputFileNMax; _inputFileN++){

         TFile f(_INPUT->allInputs_[iSource].fileNames_[_inputFileN]);
         if (f.IsOpen()) 
           std::cout<<std::endl<<"processing file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<std::endl;
         else{
            std::cout<<"ERROR detected in Selection::LoopOverInputFiles: file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<" was not found"<<std::endl;
            return;
         } 
         f.cd("ggNtuplizer");
         tree = (TTree*)gDirectory->Get("EventTree");
         if (tree){
           _eventTree.Init(tree);
           //if (_sample!=_config.DATA) SetMCSpecificAddresses();
         }   
         else{
           std::cout<<"ERROR detected in Selection::LoopOverInputFiles: tree in the file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<" does not exist"<<std::endl;
           return;
         }  
         //determine _lumiWeight
         if (_sample==_config.DATA) _lumiWeight=1;
         else _lumiWeight=_INPUT->allInputs_[iSource].lumiWeights_[_inputFileN];
         std::cout<<"_lumiWeight="<<_lumiWeight<<std::endl;
         _puWeight=new TPuReweight(_config.GetPileupDataFileName(),_INPUT->allInputs_[iSource].fileNames_[_inputFileN]);      

         LoopOverTreeEvents();//method of this class (Selection)

         _eventTree.fChain=0;//field of TEventTree
         delete _puWeight;
       }//loop over _inputFileN ends
       std::cout<<"the output will be saved to "<<_fileOut->GetName()<<std::endl<<std::endl;
      _fileOut->cd();
      _outTree->Write();
      _fileOut->Close();
      //delete outTree;
      _outTree = 0;
    } //loop over iSource ends
}


void Selection::LoopOverTreeEvents()
{

   if (_eventTree.fChain == 0) return;
   Long64_t nentries = _eventTree.fChain->GetEntries();
   if (_isDebugMode) 
     {
       if (_eventTree.fChain->GetEntries()<_debugModeNEntries) nentries=_eventTree.fChain->GetEntries();
       else nentries=_debugModeNEntries;
     }

   int nTotal=0;
   int nPassed=0;
   int nBlind=0;
   float nTotalW=0;//weighted
   float nPassedW=0;//weighted
   float nBlindW=0;

   //goodLeptonPhotonPairs(two-dimentional array of bool-s)
   //memory allocation for some variables: 
   int nLeptonMax;
   if (_channel==_config.MUON) nLeptonMax=_eventTree.kMaxnMu;
   else if (_channel==_config.ELECTRON) nLeptonMax=_eventTree.kMaxnEle;
     //kMaxnMu - field of TEventTree
   else{
       std::cout<<"Error detected in Selection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
       return;
   }

   int nCands=_eventTree.kMaxnPho*nLeptonMax;
   TFullCuts::Candidate cands[nCands];

   CheckMaxNumbersInTree();

   TPhotonCuts emptyPhoton;
   std::cout<<"nentries="<<nentries<<std::endl;

   for (Long64_t ientry=0; ientry<nentries; ientry++){

   //loop over events in the tree
     _eventTree.GetEntryNeededBranchesOnly(_channel,_sample,ientry);

     if (_channel==_config.MUON) _nLe=_eventTree.treeLeaf.nMu;
     else if (_channel==_config.ELECTRON) _nLe=_eventTree.treeLeaf.nEle;
     else{
       std::cout<<"Error detected in  Selection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
       return;
     }
     _totalWeight = _lumiWeight;
     if (!_eventTree.treeLeaf.isData && !_isNoPuReweight)
       _totalWeight*=_puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
     nTotalW+=_totalWeight;
     nTotal+=1;

     TFullCuts fullCuts;
//     bool selPassed=1;
     bool selPassed = fullCuts.VeryPreliminaryCut(_eventTree.treeLeaf,_photonCorrector,
       _channel,_vgamma,_isVJets,nCands,cands,_passed);   
       
     
     if (!selPassed) continue;
       //added blinding prescaling into the beginning

     for (int icand=0; icand<nCands; icand++){

     if (_sample==_config.DATA && _blind==_config.BLIND_PRESCALE && (_eventTree.treeLeaf.event % _config.GetBlindPrescale() != 0)) continue;

     nBlind+=1;
     nBlindW+=_totalWeight;

       nPassedW+=_totalWeight;
       nPassed+=1;
       float puWeightVal=1;
       float puTrueVal=1;

       if (_sample!=_config.DATA){
         puWeightVal=_puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
         puTrueVal=_eventTree.treeLeaf.puTrue->at(1);
       }//end of if (_sample!=_config.DATA)     
       
       _selEvTree.SetValues(_channel, _sample, _eventTree.treeLeaf,  cands[icand],_inputFileN, 
               _totalWeight, puWeightVal, puTrueVal, _photonCorrector);
       _outTree->Fill();

     }//end of loop over icand

  } //end of loop over ientry in the tree

  std::cout<<"Summary:"<<std::endl;
  std::cout<<"nEntries="<<nentries<<", nTotal="<<nTotal;
  if (nTotal==0) return;
  std::cout<<", nPassed="<<nPassed<<", eff="<<(float)nPassed/nTotal<<", nBlind="<<nBlind<<std::endl;
  std::cout<<"Weighted:"<<"nTotal="<<nTotalW<<", nPassed="<<nPassedW<<", eff="<<(float)nPassedW/nTotalW<<", nBlind="<<nBlindW<<std::endl;
  std::cout<<", metFiltersPassed="<<_passed.metFiltersPassed<<", triggerPassed="<<_passed.triggerPassed<<", goodVertexPassed="<<_passed.goodVertexPassed<<", vgvjOverlapPassed="<<_passed.vgvjOverlapPassed<<", phoPt="<<_passed.phoPtPassed<<", phoEta="<<_passed.phoEtaPassed<<std::endl;
  std::cout<<"leptonPt="<<_passed.leptonPtPassed<<", leptonEta="<<_passed.leptonEtaPassed<<", dR="<<_passed.dRPassed<<std::endl;
  std::cout<<"nBlind="<<nBlind<<std::endl;

}

bool Selection::CheckMaxNumbersInTree()
{
   if ((_channel=_config.MUON) 
       && (_eventTree.fChain->GetMaximum("nMu")>_eventTree.kMaxnMu))
     //kMaxnMu - field of TEventTree
     {
       PrintErrorMessageMaxNumberOf(_MUON);
          //methof of this class (Selection)
       return 0;
     }
   if (_channel==_config.ELECTRON 
       && _eventTree.fChain->GetMaximum("nEle")>_eventTree.kMaxnEle)
     {
       PrintErrorMessageMaxNumberOf(_ELECTRON);
          //methof of this class (Selection)
       return 0;
     }
   if (_eventTree.fChain->GetMaximum("nPho")>_eventTree.kMaxnPho)
     {
       PrintErrorMessageMaxNumberOf(_PHOTON);
          //methof of this class (Selection)
       return 0;
     }
//   if (fChain->GetMaximum("nJet")>kMaxnJet)
//     {
//       PrintErrorMessageMaxNumberOf(_JET);
//          //methof of this class (Selection)
//       return 0;
//     }
//   if (fChain->GetMaximum("nLowPtJet")>kMaxnLowPtJet)
//     {
//       PrintErrorMessageMaxNumberOf(LOWPT_JET);
//          //methof of this class (Selection)
//       return 0;
//     }
//   if (!treeLeaf.isData && fChain->GetMaximum("nMC")>kMaxnMC)
//     {
//       PrintErrorMessageMaxNumberOf(_MC);
//          //methof of this class (Selection)
//       return 0;
//     }
   return 1;
}

void Selection::PrintErrorMessageMaxNumberOf(int particle)
{
       std::cout<<std::endl;
       std::cout<<"Error detected in Selection::PrintErrorMessageMaxNumberOf:"<<std::endl;
       if (particle==_MUON)
         std::cout<<"maximum number of muons in the file nMu="<<_eventTree.fChain->GetMaximum("nMu")<<", when kMaxnMu="<<_eventTree.kMaxnMu<<" only"<<std::endl;
       else if (particle==_ELECTRON)
         std::cout<<"maximum number of electrons in the file nEle="<<_eventTree.fChain->GetMaximum("nEle")<<", when kMaxnEle="<<_eventTree.kMaxnEle<<" only"<<std::endl;
       else if (particle==_PHOTON)
         std::cout<<"maximum number of photons in the file nPho="<<_eventTree.fChain->GetMaximum("nPho")<<", when kMaxnPho="<<_eventTree.kMaxnPho<<" only"<<std::endl;
//       else if (particle==_JET)
//         std::cout<<"maximum number of jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnJet="<<kMaxnJet<<" only"<<std::endl;
//      else if (particle==LOWPT_JET)
//         std::cout<<"maximum number of low Pt jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnLowPtJet="<<kMaxnLowPtJet<<" only"<<std::endl;
//      else if (particle==_MC)
//         std::cout<<"maximum number of mc particles in the file nMC="<<fChain->GetMaximum("nMC")<<", when kMaxnMC="<<kMaxnMC<<" only"<<std::endl;
       std::cout<<"please, go to TEventTree.h to increase this number up to proper value"<<std::endl;
       std::cout<<std::endl;
}

void Selection::ExtraSelection(int year, int channel, int vgamma, int sampleMode, int blind, int wp)
{
  TFullCuts fullCut;
  TConfiguration config;
  _blind=blind;
  TAllInputSamples INPUT(channel,vgamma,"../Configuration/config.txt");
  _tr=0;
  _trReduced=0;
  _tr1=0;
  _tr2=0;
  _tr3=0;

  for (int i=0; i<INPUT.nSources_; i++){

    int sample=INPUT.allInputs_[i].sample_;
    if(sampleMode==ALL);
    else if(sampleMode==DATA && sample!=config.DATA) continue;
    else if(sampleMode==SIGMC && sample!=config.SIGMC) continue;
    else if(sampleMode==BKGMC && sample!=config.BKGMC) continue;
    else if(sampleMode==MC && sample==config.DATA) continue;
    else if(sampleMode==NOBKG && sample==config.BKGMC) continue;

    
    std::cout<<"ExtraSelection: sample="<<_config.StrSample(sample)<<", _blind="<<_config.StrBlindType(_blind)<<std::endl;
    if (sample==config.DATA){
      ExtraSelectionOne(INPUT,i, config, fullCut,year,channel,vgamma,wp,_blind);
    }
    else{
      ExtraSelectionOne(INPUT,i,config,fullCut,year,channel,vgamma,wp,config.UNBLIND);
    }
  }//end of loop over i (for (int i=0; i<INPUT.nSources_; i++))

}

void Selection::ExtraSelectionOne(TAllInputSamples &INPUT, int iSource, TConfiguration& config, TFullCuts &fullCut, int year, int channel, int vgamma, int wp, int blind)
{

    std::cout<<"ExtraSelection; year="<<year<<", channel="<<_config.StrChannel(channel)<<", vgamma="<<_config.StrVgType(vgamma)<<", wp="<<wp<<",blind="<<_config.StrBlindType(blind)<<std::endl;

    TString fInName = config.GetSelectedName(config.VERY_PRELIMINARY,channel,vgamma,blind,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);

    TFile fIn(fInName);
    if (!fIn.IsOpen()){
      std::cout<<"file "<<fInName<<" can't be open"<<std::endl;
      return;
    }

    std::cout<<"file "<<fInName<<" was open"<<std::endl;
    _tr = (TTree*)fIn.Get("selectedEvents");

    if (INPUT.allInputs_[iSource].sample_==_config.SIGMC || 
        (INPUT.allInputs_[iSource].sample_==_config.DATA && blind==_config.UNBLIND)){

      TString fOutName2=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,blind,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut2(fOutName2,"recreate");
      std::cout<<"For TemplateMethodCut Selection cut:      "<<fullCut.RangeForTemplateMethodCut(year,channel,vgamma,wp).GetTitle()<<std::endl;
      _tr2 = new TTree("selectedEvents","selected events, one candidate per event");
       _trReduced = _tr->CopyTree(fullCut.RangeForTemplateMethodCut(year,channel,vgamma,wp));

      PickHardestPhotonInEvent(_tr2, _trReduced);

      _tr2->Write();
      std::cout<<"Will be saved to file:      "<<fOutName2<<std::endl;

    }

//    if (1){
      TString fOutName3=config.GetSelectedName(config.FULLY,channel,vgamma,blind,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut3(fOutName3,"recreate");
      _tr3 = new TTree("selectedEvents","selected events, one candidate per event");
      _trReduced = _tr->CopyTree(fullCut.RangeFullCut(year,channel,vgamma,wp));

      std::cout<<std::endl;
      std::cout<<"full Selection cut:      "<<fullCut.RangeFullCut(year,channel,vgamma,wp).GetTitle()<<std::endl;
      std::cout<<std::endl;
      
      PickHardestPhotonInEvent(_tr3, _trReduced);

      _tr3->Write();
      if (INPUT.allInputs_[iSource].sample_==_config.DATA && blind==_config.UNBLIND);
      else{
        std::cout<<"before hardest photon selection: "<<_trReduced->GetEntries()<<std::endl;
        std::cout<<"after  hardest photon selection: "<<_tr3->GetEntries()<<std::endl;
      }
//    }

}

void Selection::PickHardestPhotonInEvent(TTree* trToBeWritten, TTree* trReduced)
{
      TSelectedEventsTree selEvTree;
      selEvTree.SetAsInputTree(trReduced);
      selEvTree.SetAsOutputTree(trToBeWritten);
      long nEntries = trReduced->GetEntries();
      long eventCurr=0;
      long eventPrev=-1;
      for (long ientry=0; ientry<nEntries; ientry++){
        trReduced->GetEntry(ientry);
        eventPrev=eventCurr;
        eventCurr = selEvTree._event;
        if (eventCurr==eventPrev) continue;
        trToBeWritten->Fill();
      }

}
