#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TPad.h"
#include "TStyle.h"
#include "Labels.h"
#include <vector>
#include <utility>
#include <iostream>

using std::vector;
using std::pair;

struct Distribution {
  TString 	  histName;
  TString 	  xTitle;
  pair<float,float> xRange;
  pair<float,float> yRange;
  int		   rebinFactor;
  bool             logY;
};

struct Histogram {
  TH1F*   	  histo;
  Distribution    dist;
  int      	  color;
  int	   	  markerStyle;
  TString	  legendTitle;
  bool		   shapeOnly;
};

void ratioPlot(const TH1F* data, const TH1F* MC, TH1F *hRatio) {
  
  const int nbins   = MC->GetNbinsX();
    
  for (int i=1; i < nbins+1; ++i) {
    if ( MC->GetBinContent(i) == 0 ) continue;
    double value = data->GetBinContent(i)/MC->GetBinContent(i);
    double error = data->GetBinError(i)/MC->GetBinContent(i); 
    hRatio->SetBinContent(i,value);
    hRatio->SetBinError(i,error);
  } 

}


void makePlot(const vector<Histogram>& histograms, TString &cname) {
 
  TF1 *line = new TF1("line","1.",-1E10,1E10);
  line->SetLineStyle(3);
  line->SetLineWidth(1);
 
  gStyle->SetOptStat(0);
  
  TCanvas c;
  
  // Split the canvas
  TPad can_1("can_1", "can_1", 0.0, 0.4, 1.0, 1.0);
  can_1.SetTopMargin(0.04);
  can_1.SetBottomMargin(0.0);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.05);
  can_1.SetFrameBorderMode(0);
  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.36);
  can_2.SetTopMargin(0.05);
  can_2.SetBottomMargin(0.30);
  can_2.SetLeftMargin(0.1);
  can_2.SetRightMargin(0.05);
  can_2.SetFrameBorderMode(0);
 
  can_1.Draw();
  can_2.Draw();
  can_1.cd();
    
  int loop=0;
  for (auto H : histograms) {    
    TH1F *h = H.histo;
    
    h->Sumw2();
    
    // Main pad
    h->SetMarkerStyle(H.markerStyle);
    h->SetLineColor(H.color);
    h->SetMarkerColor(H.color);
    h->GetXaxis()->SetRangeUser(H.dist.xRange.first, H.dist.xRange.second);
    h->GetYaxis()->SetRangeUser(H.dist.yRange.first, H.dist.yRange.second);
    h->GetXaxis()->SetTitle(H.dist.xTitle);
    h->SetTitle("");
    
    float textsize = 18/(gPad->GetWh()*gPad->GetAbsHNDC());
    h->GetYaxis()->SetTitleSize(textsize);
    h->GetYaxis()->SetLabelSize(textsize);
    h->GetXaxis()->SetTitleSize(textsize);
    h->GetXaxis()->SetLabelSize(textsize);
    h->GetYaxis()->SetTitleOffset(0.8);
    
    if (H.dist.rebinFactor > 1) h->Rebin(H.dist.rebinFactor);

    if (H.shapeOnly) h->Scale(1./h->Integral());   
    if (loop == 0) h->Draw("PE1");
    else h->Draw("PE1same");
    
    if (H.dist.logY) can_1.SetLogy();
    
    // Ratio pad
    can_2.cd();
    if (loop > 0) {
      TH1F * ratio = (TH1F*)h->Clone();
      ratioPlot(h, histograms.at(0).histo, ratio);
            
      textsize = 18/(gPad->GetWh()*gPad->GetAbsHNDC());
      ratio->GetYaxis()->SetTitleSize(textsize);
      ratio->GetYaxis()->SetLabelSize(textsize);
      ratio->GetXaxis()->SetTitleSize(textsize);
      ratio->GetXaxis()->SetLabelSize(textsize);
      ratio->GetYaxis()->SetTitleOffset(0.5);
  
      ratio->GetYaxis()->SetNdivisions(203);
      ratio->GetYaxis()->SetRangeUser(0.5,1.5);

      // Per histogram cosmetics
      ratio->GetXaxis()->SetTitle(H.dist.xTitle);
      ratio->SetLineColor(H.color);
      ratio->SetMarkerColor(H.color);
      ratio->SetMarkerStyle(H.markerStyle);
      
      if (loop == 1) ratio->Draw();
      else ratio->Draw("same");
    }
       
    line->Draw("same");

    // Now go back to main pad and add the legend
    can_1.cd();
    myMarkerText(0.8, 0.9-loop*0.07, H.color, H.markerStyle, TString::Format("%s\n", H.legendTitle.Data()), 0.05);
  
    ++loop;
  }

  c.Print(TString::Format("plots/%s.pdf", cname.Data()));

}


