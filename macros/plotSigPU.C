////////Plot the same variable from different samples to compare
#include "globals.h"

void makePlot(std::vector<TFile*> f, std::vector<TString> sample, std::vector<double> xsec, TString outpath, TString histName, TString plotTitle, TString XaxisLabel, TString plotType){
  TCanvas C;
  int nSamples=sample.size(); 
  TH1D ** hist = new TH1D*[nSamples]; 
  TH1D * events;
  double sumOfWeights;
  TString folder=plotType;

  //load histos
  for(int i=0;i<nSamples;i++){
    if(!f[i]){std::cout<<"File "<<f[i]->GetName()<<" does not exist"<<std::endl; return;}
    if(!f[i]->Get(histName)){std::cout<<histName<<" histogram not found for "<<sample[i]<<std::endl; return;}
    if(!f[i]->Get("events")){std::cout<<"events histogram not found for "<<sample[i]<<std::endl; return;}
    hist[i] = (TH1D*)f[i]->Get(histName);
    events = (TH1D*)f[i]->Get("events"); 
    sumOfWeights = events->GetBinContent(2); 
    if(sumOfWeights==0){std::cout<<histName<<" sumOfWeights = 0 for "<<sample[i]<<std::endl; return;}
    if(hist[i]->Integral()==0){std::cout<<histName<<" integral = 0 for "<<sample[i]<<std::endl; return;}
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
  TString label; label = (plotType=="xsec"||plotType=="log") ? "integral" : "entries";
  hist[1]->Add(hist[2]); 
  hist[3]->Add(hist[4]);
  if(plotType=="norm") for(int i=0;i<4;i++){if(i==2) continue; hist[i]->Scale(1./hist[i]->Integral());}

  hist[0]->Draw("hist"); 
  hist[1]->Draw("hist sames"); 
  hist[3]->Draw("hist sames"); 
  
  float histMax=0; for(int i=0;i<4;i++){if(i==2) continue; if(histMax<hist[i]->GetMaximum()) histMax=hist[i]->GetMaximum();}
  hist[0]->GetYaxis()->SetRangeUser(0,histMax*1.4);   
  if(plotType=="log") hist[0]->GetYaxis()->SetRangeUser(0.0000001, histMax*3);    
  if(plotType=="norm") for(int i=0;i<4;i++){if(i==2) continue;  entries.push_back(Form("%.0f", hist[i]->GetEntries()));} 
  if(plotType=="xsec"||plotType=="log") for(int i=0;i<4;i++){if(i==2) continue;  entries.push_back(Form("%.5f", hist[i]->Integral())); }
  TLegend * leg = new TLegend(0.68,0.78,0.98,0.95);
  leg->AddEntry(hist[0], "ttbar, "+label+" = "+entries[0]+"", "l"); 
  leg->AddEntry(hist[1], "ttH, "+label+" = "+entries[1], "l"); 
  leg->AddEntry(hist[3], "tH, "+label+" = "+entries[2], "l"); 
  leg->Draw();     
  if(plotType=="log") C.SetLogy(1); 
  C.Print(outpath+"/"+folder+"/"+histName+".png"); 
  if(plotType=="log") C.SetLogy(0); 
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

  //plot with crossections,logscale, or normalized to one
  TString plotType[3] = {"xsec", "log", "norm"};
  for (int k=0;k<3;k++){

    //plot forward jet eta for the b-tagged signal regions
    const int numSR=5;
    for(int i=0;i<numSR;i++){
      std::string SR = std::to_string(i);
      for(int j=0;j<1;j++){
	makePlot(f,sample,xsec,outpath,"fwdJet"+std::to_string(j)+"Eta_SRB"+SR,  "SRB"+SR, "most forward jet per event |#eta|", plotType[k]);  
	makePlot(f,sample,xsec,outpath,"fwdJet"+std::to_string(j)+"Pt_SRB"+SR,   "SRB"+SR, "most forward jet per event p_{T}", plotType[k]); 
      } 
      makePlot(f,sample,xsec,outpath,"fwdBJet1Eta_SRB"+SR,"SRB"+SR, "most forward jet per event |#eta|", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"fwdBJet1Pt_SRB"+SR, "SRB"+SR, "most forward jet per event p_{T} [GeV]",  plotType[k]); 
      makePlot(f,sample,xsec,outpath,"lep_pt_SRB"+SR,     "SRB"+SR, "lepton p_{T} [GeV]", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"lep_eta_SRB"+SR,    "SRB"+SR, "lepton #eta", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"leadJetPt_SRB"+SR,  "SRB"+SR, "leading jet p_{T} [GeV]", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"leadJetEta_SRB"+SR, "SRB"+SR, "leading jet #eta", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"leadBJetPt_SRB"+SR, "SRB"+SR, "leading bjet p_{T} [GeV]", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"leadBJetEta_SRB"+SR,"SRB"+SR, "leading bjet #eta", plotType[k]); 
      makePlot(f,sample,xsec,outpath,"Hbjets_m_SRB"+SR,   "SRB"+SR, "dijet mass closest to Higgs [GeV]", plotType[k]); 
    }
     

    //plot inclusive variables
    //makePlot(f,sample,xsec,outpath,"events",      "", "event counter", plotType[k]);                                    
    makePlot(f,sample,xsec,outpath,"NLep_nocuts",   "", "number of leptons (no cuts)", plotType[k]);                     
    makePlot(f,sample,xsec,outpath,"MET_nocuts",    "", "MET (no cuts) [GeV]", plotType[k]);                              
    makePlot(f,sample,xsec,outpath,"Njets_nocuts",  "", "number of jets (no cuts)", plotType[k]);                         
    makePlot(f,sample,xsec,outpath,"jet_pt_nocuts", "", "jet p_{T} [GeV] (no cuts)", plotType[k]);                              
    makePlot(f,sample,xsec,outpath,"lep_pt_nocuts", "", "lepton p_{T} [GeV] (no cuts)", plotType[k]);                           
    makePlot(f,sample,xsec,outpath,"MET",           "", "MET [GeV]", plotType[k]);                                        
    makePlot(f,sample,xsec,outpath,"Njets",         "", "number of jets", plotType[k]);                                   
    makePlot(f,sample,xsec,outpath,"Nbjets",        "", "number of bjets", plotType[k]);                                  
    makePlot(f,sample,xsec,outpath,"sumAllPt",      "", "sum of p_{T} [GeV] of all objects", plotType[k]);                      
    makePlot(f,sample,xsec,outpath,"lep_pt",        "", "lepton p_{T} [GeV]", plotType[k]);                                     
    makePlot(f,sample,xsec,outpath,"lep_eta",       "", "lepton #eta", plotType[k]);                                      
    makePlot(f,sample,xsec,outpath,"Hbjets_m",      "", "dijet mass closest to Higgs [GeV]", plotType[k]);

  }

  for(int i=0;i<nSamples;i++){f[i]->Close(); delete f[i];}

  gROOT->ProcessLine(".q");
}
