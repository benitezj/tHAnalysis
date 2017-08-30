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


void plot(const vector<Histogram>& histograms, TString &cname, TString &sample) {
 
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
    h->GetYaxis()->SetTitle("A.U.");

    if (H.dist.logY) can_1.SetLogy();

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
      ratio->GetYaxis()->SetRangeUser(0.95,1.05);

      // Per histogram cosmetics
      ratio->GetXaxis()->SetTitle(H.dist.xTitle);
      ratio->SetLineColor(H.color);
      ratio->SetMarkerColor(H.color);
      ratio->SetMarkerStyle(H.markerStyle);
      ratio->GetYaxis()->SetTitle("Ratio");
      
      if (loop == 1) ratio->Draw();
      else ratio->Draw("same");
    }
    
       
    line->Draw("same");

    // Now go back to main pad and add the legend
    can_1.cd();
    ATLASLabel(0.2,0.55,"Simulation Internal",1);
    myText(0.2,0.49,1,"#sqrt{s}=13 TeV, 3000 fb^{-1}"); 
    myText(0.2,0.43,1,sample);    
    myMarkerText(0.22, 0.38-loop*0.06, H.color, H.markerStyle, TString::Format("%s\n", H.legendTitle.Data()));
  
    ++loop;
  }

  c.Print(TString::Format("%s_%s.pdf", cname.Data(), sample.Data()));

}


void plotSmearedVsUnsmeared() {

  TString sample = "ttbar";
  
  vector<Distribution> dists = { //{"Njets_HGTD_nocuts", "N_{jets} in HGTD", {0,10}, {0,1500}, 1, false}, 
  				 //{"Nbjets_HGTD_nocuts", "N_{b-jets} in HGTD", {0,5}, {0,1500}, 1, false} 
				 //{"H3_m_SRB3", "SRB3 m(b#bar{b}) [GeV]", {0,350}, {0,1500}, 2, false},
				 //{"jfwd_eta_SRB3", "SRB3 #eta(most forward jet)", {0,4}, {0,1500}, 2, false},
				 //{"jfwd_eta_SRB4", "SRB4 #eta(most forward jet)", {0,4}, {0,1500}, 1, false},
				 //{"Njets_SRB3", "N_{b-jets}", {-0.5,5.5}, {0,1500}, 1, false}
				 {"Nbjets", "N_{b-jets}", {-0.5,5.5}, {0,1500}, 1, false},
				 {"NLightjets", "N_{l-jets}", {-0.5,9.5}, {0,1500}, 1, false}
			       };

  TFile *f1 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_noPUJets_TC_PU0.02/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f2 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_noTC/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f3 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.02/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f4 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.02_HGTD_purej/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f5 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.05/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");			  
  TFile *f6 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.05_HGTD_purej/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f7 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f8 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");			  			  			  
  TFile *f9 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");
  TFile *f10 = TFile::Open(TString::Format("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta2p47/mu200_TC_PU0.10_HGTD_purej/%s/%s.root", 
  			  sample.Data(), sample.Data()), "read");			  
 			  
			  			  			  
  for (auto dist : dists) {
    TH1F *h1  = (TH1F*)f1->Get(dist.histName);
    TH1F *h2  = (TH1F*)f2->Get(dist.histName);
    TH1F *h3  = (TH1F*)f3->Get(dist.histName);
    TH1F *h4  = (TH1F*)f4->Get(dist.histName);
    TH1F *h5  = (TH1F*)f5->Get(dist.histName);
    TH1F *h6  = (TH1F*)f6->Get(dist.histName);
    TH1F *h7  = (TH1F*)f7->Get(dist.histName);
    TH1F *h8  = (TH1F*)f8->Get(dist.histName);
    TH1F *h9  = (TH1F*)f9->Get(dist.histName);
    TH1F *h10 = (TH1F*)f10->Get(dist.histName);
  
    // Histogram cosmetics
    Histogram H1  = {h1, dist, 1, 24, "#mu=200 , no PU jets", true};
    Histogram H2  = {h2, dist, kRed, 24, "#mu=200 , PU jets, no TC", true};
    Histogram H3  = {h3, dist, kMagenta, 24, "#mu=200 , TC", true};
    Histogram H4  = {h4, dist, kMagenta+3, 24, "#mu=200 , TC + HGTD, #epsilon_{PU}=5%", true};
    Histogram H5  = {h5, dist, kMagenta+3, 24, "#mu=200 , TC, #epsilon_{PU}=5%", true};
    Histogram H6  = {h6, dist, kYellow+3, 24, "#mu=200 , TC + HGTD, #epsilon_{PU}=5%", true};
    Histogram H7  = {h7, dist, kBlue, 25, "#mu=200 , TC + HGTD + l-jet rejection impr.", true};
    Histogram H8  = {h8, dist, kMagenta+3, 26, "#mu=200 , TC + HGTD + c-jet rejection impr.", true};
    Histogram H9  = {h9, dist, kGreen+3, 27, "#mu=200 , TC + HGTD + b-jet efficiency impr.", true};
    Histogram H10 = {h10, dist, kYellow+3, 24, "#mu=200 , TC + HGTD, #epsilon_{PU}=10%", true};
    
    // Histograms to plot on the same canvas
    vector<Histogram> histograms = {H6, H7, H8, H9};
    
    plot(histograms, dist.histName, sample);
    
  } 

}
