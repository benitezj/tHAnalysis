#include "TFile.h"
#include "TH1F.h"
#include "TString.h"

#include <vector>
#include <utility>


std::pair<float,float> LLR(const TH1F *hSig, const TH1F *hBkg) {

  std::pair<float,float> LLR={0.,0.};
  
  const unsigned int nbins = hSig->GetNbinsX();
  for (unsigned int b=1; b < nbins+1; ++b) {
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

void calculateSignificance() {

  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles";
  
  // Distributions to use for calculating significance
  std::vector<TString> dists = { "jfwd_eta_loose",
  				 "jfwd_eta",
  				 "jfwd_eta_SRB3",
  			         "jfwd_eta_SRB4",
				 "jfwd_eta_SRMbbH3_SRB3",
				 "jfwd_eta_SRMbbH3_SRB4",
				 "jfwd_eta_B3_Opt1",
				 "jfwd_eta_B4_Opt1",
				 "jfwd_eta_B3_Opt2",
				 "jfwd_eta_B4_Opt2",
				 "jfwd_eta_B3_Opt3",
				 "jfwd_eta_B4_Opt3",
				 "jfwd_eta_B3_Opt4",
				 "jfwd_eta_B4_Opt4" };
  
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
    
  // Loop over distributions and make efficiency plots
  for (auto dist : dists) {
    h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get(TString::Format("%s", dist.Data()));
    h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get(TString::Format("%s", dist.Data()));
    h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get(TString::Format("%s", dist.Data()));
    h_sg_tH     = (TH1F*)f_sg_tH->Get(TString::Format("%s", dist.Data()));
    h_sg_tWH    = (TH1F*)f_sg_tWH->Get(TString::Format("%s", dist.Data()));
    
    if (!h_bg_ttbar) continue;
    
    // Here we should normalize the histograms according to xsec and add tH+tWH in a single histogram and all bg in another histogram
    h_bg_ttbar->Scale(scale_ttbar);
    h_bg_ttH_sl->Scale(scale_ttH_sl);
    h_bg_ttH_dl->Scale(scale_ttH_dl);
    h_sg_tH->Scale(scale_tH);
    h_sg_tWH->Scale(scale_tWH);
    
    h_bg_ttbar->Add(h_bg_ttH_sl);
    h_bg_ttbar->Add(h_bg_ttH_dl);
    h_sg_tH->Add(h_sg_tWH);
    
    std::pair<float,float> llr = LLR(h_sg_tH, h_bg_ttbar);
    printf("Distribution:  %35s   -  LLR = %5.3f +- %3.3f\n", dist.Data(), llr.first, llr.second); 
    
  }

}

