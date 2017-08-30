#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> 
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TLine.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TEllipse.h>
#include <TLorentzVector.h>
#include <TPaveStats.h>
#include <TFrame.h>
#include <TGraph.h>

#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasStyle.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasUtils.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasLabels.C"

using namespace std;

float LUMI=3000000; //3000/fb  but in /pb (crossections are in pb)

TLatex text;

TCanvas C("C","",700,600);

void setGlobals(){
  ///canvas definition
  C.SetTopMargin(0.05);
  C.SetRightMargin(0.05);
  C.SetLeftMargin(0.15);
  C.SetBottomMargin(0.15);

  //text style ...

}


void DrawECMS(float x=0.24,float y=0.96){
  text.SetTextColor(1);
  text.SetTextSize(0.035);
  char txt[100];
  if(LUMI>0)sprintf(txt,"pp#rightarrowtHqb, H#rightarrowb#bar{b},    #sqrt{s}=13 TeV,  L = %d fb^{-1}",(int)(LUMI/1000));
  else sprintf(txt,"pp#rightarrowtHqb, H#rightarrowb#bar{b},    #sqrt{s}=13 TeV");
  text.DrawLatexNDC(x,y,txt);
}

TString FitGausFormula("[0]*exp(-0.5*(x-[1])**2/[2]**2)");
TString FitGausText("Mean=%.2f+/-%.2f,  Sigma=%.2f+/-%.2f");


std::vector<std::string> m_bkgs={"tH","tWH","ttH_dilep","ttH_semilep","ttbar"};
//std::vector<std::string> m_bkgs={"tH","ttbar"};
std::map<std::string,int> color;

std::map<std::string,TH1F*> m_histos;

///get total signal
TH1F*HSig=NULL;
///get total background
TH1F*HBkg=NULL;


//////////////Load the crossections
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



std::map<std::string,TFile*> getSamples(TString inputpath, std::vector<std::string>  samplelist){
  std::map<std::string,TFile*> files;

  for(int b=0;b<samplelist.size();b++){
    files[samplelist[b]]=NULL;

    //TFile*F=new TFile(inputpath+"/tH2017_"+samplelist[b].c_str()+".root","read");
    TFile*F=new TFile(inputpath+"/"+samplelist[b].c_str()+"/"+samplelist[b].c_str()+".root","read");
    if(!F) continue;
    if(F->IsZombie()) continue;
    if(!(F->Get("events"))) continue;
    files[samplelist[b]]=F;
  }

  return files;
}



//std::map<std::string,TH1F*> 
void getHistos(TString inputpath,std::vector<std::string> bkgs, TString histoname){

  //set colors here, this function should be called always
  color["ttbar"]=kOrange;
  color["tH"]=kRed;
  color["tWH"]=kGreen;
  color["ttH_dilep"]=kBlue;
  color["ttH_semilep"]=kPink-4;
  



  std::map<std::string,float> xs=getCrossections();
  std::map<std::string,TFile*> samples=getSamples(inputpath,bkgs);
  //std::map<std::string,TH1F*> histos;
  for(int b=0;b<bkgs.size();b++){
    m_histos[bkgs[b]]=NULL;

    ////Get the initial sum of weights
    if(!samples[bkgs[b]]) continue;
    TH1F*Hevents=((TH1F*)(samples[bkgs[b]]->Get("events")));
    if(!Hevents) continue;

    ///
    TH1F*H=(TH1F*)(samples[bkgs[b]]->Get(histoname.Data()));
    if(!H) continue;
    //cout<<bkgs[b].c_str()<<" "<<xs[bkgs[b]]<<" "<<Hevents->GetBinContent(2)<<endl;

    ///Rebin
    H->Rebin(4);

    ///Scale to cross section and lumi
    if(LUMI==0) H->Scale(1./H->Integral());
    else H->Scale(LUMI*xs[bkgs[b].c_str()]/Hevents->GetBinContent(2));

    m_histos[bkgs[b]]=H;
  }


  
  ///get total signal
  HSig=(TH1F*)m_histos["tH"]->Clone("HSig");
  if(m_histos["tWH"])HSig->Add((TH1F*)m_histos["tWH"]);


  ///get total background
  HBkg=(TH1F*)m_histos["ttbar"]->Clone("HBkg");
  if(m_histos["ttH_dilep"])HBkg->Add((TH1F*)m_histos["ttH_dilep"]);
  if(m_histos["ttH_semilep"])HBkg->Add((TH1F*)m_histos["ttH_semilep"]);

 

  //return m_histos;
}




