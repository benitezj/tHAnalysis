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


void plot(const vector<Histogram>& histograms, TString &cname) {
 
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
    
    // Main pad
    h->SetMarkerStyle(H.markerStyle);
    h->SetLineColor(H.color);
    h->SetMarkerColor(H.color);
    h->GetXaxis()->SetRangeUser(H.dist.xRange.first, H.dist.xRange.second);
    h->GetXaxis()->SetTitle(H.dist.xTitle);
    h->SetTitle("");
    
    float textsize = 18/(gPad->GetWh()*gPad->GetAbsHNDC());
    h->GetYaxis()->SetTitleSize(textsize);
    h->GetYaxis()->SetLabelSize(textsize);
    h->GetXaxis()->SetTitleSize(textsize);
    h->GetXaxis()->SetLabelSize(textsize);
    h->GetYaxis()->SetTitleOffset(0.8);

    if (loop == 0) h->Draw("PE1");
    else h->Draw("PE1same");
    
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
    myMarkerText(0.55, 0.7-loop*0.06, H.color, H.markerStyle, TString::Format("%s , %.0f\n", H.legendTitle.Data(), H.histo->Integral()));
  
    ++loop;
  }

  c.Print(TString::Format("%s.pdf", cname.Data()));

}


void plotSmearedVsUnsmeared() {

  TString sample = "tH";
  
  vector<Distribution> dists = { {"Njets_HGTD_nocuts", "N_{jets} in HGTD", {0,10}, {0,1500}, 1, false}, 
  				 {"Nbjets_HGTD_nocuts", "N_{b-jets} in HGTD", {0,5}, {0,1500}, 1, false}, 
				 {"H3_m_SRB3", "SRB3 m(b#bar{b}) [GeV]", {0,350}, {0,1500}, 2, false},
				 {"jfwd_eta_SRB3", "SRB3 #eta(most forward jet)", {0,4}, {0,1500}, 1, false}
			       };

  TFile *f1 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu0/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f2 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_noTC/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f3 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f4 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC_PU0.02_HGTD/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f5 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC_PU0.05_HGTD/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f6 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC_PU0.10_HGTD/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");			  			  
  TFile *f7 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC_HS0.70_HGTD/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f8 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC_HS0.80_HGTD/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f9 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu200_TC_HS0.90_HGTD/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
			  			  
  for (auto dist : dists) {
    TH1F *h1 = (TH1F*)f1->Get(dist.histName);
    TH1F *h2 = (TH1F*)f2->Get(dist.histName);
    TH1F *h3 = (TH1F*)f3->Get(dist.histName);
    TH1F *h4 = (TH1F*)f4->Get(dist.histName);
    TH1F *h5 = (TH1F*)f5->Get(dist.histName);
    TH1F *h6 = (TH1F*)f6->Get(dist.histName);
    TH1F *h7 = (TH1F*)f7->Get(dist.histName);
    TH1F *h8 = (TH1F*)f8->Get(dist.histName);
    TH1F *h9 = (TH1F*)f9->Get(dist.histName);
  
    // Histogram cosmetics
    Histogram H1 = {h1, dist, 1, 24, "#mu=0", false};
    Histogram H2 = {h2, dist, kRed, 25, "#mu=200 - no TC", false};
    Histogram H3 = {h3, dist, kMagenta, 26, "#mu=200 - TC", false};
    Histogram H4 = {h4, dist, kMagenta+3, 27, "#mu=200 - TC + HGTD", false};
    Histogram H5 = {h5, dist, kMagenta+3, 28, "#mu=200 - HGTD PU=5%", false};
    Histogram H6 = {h6, dist, kMagenta+3, 32, "#mu=200 - HGTD PU=10%", false};
    Histogram H7 = {h7, dist, kOrange+3, 27, "#mu=200 - HGTD HS=70%", false};
    Histogram H8 = {h8, dist, kOrange+3, 28, "#mu=200 - HGTD HS=80%", false};
    Histogram H9 = {h9, dist, kOrange+3, 32, "#mu=200 - HGTD HS=90%", false};
    
    // Histograms to plot on the same canvas
    vector<Histogram> histograms = {H1, H2, H3, H4, H5, H6, H7, H8, H9};
    
    plot(histograms, dist.histName);
    
  } 

}
