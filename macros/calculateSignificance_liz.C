#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include <vector>
#include <utility>

//Make a class for the setups that stores LLRs
class Setup {
  std::pair<float,float> llr,llr_SRB3,llr_SRB4;
  std::vector<std::vector<float>> llrPerBinSRB3, llrPerBinSRB4; 
  TString dir; 
public:
  Setup (TString); 
  void SetLLR(std::pair<float,float> LLR){llr = LLR;}
  void SetLLR_SRB3(std::pair<float,float> LLR){llr_SRB3 = LLR;}
  void SetLLR_SRB4(std::pair<float,float> LLR){llr_SRB4 = LLR;}
  void SetLLRPerBin_SRB3(std::vector<std::vector<float>> LLR){llrPerBinSRB3 = LLR;}
  void SetLLRPerBin_SRB4(std::vector<std::vector<float>> LLR){llrPerBinSRB4 = LLR;}
  void SetDir(TString DIR){dir = DIR;}
  TString GetDir(){return dir;}
  std::pair<float,float> GetLLR(void){return llr;}
  std::pair<float,float> GetLLR_SRB3(void){return llr_SRB3;}
  std::pair<float,float> GetLLR_SRB4(void){return llr_SRB4;}
  std::vector<std::vector<float>> GetLLRPerBin_SRB3(void){return llrPerBinSRB3;}
  std::vector<std::vector<float>> GetLLRPerBin_SRB4(void){return llrPerBinSRB4;}
};

Setup::Setup(TString DIR){
  dir = DIR; 
}

// Get setups with the same HS/PU efficiency
std::vector<Setup*> GetSameEffSetups(std::vector<Setup*> mySetups, TString myStr){
  std::vector<Setup*> sameEffSetups; 
  for(auto s: mySetups){
    TString DIR = s->GetDir(); 
    if(DIR.Contains(myStr)) sameEffSetups.push_back(s); 
  }
  if(sameEffSetups.size()<3){std::cout<<"sameEffSetups < 3 for string "<<myStr<<std::endl; return sameEffSetups;}
  else{
    TString DIR1 = sameEffSetups.at(0)->GetDir();
    TString DIR2 = sameEffSetups.at(1)->GetDir();
    TString DIR3 = sameEffSetups.at(2)->GetDir();
    
    if(DIR1.Contains("HGTD")||DIR2.Contains("purej")) std::cout<<"error in sameEffSetups order"<<std::endl; 
  }
  return sameEffSetups; 
}

// Get a particular setup by specifying the directory name
Setup * GetSetup(std::vector<Setup*> mySetups, TString search){ 
  Setup * mySetup; 
  int numSetups=0;
  for(auto s: mySetups){
    TString DIR = s->GetDir(); 
    if(DIR.Contains(search)){ mySetup = (Setup*)s; break;}
  }
  return mySetup; 
}


float getMaxErrorHist(const TH1F *h) {

  const unsigned int nbins = h->GetNbinsX();
  
  // First calculate maximum relative error in each bin
  std::vector<float> errors;
  for (unsigned int b=1; b < nbins+1; ++b) {
    if (h->GetBinContent(b) != 0) errors.push_back(h->GetBinError(b)/h->GetBinContent(b));
  }
  float maxErr = *std::max_element(errors.begin(), errors.end());

  return maxErr;
}


int getRebinFactor(const TH1F *bg, float rebinThreshold) {

  int rebinFactor = 1;
  const unsigned int nbins = bg->GetNbinsX();
  
  // First calculate maximum relative error in each bin
  float maxErr = getMaxErrorHist(bg);
  
  // Rebin until maxErr falls below threshold
  while (maxErr > rebinThreshold) {
  
    // First make a clone of the histogram
    TH1F *htemp = (TH1F*)bg->Clone();
    
    // Increment rebin factor by 1
    ++rebinFactor;
    
    // Rebin histogram
    htemp->Rebin(rebinFactor);
    
    // Recalculate maximum error
    maxErr = getMaxErrorHist(htemp);
  
    // Delete htemp
    delete htemp;
  }
  
  //std::cout << "Rebin factor = " << rebinFactor << std::endl;  
  return rebinFactor;
}


//Functions for calculating xsecs for tH, tWH
float ftH(float x){
  float kt = .2086 - .338950*x + .184450*x*x;
  return kt; 
}
float ftWH(float x){
  float kt = .0428000 - .0627000*x + .0449000*x*x;
  return kt; 
}