/////////////////////////Some tools 
TH1F* makeHistRatio(TH1* HNum,TH1* HDenom,float maxerr=0.5){
  if(HNum==NULL || HDenom == NULL){ 
    cout<<"Histograms are NULL"<<endl;
    return 0;
  }
  if(HNum->GetNbinsX() != HDenom->GetNbinsX()){ 
    cout<<"Histograms bins don't match"<<endl;
    return 0;
  }
  
  TH1F* HRatio=(TH1F*)HNum->Clone("HRatio");
  for(int b=0;b<=HNum->GetNbinsX()+1;b++){
    if(HDenom->GetBinContent(b)>0. &&
       HNum->GetBinError(b)/HNum->GetBinContent(b)<maxerr
       ){
      HRatio->SetBinContent(b,HNum->GetBinContent(b)/HDenom->GetBinContent(b));
      HRatio->SetBinError(b,HNum->GetBinError(b)/HDenom->GetBinContent(b));
    }else {
      HRatio->SetBinContent(b,0.);
      HRatio->SetBinError(b,0.);
    }
  }

  return HRatio;
}

double dR_fn(float eta1, float eta2, float phi1, float phi2){
  double deta= fabs(eta1 - eta2);      double dphi= fabs(phi1 - phi2);
  if (dphi > TMath::Pi() ) dphi = 2*(TMath::Pi()) - dphi;
  return sqrt((dphi*dphi)+(deta*deta));
}




