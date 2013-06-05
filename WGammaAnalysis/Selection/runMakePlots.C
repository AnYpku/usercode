{
    //gROOT->ProcessLine(".L makePresentationPlots.C+");
  gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");
  gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  gROOT->ProcessLine(".L ../Configuration/TAllInputSamples.C+");
  gROOT->ProcessLine(".L TSelectionPlots.C+");
  gStyle->SetCanvasColor(0);
    //makePresentationPlots();

  string confFile = "../Configuration/config.txt";
  string plotVar="WMt";
  int nBins=16;
  float binLimits[nBins+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160};
  TSelectionPlots plotter;
  plotter.GetTrees(TInputSample::MUON, confFile, "DATA Wg_to_munu Wjets_to_lnu DYjets_to_ll Zg Wg_to_taunu multibosons topNotJets");
  plotter.SetHistograms(plotVar,nBins,binLimits,"lePhoDeltaR>0.7");
  plotter.DrawSpectrumDataVsMC();


/*
  string confFile = "../Configuration/config.txt";
   int nBins=50;
   float binLimits[nBins+1];
   
   int nVars=10;
   string plotVars[nVars];
   float varMin[nVars];
   float varMax[nVars];
   
   plotVars[0]="leptonEta";    varMin[0]=-6;    varMax[0]=6;
   plotVars[1]="leptonPhi";    varMin[1]=-3.14; varMax[1]=3.14;
   plotVars[2]="leptonPt";     varMin[2]=0;     varMax[2]=150;
   plotVars[3]="phoEta";       varMin[3]=-6;    varMax[3]=6;
   plotVars[4]="phoPhi";       varMin[4]=-3.14; varMax[4]=3.14;
   plotVars[5]="phoEt";        varMin[5]=0;     varMax[5]=60;
   plotVars[6]="lePhoDeltaR";  varMin[6]=0;     varMax[6]=6;
   plotVars[7]="WMt";          varMin[7]=0;     varMax[7]=150;
   plotVars[8]="pfMETPhi";     varMin[8]=-3.14; varMax[8]=3.14;
   plotVars[9]="pfMET";        varMin[9]=0;     varMax[9]=150;

   for (int i=0; i<nVars; i++){
     TSelectionPlots plotter;
     plotter.GetTrees(TInputSample::MUON, confFile);
     for (int j=0; j<nBins+1; j++){
       binLimits[j]=varMin[i]+j*(varMax[i]-varMin[i])/nBins;
     }
     std::cout<<std::endl;
     std::cout<<"plotVars[i]="<<plotVars[i]<<std::endl;
     plotter.SetHistograms(plotVars[i], nBins, binLimits);
 
     TString varStr(plotVars[i].c_str());
     std::cout<<"varStr="<<varStr<<std::endl;
     TString nameForSave=varStr;
     nameForSave+=".png";
     TString nameCanvas="canv_";
     nameCanvas+=varStr;
     plotter.DrawSpectrumSigVsBkg(nameCanvas,nameForSave);    
   }
*/
}
