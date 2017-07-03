#include "globals.h"

void plotSamples(TString histo="Nbjets", 
		 TString inputpath="/data/tHAnalysis/July1", 
		 TString outpath="./Plots"){
  
  SetAtlasStyle();
  gStyle->SetPalette(1,0);
    
  std::vector<std::string> m_bkgs={"ttbar","tHqb","tWH","ttH_dilep","ttH_semilep"};
  std::map<std::string,int> color;
  color["ttbar"]=1;
  color["tHqb"]=2;
  color["tWH"]=3;
  color["ttH_dilep"]=4;
  color["ttH_semilep"]=6;//5 is yellow
  

  std::map<std::string,TH1F*> histos=getHistos(inputpath,m_bkgs,histo);
  
  float maxy=0;
  float miny=0.1;
  for(int b=0;b<m_bkgs.size();b++){
    if(!histos[m_bkgs[b]]) continue;
    if(histos[m_bkgs[b]]->GetBinContent(histos[m_bkgs[b]]->GetMaximumBin())>maxy)
      maxy=histos[m_bkgs[b]]->GetBinContent(histos[m_bkgs[b]]->GetMaximumBin());
  }

  TCanvas C;
  TH1F*HFrame=(TH1F*)histos[m_bkgs[0]]->Clone("Hframe");
  HFrame->Scale(0);
  HFrame->GetYaxis()->SetTitle("Events");
  HFrame->GetYaxis()->SetRangeUser(miny,1.5*maxy);
  HFrame->GetXaxis()->SetTitle(histo);
  HFrame->Draw("hist");

  TLegend Leg(0.73,0.6,1.0,0.93);
  Leg.SetBorderSize(0);
  Leg.SetLineColor(0);
  Leg.SetFillStyle(0);

  for(int b=0;b<m_bkgs.size();b++){

    if(!histos[m_bkgs[b]])continue;

    histos[m_bkgs[b]]->SetLineColor(color[m_bkgs[b]]);
    histos[m_bkgs[b]]->Draw("histsame");

    char txt[100];
    sprintf(txt,"%s (%d)",m_bkgs[b].c_str(),(int)(histos[m_bkgs[b]]->Integral()));
    Leg.AddEntry(histos[m_bkgs[b]],txt,"l");
  }

  Leg.Draw();
  C.SetLogy(0);
  C.Print(outpath+"/plotSamples_"+histo+".png");
  
  HFrame->GetYaxis()->SetRangeUser(miny,100*maxy);
  C.Update();
  C.SetLogy(1);
  C.Print(outpath+"/plotSamples_"+histo+"_log.png");

}