/////fit for signal 
std::pair<float,float> runFit(TString label=""){
  
  //// Generate the Asimov data
  TH1F* data=(TH1F*)HBkg->Clone("data");
  data->Add(HSig);
  for(int b=0;b<=data->GetNbinsX()+1;b++)
    data->SetBinError(b,sqrt(data->GetBinContent(b)));


  /////////////PDF's
  RooRealVar x("x","forward jet |#eta|",data->GetXaxis()->GetXmin(),data->GetXaxis()->GetXmax());
  x.setBins(data->GetNbinsX());

  RooDataHist SigDataHist("SigDataHist","",RooArgList(x),HSig);
  RooHistPdf SigPdf("SigPdf","",RooArgSet(x),SigDataHist);
  RooRealVar yieldSig("yieldSig","yieldSig",0.00001,0.1);

  // RooDataHist BkgDataHist("BkgDataHist","",RooArgList(x),HBkg);
  // RooHistPdf BkgPdf("BkgPdf","",RooArgSet(x),BkgDataHist);
  // RooAddPdf PDF("PDF","PDF",RooArgList(SigPdf,BkgPdf),RooArgList(yieldSig));

  TH1F* HttH=(TH1F*)HBkg->Clone("HttH");   
  HttH->Scale(0);
  if(m_histos["ttH_dilep"]) HttH->Add((TH1F*)m_histos["ttH_dilep"]);
  if(m_histos["ttH_semilep"]) HttH->Add((TH1F*)m_histos["ttH_semilep"]);
  RooDataHist TTHDataHist("BkgDataHist","",RooArgList(x),HttH);
  RooHistPdf TTHPdf("TTHPdf","",RooArgSet(x),TTHDataHist);
  //RooRealVar yieldTTH("yieldTTH","yieldTTH",0.00001,0.1);
  RooRealVar yieldTTH("yieldTTH","yieldTTH",HttH->Integral()/data->Integral());

  // RooRealVar N_kappa("N_kappa","Dimension of systematic variation of Nbkg",1.2);
  // RooRealVar N_beta("N_beta","This is the real nuisance on N",0.,-5.,5.);
  // RooFormulaVar N_alpha("N_alpha","pow(@0,@1)",RooArgList(N_kappa,N_beta));
  // RooFormulaVar yieldTTH_nuis("yieldTTH_nuis","@0*@1",RooArgList(yieldTTH,N_alpha));


  TH1F* Htt=(TH1F*)HBkg->Clone("Htt");   
  Htt->Scale(0);
  if(m_histos["ttbar"]) Htt->Add((TH1F*)m_histos["ttbar"]);
  RooDataHist TTDataHist("BkgDataHist","",RooArgList(x),Htt);
  RooHistPdf TTPdf("TTPdf","",RooArgSet(x),TTDataHist);
  RooFormulaVar yieldTT("yieldTT","1-yieldSig-yieldTTH",RooArgSet(yieldSig,yieldTTH));
  RooAddPdf PDF("PDF","PDF",RooArgList(SigPdf,TTHPdf,TTPdf),RooArgList(yieldSig,yieldTTH));


  ////Print the inputs
  std::cout<<"Input data : "<<setprecision(5)<<data->Integral()<<endl;
  std::cout<<"Input tt : "<<setprecision(5)<<Htt->Integral()<<endl;
  std::cout<<"Input ttH : "<<setprecision(5)<<HttH->Integral()<<endl;
  std::cout<<"Input tH : "<<setprecision(5)<<HSig->Integral()<<endl;


  ////////////Fit
  RooDataHist Data("data","data",RooArgSet(x),data);
  RooChi2Var chi2("chi","chi",PDF,Data,RooFit::DataError(RooAbsData::Poisson));
  RooMinuit minuit(chi2);
  minuit.migrad();
  std::cout<<"Fit tt: "<<setprecision(5)<<yieldTT.getVal()*data->Integral()<<"+/-"<<setprecision(3)<<sqrt(yieldSig.getError()*yieldSig.getError()+yieldTTH.getError()*yieldTTH.getError())*data->Integral()<<std::endl;
  std::cout<<"Fit ttH: "<<setprecision(5)<<yieldTTH.getVal()*data->Integral()<<"+/-"<<setprecision(3)<<yieldTTH.getError()*data->Integral()<<std::endl;
  std::cout<<"Fit tH: "<<setprecision(5)<<yieldSig.getVal()*data->Integral()<<"+/-"<<setprecision(3)<<yieldSig.getError()*data->Integral()<<std::endl;


  ///////////////// define plot
  RooPlot* plot=x.frame();
  Data.plotOn(plot);
  PDF.plotOn(plot,RooFit::Components(SigPdf),RooFit::FillColor(color["tH"]),RooFit::DrawOption("f"),RooFit::MoveToBack());
  PDF.plotOn(plot,RooFit::Components(RooArgSet(SigPdf,TTHPdf)),RooFit::FillColor(color["ttH_dilep"]),RooFit::DrawOption("f"),RooFit::MoveToBack());
  PDF.plotOn(plot,RooFit::FillColor(color["ttbar"]),RooFit::DrawOption("f"),RooFit::MoveToBack());

  plot->SetTitle("");
  plot->GetYaxis()->SetRangeUser(1,data->GetMaximum()*100);
  plot->GetYaxis()->SetTitle("Events ");
  plot->GetYaxis()->SetTitleOffset(1.2);
  plot->GetYaxis()->SetTitleSize(0.042);
  plot->GetYaxis()->SetLabelSize(0.035);
  plot->GetYaxis()->SetNdivisions(8);
  plot->GetXaxis()->SetTitle("");   
  plot->GetXaxis()->SetTitleSize(0.045);
  plot->GetXaxis()->SetLabelSize(0);//0.035);
  plot->GetXaxis()->SetNdivisions(8);


  TLegend leg(0.7,0.68,0.90,0.90);
  leg.SetFillStyle (0);
  leg.SetFillColor (0);
  leg.SetBorderSize(0);
  leg.SetTextSize(.04);

  data->SetMarkerStyle(8);
  leg.AddEntry(data,"data","pl");
  Htt->SetFillColor(color["ttbar"]);
  leg.AddEntry(Htt,"t#bar{t}","f");
  HttH->SetFillColor(color["ttH_dilep"]);
  leg.AddEntry(HttH,"ttH","f");
  HSig->SetFillColor(color["tH"]);
  leg.AddEntry(HSig,"tHqb + tWH","f");


  TCanvas C("C","",700,800);
  TPad pad1("pad1","",0,0.25,1,1);
  TPad pad2("pad2","",0,0,1,0.25);

  pad1.SetBottomMargin(0.01);
  pad2.SetTopMargin(0.05);
  pad2.SetBottomMargin(0.5);

  pad1.cd();
  pad1.SetLogy(1);
  plot->Draw();
  leg.Draw();

  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.16,0.85,label.Data());

  DrawECMS();

  pad2.cd();
  TH1F * HFit=(TH1F*)data->Clone("HFit");
  HFit->Scale(0);

  ////Show the data/fit
  //HFit=(TH1F*)PDF.fillHistogram(HFit,RooArgList(x),data->Integral());
  //TH1F* hratio= makeHistRatio(HFit,data,0.5);
  //hratio->GetYaxis()->SetRangeUser(0.8,1.2);
  //hratio->GetYaxis()->SetTitle("Data/Pred.");


  ////Show the data - bkg
  PDF.fillHistogram(HFit,RooArgList(x),data->Integral());
  TH1F * HFitSig =(TH1F*)data->Clone("HFit");
  HFitSig->Scale(0);
  SigPdf.fillHistogram(HFitSig,RooArgList(x),yieldSig.getVal()*data->Integral());
  HFit->Add(HFitSig,-1);
  TH1F* hratio   =(TH1F*)data->Clone("HDiff");
  hratio->Divide(HFit);
  hratio->GetYaxis()->SetRangeUser(1-0.05,1.+0.05);
  hratio->GetYaxis()->SetTitle("Data/Bkg");


  hratio->SetTitle("");
  hratio->SetStats(0);
  hratio->GetYaxis()->SetTitleOffset(0.35);
  hratio->GetYaxis()->SetTitleSize(0.12);
  hratio->GetYaxis()->SetLabelSize(0.10);
  hratio->GetYaxis()->SetNdivisions(5);
  hratio->GetXaxis()->SetTitle("forward light jet |#eta|");
  hratio->GetXaxis()->SetTitleOffset(1.1);
  hratio->GetXaxis()->SetTitleSize(0.18);
  hratio->GetXaxis()->SetLabelSize(0.17);
  hratio->GetXaxis()->SetNdivisions(8);
  hratio->SetMarkerStyle(8);
  hratio->Draw("histpe");
  
  TLine line;
  line.DrawLine(data->GetXaxis()->GetXmin(),1.0,data->GetXaxis()->GetXmax(),1.0);
  //line.DrawLine(data->GetXaxis()->GetXmin(),.0,data->GetXaxis()->GetXmax(),.0);

  TH1F * HSig10=(TH1F*)HSig->Clone("HSig10");
  //HSig10->Scale(10.);
  HSig10->Add(HFit);
  HSig10->Divide(HFit);
  HSig10->SetLineColor(color["tH"]);
  HSig10->SetFillColor(0);
  HSig10->Draw("histsame");

  C.Clear();
  pad1.Draw();
  pad2.Draw();
  C.Print("Fit.png");


  //return the signal yield
  std::pair<float,float> yield;
  yield.first=yieldSig.getVal()*data->Integral();
  yield.second=yieldSig.getError()*data->Integral();
  
  return yield;
}