std::pair<float,float> LLR(const TH1F *hSig, const TH1F *hBkg, std::vector<std::vector<float>>& LLR_perBin) {

  std::pair<float,float> LLR={0.,0.};
  
  const unsigned int nbins = hSig->GetNbinsX();
  for (unsigned int b=1; b < nbins+1; ++b) {
    double iS = hSig->GetBinContent(b);
    double iB = hBkg->GetBinContent(b);
    double idS, idB;
    idS = hSig->GetBinError(b); idB = hBkg->GetBinError(b);
    if( iB > 0 && iS > 0) {
      double iLSB = log(1 + iS/iB);
      float perbin1 = 2 * ( (iS+iB) * iLSB - iS );
      float perbin2 = iLSB * iLSB * idS * idS;
      float perbin3 = (iLSB - iS/iB) * (iLSB - iS/iB) * idB * idB;
      LLR_perBin.push_back({perbin1, perbin2, perbin3}); 
      LLR.first  += 2 * ( (iS+iB) * iLSB - iS );
      LLR.second += iLSB * iLSB * idS * idS + (iLSB - iS/iB) * (iLSB - iS/iB) * idB * idB;
    }
  }
  LLR.first = sqrt(LLR.first);
  if(LLR.first > 0){
    LLR.second = sqrt(LLR.second) / LLR.first;
      
  }
  return LLR;
}

