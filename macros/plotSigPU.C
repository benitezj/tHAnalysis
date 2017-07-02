////////Plot the same variable from different samples to compare
#include "globals.h"

void makePlot(std::vector<TFile*> f, std::vector<TString> sample, std::vector<double> xsec, TString outpath, TString histName, TString plotTitle, TString XaxisLabel, bool log){
  TCanvas C;
  int nSamples=sample.size(); 
  TH1D ** hist = new TH1D*[nSamples]; 
  TH1D * events;
  double sumOfWeights;
  TString LOG=""; if(log) LOG="_log";
  TString folder=""; if(log) folder="/LogScale";

  //load histos
  for(int i=0;i<nSamples;i++){
    if(!f[i]){std::cout<<"File "<<f[i]->GetName()<<" does not exist"<<std::endl; return;}
    if(!f[i]->Get(histName)){std::cout<<histName<<" histogram not found for "<<sample[i]<<std::endl; return;}
    if(!f[i]->Get("events")){std::cout<<"events histogram not found for "<<sample[i]<<std::endl; return;}
    hist[i] = (TH1D*)f[i]->Get(histName);
    events = (TH1D*)f[i]->Get("events"); 
    sumOfWeights = events->GetBinContent(2); 
    if(sumOfWeights==0){std::cout<<histName<<" sumOfWeights = 0 for "<<sample[i]<<std::endl; return;}
    hist[i]->Scale(xsec.at(i)/sumOfWeights); 
    hist[i]->GetXaxis()->SetTitle(XaxisLabel);
    hist[i]->SetLineColor(i+1); 
    hist[i]->SetLineWidth(2); 
    hist[i]->SetFillStyle(0); 
    hist[i]->SetStats(0); 
    hist[i]->SetTitle(plotTitle);
    //delete events;
  }

  //Plot histos
  C.Clear();
  std::vector<TString> entries;
  float histMax=0; for(int i=0;i<nSamples;i++) if(histMax<hist[i]->GetMaximum()) histMax=hist[i]->GetMaximum(); 
  hist[0]->Draw("hist");    
  hist[0]->GetYaxis()->SetRangeUser(0.00000001, histMax*1.2);    
  entries.push_back(Form("%.5f", hist[0]->Integral())); 
  hist[1]->Add(hist[2]); 
  hist[3]->Add(hist[4]);
  hist[1]->Draw("sames hist"); entries.push_back(Form("%.5f", hist[1]->Integral()));
  hist[3]->Draw("sames hist"); entries.push_back(Form("%.5f", hist[3]->Integral()));
  TLegend * leg = new TLegend(0.68,0.78,0.98,0.95);
  leg->AddEntry(hist[0], "ttbar, integral = "+entries[0]+"", "l"); 
  leg->AddEntry(hist[1], "ttH, integral = "+entries[1], "l"); 
  leg->AddEntry(hist[3], "tH, integral = "+entries[2], "l"); 
  leg->Draw();     
  C.SetLogy((int)log); 
  C.Print(outpath+folder+"/"+histName+".png"); 
  C.SetLogy(0); 
  delete leg;
  for(int i=0;i<nSamples;i++) delete hist[i]; 
  delete events; 
}