///////////////////////////Significance calculation
std::pair<float,float> getSignificance(TString label, int method=0){
  std::pair<float,float> sig;
  sig.first=0.;
  sig.second=0.;
  
  if(HSig==NULL || HBkg == NULL){ 
    std::cout<<"Histograms are NULL"<<std::endl;
    return sig;
  }
  if(HSig->GetNbinsX() != HBkg->GetNbinsX()){ 
    std::cout<<"Histograms bins don't match"<<std::endl;
    return sig;
  }
  

  ///Calculate signifincance based on binned histogram counts
  if(method==0 || method==1){
    for(int bin=1; bin<=HSig->GetNbinsX(); bin++) {
      double iS  = HSig->GetBinContent(bin);
      double iB  = HBkg->GetBinContent(bin);
      double idS = HSig->GetBinError(bin);
      double idB = HBkg->GetBinError(bin);
      double dB = 0; //add MC stat error?

      if( iB > 0. && iS > 0.) {
	switch(method) {
	case 0:
	  {
	    double iLSB = log(1 + iS/iB); //log() = ln()
	    sig.first  += 2. * ( (iS+iB)*iLSB  -  iS );
	    sig.second += 4. * ( pow(iLSB*idS,2)  +  pow((iLSB - iS/iB)*idB,2) );
	    break;
	  }
	case 1:
	  {
	    float iSig = (iS*iS) / (iB+dB*dB);
	    sig.first  += iSig;
	    sig.second += ( pow(idS*(2*iB+iS)/(iS*(iS+iB)),2) + pow(idB/(iS+iB),2) ) * iSig*iSig;
	    break;
	  }
	default:
	  break;
	}
      }
     
    }
    
    sig.first = sqrt(sig.first);
    if(sig.first > 0.) 
      sig.second = sqrt(sig.second) / (2* sig.first);
  }
  

  ////Run a fit the distribution
  if(method==2){
    std::pair<float,float> yield=runFit(label);
    
    sig.first = HSig->Integral() / yield.second;
   
    for(int b=1; b<=HSig->GetNbinsX(); b++) 
      sig.second += HSig->GetBinError(b)*HSig->GetBinError(b);      
    sig.second = sqrt(sig.second) / yield.second;
 
  }
  
  return sig;
}

