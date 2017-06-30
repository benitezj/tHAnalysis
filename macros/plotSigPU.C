////////Plot the same variable from different samples to compare
#include "globals.h"

void makePlot(std::vector<TFile*> f, std::vector<TString> sample, TString outpath, TString histName, TString plotTitle, TString XaxisLabel, bool normalized){
  TCanvas C;
  int nSamples=sample.size(); 
  TH1D ** hist = new TH1D*[nSamples]; 

  //load histos
  for(int i=0;i<nSamples;i++){
    if(!f[i]){std::cout<<"File "<<f[i]->GetName()<<" does not exist"<<std::endl; return;}
    if(!f[i]->Get(histName)){std::cout<<histName<<" not found for "<<sample[i]<<std::endl; return;}
    hist[i] = (TH1D*)f[i]->Get(histName);
    if(hist[i]->Integral()==0){std::cout<<histName<<" integral = 0 for "<<sample[i]<<std::endl; return;}
    if(normalized==1) hist[i]->Scale(1./hist[i]->Integral()); 
    hist[i]->GetXaxis()->SetTitle(XaxisLabel);
    hist[i]->SetLineColor(i+1); 
    hist[i]->SetLineWidth(2); 
    hist[i]->SetFillStyle(0); 
    hist[i]->SetStats(0); 
    hist[i]->SetTitle(plotTitle);
  }

  //Plot histos
  C.Clear();
  std::vector<TString> entries;
  float histMax=0; for(int i=0;i<nSamples;i++) if(histMax<hist[i]->GetMaximum()) histMax=hist[i]->GetMaximum(); 
  hist[0]->Draw("hist");    
  hist[0]->GetYaxis()->SetRangeUser(0, histMax*1.2);    
  entries.push_back(Form("%.0f", hist[0]->GetEntries())); 
  for(int i=1;i<nSamples;i++){ hist[i]->Draw("sames hist"); entries.push_back(Form("%.0f", hist[i]->GetEntries()));}
  TLegend * leg = new TLegend(0.68,0.78,0.98,0.95);
  for(int i=0;i<nSamples;i++) leg->AddEntry(hist[i], sample[i]+", "+entries[i]+" entries", "l"); 
  leg->Draw();     
  if(normalized==0) C.Print(outpath+"/signalPU_"+histName+".png"); 
  if(normalized==1) C.Print(outpath+"/signalPU_"+histName+"_normalized.png"); 
  delete leg;
  for(int i=0;i<nSamples;i++) delete hist[i]; 

}

void plotSigPU(){

  //sample names and input/output paths
  std::vector<TString> sample = {"tHqb", "ttbar", "tWH", "ttH_dilep", "ttH_semilep"};  
  int nSamples = sample.size();   
  std::vector<TFile*> f; for(int i=0;i<nSamples;i++) f.push_back(TFile::Open("/data/tHAnalysis/June30/tH2017_"+sample[i]+".root"));
  TString outpath="/afs/cern.ch/user/e/ehelfenb/www/tHAnalysis";

  //make normalized and non-normalized plots
  for(int norm=0;norm<2;norm++){
    //plot forward jet eta for the signal regions
    const int numSR=5;
    TString whichJet[4]={"", "2nd", "3rd", "4th"};
    for(int j=0;j<4;j++){
      TString jet=Form("%d",j+1); 
      for(int i=0;i<numSR;i++){
	TString SR=Form("%d",i); 
	SR="SRB"+SR;
	makePlot(f, sample, outpath, "fwdJet"+jet+"Eta_"+SR, SR, whichJet[j]+" most forward jet |#eta|", (bool)norm);
	if(j==0) makePlot(f, sample, outpath, "fwdBJet"+jet+"Eta_"+SR, SR, whichJet[j]+" most forward bjet |#eta|", (bool)norm);
      }
    } 

    //plot inclusive variables
    makePlot(f, sample, outpath, "MET", "", "MET [GeV]", (bool)norm); 
    makePlot(f, sample, outpath, "MET_nocuts", "", "MET (no cuts) [GeV]", (bool)norm); 
    makePlot(f, sample, outpath, "Njets", "", "number of jets", (bool)norm); 
    makePlot(f, sample, outpath, "Njets_nocuts", "", "number of jets (no cuts)", (bool)norm); 
    makePlot(f, sample, outpath, "Nbjets", "", "number of bjets", (bool)norm); 
    makePlot(f, sample, outpath, "NLep_nocuts", "", "number of leptons (no cuts)", (bool)norm); 
    makePlot(f, sample, outpath, "pTL", "", "lepton p_{T} [GeV]", (bool)norm); 
  }

  for(int i=0;i<nSamples;i++){f[i]->Close(); delete f[i];}

  gROOT->ProcessLine(".q");
}
