#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCut.h"
#include "TString.h"
#include "TError.h"


void testTree() {

  // Suppress warnings
  gErrorIgnoreLevel = kWarning;

  // setup
  TString setup = "mu200_TC_PU0.02_HGTD_HGTDbtag_lrej_purej";

  // Directory where input root files are stored
  TString dir = "/usatlas/u/sargyrop/tHFramework/OutputRootFiles";
  
  // samples
  //TFile *f_bg_ttbar  = TFile::Open(TString::Format("%s/%s/ttbar/ttbar.root", dir.Data(), setup.Data()), "read");
  //TFile *f_bg_ttH_sl = TFile::Open(TString::Format("%s/%s/ttH_semilep/ttH_semilep.root", dir.Data(), setup.Data()), "read");
  //TFile *f_bg_ttH_dl = TFile::Open(TString::Format("%s/%s/ttH_dilep/ttH_dilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tH     = TFile::Open(TString::Format("%s/%s/tH/tH.root", dir.Data(), setup.Data()), "read");
  //TFile *f_sg_tWH    = TFile::Open(TString::Format("%s/%s/tWH/tWH.root", dir.Data(), setup.Data()), "read");
  
  // Get the trees
  //TTree *t_bg_ttbar  = (TTree*)f_bg_ttbar->Get("ntuple");
  //TTree *t_bg_ttH_sl = (TTree*)f_bg_ttH_sl->Get("ntuple");
  //TTree *t_bg_ttH_dl = (TTree*)f_bg_ttH_dl->Get("ntuple");
  TTree *t_sg_tH     = (TTree*)f_sg_tH->Get("ntuple");
  //TTree *t_sg_tWH    = (TTree*)f_sg_tWH->Get("ntuple");
    
  // Baseline cuts to apply
  TCut nLep    = "numSignalLeptons == 1";
  TCut nJets   = "numSignalJets >= 4 && numSignalJets <= 5"; 
  TCut nBJets  = "Nbjets == 3"; 
  TCut nLJets  = "Nljets >= 1 && Nljets <= 3"; 
  TCut pTSum   = "pTSum >= 300"; 
  TCut fwdJEta = "fabs(mostFwdLJet_eta) > 2.4";
  
  // Pre-selection
  TCut preSel  = nLep && nJets && nBJets && nLJets && pTSum && fwdJEta;
  TCut wPreSel = preSel * "eventWeight"; // eventWeight already contains the trigger weight
  
  // Histograms from the TTree
  TH1F *h_sg_tH     = new TH1F("h_sg_tH", "", 80, 0, 4);
   
  // Now apply the cuts
  t_sg_tH->Draw("fabs(mostFwdLJet_eta) >> h_sg_tH", preSel);
      
         
  // Histograms from the TTree
  TH1F *h_sum_tH     = new TH1F("h_sum_tH", "", 1, -1.E10, 1.E10);
      
  // Sum of weights 
  t_sg_tH->Draw("eventWeight >> h_sum_tH", "eventWeight != 0");
                
  // Scale the histograms 
  //float scale_ttbar  = 3000000.*452.2944528/h_sum_ttbar->Integral();
  //float scale_ttH_sl = 3000000.*0.22276/h_sum_ttH_sl->Integral();
  //float scale_ttH_dl = 3000000.*0.05343/h_sum_ttH_dl->Integral();
  float scale_tH     = 3000000.*0.054157/h_sum_tH->Integral();
  //float scale_tWH    = 3000000.*0.014425/h_sum_tWH->Integral();
            
  // Normalize histograms
  h_sg_tH->Scale(scale_tH);
            
  
  // Histogram from TFile directly
  TH1F *h_sg_tH_stored = (TH1F*)f_sg_tH->Get("jfwd_eta_SRB3");
  
  // Normalise
  TH1F *h_sg_tH_norm     = (TH1F*)f_sg_tH->Get("events");
  scale_tH     = 3000000.*0.054157/h_sg_tH_norm->GetBinContent(2);
  h_sg_tH_stored->Scale(scale_tH);
  
  // Now plot the two files
  TCanvas c;
  c.cd();
  h_sg_tH->SetMarkerColor(kRed);
  h_sg_tH->SetLineColor(kRed); 
  h_sg_tH->SetMarkerStyle(24);
  h_sg_tH_stored->SetMarkerColor(kBlue);
  h_sg_tH_stored->SetLineColor(kBlue);
  h_sg_tH_stored->SetMarkerStyle(24);
  h_sg_tH->Draw();
  h_sg_tH_stored->Draw("same");
  c.Print("test.pdf");
 
}
