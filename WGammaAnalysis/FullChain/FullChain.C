#include "FullChain.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include "../Selection/Selection.h"
#include "../DDBkgTemplateMethod/TTemplates.h"
#include "../DDBkgTemplateMethod/AuxTemplates.C"
#include "../PrepareYields/TPrepareYields.h"
#include "../PrepareYields/AuxPrepareYields.C"
#include "../AcceptanceAndEfficiency/CalcAccAndEff.h"
#include "../CrossSection/CalcCrossSection.h"

#include <iostream>

FullChain::FullChain()
{
//  _INPUT=new TAllInputSamples(_channel,"../Configuration/config.txt");
//  _phoPtBinLimits = new float[_config.GetNPhoPtBins()];
//  _config.GetPhoPtBinsLimits(_phoPtBinLimits);
}

FullChain::~FullChain()
{
//  delete _INPUT;
//  delete _phoPtBinLimits;
}

void FullChain::SetDefaultFullChainParameters(TConfiguration::AnalysisParameters& anPars, TString varKin)
{
  anPars.year=2012;//2012, 2011
  anPars.channel=TConfiguration::MUON;//MUON, ELECTRON
  anPars.vgamma=TConfiguration::W_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.templFits=TConfiguration::TEMPL_CHISO;

  anPars.sampleMode=Selection::ALL;
  anPars.analyzedSamples="";
  anPars.configfile="../Configuration/config.txt";
  anPars.isNoPuReweight=0;
  anPars.isDebugMode=0;
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;//WP_LOOSE,WP_MEDIUM,WP_TIGHT
  anPars.cutAdd="1";

  if (varKin=="phoEt")
    SetAnalysisKinParameters(anPars);
  else
    SetDiffKinFullChainParameters(anPars,varKin);

}

void FullChain::SetAnalysisKinParameters(TConfiguration::AnalysisParameters& anPars)
{
    anPars.varKin="phoEt";
//    anPars.nKinBins=26;
//    anPars.nKinBins=2;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    for (int ib=0; ib<anPars.nKinBins+1; ib++)
//      anPars.kinBinLims[ib]=15+2.5*ib;
//    anPars.nKinBins=3;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=15;
//    anPars.kinBinLims[1]=20;
//    anPars.kinBinLims[2]=25;
//    anPars.kinBinLims[3]=30;


  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);

}

void FullChain::SetDiffKinFullChainParameters(TConfiguration::AnalysisParameters& anPars, TString varKin)
{
  if (varKin=="phoEtNoPhosphor"){
    anPars.varKin="phoEtNoPhosphor";

  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);

//    anPars.nKinBins=26;
//    anPars.nKinBins=2;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    for (int ib=0; ib<anPars.nKinBins+1; ib++)
//      anPars.kinBinLims[ib]=15+2.5*ib;

  }

  if (varKin=="phoSCEta" || varKin=="phoEta"){
    anPars.varKin=varKin;

//    anPars.nKinBins=3;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=-2.5;
//    anPars.kinBinLims[1]=-1.5;
//    anPars.kinBinLims[2]=1.5;
//    anPars.kinBinLims[3]=2.5;


//    anPars.nKinBins=4;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=-2.5;
//    anPars.kinBinLims[1]=-1.5;
//    anPars.kinBinLims[2]=0;
//    anPars.kinBinLims[3]=1.5;
//    anPars.kinBinLims[4]=2.5;

    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.5;
    anPars.kinBinLims[1]=-2.366;
    anPars.kinBinLims[2]=-2.166;
    anPars.kinBinLims[3]=-1.966;
    anPars.kinBinLims[4]=-1.766;
    anPars.kinBinLims[5]=-1.566;
    anPars.kinBinLims[6]=-1.4442;
    anPars.kinBinLims[7]=-1.08315;
    anPars.kinBinLims[8]=-0.7221;
    anPars.kinBinLims[9]=-0.36105;
    anPars.kinBinLims[10]=0;
    anPars.kinBinLims[11]=0.36105;
    anPars.kinBinLims[12]=0.7221;
    anPars.kinBinLims[13]=1.08315;
    anPars.kinBinLims[14]=1.4442;
    anPars.kinBinLims[15]=1.566;
    anPars.kinBinLims[16]=1.766;
    anPars.kinBinLims[17]=1.966;
    anPars.kinBinLims[18]=2.166;
    anPars.kinBinLims[19]=2.366;
    anPars.kinBinLims[20]=2.5;
  }
  if (varKin=="phoPhi"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
  }
  if (varKin=="lepton1Pt" || varKin=="lepton2Pt"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=26+10*ib;
  }
  if (varKin=="lepton1Phi" || varKin=="lepton2Phi"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
  }
  if (varKin=="lepton1Eta" || varKin=="lepton2Eta"){
    anPars.varKin=varKin;
    anPars.nKinBins=16;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.1;
    anPars.kinBinLims[1]=-1.966;
    anPars.kinBinLims[2]=-1.766;
    anPars.kinBinLims[3]=-1.566;
    anPars.kinBinLims[4]=-1.4442;
    anPars.kinBinLims[5]=-1.08315;
    anPars.kinBinLims[6]=-0.7221;
    anPars.kinBinLims[7]=-0.36105;
    anPars.kinBinLims[8]=0;
    anPars.kinBinLims[9]=0.36105;
    anPars.kinBinLims[10]=0.7221;
    anPars.kinBinLims[11]=1.08315;
    anPars.kinBinLims[12]=1.4442;
    anPars.kinBinLims[13]=1.566;
    anPars.kinBinLims[14]=1.766;
    anPars.kinBinLims[15]=1.966;
    anPars.kinBinLims[16]=2.1;
  }
  if (varKin=="WMt"){
    anPars.varKin=varKin;
    anPars.nKinBins=20;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+5*ib;
  }

  if (varKin=="Mleplep"){
    anPars.varKin=varKin;
    anPars.nKinBins=30;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+2*ib;
  }

  if (varKin=="Mpholeplep"){
    anPars.varKin=varKin;
    anPars.nKinBins=11;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=80+2*ib;
  }

  if (varKin=="Mpholep1" || varKin=="Mpholep2"){
    anPars.varKin=varKin;
    anPars.nKinBins=30;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=30+5*ib;
  }

  if (varKin=="lep1PhoDeltaR" || varKin=="lep2PhoDeltaR"){
    anPars.varKin=varKin;
    anPars.nKinBins=55;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0.0+0.1*ib;
  }
  if (varKin=="pfMET"){
    anPars.varKin="pfMET";
    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+5*ib;
  }
  if (varKin=="pfMETPhi"){
    anPars.varKin="pfMETPhi";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
 }
}

