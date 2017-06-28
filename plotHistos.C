#include "globals.h"

void plotSignalRegions(TFile * f, TString outpath, TString histName, TString saveImageAs, TString plotTitle, TString XaxisLabel){  
  int numSR=5; //number of signal regions
  TCanvas C;
  gStyle->SetHistMinimumZero();  
  
  //create histos for different signal regions
  TH1D ** hist = new TH1D*[numSR];
  for(int i=0;i<numSR;i++){
    TString SR_idx=Form("%d",i);
    if(! f->Get(histName+SR_idx)){ 
      std::cout<<histName+SR_idx<<" not found"<<std::endl; 
      continue; 
    }
    hist[i] = (TH1D*)f->Get(histName+SR_idx);
    hist[i]->GetXaxis()->SetTitle(XaxisLabel);
    hist[i]->SetLineColor(i+1); 
    hist[i]->SetFillStyle(0); 
    hist[i]->SetStats(0); 
    hist[i]->SetTitle(plotTitle);
  } 

  //Plot histos on the same canvas
  C.Clear(); 
  float histMax=0; for(int i=0;i<numSR;i++) if(histMax<hist[i]->GetMaximum()) histMax=hist[i]->GetMaximum(); 
  hist[0]->Draw("hist");    
  hist[0]->GetYaxis()->SetRangeUser(0, histMax*1.2); 
  TLegend * leg = new TLegend(0.78,0.77,0.98,0.95);
  leg->AddEntry(hist[0], "SR1lep0b", "l"); 
  for(int i=1;i<numSR;i++){
    TString SR_idx=Form("%d",i);
    hist[i]->Draw("sames hist"); 
    leg->AddEntry(hist[i], "SR1lep"+SR_idx+"b", "l"); 
  }
  leg->Draw();   
  C.Print(outpath+"/"+saveImageAs+".png"); 
  delete leg;
  for(int i=0;i<numSR;i++) delete hist[i]; 
}

void plotHist(TFile * f, TString outpath, TString histName, TString saveImageAs, TString plotTitle, TString XaxisLabel){
  if(!f->Get(histName)){ std::cout<<histName<<" not found"<<std::endl;  return;}

  TCanvas C;
  gStyle->SetHistMinimumZero();  
  gStyle->SetOptStat("emr");

  TH1D * hist = (TH1D*)f->Get(histName); 
  hist->GetXaxis()->SetTitle(XaxisLabel); 
  hist->SetTitle(plotTitle); 
  hist->Draw("hist"); 
  C.Print(outpath+"/"+saveImageAs+".png"); 
  delete hist; 
}

void plotHist2D(TFile * f, TString outpath, TString histName, TString saveImageAs, TString plotTitle, TString XaxisLabel, TString YaxisLabel){
  if(!f->Get(histName)){ std::cout<<histName<<" not found"<<std::endl;  return;}

  TCanvas C;
  gStyle->SetHistMinimumZero();  
  gStyle->SetOptStat("emr");

  TH2D * hist = (TH2D*)f->Get(histName); 
  hist->GetXaxis()->SetTitle(XaxisLabel); 
  hist->GetYaxis()->SetTitle(YaxisLabel); 
  hist->SetTitle(plotTitle); 
  hist->Draw("colz"); 
  C.Print(outpath+"/"+saveImageAs+".png"); 
  delete hist; 
}

void plotHistos(){
  TString sample[3]={"tH", "tWH", "ttbar"};
  
  for(int i=0;i<3;i++){

    TString outpath="/afs/cern.ch/user/e/ehelfenb/www/tHAnalysis/"+sample[i];
    TString rootFile="/nfs/home/ehelfenb/tHAnalysis/"+sample[i]+"2017.root";
    TFile *f = TFile::Open(rootFile); 

    plotSignalRegions(f,outpath,"tH2017__1stfwdJetEta", "1stfwdJetEta", "", "most forward jet |#eta|");
    plotSignalRegions(f,outpath,"tH2017__2ndfwdJetEta", "2ndfwdJetEta", "", "2nd most forward jet |#eta|");
    plotSignalRegions(f,outpath,"tH2017__3rdfwdJetEta", "3rdfwdJetEta", "", "3rd most forward jet |#eta|");

    plotSignalRegions(f,outpath,"tH2017__1stfwdBJetEta", "1stfwdBJetEta", "", "most forward bjet |#eta|");
    plotSignalRegions(f,outpath,"tH2017__2ndfwdBJetEta", "2ndfwdBJetEta", "", "2nd most forward bjet |#eta|");
    plotSignalRegions(f,outpath,"tH2017__3rdfwdBJetEta", "3rdfwdBJetEta", "", "3rd most forward bjet |#eta|");

    plotHist(f,outpath,"tH2017__Njets_nocuts", "Njets_nocuts", "","number of jets (no cuts)");
    plotHist(f,outpath,"tH2017__Njets", "Njets", "", "number of jets");
    plotHist(f,outpath,"tH2017__Nbjets_nocuts", "Nbjets_nocuts", "", "number of bjets (no cuts)");
    plotHist(f,outpath,"tH2017__Nbjets", "Nbjets", "", "number of bjets");
    plotHist(f,outpath,"tH2017__MET", "MET", "", "MET [GeV]");

    f->Close();
    delete f; 
  }
  gROOT->ProcessLine(".q");
}