std::pair<float,float> calculateSignificanceLoop(const TString& setup, const TString& dist, float kt, std::vector<std::vector<float>>& LLR_perBin) {
  
  // Directory where input root files are stored
  TString dir = "../OutputRootFiles/August7";

  // samples
  TFile *f_bg_ttbar  = TFile::Open(TString::Format("%s/%s/ttbar/ttbar.root", dir.Data(), setup.Data()), "read");
  TFile *f_bg_ttH_sl = TFile::Open(TString::Format("%s/%s/ttH_semilep/ttH_semilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_bg_ttH_dl = TFile::Open(TString::Format("%s/%s/ttH_dilep/ttH_dilep.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tH     = TFile::Open(TString::Format("%s/%s/tH/tH.root", dir.Data(), setup.Data()), "read");
  TFile *f_sg_tWH    = TFile::Open(TString::Format("%s/%s/tWH/tWH.root", dir.Data(), setup.Data()), "read");
    
  // Normalization
  TH1F *h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get("events");
  TH1F *h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get("events");
  TH1F *h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get("events");
  TH1F *h_sg_tH     = (TH1F*)f_sg_tH->Get("events");
  TH1F *h_sg_tWH    = (TH1F*)f_sg_tWH->Get("events");
  
  // Change xsec depending on value of k_t

  float k1 = 1;// ftH(kt);
  float k2 = 1; //ftWH(kt);
  //std::cout<<"\n k1 = "<<k1<<std::endl;
  //std::cout<<"\n k2 = "<<k2<<std::endl;

  float scale_ttbar  = 3000000.*452.2944528/h_bg_ttbar->GetBinContent(2);
  float scale_ttH_sl = 3000000.*0.22276/h_bg_ttH_sl->GetBinContent(2);
  float scale_ttH_dl = 3000000.*0.05343/h_bg_ttH_dl->GetBinContent(2);
  float scale_tH     = /*3000000.*k1/h_sg_tH->GetBinContent(2);*/ /*k1*3000000.*0.054157/h_sg_tH->GetBinContent(2);*/ k1*3000000.*0.04284/h_sg_tH->GetBinContent(2);
  float scale_tWH    = /*3000000.*k2/h_sg_tWH->GetBinContent(2);*/ /*k2*3000000.*0.014425/h_sg_tWH->GetBinContent(2);*/ k2*3000000.*0.014425/h_sg_tWH->GetBinContent(2);

  // Get Histograms 
  h_bg_ttbar  = (TH1F*)f_bg_ttbar->Get(TString::Format("%s", dist.Data()));
  h_bg_ttH_sl = (TH1F*)f_bg_ttH_sl->Get(TString::Format("%s", dist.Data()));
  h_bg_ttH_dl = (TH1F*)f_bg_ttH_dl->Get(TString::Format("%s", dist.Data()));
  h_sg_tH     = (TH1F*)f_sg_tH->Get(TString::Format("%s", dist.Data()));
  h_sg_tWH    = (TH1F*)f_sg_tWH->Get(TString::Format("%s", dist.Data()));

  if (!h_bg_ttbar) return {};
    
  // Here we should normalize the histograms according to xsec and add tH+tWH in a single histogram and all bg in another histogram
  h_bg_ttbar->Scale(scale_ttbar);
  h_bg_ttH_sl->Scale(scale_ttH_sl);
  h_bg_ttH_dl->Scale(scale_ttH_dl);
  h_sg_tH->Scale(scale_tH);
  h_sg_tWH->Scale(scale_tWH);
    
  h_bg_ttbar->Add(h_bg_ttH_sl);
  h_bg_ttbar->Add(h_bg_ttH_dl);
  h_sg_tH->Add(h_sg_tWH);
  

  // Now perform a dynamic rebining
  // The histograms are rebinned so that the sum of backgrounds has a relative error 
  // of less than thr%  in each bin
  float thr = 10;
  int rebin = getRebinFactor(h_bg_ttbar, thr*0.01);
  h_bg_ttbar->Rebin(rebin);
  h_sg_tH->Rebin(rebin);
  
  std::pair<float,float> llr = LLR(h_sg_tH, h_bg_ttbar, LLR_perBin);
  //printf("Distribution:  %35s   -  LLR = %6.5f +- %6.5f\n", dist.Data(), llr.first, llr.second); 
    
  return llr;
}

void calculateSignificance_liz() {

  // Suppress warnings
  gErrorIgnoreLevel = kError;

  // Different setups used
  std::vector<TString> setupDirs = {//"mu0",                                     
				       	            //"mu200_noPUJets_TC_PU0.02",		        
					            //"mu200_noTC",			        
						    "mu200_TC_PU0.02",		      
						    "mu200_TC_PU0.05",		      
						    "mu200_TC_PU0.10",		      
						    "mu200_TC_HS0.70",		      
						    "mu200_TC_HS0.80",		      
						    "mu200_TC_HS0.90",
						    "mu200_TC_PU0.02_HGTD",		      
						    "mu200_TC_PU0.05_HGTD",		      
						    "mu200_TC_PU0.10_HGTD",		      
						    "mu200_TC_HS0.70_HGTD",		      
						    "mu200_TC_HS0.80_HGTD",		      
						    "mu200_TC_HS0.90_HGTD",	      
						    "mu200_TC_PU0.02_HGTD_HGTDpuRejx2",	      
						    "mu200_TC_PU0.05_HGTD_HGTDpuRejx2",	      
						    "mu200_TC_PU0.10_HGTD_HGTDpuRejx2",	      
						    "mu200_TC_HS0.70_HGTD_HGTDpuRejx2",	      
						    "mu200_TC_HS0.80_HGTD_HGTDpuRejx2",	      
						    "mu200_TC_HS0.90_HGTD_HGTDpuRejx2"};
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej",
						    // "mu200_TC_PU0.05_HGTD_purej",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej_fwdEle",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej_fwdEle",
						    // "mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej_fwdEle"};
  
  // Make a vector with all the setups
  const int n = setupDirs.size();
  Setup ** setup = new Setup*[n];
  std::vector<Setup*> mySetups;  

  for(unsigned int i=0; i < setupDirs.size(); ++i){
    setup[i] = new Setup(setupDirs.at(i)); 
    mySetups.push_back(setup[i]); 
   }

  

  // Distributions to use for calculating significance
  std::vector<TString> dists = {"jfwd_eta_SRB3",
				"jfwd_eta_SRB4"};
                                 //"lep_eta_SRB3", "lep_eta_SRB4"}; 
				 //"jfwd_eta_SRMbbH3_SRB3",
				 //"jfwd_eta_SRMbbH3_SRB4"};

  // Option to rebin histograms
  TString binScheme="_rebinned";

  // Create text file with table of significances
  //std::ofstream output("macros/significanceTables_"+binScheme+kappaT+".txt");
    
  // Calculate LLRs
  for (float kt=1.;kt<1.5;kt+=0.5){
    printf("k_t = %.1f\n", kt);
    for (unsigned int i=0; i < setupDirs.size(); ++i ) {
      auto s = setupDirs.at(i);
      std::cout << "Calculating LLR for : " << s << std::endl;
      std::pair<float,float> sig, sigB3, sigB4, sigB3_Mbb, sigB4_Mbb;   
           
      for(auto d : dists) {

	std::vector<std::vector<float>> llrPerBin; 
	sig = calculateSignificanceLoop(s, d, kt, llrPerBin);
	
	if(d.Contains("SRB3")){
	  sigB3 = sig; 
	  mySetups.at(i)->SetLLRPerBin_SRB3(llrPerBin);
	}
	else if (d.Contains("SRB4")){
	  sigB4 = sig;
	  mySetups.at(i)->SetLLRPerBin_SRB4(llrPerBin);
	}
      	
      }
	
      float sum  = std::sqrt(std::pow(sigB3.first,2)+std::pow(sigB4.first,2));
      float sumE = std::sqrt(std::pow(sigB3.first*sigB3.second,2)+std::pow(sigB4.first*sigB4.second,2))/sum;


      printf("Distribution:  %35s   -  LLR = %6.7f +- %6.7f\n", "Sum", sum, sumE); 
      
      mySetups.at(i)->SetLLR({sum,sumE});
      mySetups.at(i)->SetLLR_SRB3(sigB3); 
      mySetups.at(i)->SetLLR_SRB4(sigB4);

   
    }
  }
  //______________________Tables ______________________
  // put the LLRs in tables that can be copied and pasted into a LaTeX document

  // output<<std::setprecision(3)<<std::fixed; 
 
  // //SRB3+SRB4 significances added in quadrature
  // std::vector<TString> setupEffs = {"0.02", "0.05", "0.10", "0.70", "0.80", "0.90"};

  // output<<"LLR SRB3 + SRB4"<<std::endl; 
  // output<<"\\[\\begin{array}{|l|c|c|c|} \n \\hline"<<std::endl; 
  // output<<"\\text{Setup} & \\text{No HGTD} & \\text{HGTD w/ HS eff + 15\\%} & \\text{HGTD w/ PU eff } \\times 0.5 \\\\  \\hline"<<std::endl; 
 
  // for(auto eff: setupEffs){
  //   std::vector<Setup*> sameEffSetups = GetSameEffSetups(mySetups, eff); 
  //   if(sameEffSetups.size()<3) continue;

  //   std::pair<float,float> x0 = sameEffSetups.at(0)->GetLLR();
  //   std::pair<float,float> x1 = sameEffSetups.at(1)->GetLLR();
  //   std::pair<float,float> x2 = sameEffSetups.at(2)->GetLLR();

  //   //Percent difference
  //   float pd1 = (x1.first-x0.first)/x0.first;
  //   float pd2 = (x2.first-x0.first)/x0.first;
  //   TString sign1,sign2; 
  //   if(pd1>0) sign1="+"; else sign1="-";
  //   if(pd2>0) sign2="+"; else sign2="-";
  //   pd1=fabs(pd1)*100; 
  //   pd2=fabs(pd2)*100; 

  //   //Errors on percent difference
  //   float pd1err = 100*sqrt(std::pow(1/x0.first * x1.second, 2) + std::pow(x1.first * x0.second/(x0.first*x0.first), 2));
  //   float pd2err = 100*sqrt(std::pow(1/x0.first * x2.second, 2) + std::pow(x2.first * x0.second/(x0.first*x0.first), 2));

  //   TString HSorPU;
  //   if(eff.Contains("0.02") || eff.Contains("0.05") || eff.Contains("0.10")) HSorPU = "PU";
  //   else HSorPU = "HS"; 
  //   output<<"\\text{mu200, TC, "<<HSorPU<<" eff  "<<eff;
  //   output<<"} & "<<sameEffSetups.at(0)->GetLLR().first<<" \\pm "<<sameEffSetups.at(0)->GetLLR().second<<" & ";
  //   output<<sameEffSetups.at(1)->GetLLR().first<<" \\pm "<<sameEffSetups.at(1)->GetLLR().second<<" ("<<sign1<<pd1<<" \\pm "<<pd1err<<"\\%) & ";
  //   output<<sameEffSetups.at(2)->GetLLR().first<<" \\pm "<<sameEffSetups.at(2)->GetLLR().second<<" ("<<sign2<<pd2<<" \\pm "<<pd2err<<"\\%) \\\\ \\hline"<<std::endl;

  // }
  
  // output<<"\\end{array} \\]"<<std::endl; 

  // output<<"\n \n \n"<<std::endl; 

  // //-------------Compare btagging schemes (70% WP)
  // output<<"btagging improvements for mu200, TC, PU eff 0.05, HGTD w/ PU eff $\\times 0.5$"<<std::endl; 
  // output<<"\\[\\begin{array}{|c|c|c|c|} \n \\hline"<<std::endl; 
  // output<<"\\text{No btagging improvement} & \\text{l-jet rej } \\times 2.8 & \\text{c-jet rej } \\times 1.1 & \\text{b-jet eff } \\times 1.1 \\\\  \\hline"<<std::endl; 
 
  // Setup * setup0 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_purej");
  // Setup * setup1 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej");
  // Setup * setup2 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej");
  // Setup * setup3 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej"); 
      
  // std::pair<float,float> x0 = setup0->GetLLR();
  // std::pair<float,float> x1 = setup1->GetLLR();
  // std::pair<float,float> x2 = setup2->GetLLR();
  // std::pair<float,float> x3 = setup3->GetLLR();

  // //Percent difference
  // float pd1 = (x1.first-x0.first)/x0.first;
  // float pd2 = (x2.first-x0.first)/x0.first;
  // float pd3 = (x3.first-x0.first)/x0.first;
  // TString sign1,sign2,sign3;
  // if(pd1>=0) sign1="+"; else sign1="-";
  // if(pd2>=0) sign2="+"; else sign2="-";
  // if(pd3>=0) sign3="+"; else sign3="-";
  // pd1=fabs(pd1)*100; 
  // pd2=fabs(pd2)*100;
  // pd3=fabs(pd3)*100;

  // //Errors on percent difference
  // float pd1err = 100*sqrt(std::pow(1/x0.first * x1.second, 2) + std::pow(x1.first * x0.second/(x0.first*x0.first), 2));
  // float pd2err = 100*sqrt(std::pow(1/x0.first * x2.second, 2) + std::pow(x2.first * x0.second/(x0.first*x0.first), 2));
  // float pd3err = 100*sqrt(std::pow(1/x0.first * x3.second, 2) + std::pow(x3.first * x0.second/(x0.first*x0.first), 2));

  // output<<x0.first<<" \\pm "<<x0.second<<" & ";
  // output<<x1.first<<" \\pm "<<x1.second<<" ("<<sign1<<pd1<<"\\pm "<<pd1err<<"\\%) & ";
  // output<<x2.first<<" \\pm "<<x2.second<<" ("<<sign2<<pd2<<"\\pm "<<pd2err<<"\\%) & ";
  // output<<x3.first<<" \\pm "<<x3.second<<" ("<<sign3<<pd3<<"\\pm "<<pd3err<<"\\%) \\\\ \\hline"<<std::endl;
  
  // output<<"\\end{array} \\]"<<std::endl;   

  // output<<"\n \n \n"<<std::endl; 

  
  // //-----------------Compare btagging schemes (85% WP)
  // output<<"btagWP85"<<std::endl;
  // output<<"btagging improvements for mu200, TC, PU eff 0.05, HGTD w/ PU eff $\\times 0.5$"<<std::endl; 
  // output<<"\\[\\begin{array}{|c|c|c|c|} \n \\hline"<<std::endl; 
  // output<<"\\text{No btagging improvement} & \\text{l-jet rej } \\times 1.6 & \\text{c-jet rej } \\times 1.1 & \\text{b-jet eff } \\times 1.1 \\\\  \\hline"<<std::endl; 
 
  // setup0 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_purej");
  // setup1 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej");
  // setup2 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej");
  // setup3 = GetSetup(mySetups, "mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej"); 
      
  // x0 = setup0->GetLLR();
  // x1 = setup1->GetLLR();
  // x2 = setup2->GetLLR();
  // x3 = setup3->GetLLR();

  // //Percent difference
  // pd1 = (x1.first-x0.first)/x0.first;
  // pd2 = (x2.first-x0.first)/x0.first;
  // pd3 = (x3.first-x0.first)/x0.first;
 
  // if(pd1>=0) sign1="+"; else sign1="-";
  // if(pd2>=0) sign2="+"; else sign2="-";
  // if(pd3>=0) sign3="+"; else sign3="-";
  // pd1=fabs(pd1)*100; 
  // pd2=fabs(pd2)*100;
  // pd3=fabs(pd3)*100;

  // //Errors on percent difference
  // pd1err = 100*sqrt(std::pow(1/x0.first * x1.second, 2) + std::pow(x1.first * x0.second/(x0.first*x0.first), 2));
  // pd2err = 100*sqrt(std::pow(1/x0.first * x2.second, 2) + std::pow(x2.first * x0.second/(x0.first*x0.first), 2));
  // pd3err = 100*sqrt(std::pow(1/x0.first * x3.second, 2) + std::pow(x3.first * x0.second/(x0.first*x0.first), 2));

  // output<<x0.first<<" \\pm "<<x0.second<<" & ";
  // output<<x1.first<<" \\pm "<<x1.second<<" ("<<sign1<<pd1<<"\\pm "<<pd1err<<"\\%) & ";
  // output<<x2.first<<" \\pm "<<x2.second<<" ("<<sign2<<pd2<<"\\pm "<<pd2err<<"\\%) & ";
  // output<<x3.first<<" \\pm "<<x3.second<<" ("<<sign3<<pd3<<"\\pm "<<pd3err<<"\\%) \\\\ \\hline"<<std::endl;
  
  // output<<"\\end{array} \\]"<<std::endl;   

  // output<<"\n\n\n"<<std::endl; 


  // //------------Compare addition of fwd electrons (70% WP)
  // output<<"btagWP70"<<std::endl;
  // output<<"btagging improvements for mu200, TC, PU eff 0.05, HGTD w/ PU eff $\\times 0.5$"<<std::endl; 
  // output<<"\\[\\begin{array}{|c|c|c|} \n \\hline"<<std::endl; 
  // output<<" & \\vert \\eta (electron) \\vert < 2.47 & \\vert \\eta (electron) \\vert < 4.0 \\\\  \\hline"<<std::endl; 
 
  // setup1b = GetSetup(mySetups, "btagWP70/mu200_TC_PU0.05_HGTD_HGTDbtag_lrej_purej_fwdEle");
  // setup2b = GetSetup(mySetups, "btagWP70/mu200_TC_PU0.05_HGTD_HGTDbtag_crej_purej_fwdEle");
  // setup3b = GetSetup(mySetups, "btagWP70/mu200_TC_PU0.05_HGTD_HGTDbtag_beff_purej_fwdEle"); 

  // x1b = setup1b->GetLLR();
  // x2b = setup2b->GetLLR();
  // x3b = setup3b->GetLLR();

  // //Percent difference
  // pd1 = (x1b.first-x1.first)/x1.first;
  // pd2 = (x2b.first-x2.first)/x2.first;
  // pd3 = (x3b.first-x3.first)/x3.first;
 
  // if(pd1>=0) sign1="+"; else sign1="-";
  // if(pd2>=0) sign2="+"; else sign2="-";
  // if(pd3>=0) sign3="+"; else sign3="-";
  // pd1=fabs(pd1)*100; 
  // pd2=fabs(pd2)*100;
  // pd3=fabs(pd3)*100;

  // //Errors on percent difference
  // pd1err = 100*sqrt(std::pow(1/x1.first * x1b.second, 2) + std::pow(x1b.first * x1.second/(x1.first*x1.first), 2));
  // pd2err = 100*sqrt(std::pow(1/x2.first * x2b.second, 2) + std::pow(x2b.first * x2.second/(x2.first*x2.first), 2));
  // pd3err = 100*sqrt(std::pow(1/x3.first * x3b.second, 2) + std::pow(x3b.first * x3.second/(x3.first*x3.first), 2));

  // output<<"\\text{l-jet rej }\\times 2.8 & "<<x1.first<<" \\pm "<<x1.second<<" & "<<x1b.first<<" \\pm "<<x1b.second<<" ("<<sign1<<pd1<<"\\pm"<<pd1err<<" \\%) \\\\ \\hline "<<std::endl; 
  // output<<"\\text{c-jet rej }\\times 1.1 & "<<x2.first<<" \\pm "<<x2.second<<" & "<<x2b.first<<" \\pm "<<x2b.second<<" ("<<sign2<<pd2<<"\\pm"<<pd2err<<" \\%) \\\\ \\hline "<<std::endl; 
  // output<<"\\text{b-jet eff }\\times 1.1 & "<<x3.first<<" \\pm "<<x3.second<<" & "<<x3b.first<<" \\pm "<<x3b.second<<" ("<<sign3<<pd3<<"\\pm"<<pd3err<<" \\%) \\\\ \\hline "<<std::endl; 
  
  // output<<"\\end{array} \\]"<<std::endl;   


  for(unsigned int i=0; i < setupDirs.size(); ++i) delete setup[i];

}
