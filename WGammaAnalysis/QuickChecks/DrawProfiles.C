#include "TTree.h"
#include "TCut.h"
#include "TString.h"
#include "TCanvas.h"

void DrawProfile(TTree* trData, TTree* trSign, TCut cut, TString var1, TString var2)
{

  trData->SetLineColor(1);
  trData->SetMarkerColor(1);

  trSign->SetLineColor(2);
  trSign->SetMarkerColor(2);

  TString strDraw1=var1;
  strDraw1+=":";
  strDraw1+=var2;

  TString strDraw2=var2;
  strDraw2+=":";
  strDraw2+=var1;

  TString strCanv1Name="canv1_";
  strCanv1Name+=strDraw1;
  strCanv1Name+="_";
  strCanv1Name+=cut;

  TString strCanv2Name="canv2_";
  strCanv2Name+=strDraw2;
  strCanv2Name+="_";
  strCanv2Name+=cut;

  TString strCanv3Name=strCanv1Name;
  strCanv3Name+="_changeDrawOrder";

  TString strCanv4Name=strCanv2Name;
  strCanv4Name+="_changeDrawOrder";

  TCanvas* canv1 = new TCanvas(strCanv1Name,strCanv1Name);
  trData->Draw(strDraw1, cut, "profile");
  trSign->Draw(strDraw1, cut, "profile same");

  TCanvas* canv2 = new TCanvas(strCanv2Name,strCanv2Name);
  trData->Draw(strDraw2,cut,"profile");
  trSign->Draw(strDraw2,cut,"profile same");

  TCanvas* canv3 = new TCanvas(strCanv3Name,strCanv3Name);
  trSign->Draw(strDraw1, cut, "profile");
  trData->Draw(strDraw1, cut, "profile same");

  TCanvas* canv4 = new TCanvas(strCanv4Name,strCanv4Name);
  trSign->Draw(strDraw2,cut,"profile");
  trData->Draw(strDraw2,cut,"profile same");

}