void plotSigPU(){

  //sample names and input/output paths
  std::vector<TString> sample = {"ttbar", "ttH_dilep", "ttH_semilep", "tHqb", "tWH"};  
  std::vector<double> xsec={452.2944528, 0.05343, 0.22276, 0.054157, 0.27*0.054157};//tWH xsec not exact
  int nSamples = sample.size();   
  std::vector<TFile*> f; for(int i=0;i<nSamples;i++) f.push_back(TFile::Open("runJob_"+sample[i]+"/tH2017_"+sample[i]+".root"));//"/data/tHAnalysis/July1/tH2017_"+sample[i]+".root"));
  TString outpath="/afs/cern.ch/user/e/ehelfenb/www/tHAnalysis/SignalPileup";

  //plot once without log scale, once with log scale
  for (int log=0;log<2;log++){

    //plot forward jet eta for the b-tagged signal regions
    const int numSR=5;
    for(int i=0;i<numSR;i++){
      std::string SR = std::to_string(i);
      for(int j=0;j<1;j++){
	makePlot(f,sample,xsec,outpath+"/ForwardJets","fwdJet"+std::to_string(j)+"Eta_SRB"+SR,  "SRB"+SR, "most forward jet per event |#eta|", (bool)log);  
	makePlot(f,sample,xsec,outpath+"/ForwardJets","fwdJet"+std::to_string(j)+"Pt_SRB"+SR,   "SRB"+SR, "most forward jet per event p_{T}", (bool)log); 
      } 
      makePlot(f,sample,xsec,outpath,"fwdBJet1Eta_SRB"+SR,"SRB"+SR, "most forward jet per event |#eta|", (bool)log); 
      makePlot(f,sample,xsec,outpath,"fwdBJet1Pt_SRB"+SR, "SRB"+SR, "most forward jet per event p_{T} [GeV]",  (bool)log); 
      makePlot(f,sample,xsec,outpath,"lep_pt_SRB"+SR,     "SRB"+SR, "lepton p_{T} [GeV]", (bool)log); 
      makePlot(f,sample,xsec,outpath,"lep_eta_SRB"+SR,    "SRB"+SR, "lepton #eta", (bool)log); 
      makePlot(f,sample,xsec,outpath,"leadJetPt_SRB"+SR,  "SRB"+SR, "leading jet p_{T} [GeV]", (bool)log); 
      makePlot(f,sample,xsec,outpath,"leadJetEta_SRB"+SR, "SRB"+SR, "leading jet #eta", (bool)log); 
      makePlot(f,sample,xsec,outpath,"leadBJetPt_SRB"+SR, "SRB"+SR, "leading bjet p_{T} [GeV]", (bool)log); 
      makePlot(f,sample,xsec,outpath,"leadBJetEta_SRB"+SR,"SRB"+SR, "leading bjet #eta", (bool)log); 
      makePlot(f,sample,xsec,outpath,"Hbjets_m_SRB"+SR,   "SRB"+SR, "dijet mass closest to Higgs [GeV]", (bool)log); 
    }
     

    //plot inclusive variables
    //makePlot(f,sample,xsec,outpath,"events",      "", "event counter", (bool)log);                                    
    makePlot(f,sample,xsec,outpath,"NLep_nocuts",   "", "number of leptons (no cuts)", (bool)log);                     
    makePlot(f,sample,xsec,outpath,"MET_nocuts",    "", "MET (no cuts) [GeV]", (bool)log);                              
    makePlot(f,sample,xsec,outpath,"Njets_nocuts",  "", "number of jets (no cuts)", (bool)log);                         
    makePlot(f,sample,xsec,outpath,"jet_pt_nocuts", "", "jet p_{T} [GeV] (no cuts)", (bool)log);                              
    makePlot(f,sample,xsec,outpath,"lep_pt_nocuts", "", "lepton p_{T} [GeV] (no cuts)", (bool)log);                           
    makePlot(f,sample,xsec,outpath,"MET",           "", "MET [GeV]", (bool)log);                                        
    makePlot(f,sample,xsec,outpath,"Njets",         "", "number of jets", (bool)log);                                   
    makePlot(f,sample,xsec,outpath,"Nbjets",        "", "number of bjets", (bool)log);                                  
    makePlot(f,sample,xsec,outpath,"sumAllPt",      "", "sum of p_{T} [GeV] of all objects", (bool)log);                      
    makePlot(f,sample,xsec,outpath,"lep_pt",        "", "lepton p_{T} [GeV]", (bool)log);                                     
    makePlot(f,sample,xsec,outpath,"lep_eta",       "", "lepton #eta", (bool)log);                                      
    makePlot(f,sample,xsec,outpath,"Hbjets_m",      "", "combined mass dijets closest to higgs mass [GeV]", (bool)log);

  }

  for(int i=0;i<nSamples;i++){f[i]->Close(); delete f[i];}

  gROOT->ProcessLine(".q");
}
