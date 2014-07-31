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
       float MuIsolation2012(float muPt, 
                 float muPFIsoR04_NH, float muPFIsoR04_Pho, 
                 float muPFIsoR04_PU,float muPFIsoR04_CH);
        //mu Isolation as recommended here:
        //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Muon_Isolation
       float MuIsolation2011(float muPt, 
                 float muIsoTrk, float muIsoEcal, 
                 float muIsoHcal,float rho2011);
       bool MuId(int year, float muChi2NDF, float muD0, float muDZ, 
                int muNumberOfValidMuonHits, int muNumberOfValidTrkHits, 
                int muNumberOfValidPixelHits, int muNumberOfValidTrkLayers, 
                int muStations, int muType);
        //tight muon ID as recommended here:
        //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon
        //year: 2012 - 8 TeV ID, 2011 - repeat ID from 7 TeV

       TCut RangeId(int year);
       TCut RangeIsolation(int year);
       TCut RangeTriggerMatch();
       TCut RangeMuon(int year, bool doIsoCut=1, bool doIdCut=1, bool doTrgCut=1);

     private:

       const static float _muPtCut=26.;
       const static float _extraMuPtCut=10.;
       const static float _muEtaCut=2.1;
       const static float _extraMuEtaCut=2.4;
       const static float _D0Cut2012=0.20;//it is 2 mm, impact parameter
       const static float _DZCut2012=0.50;//it is 5 mm, longitudinal Distance
       const static float _D0Cut2011=0.02;//it is 0.2 mm, impact parameter
       const static float _DZCut2011=0.10;//it is 1 mm, longitudinal Distance
       const static float _muChi2NDFCut=10;
       const static int _muStationsCut=1;
       const static int _muNOfValidTrkHitsCut = 10;
       const static int _muNOfValidPixelHitsCut=0;
       const static int _muNOfValidTrkLayersCut=5;
       const static int _muNOfValidMuonHitsCut=0;
       const static float _isoTightCut2012=0.12;
       const static float _isoLooseCut2012=0.20;
       const static float _isoCut2011=0.10;
       const static int _muType2012=46;
       
  };

#endif //#ifndef TMuonCuts_h
