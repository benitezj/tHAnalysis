////////Plot the same variable from different samples to compare
#include "globals.h"

void makePlot(std::vector<TFile*> f, std::vector<TString> sample, TString outpath, TString histName, TString plotTitle, TString XaxisLabel){
  TCanvas C;
  int nSamples=sample.size(); 
  TH1D ** hist = new TH1D*[nSamples]; 

  //load histos
  for(int i=0;i<nSamples;i++){
    if(!f[i]){std::cout<<"File "<<f[i]->GetName()<<" does not exist"<<std::endl; return;}
    if(!f[i]->Get(histName)){std::cout<<histName<<" not found for "<<sample[i]<<std::endl; return;}
    hist[i] = (TH1D*)f[i]->Get(histName);
    if(hist[i]->Integral()==0){std::cout<<histName<<" integral = 0 for "<<sample[i]<<std::endl; return;}
    hist[i]->Scale(1./hist[i]->Integral()); 
    hist[i]->GetXaxis()->SetTitle(XaxisLabel);
    hist[i]->SetLineColor(i+1); 
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
  TLegend * leg = new TLegend(0.77,0.83,0.98,0.95);
  for(int i=0;i<nSamples;i++) leg->AddEntry(hist[i], sample[i]+", "+entries[i]+" entries", "l"); 
  leg->Draw();     
  C.Print(outpath+"/signalPU_"+histName+".png"); 
  delete leg;
  for(int i=0;i<nSamples;i++){
    delete hist[i]; 
  }
}

void plotSigPU(){

  //sample names and input/output paths
  std::vector<TString> sample = {"tHqb", "ttbar"};  
  int nSamples = sample.size();   
  std::vector<TFile*> f; for(int i=0;i<nSamples;i++) f.push_back(TFile::Open("/nfs/home/ehelfenb/tHAnalysis/tH2017_"+sample[i]+".root"));
  TString outpath="/afs/cern.ch/user/e/ehelfenb/www/tHAnalysis";

  //plot forward jet eta for the signal regions
  const int numSR=5;
  TString whichJet[4]={"", "2nd", "3rd", "4th"};
  for(int j=0;j<4;j++){
    TString jet=Form("%d",j+1); 
    for(int i=0;i<numSR;i++){
      TString SR=Form("%d",i); 
      SR="SRB"+SR;
      makePlot(f, sample, outpath, "fwdJet"+jet+"Eta_"+SR, SR, whichJet[j]+" most forward jet |#eta|");
    }
  } 

  //plot inclusive variables
  makePlot(f, sample, outpath, "MET", "", "MET [GeV]"); 
  makePlot(f, sample, outpath, "MET_nocuts", "", "MET (no cuts) [GeV]"); 
  makePlot(f, sample, outpath, "Njets", "", "number of jets"); 
  makePlot(f, sample, outpath, "Nbjets", "", "number of bjets"); 
  makePlot(f, sample, outpath, "Njets_nocuts", "", "number of jets (no cuts)"); 
  makePlot(f, sample, outpath, "fwdBJet1Eta", "", "most forward bjet |#eta|"); 

  for(int i=0;i<nSamples;i++){f[i]->Close(); delete f[i];}

  gROOT->ProcessLine(".q");
}
