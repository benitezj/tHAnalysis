#ifndef FLAVOURTAGGINGPERFORMANCEFUNCTIONS_CXX
#define FLAVOURTAGGINGPERFORMANCEFUNCTIONS_CXX

#include "UpgradePerformanceFunctions/UpgradePerformanceFunctions.h"

#include <map>
#include <iterator>
#include <vector>
using std::map;

#include <TF2.h>
#include <TH2D.h>
#include <TFile.h>

#ifdef ROOTCORE
// Framework include(s):
#include "PathResolver/PathResolver.h"
#include "CalibrationDataInterface/CalibrationDataContainer.h"
#endif // ROOTCORE

void UpgradePerformanceFunctions::setFlavourTaggingCalibrationFilename(TString flavourTaggingCalibrationFilename) {
  std::cout << "Flavour tagging histogram input filename is " 
	    << flavourTaggingCalibrationFilename << std::endl;
  std::string file = flavourTaggingCalibrationFilename.Data();
#ifdef ROOTCORE
  // Get file from data path
  file = PathResolverFindCalibFile(file);
  std::cout << "Found flavour tagging histogram file: " << file << std::endl;
#endif // ROOTCORE
  m_flavourTaggingCalibrationFilename = file;
}

float UpgradePerformanceFunctions::getFlavourTagEfficiency(float ptMeV, float eta, char flavour, TString tagger, int operating_point, bool track_confirmation) {
  double ptGeV = ptMeV/1000.;

  // the function returns b/c/l-tagging efficincies obtained using ttbar samples

  // flavour is a char: IN CAPS!
  // 'B' (to get the b-tag efficiency) 
  // 'C' (to get the c-tag efficiency) 
  // 'L' (to get the mistag rate)
  // 'P' (to get the PU-tag efficiency)

  // pT range protection
  //  if (ptGeV<20) ptGeV = 20;
  if (ptGeV<20) return 0.;
  //Run2 settings
  if(m_layout==UpgradePerformanceFunctions::UpgradeLayout::run2){

    static TFile* ff = 0;
    std::string calibFile = m_flavourTaggingCalibrationFilename.Data(); 
    if (ff==0) ff = new TFile(calibFile.c_str(), "READ");
    
    static TH2D* fEffs[16] = {0};   // {60,70,77,85} * {B,C,T,L}
    
    static std::vector<int> wpoints  = {60,70,77,85};
    static std::vector<std::string> flavours = {"B","C","Light","T"};

    std::string sflavour = std::string(1, flavour);
    if(sflavour == "L") sflavour = "Light";

    unsigned int iopt = std::distance(wpoints.begin(), std::find(wpoints.begin(), wpoints.end(), operating_point));
    if(iopt > wpoints.size()) throw std::runtime_error("getFlavourTagEfficiency:: Unsupported Working point"); 

    unsigned int iflav = std::distance(flavours.begin(), std::find(flavours.begin(), flavours.end(), sflavour));
    if(iopt > wpoints.size()) throw std::runtime_error("getFlavourTagEfficiency:: Unsupported flavour"); 

    auto ix = (iopt*4)+iflav;

#ifdef ROOTCORE
    if (fEffs[ix]==0){
      
      //DUMMY FOR NOW
      // get MC efficiency from CDI files!

      TString hfName = Form("MV2c10/AntiKt4EMTopoJets/FixedCutBEff_%d/%s/",operating_point,sflavour.c_str());
      //std::cout << "FTAG HISTO NAME: " << hfName << std::endl;

      ff->cd(hfName);
      Analysis::CalibrationDataHistogramContainer* cHCont = (Analysis::CalibrationDataHistogramContainer*) ff->Get(hfName+"default_Eff");

      fEffs[ix] = (TH2D*)cHCont->GetValue("result"); 

    }
#endif
    
    //eta&pt bins
    auto eta_bin = fEffs[ix]->GetXaxis()->FindBin(eta);
    auto pt_bin  = fEffs[ix]->GetYaxis()->FindBin(ptMeV*0.001);
    if(eta_bin==0) eta_bin=1;
    if(pt_bin==0)  pt_bin=1;
    if(eta_bin > fEffs[ix]->GetNbinsX()) eta_bin = fEffs[ix]->GetNbinsX();
    if(pt_bin  > fEffs[ix]->GetNbinsY()) pt_bin  = fEffs[ix]->GetNbinsX();

    //std::cout << sflavour << " : pt " << ptMeV*0.001 << " : eta " << eta << " : eff " << fEffs[ix]->GetBinContent(eta_bin,pt_bin) << std::endl; //debugging

    return fEffs[ix]->GetBinContent(eta_bin,pt_bin);
  }
  

  //--Upgrade settings
  // high pT approximation
  double highpt_factor = 1;
  if (ptGeV>300) {
    double x = ptGeV; if (x>1000) x = 1000;
    if (tagger=="mv1_flat_high_pt") {
      if (flavour=='C') {
	highpt_factor = 1.6070808467835-0.00243838298663*x+1.4234528100804e-06*x*x-1.3617421117693e-10*x*x*x;
      } else if (flavour=='L' || flavour=='P') {
	highpt_factor = 7.5679905186556-0.029707488878509*x+2.8845677013756e-05*x*x-9.327955049668e-09*x*x*x;
	highpt_factor = exp(highpt_factor-1);
      }
      highpt_factor = 1./highpt_factor;
    } else if (tagger=="mv1" || tagger=="mv2c00" || tagger=="mv2c10" || tagger=="mv2c20") {
      double p1 = 0;
      if (flavour=='B') p1 = -6.17434e-04;
      else if (flavour=='C') p1 = -7.20231e-04;
      else /* L,P */ p1 = 2.78547e-04;
      highpt_factor = 1 + p1*(x-300);
    }
  }
  if (tagger=="mv1_flat_high_pt") tagger = "mv1";

  // load parameterizations
  const int nfl = 4; // 0='L', 1='C', 2='B', 3='P'

  std::string funname = "fitfun_mu";
  funname += std::to_string(int(m_avgMu));
  if (m_bUseHGTD0 && m_bUseHGTDbtag) funname += "_HGTD0";
  if (m_bUseHGTD1 && m_bUseHGTDbtag) funname += "_HGTD1";
  funname += "_"; funname += tagger;
  funname += "_"; funname += std::to_string(operating_point);
  if (track_confirmation) funname += "_TC";

  static std::map<std::string, std::vector<TF2*> > funmap;
  static TFile* ff = 0;

  std::vector<TF2*>* funPtr = 0;

  std::map<std::string, std::vector<TF2*> >::iterator funmap_iter = funmap.find(funname);
  if (funmap_iter==funmap.end()) {
    // add new parameterization
    if (ff==0) {
      std::string calibFile = m_flavourTaggingCalibrationFilename.Data();
      std::cout << "Opening " << calibFile << std::endl;
      ff = new TFile(calibFile.c_str(), "READ");
    }
    std::cout << "Loading " << funname << std::endl;
    TF2* funs[nfl]= {0};
    funs[0] = (TF2*)ff->Get((funname+"_0").c_str());
    funs[1] = (TF2*)ff->Get((funname+"_1").c_str());
    funs[2] = (TF2*)ff->Get((funname+"_2").c_str());
    funs[3] = (TF2*)ff->Get((funname+"_3").c_str());
    funPtr = &(funmap[funname] = std::vector<TF2*>(funs,funs+nfl));
  } else {
    // use already loaded parameterization
    funPtr = &(funmap_iter->second);
  }

  // load additional pt limits
  static double pt_limits[nfl] = {0};
  if (pt_limits[0]==0) {
    TF1* fun_pt_limits = (TF1*)ff->Get("pt_limits");
    if (fun_pt_limits) {
      std::cout << "Loading additional pT limits:";
      for (int i = 0; i<nfl; ++i) std::cout << " " << (pt_limits[i] = fun_pt_limits->Eval(i));
      std::cout << std::endl;
    } else {
      std::cout << "No additional pT limits, using default" << std::endl;
      for (int i = 0; i<nfl; ++i) pt_limits[i] = 300; // GeV
    }
  }
  if (flavour=='L' && ptGeV>pt_limits[0]) ptGeV = pt_limits[0];
  if (flavour=='C' && ptGeV>pt_limits[1]) ptGeV = pt_limits[1];
  if (flavour=='B' && ptGeV>pt_limits[2]) ptGeV = pt_limits[2];
  if (flavour=='P' && ptGeV>pt_limits[3]) ptGeV = pt_limits[3];

  // calculate the efficiency
  float eff = -1;
  if (flavour=='L') eff = (*funPtr)[0]->Eval(ptGeV,fabs(eta));
  else if (flavour=='C') eff = (*funPtr)[1]->Eval(ptGeV,fabs(eta));
  else if (flavour=='B') eff = (*funPtr)[2]->Eval(ptGeV,fabs(eta));
  else if (flavour=='P') eff = (*funPtr)[3]->Eval(ptGeV,fabs(eta));

  // Spyros: Hack to get b-tagging improvement for HGTD
  float HGTDfactor = 1.0;    
  if  (m_bUseHGTDbtag && (m_bUseHGTD0 || m_bUseHGTD1)){
    float minHGTD = 0.;
    if (m_bUseHGTD0) minHGTD = m_HGTD0min;
    if (m_bUseHGTD1) minHGTD = m_HGTD1min;
    
    if (fabs(eta) > minHGTD && fabs(eta) < m_HGTDmax) {
      // Only hack the c-rejection and b-rejection - 
      // the l-rejection is already implemented in the efficiency functions above
      if (flavour == 'C'&& m_btagScheme=="crej") {
       	if     (operating_point==85) HGTDfactor = 1.1;// double check
       	else if (operating_point==70) HGTDfactor = 1.1;
      }
      if (flavour == 'B'&& m_btagScheme=="beff") {
       	if     (operating_point==85) HGTDfactor = 1./1.1;// double check
       	else if (operating_point==70) HGTDfactor = 1./1.1;
      }
    }      
  }

  return eff*highpt_factor/HGTDfactor;
}

#endif
