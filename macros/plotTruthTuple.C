#include "globals.h"

void plotTruthTuple(){

  //set style
  SetAtlasStyle();
  gStyle->SetPalette(1,0);
  LUMI=0;
  setGlobals();
  
  //get tuple
  TFile F("/nfs/uiowapc/data/tHAnalysis/August18_TRUTH1/tH.root","read");
  TTree * t = (TTree*)F.Get("ntuple");
  if(!t) return;
  gROOT->cd();

  TString cut="(truth_jfwd_pt>0)";//removes events not passing preselection

  TString outpath="./TruthPlots";
  system("rm -f ./TruthPlots/*");
  system("mkdir ./TruthPlots");


  ///b-jet from gluon
  TH1F truth_fwdb_eta("truth_fwdb_eta","",60,0,6);
  truth_fwdb_eta.GetXaxis()->SetTitle(" most forward b-quark |#eta| ");
  truth_fwdb_eta.GetYaxis()->SetTitle(" events ");
  t->Draw("abs(truth_Fwdb_eta)>>truth_fwdb_eta",cut+"*(truth_Fwdb_pt>25000)");
  C.Clear();
  truth_fwdb_eta.GetYaxis()->SetRangeUser(0,truth_fwdb_eta.GetBinContent(truth_fwdb_eta.GetMaximumBin())*1.4);
  truth_fwdb_eta.Draw("hist");
  DrawECMS();
  ATLASLabel(0.18,0.88,"Simulation internal",1);
  C.Print(outpath+"/truth_fwdb_eta.png");
  C.Print(outpath+"/truth_fwdb_eta.eps");
  cout<<"Fraction of fwd b-quark  0<|eta|<2.4 ="<<truth_fwdb_eta.Integral(1,23)/truth_fwdb_eta.Integral()<<endl;
  cout<<"Fraction of fwd b-quark  2.4<|eta|<3.8 ="<<truth_fwdb_eta.Integral(24,38)/truth_fwdb_eta.Integral()<<endl;

  
  TH1F truth_fwdb_pt("truth_fwdb_pt","",30,0,300);
  truth_fwdb_pt.GetXaxis()->SetTitle(" most forward b-quark p_{T}  [GeV]");
  truth_fwdb_pt.GetYaxis()->SetTitle(" events ");
  t->Draw("truth_Fwdb_pt/1000>>truth_fwdb_pt",cut);
  C.Clear();
  truth_fwdb_pt.GetYaxis()->SetRangeUser(0,truth_fwdb_pt.GetBinContent(truth_fwdb_pt.GetMaximumBin())*1.4);
  truth_fwdb_pt.Draw("hist");
  DrawECMS();
  ATLASLabel(0.18,0.88,"Simulation internal",1);
  C.Print(outpath+"/truth_fwdb_pt.png");
  C.Print(outpath+"/truth_fwdb_pt.eps");


  ///fwd light quark
  TH1F truth_jfwd_eta("truth_jfwd_eta","",60,0,6);
  truth_jfwd_eta.GetXaxis()->SetTitle(" light quark |#eta| ");
  truth_jfwd_eta.GetYaxis()->SetTitle(" events ");
  t->Draw("abs(truth_jfwd_eta)>>truth_jfwd_eta",cut+"*(truth_jfwd_pt>25000)");
  C.Clear();
  truth_jfwd_eta.GetYaxis()->SetRangeUser(0,truth_jfwd_eta.GetBinContent(truth_jfwd_eta.GetMaximumBin())*1.4);
  truth_jfwd_eta.Draw("hist");
  DrawECMS();
  ATLASLabel(0.18,0.88,"Simulation internal",1);
  C.Print(outpath+"/truth_jfwd_eta.png");
  C.Print(outpath+"/truth_jfwd_eta.eps");
  cout<<"Fraction of light quark 0<|eta|<2.4 ="<<truth_jfwd_eta.Integral(1,23)/truth_jfwd_eta.Integral()<<endl;
  cout<<"Fraction of light quark 2.4<|eta|<3.8 ="<<truth_jfwd_eta.Integral(24,38)/truth_jfwd_eta.Integral()<<endl;

  
  TH1F truth_jfwd_pt("truth_jfwd_pt","",30,0,300);
  truth_jfwd_pt.GetXaxis()->SetTitle(" light quark p_{T}  [GeV]");
  truth_jfwd_pt.GetYaxis()->SetTitle(" events ");
  t->Draw("truth_jfwd_pt/1000>>truth_jfwd_pt",cut);
  C.Clear();
  truth_jfwd_pt.GetYaxis()->SetRangeUser(0, truth_jfwd_pt.GetBinContent(truth_jfwd_pt.GetMaximumBin())*1.4);
  truth_jfwd_pt.Draw("hist");
  DrawECMS();
  ATLASLabel(0.18,0.88,"Simulation internal",1);
  C.Print(outpath+"/truth_jfwd_pt.png");
  C.Print(outpath+"/truth_jfwd_pt.eps");



  ///sum of all 4 b's
  TH1F truth_allb_eta("truth_allb_eta","",60,0,6);
  truth_allb_eta.GetXaxis()->SetTitle(" b-quark |#eta| ");
  truth_allb_eta.GetYaxis()->SetTitle(" # b-quarks / event");
  //truth_allb_eta.GetYaxis()->SetTitle(" # of truth b-quarks ");
  t->Draw("abs(truth_b_eta)>>truth_allb_eta",cut+"*(truth_b_pt>25000)");
  t->Draw("abs(truth_Topb_eta)>>+truth_allb_eta",cut+"*(truth_Topb_pt>25000)");
  t->Draw("abs(truth_Hb1_eta)>>+truth_allb_eta",cut+"*(truth_Hb1_pt>25000)");
  t->Draw("abs(truth_Hb2_eta)>>+truth_allb_eta",cut+"*(truth_Hb2_pt>25000)");
  truth_allb_eta.Scale(1./t->GetEntries());

  TH1F truth_b_eta("truth_b_eta","",60,0,6);
  t->Draw("abs(truth_b_eta)>>truth_b_eta",cut+"*(truth_b_pt>25000)");
  truth_b_eta.Scale(1./t->GetEntries());

  TH1F truth_Topb_eta("truth_Topb_eta","",60,0,6);
  t->Draw("abs(truth_Topb_eta)>>truth_Topb_eta",cut+"*(truth_Topb_pt>25000)");
  truth_Topb_eta.Scale(1./t->GetEntries());

  TH1F truth_Hb1_eta("truth_Hb1_eta","",60,0,6);
  t->Draw("abs(truth_Hb1_eta)>>truth_Hb1_eta",cut+"*(truth_Hb1_pt>25000)");
  truth_Hb1_eta.Scale(1./t->GetEntries());

  TH1F truth_Hb2_eta("truth_Hb2_eta","",60,0,6);
  t->Draw("abs(truth_Hb2_eta)>>truth_Hb2_eta",cut+"*(truth_Hb2_pt>25000)");
  truth_Hb2_eta.Scale(1./t->GetEntries());

  truth_b_eta.SetFillColor(2);
  truth_Topb_eta.SetFillColor(3);
  truth_Hb1_eta.SetFillColor(4);
  truth_Hb2_eta.SetFillColor(5);

  TLegend bleg(0.65,0.65,0.95,0.9);
  bleg.SetBorderSize(0);
  bleg.SetLineColor(0);
  bleg.SetFillStyle(0);
  bleg.AddEntry(&truth_b_eta,"initial b-quark","f");
  bleg.AddEntry(&truth_Topb_eta,"top b-quark","f");
  bleg.AddEntry(&truth_Hb1_eta,"Higgs leading b-quark","f");
  bleg.AddEntry(&truth_Hb2_eta,"Higgs sub-leading b-quark","f");

  THStack bstack;
  bstack.Add(&truth_b_eta,"f");
  bstack.Add(&truth_Topb_eta,"f");
  bstack.Add(&truth_Hb1_eta,"f");
  bstack.Add(&truth_Hb2_eta,"f");

  C.Clear();
  truth_allb_eta.GetYaxis()->SetRangeUser(0,truth_allb_eta.GetBinContent(truth_allb_eta.GetMaximumBin())*1.4);
  truth_allb_eta.GetYaxis()->SetLabelSize(0.035);
  truth_allb_eta.Draw("histp");
  bstack.Draw("histsame");
  truth_allb_eta.Draw("histpsame");
  DrawECMS();
  ATLASLabel(0.18,0.88,"Simulation internal",1);
  bleg.Draw();
  C.Print(outpath+"/truth_allb_eta.png");
  C.Print(outpath+"/truth_allb_eta.eps");
  //cout<<"fraction b-quarks 0<|eta|<2.4 ="<<truth_allb_eta.Integral(1,23)/truth_allb_eta.Integral()<<endl;
  //cout<<"fraction b-quark  2.4<|eta|<3.8 ="<<truth_allb_eta.Integral(24,38)/truth_allb_eta.Integral()<<endl;

  cout<<"# of b-quarks / event in 0<|eta|<2.4 ="<<truth_allb_eta.Integral(1,23)<<endl;
  cout<<"# of b-quarks / event in 2.4<|eta|<3.8 ="<<truth_allb_eta.Integral(24,38)<<endl;


  gROOT->ProcessLine(".q");

}

