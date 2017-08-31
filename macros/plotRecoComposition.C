#include "globals.h"

void plotRecoComposition(TString bkg="ttbar",TString jettype="alljet", TString region="noBcuts"){

  //set style
  SetAtlasStyle();
  gStyle->SetPalette(1,0);
  LUMI=0;
  setGlobals();
  
  //TString PRODUCION="August18_TRUTH1";
  TString PRODUCION="August28_TRUTH1_mu200_PU05";

  //get tuple
  TFile F(TString("/nfs/uiowapc/data/tHAnalysis/")+PRODUCION+"/"+bkg+".root","read");
  gROOT->cd();

  TString outpath="./RecoTruthComposition";
  outpath+="_"+bkg;
  system(TString("rm -rf ")+outpath);
  system(TString("mkdir ")+outpath);


  ///b-jet from gluon
  TH1F* Jet_eta = (TH1F*)F.Get(jettype+"truth_eta_"+region);
  TH1F* JetB_eta = (TH1F*)F.Get(jettype+"truthB_eta_"+region);
  TH1F* JetC_eta = (TH1F*)F.Get(jettype+"truthC_eta_"+region);
  TH1F* JetT_eta = (TH1F*)F.Get(jettype+"truthTau_eta_"+region);
  TH1F* JetL_eta = (TH1F*)F.Get(jettype+"truthL_eta_"+region);
  Jet_eta->GetXaxis()->SetTitle(" jet |#eta| ");
  Jet_eta->GetYaxis()->SetTitle(" # of jets ");

  JetB_eta->SetFillColor(4);
  JetC_eta->SetFillColor(3);
  JetT_eta->SetFillColor(2);
  JetL_eta->SetFillColor(6);


  TLegend leg(0.7,0.65,0.9,0.9);
  leg.SetBorderSize(0);
  leg.SetLineColor(0);
  leg.SetFillStyle(0);
  leg.AddEntry(JetL_eta,"light jet","f");
  leg.AddEntry(JetT_eta,"tau","f");
  leg.AddEntry(JetC_eta,"c-jet","f");
  leg.AddEntry(JetB_eta,"b-jet","f");

  THStack stack;
  stack.Add(JetL_eta,"f");
  stack.Add(JetT_eta,"f");
  stack.Add(JetC_eta,"f");
  stack.Add(JetB_eta,"f");

  C.Clear();
  Jet_eta->GetYaxis()->SetRangeUser(0,Jet_eta->GetBinContent(Jet_eta->GetMaximumBin())*1.4);
  Jet_eta->Draw("histp");
  stack.Draw("histsame");
  Jet_eta->Draw("histpsame");
  DrawECMS();
  ATLASLabel(0.18,0.88,"Simulation internal",1);
  leg.Draw();
  C.Print(outpath+"/"+bkg+"_"+jettype+"_"+region+"_eta.png");
  C.Print(outpath+"/"+bkg+"_"+jettype+"_"+region+"_eta.eps");

  cout<<"Global Fraction of b-jets  ="<<JetB_eta->Integral()/Jet_eta->Integral()<<endl;
  cout<<"Global Fraction of c-jets ="<<JetC_eta->Integral()/Jet_eta->Integral()<<endl;
  cout<<"Global Fraction of light  ="<<JetL_eta->Integral()/Jet_eta->Integral()<<endl;

  cout<<"Fraction of b-jets in 2.4<|eta|<3.8 ="<<JetB_eta->Integral(24,38)/Jet_eta->Integral(24,38)<<endl;
  cout<<"Fraction of c-jets in 2.4<|eta|<3.8 ="<<JetC_eta->Integral(24,38)/Jet_eta->Integral(24,38)<<endl;
  cout<<"Fraction of light jets in 2.4<|eta|<3.8 ="<<JetL_eta->Integral(24,38)/Jet_eta->Integral(24,38)<<endl;



  gROOT->ProcessLine(".q");

}

