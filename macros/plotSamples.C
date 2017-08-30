#include "globals.h"

void plotSamples(TString histo="Nbjets", 
		 TString inputpath="/data/tHAnalysis/July1", 
		 TString outpath="./Plots",
		 TString label=""){
 
  //LUMI=0;//set to 0 for shape comparisons
  SetAtlasStyle();
  gStyle->SetPalette(1,0);
  setGlobals();
    

  getHistos(inputpath,m_bkgs,histo);
  //cout<<m_histos.size()<<endl;
  if(m_histos[m_bkgs[0]]==0){cout<<" m_bkgs[0] = NULL"<<endl; return;}


  /////
  float maxy=0;
  float miny=1;
  for(int b=0;b<m_bkgs.size();b++){
    if(m_histos[m_bkgs[b]]==0) continue;
    //cout<<b<<" "<<m_histos[m_bkgs[b]]<<endl;
    if(m_histos[m_bkgs[b]]->GetBinContent(m_histos[m_bkgs[b]]->GetMaximumBin())>maxy)
      maxy=m_histos[m_bkgs[b]]->GetBinContent(m_histos[m_bkgs[b]]->GetMaximumBin());
  }
  if(maxy==0.){cout<<"There are no histograms"<<endl; return;}//it means there were not histograms, or they are all empty
  
  ///



  TH1F*HFrame=(TH1F*)m_histos[m_bkgs[0]]->Clone("Hframe");
  HFrame->Scale(0);
  HFrame->GetYaxis()->SetTitle("Events");
  HFrame->GetYaxis()->SetRangeUser(miny,1.3*maxy);
  HFrame->GetXaxis()->SetTitle(histo);
  if(histo.Contains("jfwd_eta")) HFrame->GetXaxis()->SetTitle("forward light jet |#eta|");
  HFrame->Draw("hist");

  //TLegend Leg(0.73,0.6,1.0,0.93);
  TLegend Leg(0.81,0.5,1.0,0.90);
  Leg.SetBorderSize(0);
  Leg.SetLineColor(0);
  Leg.SetFillStyle(0);

  THStack stack;

  for(int b=0;b<m_bkgs.size();b++){
    if(m_histos[m_bkgs[b]]==NULL)continue;
    char txt[100];
    sprintf(txt,"#splitline{%s}{(%d)}",m_bkgs[b].c_str(),(int)(m_histos[m_bkgs[b]]->Integral() + 0.5));

    //m_histos[m_bkgs[b]]->SetLineColor(color[m_bkgs[b]]);
    //m_histos[m_bkgs[b]]->Draw("histsame");
    //Leg.AddEntry(m_histos[m_bkgs[b]],txt,"l");

    m_histos[m_bkgs[b]]->SetLineColor(color[m_bkgs[b]]);
    m_histos[m_bkgs[b]]->SetFillColor(color[m_bkgs[b]]);
    stack.Add(m_histos[m_bkgs[b]],"f");
    Leg.AddEntry(m_histos[m_bkgs[b]],txt,"f");
  }

  stack.Draw("histsame");

  // //show total signal and background
  // HBkg->SetMarkerColor(1);
  // HBkg->SetLineColor(1);
  // HBkg->Draw("histpesame");
  // HSig->SetMarkerColor(color["tH"]);
  // HSig->SetLineColor(color["tH"]);
  // HSig->Draw("histpesame");

  Leg.Draw();

  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.16,0.85,label.Data());

  DrawECMS();

  //print linear
  C.SetLogy(0);
  C.Print(outpath+"/plotSamples_"+histo+".png");
  
  ///print log
  HFrame->GetYaxis()->SetRangeUser(miny,100*maxy);
  C.Update();
  C.SetLogy(1);
  C.Print(outpath+"/plotSamples_"+histo+"_log.png");


  ////
  //Show print S/sqrt(B)
  ///
  TH1F*HSoB=(TH1F*)HSig->Clone("HSoB");
  for(int b=1;b<=HSig->GetNbinsX();b++)
    if(HBkg->GetBinContent(b)>0)
      HSoB->SetBinContent(b,HSig->GetBinContent(b)/sqrt(HBkg->GetBinContent(b)));
    else HSoB->SetBinContent(b,0);
  HSoB->GetYaxis()->SetRangeUser(0,3);
  HSoB->GetYaxis()->SetTitle("S/sqrt(B)");
  HSoB->GetXaxis()->SetTitle( HFrame->GetXaxis()->GetTitle());
  C.SetLogy(0);
  HSoB->Draw("hist");
  C.Print(outpath+"/plotSamples_"+histo+"_SoB.png");


}
