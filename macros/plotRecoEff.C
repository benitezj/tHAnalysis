#include "globals.h"

void plotRecoEff(){
  
  TFile FtH("/nfs/uiowapc/data/tHAnalysis/August18_TRUTH1/tH.root");
  
  TH1F*HRecoEffB3=(TH1F*)FtH.Get("effReco_SRB3");
  TH1F*HRecoEffB4=(TH1F*)FtH.Get("effReco_SRB4");

  TCanvas C("C","",700,600);

  // C.Clear();
  // HRecoEffB3->Draw("histtext");
  // C.Print("effReco_SRB3.png");

  // C.Clear();
  // HRecoEffB4->Draw("histtext");
  // C.Print("effReco_SRB4.png");

  
  TH1F HEffLabeled("HEffLabeled","",13,0.5,13.5);
  HEffLabeled.SetNdivisions(0);
  HEffLabeled.SetTickLength(0.00,"X");
  HEffLabeled.GetXaxis()->SetBinLabel(1,"");
  HEffLabeled.GetXaxis()->SetBinLabel(2,"light q (3 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(3,"H (3 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(4,"t (3 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(5,"t + H (3 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(6,"tHq (3 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(7,"");
  HEffLabeled.GetXaxis()->SetBinLabel(8,"light q (4 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(9,"H (4 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(10,"t (4 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(11,"t + H (4 tag)");
  HEffLabeled.GetXaxis()->SetBinLabel(12,"tHq (4 tag)");

  HEffLabeled.SetBinContent(1,0);
  HEffLabeled.SetBinContent(2,HRecoEffB3->GetBinContent(4)/HRecoEffB3->GetBinContent(1));
  HEffLabeled.SetBinContent(3,HRecoEffB3->GetBinContent(3)/HRecoEffB3->GetBinContent(1));
  HEffLabeled.SetBinContent(4,HRecoEffB3->GetBinContent(2)/HRecoEffB3->GetBinContent(1));
  HEffLabeled.SetBinContent(5,HEffLabeled.GetBinContent(3)*HEffLabeled.GetBinContent(4));
  HEffLabeled.SetBinContent(6,HEffLabeled.GetBinContent(2)*HEffLabeled.GetBinContent(3)*HEffLabeled.GetBinContent(4));
  HEffLabeled.SetBinContent(7,0);
  HEffLabeled.SetBinContent(8,HRecoEffB4->GetBinContent(4)/HRecoEffB4->GetBinContent(1));
  HEffLabeled.SetBinContent(9,HRecoEffB4->GetBinContent(3)/HRecoEffB4->GetBinContent(1));
  HEffLabeled.SetBinContent(10,HRecoEffB4->GetBinContent(2)/HRecoEffB4->GetBinContent(1));
  HEffLabeled.SetBinContent(11,HEffLabeled.GetBinContent(9)*HEffLabeled.GetBinContent(10));
  HEffLabeled.SetBinContent(12,HEffLabeled.GetBinContent(8)*HEffLabeled.GetBinContent(9)*HEffLabeled.GetBinContent(10));

  C.SetBottomMargin(0.3);
  C.Clear();
  HEffLabeled.SetStats(0);
  HEffLabeled.GetYaxis()->SetRangeUser(0,1);
  HEffLabeled.GetYaxis()->SetTitle("reco. efficiency");
  HEffLabeled.GetXaxis()->SetLabelSize(0.05);
  HEffLabeled.GetXaxis()->SetLabelOffset(0.01);
  HEffLabeled.SetFillColor(kGray);
  HEffLabeled.SetLineColor(1);
  HEffLabeled.SetBarWidth(0.5);
  HEffLabeled.SetBarOffset(0.5);
  gStyle->SetPaintTextFormat("1.2f");
  HEffLabeled.Draw("histbtext0");
  C.Print("effReco.png");

  gROOT->ProcessLine(".q");
}
