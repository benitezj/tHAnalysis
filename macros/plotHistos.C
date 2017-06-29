////////Plot inclusive variables for a single sample
#include "globals.h"

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

    plotHist(f,outpath,"Njets", "", "number of jets");
    plotHist(f,outpath,"Nbjets", "", "number of bjets");
    plotHist(f,outpath,"MET", "", "MET [GeV]");

    f->Close();
    delete f; 
  }
  gROOT->ProcessLine(".q");
}

