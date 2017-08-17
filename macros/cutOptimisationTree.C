#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCut.h"
#include "TString.h"
#include "TError.h"
#include "TGraphErrors.h"

static bool debug = false;

enum class CutType { LOW, HIGH };

struct Cut {
  TString 	  name;
  pair<float,float> range;
  float		   step;
  CutType	  cutType;
};


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


void plotSigPlot(TH1F* bg, TH1F* sig, const Cut& cut, const int i) {

  // First plot the histograms
  TCanvas c;
  c.cd();
  bg->SetLineColor(kBlue);
  sig->SetLineColor(kRed);
  bg->Draw("hist");
  sig->Draw("histsame");
  
  float max = bg->GetMaximum();
  float min = sig->GetMaximum();
  
  bg->GetYaxis()->SetRangeUser(min*0.001, max*1000);
  
  c.SetLogy();
  if (cut.cutType == CutType::LOW) c.Print(TString::Format("plots/jfwd_eta_%s_LOW_%d.pdf", cut.name.Data(), i));
  else c.Print(TString::Format("plots/jfwd_eta_%s_HIGH_%d.pdf", cut.name.Data(), i));

}


void cutOptimisationTree() {

  // Suppress warnings
  gErrorIgnoreLevel = kWarning;

  // setup
  TString setup = "mu200_TC_PU0.02";

  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles";
  
  // samples
  TFile *f_bg_ttbar  = TFile::Open(TString::Format("%s/%s/ttbar/ttbar.root", dir.Data(), setup.Data()), "read");
  TFile *f_bg_ttH_sl = TFile::Open(TString::Format("%s/%s/ttH_semilep/ttH_semilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_bg_ttH_dl = TFile::Open(TString::Format("%s/%s/ttH_dilep/ttH_dilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tH     = TFile::Open(TString::Format("%s/%s/tH/tH.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tWH    = TFile::Open(TString::Format("%s/%s/tWH/tWH.root", dir.Data(), setup.Data()), "read");
  
  // Get the trees
  TTree *t_bg_ttbar  = (TTree*)f_bg_ttbar->Get("ntuple");
  TTree *t_bg_ttH_sl = (TTree*)f_bg_ttH_sl->Get("ntuple");
  TTree *t_bg_ttH_dl = (TTree*)f_bg_ttH_dl->Get("ntuple");
  TTree *t_sg_tH     = (TTree*)f_sg_tH->Get("ntuple");
  TTree *t_sg_tWH    = (TTree*)f_sg_tWH->Get("ntuple");
    
  // Baseline cuts to apply
  TCut nLep   = "nLeptons == 1";
  TCut nJets  = "NSignalJets >= 4 && NSignalJets <= 5"; 
  TCut nBJets = "NBjets == 3"; 
  TCut nLJets = "NantiBjets >= 1 && NantiBjets <= 3"; 
  TCut pTSum  = "pTSum >= 300";
  
  // Pre-selection
  TCut preSel  = nLep && nJets && nBJets && nLJets && pTSum;
  TCut wPreSel = preSel * "eventWeight"; // eventWeight already contains the trigger weight
  
  // Additional cuts to try
  std::vector<Cut> addCuts = { {"met",        {0,500},    50,    CutType::LOW}, 
  			       {"pTSum",      {300,1300}, 100,   CutType::LOW},
			       {"H2_m",       {10,250},   10,    CutType::LOW},
			       {"H3_m",       {10,250},   10,    CutType::LOW},
			       {"FoxW1",      {0,1},      0.1, CutType::LOW},  
			       {"FoxW2",      {0,1},      0.1, CutType::LOW},  
			       {"FoxW3",      {0,1},      0.1, CutType::LOW},  
			       {"shpericity", {0,1},      0.1, CutType::LOW}, 
			       {"met",        {0,500},    50,    CutType::HIGH}, 
  			       {"pTSum",      {300,1300}, 100,   CutType::HIGH},
			       {"H2_m",       {10,250},   10,    CutType::HIGH},
			       {"H3_m",       {10,250},   10,    CutType::HIGH},
			       {"FoxW1",      {0,1},      0.1, CutType::HIGH},  
			       {"FoxW2",      {0,1},      0.1, CutType::HIGH},  
			       {"FoxW3",      {0,1},      0.1, CutType::HIGH},  
			       {"shpericity", {0,1},      0.1, CutType::HIGH},
		             };
			     
  // Run cut optimisation for each cut			     
  for (auto cut : addCuts) {
    
    float low       = cut.range.first;
    float high      = cut.range.second;
    const int  nbins = (high-low)/cut.step;   
    
    // Arrays to hold LLR
    Float_t x_cut[nbins], xe_cut[nbins], y_LLR[nbins], ye_LLR[nbins];
    
    printf("=========== Cut :   %s   ===============\n", cut.name.Data());

    // Loop over cut values 
    int counter = 0;
    for (int i=low; i <= high; i += (int)cut.step) {
          
      // Histograms from the TTree
      TH1F *h_bg_ttbar  = new TH1F("h_bg_ttbar", "", 40, 0, 4);
      TH1F *h_bg_ttH_sl = new TH1F("h_bg_ttH_sl", "", 40, 0, 4);
      TH1F *h_bg_ttH_dl = new TH1F("h_bg_ttH_dl", "", 40, 0, 4);
      TH1F *h_sg_tH     = new TH1F("h_sg_tH", "", 40, 0, 4);
      TH1F *h_sg_tWH    = new TH1F("h_sg_tWH", "", 40, 0, 4);
    
      // Update cut value
      TCut c;
      if (cut.cutType == CutType::LOW) c = TString::Format("%s < %d", cut.name.Data(), i);
      else c = TString::Format("%s >= %d", cut.name.Data(), i);
      
      if (debug) std::cout << "Apply cuts...\n";
        
      // Now apply the cuts
      t_bg_ttbar->Draw("fabs(jfwd_eta) >> h_bg_ttbar", preSel && c);
      t_bg_ttH_sl->Draw("fabs(jfwd_eta) >> h_bg_ttH_sl", preSel && c);
      t_bg_ttH_dl->Draw("fabs(jfwd_eta) >> h_bg_ttH_dl", preSel && c);
      t_sg_tH->Draw("fabs(jfwd_eta) >> h_sg_tH", preSel && c);
      t_sg_tWH->Draw("fabs(jfwd_eta) >> h_sg_tWH", preSel && c);
      
      // Check if the histograms are there
      if (!h_bg_ttbar)  { std::cerr << "ERROR: h_bg_ttbar does not exist!\n"; continue; }
      if (!h_bg_ttH_sl) { std::cerr << "ERROR: h_bg_ttbar does not exist!\n"; continue; }
      if (!h_bg_ttH_dl) { std::cerr << "ERROR: h_bg_ttbar does not exist!\n"; continue; }
      if (!h_sg_tH)     { std::cerr << "ERROR: h_bg_ttbar does not exist!\n"; continue; }
      if (!h_sg_tWH)    { std::cerr << "ERROR: h_bg_ttbar does not exist!\n"; continue; }
         
      // Histograms from the TTree
      TH1F *h_sum_ttbar  = new TH1F("h_sum_ttbar", "", 1, -1.E10, 1.E10);
      TH1F *h_sum_ttH_sl = new TH1F("h_sum_ttH_sl", "", 1, -1.E10, 1.E10);
      TH1F *h_sum_ttH_dl = new TH1F("h_sum_ttH_dl", "", 1, -1.E10, 1.E10);
      TH1F *h_sum_tH     = new TH1F("h_sum_tH", "", 1, -1.E10, 1.E10);
      TH1F *h_sum_tWH    = new TH1F("h_sum_tWH", "", 1, -1.E10, 1.E10);
      
      if (debug) std::cout << "Get SoW...\n";
      
      // Sum of weights 
      t_bg_ttbar->Draw("eventWeight >> h_sum_ttbar", "eventWeight != 0");
      t_bg_ttH_sl->Draw("eventWeight >> h_sum_ttH_sl", "eventWeight != 0");
      t_bg_ttH_sl->Draw("eventWeight >> h_sum_ttH_dl", "eventWeight != 0");
      t_sg_tH->Draw("eventWeight >> h_sum_tH", "eventWeight != 0");
      t_sg_tWH->Draw("eventWeight >> h_sum_tWH", "eventWeight != 0");
                
      // Scale the histograms 
      float scale_ttbar  = 3000000.*452.2944528/h_sum_ttbar->Integral();
      float scale_ttH_sl = 3000000.*0.22276/h_sum_ttH_sl->Integral();
      float scale_ttH_dl = 3000000.*0.05343/h_sum_ttH_dl->Integral();
      float scale_tH     = 3000000.*0.054157/h_sum_tH->Integral();
      float scale_tWH    = 3000000.*0.014425/h_sum_tWH->Integral();
      
      if (debug) std::cout << "Normalize...\n";
       
      // Normalize histograms
      h_bg_ttbar->Scale(scale_ttbar);
      h_bg_ttH_sl->Scale(scale_ttH_sl);
      h_bg_ttH_dl->Scale(scale_ttH_dl);
      h_sg_tH->Scale(scale_tH);
      h_sg_tWH->Scale(scale_tWH);
            
      // Add backgrounds and signals
      h_bg_ttbar->Add(h_bg_ttH_sl);
      h_bg_ttbar->Add(h_bg_ttH_dl);
      h_sg_tH->Add(h_sg_tWH);
            
      // Now perform a dynamic rebining
      // The histograms are rebinned so that the sum of backgrounds has a relative error 
      // of less than thr%  in each bin
      //float thr = 10;
      //int rebin = getRebinFactor(h_bg_ttbar, thr*0.01);
      //h_bg_ttbar->Rebin(rebin);
      //h_sg_tH->Rebin(rebin);
      
            
      // Plot
      if (debug) std::cout << "Plot ...\n";
      //plotSigPlot(h_bg_ttbar, h_sg_tH, cut, i);
      
      
      // Calculate significance
      if (debug) std::cout << "Calculate LLR ...\n";
      std::pair<float,float> llr = LLR(h_sg_tH, h_bg_ttbar);
      
      // Store the cut value and LLR
      if (debug) std::cout << "Store LLR ...\n";
      y_LLR[counter]  = llr.first;
      ye_LLR[counter] = llr.second;
      x_cut[counter]  = i;
      xe_cut[counter] = 0.;     
      
      // Now print the LLR
      printf("Cut:  %35s   -  LLR = %6.5f +- %6.5f\n", c.GetTitle(), llr.first, llr.second); 
      
      delete h_bg_ttbar;
      delete h_bg_ttH_sl;
      delete h_bg_ttH_dl;
      delete h_sg_tH;
      delete h_sg_tWH;
      delete h_sum_ttbar;
      delete h_sum_ttH_sl;
      delete h_sum_ttH_dl;
      delete h_sum_tH;
      delete h_sum_tWH;
      
      ++counter;
    }
    
    
    // Create TGraph to store the LLR
    TGraphErrors *g_LLR = new TGraphErrors(nbins, x_cut, y_LLR, xe_cut, ye_LLR);

    // Now plot the LLR optimisation histogram
    TCanvas c2;
    c2.cd();
    g_LLR->Draw("ALP");
    if (cut.cutType == CutType::LOW) g_LLR->GetXaxis()->SetTitle(TString::Format("%s lower than", cut.name.Data()));
    else g_LLR->GetXaxis()->SetTitle(TString::Format("%s higher than", cut.name.Data()));
    g_LLR->GetYaxis()->SetTitle("LLR");
    if (cut.cutType == CutType::LOW) c2.Print(TString::Format("plots/LLR_%s_LOW.pdf", cut.name.Data()));
    else c2.Print(TString::Format("plots/LLR_%s_HIGH.pdf", cut.name.Data()));
    
    delete g_LLR; 
  }

}