void FullChain::RunAnalysis(TConfiguration::AnalysisParameters &anPars)
{

  TConfiguration conf;  

  //very preliminary selection
  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      if (anPars.noPreSelection[ich][ivg]) continue;
      TString strAffix=TString("Preliminary Selection ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg);
      std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
      std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
      anPars.channel=ich;
      anPars.vgamma=ivg;
      Selection selection(anPars);
      selection.LoopOverInputFiles();
      std::cout<<"%_%  DONE "<<strAffix<<std::endl;
      std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
    }
  }

  //extra selection
  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      if (anPars.noExtraSelection[ich][ivg]) continue;
      TString strAffix=TString("Extra Selection ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg);
      std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
      std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
      anPars.channel=ich;
      anPars.vgamma=ivg;
      Selection selection;
      selection.ExtraSelection(anPars);
      std::cout<<"%_%  DONE "<<strAffix<<std::endl;
      std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
    }
  }


  //compute fake-gamma background by data-driven template method
  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      for (int itp=0; itp<=1; itp++){
        if (anPars.noDDBkgComputation[ich][ivg][itp]) continue;
        TString strAffix=TString("DataDriven jets->gamma bkg fits ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg)+TString(" ")+conf.StrTempl(itp);
        std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
        std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
        anPars.channel=ich;
        anPars.vgamma=ivg;
        anPars.templFits=itp;
        AuxTemplates(anPars,0);// 0 - no isMCclosure
        std::cout<<"%_%  DONE "<<strAffix<<std::endl;
        std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
      }//end of loop over itp
    }//end of loop over ivg
  }//end of loop over ich

  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      for (int itp=0; itp<=1; itp++){
        if (anPars.noSystDDBkgSidebandVariation[ich][ivg][itp]) continue;
        TString strAffix=TString("Syst DDBkg Sideband Variation ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg)+TString(" ")+conf.StrTempl(itp);
        std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
        std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
        anPars.channel=ich;
        anPars.vgamma=ivg;
        anPars.templFits=itp;
        AuxTemplatesSystSidebandVariation(anPars,0);// 0 - no isMCclosure
       std::cout<<"%_%  DONE "<<strAffix<<std::endl;
       std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;   
      }//end of loop over itp
    }//end of loop over ivg
  }//end of loop over ich    

  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
        if (anPars.noPrepareYields[ich][ivg]) continue;
        TString strAffix=TString("Prepare Yields ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg);
        std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
        std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
        anPars.channel=ich;
        anPars.vgamma=ivg;
        AuxPrepareYields(anPars); 
       std::cout<<"%_%  DONE "<<strAffix<<std::endl;
       std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;   
    }//end of loop over ivg
  }//end of loop over ich   

  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      for (int itp=0; itp<=2; itp++){
        if (anPars.noSubtractBackground[ich][ivg][itp]) continue;
        TString strAffix=TString("Subtract Background ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg)+TString(" ")+conf.StrTempl(itp);
        std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
        std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
        anPars.channel=ich;
        anPars.vgamma=ivg;
        anPars.templFits=itp;
        AuxSubtractBackground(anPars); 
       std::cout<<"%_%  DONE "<<strAffix<<std::endl;
       std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;   
      }//end of loop over itp
    }//end of loop over ivg
  }//end of loop over ich  


  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      if (anPars.noCalcAccAndEff[ich][ivg]) continue;
        TString strAffix=TString("Compute Acceptance and Efficiency ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg);
      std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
      std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
      anPars.channel=ich;
      anPars.vgamma=ivg;
      CalcAccAndEff accAndEff(anPars.channel, anPars.vgamma, anPars.isDebugMode, anPars.configfile);
      accAndEff.ComputeAccTimesEff();
      std::cout<<"%_%  DONE "<<strAffix<<std::endl;
      std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
    }// end of loop over ivg
  }// end of loop over ich

  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      if (anPars.noCalcCrossSection[ich][ivg]) continue;
        TString strAffix=TString("Compute Cross Section ")+conf.StrChannel(ich)+TString(" ")+conf.StrVgType(ivg);
      std::cout<<"%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%^%"<<std::endl;
      std::cout<<"%^%  WILL DO "<<strAffix<<std::endl;
      anPars.channel=ich;
      anPars.vgamma=ivg;
      CalcCrossSection cs(anPars.channel, anPars.vgamma, anPars.blind[ich][ivg], anPars.configfile);
      cs.Calc();
      std::cout<<"%_%  DONE "<<strAffix<<std::endl;
      std::cout<<"%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%_%"<<std::endl;
    }// end of loop over ivg
  }// end of loop over ich

}