void plot() {

  // setup
  TString setup = "mu0";

  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles";
  
  // samples
  TFile *f_bg_ttbar  = TFile::Open(TString::Format("%s/%s/ttbar/ttbar.root", dir.Data(), setup.Data()), "read");
  TFile *f_bg_ttH_sl = TFile::Open(TString::Format("%s/%s/ttH_semilep/ttH_semilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_bg_ttH_dl = TFile::Open(TString::Format("%s/%s/ttH_dilep/ttH_dilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tH     = TFile::Open(TString::Format("%s/%s/tH/tH.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tWH    = TFile::Open(TString::Format("%s/%s/tWH/tWH.root", dir.Data(), setup.Data()), "read");
  
  
  // Normalization
  TH1F *h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get("events");
  TH1F *h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get("events");
  TH1F *h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get("events");
  TH1F *h_sg_tH     = (TH1F*)f_sg_tH->Get("events");
  TH1F *h_sg_tWH    = (TH1F*)f_sg_tWH->Get("events");
  
  float scale_ttbar  = 3000000.*452.2944528/h_bg_ttbar->GetBinContent(2);
  float scale_ttH_sl = 3000000.*0.22276/h_bg_ttH_sl->GetBinContent(2);
  float scale_ttH_dl = 3000000.*0.05343/h_bg_ttH_dl->GetBinContent(2);
  float scale_tH     = 3000000.*0.054157/h_sg_tH->GetBinContent(2);
  float scale_tWH    = 3000000.*0.014425/h_sg_tWH->GetBinContent(2);
  
  vector<Distribution> dists = { {"jfwd_eta_SRB3", "SRB3 #eta(most forward jet)", {0,4}, {5.E-1, 5.E8}, 1, true},
  				 {"mindEta_ljets_bjets", "|min#Delta#eta(l-jets, b-jets)|", {0,8}, {5.E-1, 5.E8}, 2, true},
  				 {"maxdEta_ljets_bjets", "|max#Delta#eta(l-jets, b-jets)|", {0,8}, {5.E-1, 5.E8}, 2, true},
				 {"dEta_jfwd_b1", "|min#Delta#eta(most forward l-jet, b1)|", {0,8}, {5.E-1, 5.E8}, 2, true},
			       };

			  
  for (auto dist : dists) {
    h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get(TString::Format("%s", dist.histName.Data()));
    h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get(TString::Format("%s", dist.histName.Data()));
    h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get(TString::Format("%s", dist.histName.Data()));
    h_sg_tH     = (TH1F*)f_sg_tH->Get(TString::Format("%s", dist.histName.Data()));
    h_sg_tWH    = (TH1F*)f_sg_tWH->Get(TString::Format("%s", dist.histName.Data()));
    
    // Turn on sumw2
    h_bg_ttbar->Sumw2();
    h_bg_ttbar->Sumw2();
    h_bg_ttH_sl->Sumw2();
    h_bg_ttH_dl->Sumw2();
    h_sg_tH->Sumw2();
    h_sg_tWH->Sumw2();
        
    // Here we should normalize the histograms according to xsec and add tH+tWH in a single histogram and all bg in another histogram
    h_bg_ttbar->Scale(scale_ttbar);
    h_bg_ttH_sl->Scale(scale_ttH_sl);
    h_bg_ttH_dl->Scale(scale_ttH_dl);
    h_sg_tH->Scale(scale_tH);
    h_sg_tWH->Scale(scale_tWH);
    
    h_bg_ttH_sl->Add(h_bg_ttH_dl);

    // Histogram cosmetics   
    bool shapeOnly = false;
    Histogram H_TTBAR = {h_bg_ttbar, dist, 1, 24, "t#bar{t} ", shapeOnly};
    Histogram H_TTH   = {h_bg_ttH_sl, dist, kBlue, 24, "t#bar{t}h", shapeOnly};
    Histogram H_TH    = {h_sg_tH, dist, kRed, 24, "th ", shapeOnly};
    Histogram H_TWH   = {h_sg_tWH, dist, kRed+2, 24, "tWh", shapeOnly};

    // Histograms to plot on the same canvas
    vector<Histogram> histograms = {H_TTBAR, H_TTH, H_TH, H_TWH};
    
    TString canvasName = dist.histName+"_"+setup;
    makePlot(histograms, canvasName);
    
  } 

}
