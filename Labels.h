#include "TLatex.h"
#include "TString.h"
#include "TLine.h"
#include "TPave.h"
#include "TMarker.h"
#include "Rtypes.h"
#include "TPad.h"

void ATLASLabel(Double_t x,Double_t y, const char* text,Color_t color, float size=0.06, double delx=0.) 
{
  TLatex l; //l.SetTextAlign(12); 
  l.SetTextSize(size); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);

  if (delx == 0.) delx = 0.55*696*gPad->GetWh()/(472*gPad->GetWw());

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.SetTextSize(size);
    p.DrawLatex(x+delx,y,text);
    //    p.DrawLatex(x,y,"#sqrt{s}=900GeV");
  }
}


void ATLASLabelOld(Double_t x,Double_t y,bool Preliminary,Color_t color) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");
  if (Preliminary) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+0.015,y,"Preliminary");
  }
}



void ATLASVersion(char* version,Double_t x,Double_t y,Color_t color) 
{

  if (version) {
    char versionString[100];
    sprintf(versionString,"Version %s",version);
    TLatex l;
    l.SetTextAlign(22); 
    l.SetTextSize(0.04); 
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);
    l.DrawLatex(x,y,versionString);
  }
}



void myText(Double_t x,Double_t y,Color_t color,char *text, float size=0.05) 
{
  TLatex l; //l.SetTextAlign(12); 
  l.SetTextSize(size); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}

void myText(Double_t x,Double_t y,Color_t color,TString text, float tsize=0.05) 
{
  //Double_t tsize=0.05;
  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
 
void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,Int_t lcolor,char *text, double tsize=0.07) 
{
  //Double_t tsize=0.07;

  TLatex l; l.SetTextAlign(12); //
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x+0.05,y,text);

  Double_t y1=y-0.1*tsize;
  Double_t y2=y+0.1*tsize;
  Double_t x2=x-0.15*tsize;
  Double_t x1=x2-boxsize;

  //printf("x1= %f x2= %f y1= %f y2= %f \n",x1,x2,y1,y2);

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetLineColor(lcolor);
  mbox->SetBorderSize(1);
  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(1001);
  mbox->Draw();

  //TLine mline;
  //mline.SetLineWidth(4);
  //mline.SetLineColor(1);
  //mline.SetLineStyle(1);
  //Double_t y_new=(y1+y2)/2.;
  //mline.DrawLineNDC(x1,y_new,x2,y_new);

}

void myLineText(Double_t x, Double_t y,Double_t size, Int_t style, Int_t mcolor,Int_t lcolor,char const *text, double tsize=0.07) 
{
  //Double_t tsize=0.07;

  TLatex l; l.SetTextAlign(12); //
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x+0.05,y,text);

  Double_t y1=y-0.005*tsize;
  Double_t y2=y+0.01*tsize;
  Double_t x2=x-0.01*tsize;
  Double_t x1=x2-size;

  //printf("x1= %f x2= %f y1= %f y2= %f \n",x1,x2,y1,y2);

  TLine mline;
  mline.SetLineWidth(4);
  mline.SetLineColor(mcolor);
  mline.SetLineStyle(style);
  Double_t y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y_new,x2,y_new);

}



void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle, const char *text, double tsize=0.04) 
{
  //  printf("**myMarker: text= %s\ m ",text);

  //Double_t tsize=0.08;
  TMarker *marker = new TMarker(x-(0.4*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(1.5);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); //
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x+0.05,y,text);
}

