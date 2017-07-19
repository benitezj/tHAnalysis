#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"
#include "Labels.h"

#include <vector>
#include <utility>


enum class CutType { LOW, HIGH };

std::pair<float,float> LLR(const TH1F *hSig, const TH1F *hBkg, const int bin, CutType ct) {

  std::pair<float,float> LLR={0.,0.};
  const unsigned int nbins = hSig->GetNbinsX();
  unsigned int start = 1;
  unsigned int end   = nbins+1;
  if (ct == CutType::LOW)      start = bin;
  else if (ct == CutType::HIGH) end   = bin;
  
  for (unsigned int b=start; b < end+1; ++b) {
    double iS = hSig->GetBinContent(b);
    double iB = hBkg->GetBinContent(b);
    double idS = hSig->GetBinError(b);
    double idB = hBkg->GetBinError(b);
    if( iB > 0 && iS > 0) {
      double iLSB = log(1 + iS/iB);
      LLR.first  += 2 * ( (iS+iB) * iLSB - iS );
      LLR.second += iLSB * iLSB * idS * idS + (iLSB - iS/iB) * (iLSB - iS/iB) * idB * idB;
    }
  }
  LLR.first = sqrt(LLR.first);
  if(LLR.first > 0) LLR.second = sqrt(LLR.second) / LLR.first;

  return LLR;
}

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

  // Create significance histo
  TH1F *h_sig = (TH1F*)h_sg->Clone();
  
  // Loop over bins and calculate sensitivity
  for (int i=1; i < nbins+1; ++i) {
    std::pair<float,float> llr = LLR(h_sg, h_bg, i, dist.second); 
    h_sig->SetBinContent(i, llr.first);
    h_sig->SetBinError(i, llr.second);
  }
  
  // Scale significance histo to have the same integral as efficiency
  h_sig->Scale(h_eff_sg->Integral()/h_sig->Integral());
  

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
  h_sig->Draw("histsame");
  
  h_sig->SetLineColor(kMagenta+3);
  
  myText(0.6, 0.7, kBlue, "Bg rejection");
  myText(0.6, 0.65, kRed, "Sig efficiency");
  myText(0.6, 0.60, kMagenta+3, "Significance");
  
  if ( dist.second == CutType::LOW) c.Print(TString::Format("%s_eff_LOW.pdf", dist.first.Data()));
  else c.Print(TString::Format("%s_eff_HIGH.pdf", dist.first.Data()));
  
  delete h_eff_bg;
  delete h_eff_sg;
}

void plotCutEfficiency() {

  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles";
  
  // samples
  TFile *f_bg_ttbar  = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu0/ttbar/ttbar.root", "read");
  TFile *f_bg_ttH_sl = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu0/ttH_semilep/ttH_semilep.root", "read");
  TFile *f_bg_ttH_dl = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu0/ttH_dilep/ttH_dilep.root", "read");
  TFile *f_sg_tH     = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu0/tH/tH.root", "read");
  TFile *f_sg_tWH    = TFile::Open("/usatlas/u/sargyrop/tHFramework/OutputRootFiles/mu0/tWH/tWH.root", "read");
  
  
  // Normalization
  TH1F *h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get("events");
  TH1F *h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get("events");
  TH1F *h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get("events");
  TH1F *h_sg_tH     = (TH1F*)f_sg_tH->Get("events");
  TH1F *h_sg_tWH    = (TH1F*)f_sg_tWH->Get("events");
  
  float scale_ttbar  = 3000.*452.2944528/h_bg_ttbar->GetBinContent(2);
  float scale_ttH_sl = 3000.*0.22276/h_bg_ttH_sl->GetBinContent(2);
  float scale_ttH_dl = 3000.*0.05343/h_bg_ttH_dl->GetBinContent(2);
  float scale_tH     = 3000.*0.054157/h_sg_tH->GetBinContent(2);
  float scale_tWH    = 3000.*0.014425/h_sg_tWH->GetBinContent(2);
  
  // Get TH1 
  std::vector<std::pair<TString, CutType>> dists = { {"jfwd_eta_SRB3", CutType::LOW},
  						     {"jfwd_eta_SRB3", CutType::HIGH},
						     {"jfwd_eta_SRB4", CutType::LOW},
  						     {"jfwd_eta_SRB4", CutType::HIGH},
						     {"mindEta_ljets_bjets", CutType::LOW},
						     {"mindEta_ljets_bjets", CutType::HIGH},
						     {"maxdEta_ljets_bjets", CutType::LOW},
						     {"maxdEta_ljets_bjets", CutType::HIGH},
						     {"dEta_jfwd_b1", CutType::LOW},
						     {"dEta_jfwd_b1", CutType::HIGH} };
  
  // Loop over distributions and make efficiency plots
  for (auto dist : dists) {
    h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get(TString::Format("%s", dist.first.Data()));
    h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get(TString::Format("%s", dist.first.Data()));
    h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get(TString::Format("%s", dist.first.Data()));
    h_sg_tH     = (TH1F*)f_sg_tH->Get(TString::Format("%s", dist.first.Data()));
    h_sg_tWH    = (TH1F*)f_sg_tWH->Get(TString::Format("%s", dist.first.Data()));
    
    // Here we should normalize the histograms according to xsec and add tH+tWH in a single histogram and all bg in another histogram
    h_bg_ttbar->Scale(scale_ttbar);
    h_bg_ttH_sl->Scale(scale_ttH_sl);
    h_bg_ttH_dl->Scale(scale_ttH_dl);
    h_sg_tH->Scale(scale_tH);
    h_sg_tWH->Scale(scale_tWH);
    
    h_bg_ttbar->Add(h_bg_ttH_sl);
    h_bg_ttbar->Add(h_bg_ttH_dl);
    h_sg_tH->Add(h_sg_tWH);
    
    makeEffPlot(h_sg_tH, h_bg_ttbar, dist);
    
  }

}

