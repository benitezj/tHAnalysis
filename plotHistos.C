#include "globals.h"

void plotSignalRegions(TFile * f, TString outpath, TString histName, TString plotTitle, TString XaxisLabel){  
  int numSR=5; //number of signal regions
  
  //create histos for different signal regions
  TH1D ** hist = new TH1D*[numSR];
  for(int i=0;i<numSR;i++){
    TString SR_idx=Form("%d",i);
    TString fullHistName = histName+"_SRB"+SR_idx; 
    if(!f->Get(fullHistName)){ 
      std::cout<<fullHistName<<" not found"<<std::endl; 
      return; 
    }
    hist[i] = (TH1D*)f->Get(fullHistName);
    hist[i]->GetXaxis()->SetTitle(XaxisLabel);
    hist[i]->SetLineColor(i+1); 
    hist[i]->SetFillStyle(0); 
    hist[i]->SetStats(0); 
    hist[i]->SetTitle(plotTitle);
  } 

  //Plot histos
  gStyle->SetHistMinimumZero();   
  TCanvas C;
  float histMax=0; for(int i=0;i<numSR;i++) if(histMax<hist[i]->GetMaximum()) histMax=hist[i]->GetMaximum(); 
  hist[0]->Draw("hist");    
  hist[0]->GetYaxis()->SetRangeUser(0, histMax*1.2); 
  for(int i=1;i<numSR;i++) hist[i]->Draw("sames hist"); 
  
  TLegend * leg = new TLegend(0.78,0.77,0.98,0.95);
  for(int i=0;i<numSR;i++){TString SR_idx=Form("%d",i); leg->AddEntry(hist[i], "SRB"+SR_idx, "l");}
  leg->Draw();   
  
  C.Print(outpath+"/"+histName+".png"); 
  delete leg;
  for(int i=0;i<numSR;i++) delete hist[i]; 
}

void plotHist(TFile * f, TString outpath, TString histName, TString plotTitle, TString XaxisLabel){
  if(!f->Get(histName)){ std::cout<<histName<<" not found"<<std::endl;  return;}

  TCanvas C;
  gStyle->SetHistMinimumZero();  
  gStyle->SetOptStat("emr");

  TH1D * hist = (TH1D*)f->Get(histName); 
  hist->GetXaxis()->SetTitle(XaxisLabel); 
  hist->SetTitle(plotTitle); 
  hist->Draw("hist"); 
  C.Print(outpath+"/"+histName+".png"); 
  delete hist; 
}

void plotHist2D(TFile * f, TString outpath, TString histName, TString plotTitle, TString XaxisLabel, TString YaxisLabel){
  if(!f->Get(histName)){ std::cout<<histName<<" not found"<<std::endl;  return;}

  TCanvas C;
  gStyle->SetHistMinimumZero();  
  gStyle->SetOptStat("emr");

  TH2D * hist = (TH2D*)f->Get(histName); 
  hist->GetXaxis()->SetTitle(XaxisLabel); 
  hist->GetYaxis()->SetTitle(YaxisLabel); 
  hist->SetTitle(plotTitle); 
  hist->Draw("colz"); 
  C.Print(outpath+"/"+histName+".png"); 
  delete hist; 
}

void plotHistos(){
  TString sample[3]={"tHqb", "ttbar", "tWH"};  
  for(int i=0;i<2;i++){
    
    TString inpath="/nfs/home/ehelfenb/tHAnalysis/tH2017_"+sample[i]+".root";
    TString outpath="/afs/cern.ch/user/e/ehelfenb/www/tHAnalysis/"+sample[i];
    TFile *f = TFile::Open(inpath);
    if(!f){std::cout<<"File "<<inpath<<" does not exist"<<std::endl; continue;}

    plotSignalRegions(f,outpath,"fwdJet1Eta", "", "most forward jet |#eta|");
    plotSignalRegions(f,outpath,"fwdJet2Eta", "", "2nd most forward jet |#eta|");
    plotSignalRegions(f,outpath,"fwdJet3Eta", "", "3rd most forward jet |#eta|");
    plotSignalRegions(f,outpath,"fwdJet4Eta", "", "4th most forward jet |#eta|");

    plotHist(f,outpath,"Njets", "", "number of jets");
    plotHist(f,outpath,"MET", "", "MET [GeV]");

    f->Close();
    delete f; 
  }
  gROOT->ProcessLine(".q");
}

