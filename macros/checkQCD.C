#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasStyle.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasUtils.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasLabels.C"
TString outpath="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/tHAnalysis";
TString inputpath="/data/tHAnalysis/July1";

std::vector<std::string> QCDSamples={"JZ0W","JZ1W","JZ2W","JZ3W","JZ4W","JZ5W","JZ6W","JZ7W","JZ8W","JZ9W","JZ10W","JZ11W","JZ12W"};


std::map<std::string,float> getCrossections(){
 std:map<std::string,float> xS;
  
  std::ifstream infile("tHAnalysis/data/xSections.txt");
  std::string line;
  while (std::getline(infile,line)){
    std::istringstream iss(line);
    
    char B[100]="";
    iss>>B;
    float X=0;
    iss>>X;
    xS[B]=X;
  }
  
  return xS;
}


std::map<std::string,TFile*> getSamples(){
  std::map<std::string,TFile*> samples;
  
  for(int b=0;b<QCDSamples.size();b++){
    TFile*F=new TFile(inputpath+"/tH2017_"+QCDSamples[b].c_str()+".root","read");
    if(!F) continue;
    if(F->IsZombie()) continue;
    if(!(F->Get("events"))) continue;
    samples[QCDSamples[b]]=F;
  }
  
  return samples;
}


std::map<std::string,TH1F*> getQCD(TString histoname){

  std::map<std::string,float> xs=getCrossections();
  std::map<std::string,TFile*> samples=getSamples();
  std::map<std::string,TH1F*> histos;
  for(int b=0;b<QCDSamples.size();b++){
    if(!samples[QCDSamples[b]]) continue;
    TH1F*Hevents=((TH1F*)(samples[QCDSamples[b]]->Get("events")));
    if(!Hevents) continue;
    TH1F*H=(TH1F*)samples[QCDSamples[b]]->Get(histoname.Data());
    if(!H) continue;
    cout<<QCDSamples[b].c_str()<<" "<<xs[QCDSamples[b]]<<" "<<Hevents->GetBinContent(2)<<endl;
    H->Scale(xs[QCDSamples[b].c_str()]/Hevents->GetBinContent(2));
    histos[QCDSamples[b]]=H;
  }
  
  TH1F*HTot=(TH1F*)histos[QCDSamples[0]]->Clone("HTot");
  HTot->GetXaxis()->SetTitle(histoname);
  for(int b=1;b<QCDSamples.size();b++){
    HTot->Add(histos[QCDSamples[b]]);
  }
  histos["JZW"]=HTot;

  return histos;
}

void checkQCD(TString histo="jet_pt_nocuts"){
  
  SetAtlasStyle();
  gStyle->SetPalette(1,0);

  ///Load the samples and scale the histograms
  std::map<std::string,TH1F*> histos=getQCD(histo);

  ///find max of y-scale
  float maxy=0;
  for(int b=0;b<QCDSamples.size();b++){
    if(!histos[QCDSamples[b]]) continue;
    if(histos[QCDSamples[b]]->GetBinContent(histos[QCDSamples[b]]->GetMaximumBin())>maxy)
      maxy=histos[QCDSamples[b]]->GetBinContent(histos[QCDSamples[b]]->GetMaximumBin());
  }

  ///Show the JZW composition
  TCanvas C;
  TH1F*HFrame=(TH1F*)histos[QCDSamples[0]]->Clone("Hframe");
  HFrame->Scale(0);
  HFrame->GetYaxis()->SetRangeUser(1e-5,1.3*maxy);
  HFrame->GetXaxis()->SetTitle(histo);
  HFrame->Draw("hist");
  TLegend Leg(0.7,0.6,0.9,0.9);
  for(int b=0;b<QCDSamples.size();b++){
    if(!histos[QCDSamples[b]])continue;
    histos[QCDSamples[b]]->SetLineColor(b+1);
    histos[QCDSamples[b]]->Draw("histsame");
    Leg.AddEntry(histos[QCDSamples[b]],QCDSamples[b].c_str(),"l");
  }
  Leg.Draw();
  C.SetLogy(0);
  C.Print(outpath+"/checkQCD_"+histo+".png");
  C.SetLogy(1);
  C.Print(outpath+"/checkQCD_"+histo+"_log.png");

  /////Show the total
  C.Clear();
  histos["JZW"]->Draw("hist");
  C.SetLogy(0);
  C.Print(outpath+"/checkQCD_"+histo+"_Tot.png");
  C.SetLogy(1);
  C.Print(outpath+"/checkQCD_"+histo+"_Tot_log.png");
  
}
