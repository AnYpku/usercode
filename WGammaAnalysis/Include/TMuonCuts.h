#ifndef TMuonCuts_h
#define TMuonCuts_h

#include <vector>
#include "TEventTree.h" //class of this package
#include "TCut.h" //ROOT class

class TMuonCuts
  {
     public:
       TMuonCuts ();
       virtual ~TMuonCuts();
       bool HasMoreMuons(int nMu, int imu, vector <float> *muPt, vector <float> *muEta);
       bool PassedKinematics(float muPt, float muEta);
       float MuIsolation(float muPt, 
                 float muPFIsoR04_NH, float muPFIsoR04_Pho, 
                 float muPFIsoR04_PU,float muPFIsoR04_CH);
        //mu Isolation as recommended here:
        //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Muon_Isolation
       bool MuId(float muChi2NDF, float muD0, float muDZ, 
                int muNumberOfValidMuonHits, int muNumberOfValidTrkHits, 
                int muNumberOfValidPixelHits, int muNumberOfValidTrkLayers, 
                int muStations);
        //tight muon ID as recommended here:
        //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon

       TCut RangeId();
       TCut RangeIsolation();
       TCut RangeTriggerMatch();
       TCut RangeMuon(bool doIsoCut=1, bool doIdCut=1, bool doTrgCut=1);

     private:

       const static float muPtCut_=26.;
       const static float extraMuPtCut_=10.;
       const static float muEtaCut_=2.1;
       const static float extraMuEtaCut_=2.4;
       const static float impactParameterCut_=0.2;//it is 2 mm
       const static float longitudinalDistanceCut_=0.5;//it is 5 mm
       const static float muChi2NDFCut_=10;
       const static int muStationsCut_=1;
       const static int muNOfValidTrkHitsCut_ = 10;
       const static int muNOfValidPixelHitsCut_=0;
       const static int muNOfValidTrkLayersCut_=5;
       const static int muNOfValidMuonHitsCut_=0;
       const static float isoTightCut_=0.12;
       const static float isoLooseCut_=0.20;
       
  };

#endif //#ifndef TMuonCuts_h
