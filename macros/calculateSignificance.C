#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TError.h"

#include <vector>
#include <utility>


float getMaxErrorHist(const TH1F *h) {

  const unsigned int nbins = h->GetNbinsX();
  
  // First calculate maximum relative error in each bin
  std::vector<float> errors;
  for (unsigned int b=1; b < nbins+1; ++b) {
    if (h->GetBinContent(b) != 0) errors.push_back(h->GetBinError(b)/h->GetBinContent(b));
  }
  float maxErr = *std::max_element(errors.begin(), errors.end());

  return maxErr;
}


int getRebinFactor(const TH1F *bg, float rebinThreshold) {

  int rebinFactor = 1;
  const unsigned int nbins = bg->GetNbinsX();
  
  // First calculate maximum relative error in each bin
  float maxErr = getMaxErrorHist(bg);
  
  // Rebin until maxErr falls below threshold
  while (maxErr > rebinThreshold) {
  
    // First make a clone of the histogram
    TH1F *htemp = (TH1F*)bg->Clone();
    
    // Increment rebin factor by 1
    ++rebinFactor;
    
    // Rebin histogram
    htemp->Rebin(rebinFactor);
    
    // Recalculate maximum error
    maxErr = getMaxErrorHist(htemp);
  
    // Delete htemp
    delete htemp;
  }
  
  //std::cout << "Rebin factor = " << rebinFactor << std::endl;  
  return rebinFactor;
}


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

std::pair<float,float> calculateSignificanceLoop(const TString& setup, const TString& dist, const float kappa) {
  
  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles/ele_eta4p0";
   
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
  
  // Calculate tH or tWH cross-section based on kappa value
  float xsec_tH  = 2.08600e-01 - 3.38950e-01*kappa + 1.84450e-01*pow(kappa,2);
  float xsec_tWH = 4.28000e-02 - 6.27000e-02*kappa + 4.49000e-02*pow(kappa,2);
  
  // This could be hacked so that the NLO k-factor and H(bb) BR is applied for all kappa
  if (kappa == 1.0) {
    xsec_tH = 0.04284;
    xsec_tWH = 0.014425;
  }
  
  float scale_ttbar  = 3000000.*452.2944528/h_bg_ttbar->GetBinContent(2);
  float scale_ttH_sl = 3000000.*0.22276/h_bg_ttH_sl->GetBinContent(2);
  float scale_ttH_dl = 3000000.*0.05343/h_bg_ttH_dl->GetBinContent(2);
  float scale_tH     = 3000000.*xsec_tH/h_sg_tH->GetBinContent(2);
  float scale_tWH    = 3000000.*xsec_tWH/h_sg_tWH->GetBinContent(2);
    
  // Get Histograms 
  h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get(TString::Format("%s", dist.Data()));
  h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get(TString::Format("%s", dist.Data()));
  h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get(TString::Format("%s", dist.Data()));
  h_sg_tH     = (TH1F*)f_sg_tH->Get(TString::Format("%s", dist.Data()));
  h_sg_tWH    = (TH1F*)f_sg_tWH->Get(TString::Format("%s", dist.Data()));
   
  if (!h_bg_ttbar) return {};
      
  // Here we should normalize the histograms according to xsec and add tH+tWH in a single histogram and all bg in another histogram
  h_bg_ttbar->Scale(scale_ttbar);
  h_bg_ttH_sl->Scale(scale_ttH_sl);
  h_bg_ttH_dl->Scale(scale_ttH_dl);
  h_sg_tH->Scale(scale_tH);
  h_sg_tWH->Scale(scale_tWH);
    
  h_bg_ttbar->Add(h_bg_ttH_sl);
  h_bg_ttbar->Add(h_bg_ttH_dl);
  h_sg_tH->Add(h_sg_tWH);
  
  // Now perform a dynamic rebining
  // The histograms are rebinned so that the sum of backgrounds has a relative error 
  // of less than thr%  in each bin
  float thr = 10;
  int rebin = getRebinFactor(h_bg_ttbar, thr*0.01);
  h_bg_ttbar->Rebin(rebin);
  h_sg_tH->Rebin(rebin);
    
  std::pair<float,float> llr = LLR(h_sg_tH, h_bg_ttbar);
  //printf("Distribution:  %35s   -  LLR = %6.5f +- %6.5f\n", dist.Data(), llr.first, llr.second); 
    
  return llr;
}

void calculateSignificance() {

  // Suppress warnings
  gErrorIgnoreLevel = kError;

  // Scan over kappa
  bool kScan = false;
  std::vector<float> kappas;
  if (kScan) kappas = {-1, -0.8, -0.6, -0.4, -0.2, 0., 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0};
  else kappas = {1.0};

  // Different setups used
  std::vector<TString> setup = {//"mu200_noPUJets_TC_PU0.02",
				//"mu200_noTC",
				//"mu200_TC_PU0.02",
				"mu200_TC_PU0.02_HGTD_purej",
				//"mu200_TC_PU0.02_HGTD_purej_btagOP85",
				//"mu200_TC_PU0.05",
				"mu200_TC_PU0.02_HGTD_HGTDbtag_beff_purej",
				//"mu200_TC_PU0.02_HGTD_HGTDbtag_beff_purej_btagOP85",
				"mu200_TC_PU0.02_HGTD_HGTDbtag_crej_purej",
				//"mu200_TC_PU0.02_HGTD_HGTDbtag_crej_purej_btagOP85",
				"mu200_TC_PU0.02_HGTD_HGTDbtag_lrej_purej",
				//"mu200_TC_PU0.02_HGTD_HGTDbtag_lrej_purej_btagOP85",
				//"mu200_TC_PU0.05_HGTD_purej",
				//"mu200_TC_PU0.02_HGTD_purej_btagOP85"
				//"mu200_TC_PU0.05_HGTD_purej_btagOP85",
				//"mu200_TC_PU0.10",
				//"mu200_TC_PU0.10_HGTD_purej"
				};
				
  //std::vector<TString> setup = {"mu200_TC_PU0.05"};				
    
  // Distributions to use for calculating significance
  std::vector<TString> dists = { "jfwd_eta_SRB3",
  			         "jfwd_eta_SRB4"
			       };
   
  for (auto k : kappas) { 
    for (auto s : setup) {
      std::cout << "Calculating LLR for : " << s << std::endl;
      std::pair<float,float> sig, sigB3, sigB4;   
      for (auto d : dists) {
        sig = calculateSignificanceLoop(s, d, k);
        if     (d.Contains("SRB3")) sigB3 = sig;
        else if (d.Contains("SRB4")) sigB4 = sig;
      }
      float sum  = std::sqrt(std::pow(sigB3.first,2)+std::pow(sigB4.first,2));
      float sumE = std::sqrt(std::pow(sigB3.first*sigB3.second,2)+std::pow(sigB4.first*sigB4.second,2))/sum;
      printf("Distribution:  %5s  -  k = %2.1f   -   LLR = %6.5f +- %6.5f\n", "Sum", k, sum, sumE); 
    }
  }

}
