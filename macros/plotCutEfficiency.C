#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"

#include <vector>

enum class CutType { LOW, HIGH };

void makeEffPlot(const TH1F *h_sg, const TH1F *h_bg, const std::pair<TString, CutType> &dist) {

  gStyle->SetOptStat(0);
  
  // Create new histograms from existing ones
  TH1F *h_eff_bg = (TH1F*)h_bg->Clone();
  TH1F *h_eff_sg = (TH1F*)h_sg->Clone();

  const int nbins = h_sg->GetNbinsX();
  
  // Loop over bins and calculate efficiency
  for (int i=1; i < nbins+1; ++i) {
    float eff_sg(0.), eff_bg(0.);
    
    if ( dist.second == CutType::LOW) {
      eff_sg =  (h_sg->Integral(0, nbins+1) != 0.0) ? h_sg->Integral(i, nbins+1)/h_sg->Integral(0, nbins+1) : 0.0;
      eff_bg =  (h_bg->Integral(0, nbins+1) != 0.0) ? h_bg->Integral(0, i)/h_bg->Integral(0, nbins+1) : 0.0;
    }
    else {
      eff_sg =  (h_sg->Integral(0, nbins+1) != 0.0) ? h_sg->Integral(0, i)/h_sg->Integral(0, nbins+1) : 0.0;
      eff_bg =  (h_bg->Integral(0, nbins+1) != 0.0) ? h_bg->Integral(i, nbins+1)/h_bg->Integral(0, nbins+1) : 0.0;    
    }
    h_eff_sg->SetBinContent(i, eff_sg);
    h_eff_sg->SetBinError(i, 0);
    h_eff_bg->SetBinContent(i, eff_bg);
    h_eff_bg->SetBinError(i, 0);
  }

  // Plot the histograms
  TCanvas c;
  h_eff_sg->SetLineColor(kRed);
  if ( dist.second == CutType::LOW) h_eff_sg->GetXaxis()->SetTitle(TString::Format("%s higher than", dist.first.Data()));
  else h_eff_sg->GetXaxis()->SetTitle(TString::Format("%s lower than", dist.first.Data()));
  h_eff_sg->GetYaxis()->SetTitle("Cut efficiency");
  h_eff_sg->GetYaxis()->SetRangeUser(0., 1.);
  
  h_eff_sg->SetLineColor(kRed);
  
  h_eff_sg->Draw("hist");
  h_eff_bg->Draw("histsame");
  
  if ( dist.second == CutType::LOW) c.Print(TString::Format("%s_eff_LOW.pdf", dist.first.Data()));
  else c.Print(TString::Format("%s_eff_HIGH.pdf", dist.first.Data()));
  
  delete h_eff_bg;
  delete h_eff_sg;
}

void plotCutEfficiency() {

  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles";
  
  // samples
  TFile *f_bg_ttbar = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ttbar/ttbar.root", "read");
  TFile *f_sg_tH    = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/tH/tH.root", "read");
  
  // Get TH1 
  std::vector<std::pair<TString, CutType>> dists = { {"MET", CutType::LOW},
  						     {"MET", CutType::HIGH} };
  
  // Loop over distributions and make efficiency plots
  for (auto dist : dists) {
    TH1F *h_bg_ttbar = (TH1F*)f_bg_ttbar->Get(TString::Format("%s", dist.first.Data()));
    TH1F *h_sg_tH    = (TH1F*)f_sg_tH->Get(TString::Format("%s", dist.first.Data()));
    
    // Here we should normalize the histograms according to xsec and add tH+tWH in a single histogram and all bg in another histogram
    // ....
    
    makeEffPlot(h_sg_tH, h_bg_ttbar, dist);
    
  }

}

