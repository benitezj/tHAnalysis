#include "globals.h"

void plotSamples(TString histo="Nbjets", 
		 TString inputpath="/data/tHAnalysis/July1", 
		 TString outpath="./Plots"){
 
  //LUMI=0;//set to 0 for shape comparisons
 
  SetAtlasStyle();
  gStyle->SetPalette(1,0);
    

  std::map<std::string,TH1F*> histos=getHistos(inputpath,m_bkgs,histo);
  //cout<<histos.size()<<endl;
  if(histos[m_bkgs[0]]==0){cout<<" m_bkgs[0] = NULL"<<endl; return;}


  /////
  float maxy=0;
  float miny=0.01;
  for(int b=0;b<m_bkgs.size();b++){
    if(histos[m_bkgs[b]]==0) continue;
    //cout<<b<<" "<<histos[m_bkgs[b]]<<endl;
    if(histos[m_bkgs[b]]->GetBinContent(histos[m_bkgs[b]]->GetMaximumBin())>maxy)
      maxy=histos[m_bkgs[b]]->GetBinContent(histos[m_bkgs[b]]->GetMaximumBin());
  }
  if(maxy==0.){cout<<"There are no histograms"<<endl; return;}//it means there were not histograms, or they are all empty
  
  ///
  TCanvas C;
  TH1F*HFrame=(TH1F*)histos[m_bkgs[0]]->Clone("Hframe");
  HFrame->Scale(0);
  HFrame->GetYaxis()->SetTitle("Events");
  HFrame->GetYaxis()->SetRangeUser(miny,1.3*maxy);
  HFrame->GetXaxis()->SetTitle(histo);
  HFrame->Draw("hist");

  TLegend Leg(0.73,0.6,1.0,0.93);
  Leg.SetBorderSize(0);
  Leg.SetLineColor(0);
  Leg.SetFillStyle(0);

  for(int b=0;b<m_bkgs.size();b++){
    if(histos[m_bkgs[b]]==NULL)continue;

    histos[m_bkgs[b]]->SetLineColor(color[m_bkgs[b]]);
    histos[m_bkgs[b]]->Draw("histsame");

    char txt[100];
    sprintf(txt,"%s (%d)",m_bkgs[b].c_str(),(int)(histos[m_bkgs[b]]->Integral() + 0.5));
    Leg.AddEntry(histos[m_bkgs[b]],txt,"l");
  }

  //show total signal and background
  HBkg->SetMarkerColor(1);
  HBkg->SetLineColor(1);
  HBkg->Draw("histpesame");
  HSig->SetMarkerColor(color["tH"]);
  HSig->SetLineColor(color["tH"]);
  HSig->Draw("histpesame");

  Leg.Draw();

  ///calculate significance
  std::pair<float,float> signif=getSignificance(HSig,HBkg);
  std::cout<<signif.first<<"+/-"<<signif.second<<std::endl;


  //print linear
  C.SetLogy(0);
  C.Print(outpath+"/plotSamples_"+histo+".png");
  
  ///print log
  HFrame->GetYaxis()->SetRangeUser(miny,100*maxy);
  C.Update();
  C.SetLogy(1);
  C.Print(outpath+"/plotSamples_"+histo+"_log.png");

}
