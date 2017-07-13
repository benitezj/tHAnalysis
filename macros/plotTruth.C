

void plotTruth(){

  TChain t("CollectionTree");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000001.pool.1");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000023.pool.1");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000044.pool.1");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000055.pool.1");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000058.pool.1");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000026.pool.1");
  t.Add("/data/DAOD_TRUTH1/tH/DAOD_TRUTH1.07026535._000045.pool.1");

  gROOT->cd();

  TString outpath="./TruthPlots";
  system("mkdir ./TruthPlots");


  /////Indices identified in the truth record:
  // 2,3= colliding partons (eta=0)
  // 4 = Higgs
  // 5 = top
  // 6 = b
  // 7 = light quark  <--
  // 8,9 = colliding partons (eta=0)
  // 10 = Higgs 
  // 11 = top 
  // 12 = b 
  // 13 = g,q, fwd jet, Higgs
  //:: checked , 4 eta ~ 10 eta, 11 eta ~ 5 eta, 12 eta ~ 6 eta , but the later ones evaluate to 0 eta sometimes

  TString Eta="0.5*log((sqrt(TruthParticlesAux.px[IDX]*TruthParticlesAux.px[IDX]+TruthParticlesAux.py[IDX]*TruthParticlesAux.py[IDX]+TruthParticlesAux.pz[IDX]*TruthParticlesAux.pz[IDX])+TruthParticlesAux.pz[IDX])/(sqrt(TruthParticlesAux.px[IDX]*TruthParticlesAux.px[IDX]+TruthParticlesAux.py[IDX]*TruthParticlesAux.py[IDX]+TruthParticlesAux.pz[IDX]*TruthParticlesAux.pz[IDX])-TruthParticlesAux.pz[IDX]))";
  TString Pt="sqrt(TruthParticlesAux.px[IDX]*TruthParticlesAux.px[IDX]+TruthParticlesAux.py[IDX]*TruthParticlesAux.py[IDX])/1000";



  TCanvas C("Canvas");

  //Higgs
  TString H_eta=Eta;
  H_eta.ReplaceAll("IDX","4");
  TH1F HH_eta("HH_eta","Higgs eta",30,-6,6); HH_eta.GetXaxis()->SetTitle(" #eta ");
  t.Draw(H_eta+">>HH_eta");
  C.Print(outpath+"/H_eta.png");

  TString H_pt=Pt;
  H_pt.ReplaceAll("IDX","4");
  TH1F HH_pt("HH_pt","Higgs  pT",30,0,300); HH_pt.GetXaxis()->SetTitle("pT  [GeV]");
  t.Draw(H_pt+">>HH_pt");
  C.Print(outpath+"/H_pt.png");

  //top
  TString top_eta=Eta;
  top_eta.ReplaceAll("IDX","5");
  TH1F Htop_eta("Htop_eta","top",30,-6,6); Htop_eta.GetXaxis()->SetTitle(" #eta ");
  t.Draw((top_eta+">>Htop_eta").Data());
  C.Print(outpath+"/top_eta.png");

  TString top_pt=Pt;
  top_pt.ReplaceAll("IDX","5");
  TH1F Htop_pt("Htop_pt","top",30,0,300); Htop_pt.GetXaxis()->SetTitle("pT  [GeV]");
  t.Draw((top_pt+">>Htop_pt").Data());
  C.Print(outpath+"/top_pt.png");


  ///b-jet from gluon
  TString b_eta=Eta;
  b_eta.ReplaceAll("IDX","6");
  TH1F Hb_eta("Hb_eta","b quark",30,-6,6); Hb_eta.GetXaxis()->SetTitle(" #eta ");
  t.Draw(b_eta+">>Hb_eta");
  C.Print(outpath+"/b_eta.png");

  TString b_pt=Pt;
  b_pt.ReplaceAll("IDX","6");
  TH1F Hb_pt("Hb_pt","b quark pT",30,0,300); Hb_pt.GetXaxis()->SetTitle("pT  [GeV]");
  t.Draw(b_pt+">>Hb_pt");
  C.Print(outpath+"/b_pt.png");

  ///light fwd jet
  TString jfwd_eta=Eta;
  jfwd_eta.ReplaceAll("IDX","7");
  TH1F Hjfwd_eta("Hjfwd_eta","light quark",30,-6,6); Hjfwd_eta.GetXaxis()->SetTitle(" #eta ");
  t.Draw(jfwd_eta+">>Hjfwd_eta");
  C.Print(outpath+"/jfwd_eta.png");

  TString jfwd_pt=Pt;
  jfwd_pt.ReplaceAll("IDX","7");
  TH1F Hjfwd_pt("Hjfwd_pt","light quark",30,0,300); Hjfwd_pt.GetXaxis()->SetTitle("pT  [GeV]");
  t.Draw(jfwd_pt+">>Hjfwd_pt");
  C.Print(outpath+"/jfwd_pt.png");

  ///deltaEta
  TH1F HdEta_H_b("HdEta_H_b","",100,-6,6);
  t.Draw(TString("(")+H_eta+"-"+b_eta+")>>HdEta_H_b"); HdEta_H_b.GetXaxis()->SetTitle(" #eta_H - #eta_b ");
  C.Print(outpath+"/dEta_H_b.png");

  TH1F HdEta_top_b("HdEta_top_b","",100,-6,6);
  t.Draw(TString("(")+top_eta+"-"+b_eta+")>>HdEta_top_b"); HdEta_top_b.GetXaxis()->SetTitle(" #eta_top - #eta_b ");
  C.Print(outpath+"/dEta_top_b.png");

  TH1F HdEta_top_H("HdEta_top_H","",100,-6,6);
  t.Draw(TString("(")+top_eta+"-"+H_eta+")>>HdEta_top_H"); HdEta_top_H.GetXaxis()->SetTitle(" #eta_top - #eta_H ");
  C.Print(outpath+"/dEta_top_H.png");
  
  TH1F HdEta_jfwd_b("HdEta_jfwd_b","",100,-6,6);
  t.Draw(TString("(")+jfwd_eta+"-"+b_eta+")>>HdEta_jfwd_b"); HdEta_jfwd_b.GetXaxis()->SetTitle(" #eta_q - #eta_b ");
  C.Print(outpath+"/dEta_jfwd_b.png");


  TH2F H2DEta_bvsjfwd("H2DEta_bvsjfwd","",100,-6,6,100,-6,6);
  t.Draw(b_eta+":"+jfwd_eta+">>H2DEta_bvsjfwd","","colz"); 
  H2DEta_bvsjfwd.GetXaxis()->SetTitle(" #eta_q  ");
  H2DEta_bvsjfwd.GetYaxis()->SetTitle(" #eta_b ");
  C.Print(outpath+"/Eta2D_bvsjfwd.png");

  TH2F H2DEta_Hvsjfwd("H2DEta_Hvsjfwd","",100,-6,6,100,-6,6);
  t.Draw(H_eta+":"+jfwd_eta+">>H2DEta_Hvsjfwd","","colz"); 
  H2DEta_Hvsjfwd.GetXaxis()->SetTitle(" #eta_q  ");
  H2DEta_Hvsjfwd.GetYaxis()->SetTitle(" #eta_H ");
  C.Print(outpath+"/Eta2D_Hvsjfwd.png");



}

