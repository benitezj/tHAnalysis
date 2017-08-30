#ifndef PILEUPOVERLAY_CXX
#define PILEUPOVERLAY_CXX

#include "UpgradePerformanceFunctions/UpgradePerformanceFunctions.h"
#include "TFile.h"

void UpgradePerformanceFunctions::setPileupRandomSeed(unsigned seed) {
  m_pileupRandom.SetSeed(seed);
}
void UpgradePerformanceFunctions::setPileupEfficiencyScheme(PileupEff puscheme){
  m_pueff = puscheme;   
  std::cout << " UpgradePerformanceFunctions::setPileupEfficiencyScheme::You set efficiency scheme to " << m_pueff <<  std::endl;
}

void UpgradePerformanceFunctions::setPileupEff(float hseff){
  m_fEff = hseff;
   std::cout << " UpgradePerformanceFunctions::setPileupEff::You set efficiency to " << m_fEff << std::endl;

}

void UpgradePerformanceFunctions::setPileupUseTrackConf(bool usetr) {
  m_bUseTrackConf = usetr;
}

bool UpgradePerformanceFunctions::getPileupTrackConfSetting() {
  return m_bUseTrackConf;
}


void UpgradePerformanceFunctions::setPileupJetPtThresholdMeV(float ptThresholdMeV) {
  m_fPileupJetThresholdMeV = ptThresholdMeV;
}

float UpgradePerformanceFunctions::getPileupJetPtThresholdMeV() {
  return m_fPileupJetThresholdMeV;
}


void UpgradePerformanceFunctions::setPileupTemplatesPath(TString pileupTemplateFilenamePath) {

  TString sPuPath =pileupTemplateFilenamePath; 
  std::cout << " PUPath in: " << pileupTemplateFilenamePath << std::endl;

  // check the end of the string for a "/"

  float fLenPuStr = sPuPath.Length();
  TString sLast = sPuPath[fLenPuStr-1];

  if (!sLast.Contains("/"))
    sPuPath = sPuPath+"/";

  // construct the full path to different libraries
  if ( TMath::Abs(m_avgMu - 140.) < 0.1)
    sPuPath = sPuPath+"PULibrary140_1.root";

  if ( TMath::Abs(m_avgMu - 200.) < 0.1)
    sPuPath = sPuPath+"PULibrary200_1.root";


  std::cout << " PUPath: " << sPuPath << std::endl;
  initializePileupTemplates(sPuPath);
  
}




void UpgradePerformanceFunctions::initializePileupTemplates(TString pileupTemplateFilename) {
  if ( !(m_avgMu == 140 || m_avgMu == 200) ) {
    std::cout << "Error in pileup library: pileup is implemented for mu=140 and 200 only!" << std::endl;
    return;
  }

  m_fPileupTCMaxPtMeV = 70000.;
  
  // bool b05 = false;
  // bool b25 = false;
  // bool b30 = false;
  // bool b35 = false;
  // bool b40 = false;
  // bool b50 = false;
  
  // init of bools
  // if (fabs(m_fPileupJetThresholdMeV - 25000.) < 0.1)
  //   b25 = true;
  // else if (fabs(m_fPileupJetThresholdMeV - 30000.) < 0.1)
  //   b30 = true;
  // else if (fabs(m_fPileupJetThresholdMeV - 35000.) < 0.1)
  //   b35 = true;
  // else if (fabs(m_fPileupJetThresholdMeV - 40000.) < 0.1)
  //   b40 = true;
  // else if (fabs(m_fPileupJetThresholdMeV - 50000.) < 0.1)
  //   b50 = true;
  // else
  //   b05 = true;
  
  std::cout << " UpgradePerformanceFunctions::initializePileupTemplates::Your pileup jet pt threshold is " << m_fPileupJetThresholdMeV << " MeV " << std::endl;
  
  if (m_fPileupJetThresholdMeV < 20000.){
    std::cout << "WARNING::initializePileupTemplates - Lowest threshold you can get is 20 GeV and that will be automatically set now" << std::endl;
    m_fPileupJetThresholdMeV = 20000.;
  }
  // m_bHS70 = false;
  // m_bHS80 = false;
  // m_bHS90 = false;
  
  std::cout << " UpgradePerformanceFunctions::initializePileupTemplates::Loading pileup jet library file for mu="<< m_avgMu<<": " << pileupTemplateFilename << std::endl;

  std::string pileupFile = pileupTemplateFilename.Data();
#ifdef ROOTCORE
// Get file from data path
//  pileupFile = PathResolverFindCalibFile(pileupFile);
  
//  pileupFile="../public/PULibrary.root";
  std::cout << "Found pileup jet library file: " << pileupFile << std::endl;
#endif // ROOTCORE
  
  
  TFile *ftemplate = new TFile(pileupFile.c_str(),"READ");
  TString treeName;
  TString treePt;
  TString treeEta;    
  TString treePhi;    
  TString treeE;
  
  // based on your threshold, open the right tree
  // if (b25) {
  //   treeName = "PUEvtTree25";
  //   treePt = "m25_pt";
  //   treeEta = "m25_eta";
  //   treePhi = "m25_phi";
  //   treeE = "m25_E";
  // } else if (b30) {
    treeName = "PUEvtTree30";
    treePt = "m30_pt";
    treeEta = "m30_eta";
    treePhi = "m30_phi";
    treeE = "m30_E";
  // } else if (b35) {
  //   treeName = "PUEvtTree35";
  //   treePt = "m35_pt";
  //   treeEta = "m35_eta";
  //   treePhi = "m35_phi";
  //   treeE = "m35_E";
  // } else if (b40) {
  //   treeName = "PUEvtTree40";
  //   treePt = "m40_pt";
  //   treeEta = "m40_eta";
  //   treePhi = "m40_phi";
  //   treeE = "m40_E";
  // } else if (b50) {
  //   treeName = "PUEvtTree50";
  //   treePt = "m50_pt";
  //   treeEta = "m50_eta";
  //   treePhi = "m50_phi";
  //   treeE = "m50_E";
  // } else {
  //   treeName = "PUEvtTree05";
  //   treePt = "m05_pt";
  //   treeEta = "m05_eta";
  //   treePhi = "m05_phi";
  //   treeE = "m05_E";
  //   std::cout << "WARNING::initializePileupTemplates - You have set your jet threshold to " << m_fPileupJetThresholdMeV << " MeV -> outside of allowed range, no threshold set and you will get plenty of PU jets!! (and it will take ages). Have a nice day" << b05 << std::endl;
  // }
  
  std::cout << " UpgradePerformanceFunctions::initializePileupTemplates::Initializing pileupTemplates with name: " << treeName 
	    << " PtBranch name: " << treePt 
	    << " EtaBranch name: " << treeEta 
	    << " PhiBranch name: " << treePhi 
	    << " and EnBranch name: " << treeE << std::endl;
  
  m_pileupTree30 = (TTree*)ftemplate->Get(treeName); 
  std::cout << " UpgradePerformanceFunctions::initializePileupTemplates::Number of events in the Pileup Overlay library: " << m_pileupTree30->GetEntries() << std::endl;
  
  m_pileupPt30  = 0;
  m_pileupEta30 = 0;
  m_pileupPhi30 = 0;
  m_pileupE30   = 0;    
  m_pileupTree30->SetBranchAddress(treePt, &m_pileupPt30);
  m_pileupTree30->SetBranchAddress(treeEta, &m_pileupEta30);
  m_pileupTree30->SetBranchAddress(treePhi, &m_pileupPhi30);
  m_pileupTree30->SetBranchAddress(treeE, &m_pileupE30);
  
  std::cout << " UpgradePerformanceFunctions::initializePileupTemplates::loadPileupTemplates successfully completed " << std::endl;

  // initialize TC

  if (m_bUseTrackConf)
    std::cout<< " UpgradePerformanceFunctions::initializePileupTemplates::You are using Tracking Confirmation" << std::endl;
  else
    std::cout<< " WARNING :: UpgradePerformanceFunctions::initializePileupTemplates::You are NOT using Tracking Confirmation" << std::endl;

  // version from Marianna Testa from 17.7.2015
  // 6 scenarios for Mu200, central region (= bronze)

  float fHS70Nominal200[m_nPUPtBins] = {0.00655021,0.00657876,0.00578142,0.00670391};
  float fHS80Nominal200[m_nPUPtBins] = {0.0128369,  0.0132928,0.0100504,0.0121145};
  float fHS90Nominal200[m_nPUPtBins] = {0.0366681,  0.0383563,0.0180194,0.0167449};

  float fPU02Nominal200[m_nPUPtBins] = {0.853741,0.844738,0.911594,0.965879};
  float fPU05Nominal200[m_nPUPtBins] = {0.922095,0.916188,0.966736,0.989596};
  float fPU10Nominal200[m_nPUPtBins] = {0.948818,0.950567,0.98141,0.993905};

  // 6 scenarios for Mu200, medium region (= silver)

  float fHS70Medium200[m_nPUPtBins] = {0.0179052,0.0181347,0.00919735,0.00586602};
  float fHS80Medium200[m_nPUPtBins] = {0.0335118,0.0347988,0.0173066,0.00759878};
  float fHS90Medium200[m_nPUPtBins] = {0.0779999,0.0811618,0.0449882,0.0178825};

  float fPU02Medium200[m_nPUPtBins] = {0.709526,0.710547,0.81575,0.917082};
  float fPU05Medium200[m_nPUPtBins] = {0.857033,0.851397,0.906623,0.972068};
  float fPU10Medium200[m_nPUPtBins] = {0.919615,0.914685,0.941338,0.984482};

  // 6 scenarios for Mu200, extended region (= gold)

  float fHS70Extended200[m_nPUPtBins] = {0.0200319,0.0199557,0.0110303,0.0121212};
  float fHS80Extended200[m_nPUPtBins] = {0.0411415,0.0429245,0.0194322,0.0151515};
  float fHS90Extended200[m_nPUPtBins] = {0.0888737,0.0940972,0.0466814,0.0181818};

  float fPU02Extended200[m_nPUPtBins] = {0.699877,0.700218,0.809446,0.931242};
  float fPU05Extended200[m_nPUPtBins] = {0.850517,0.835516,0.904409,0.977778};
  float fPU10Extended200[m_nPUPtBins] = {0.910625,0.905812,0.933026,0.986765};

  // TO BE UPDATED
  // 6 scenarios for Mu140, central region (= bronze)

  float fHS70Nominal140[m_nPUPtBins] = {0.00655021,0.00657876,0.00578142,0.00670391};
  float fHS80Nominal140[m_nPUPtBins] = {0.0128369,  0.0132928,0.0100504,0.0121145};
  float fHS90Nominal140[m_nPUPtBins] = {0.0366681,  0.0383563,0.0180194,0.0167449};

  float fPU02Nominal140[m_nPUPtBins] = {0.853741,0.844738,0.911594,0.965879};
  float fPU05Nominal140[m_nPUPtBins] = {0.922095,0.916188,0.966736,0.989596};
  float fPU10Nominal140[m_nPUPtBins] = {0.948818,0.950567,0.98141,0.993905};

  // 6 scenarios for Mu140, medium region (= silver)

  float fHS70Medium140[m_nPUPtBins] = {0.0179052,0.0181347,0.00919735,0.00586602};
  float fHS80Medium140[m_nPUPtBins] = {0.0335118,0.0347988,0.0173066,0.00759878};
  float fHS90Medium140[m_nPUPtBins] = {0.0779999,0.0811618,0.0449882,0.0178825};

  float fPU02Medium140[m_nPUPtBins] = {0.709526,0.710547,0.81575,0.917082};
  float fPU05Medium140[m_nPUPtBins] = {0.857033,0.851397,0.906623,0.972068};
  float fPU10Medium140[m_nPUPtBins] = {0.919615,0.914685,0.941338,0.984482};

  // 6 scenarios for Mu140, extended region (= gold)

  float fHS70Extended140[m_nPUPtBins] = {0.0200319,0.0199557,0.0110303,0.0121212};
  float fHS80Extended140[m_nPUPtBins] = {0.0411415,0.0429245,0.0194322,0.0151515};
  float fHS90Extended140[m_nPUPtBins] = {0.0888737,0.0940972,0.0466814,0.0181818};

  float fPU02Extended140[m_nPUPtBins] = {0.699877,0.700218,0.809446,0.931242};
  float fPU05Extended140[m_nPUPtBins] = {0.850517,0.835516,0.904409,0.977778};
  float fPU10Extended140[m_nPUPtBins] = {0.910625,0.905812,0.933026,0.986765};

  // central region

  // mu 200

  if ( !(m_layout==Step1p6) ){


  if (m_pueff == HS && fabs(m_fEff - 0.7) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fHS70Nominal200[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.8) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fHS80Nominal200[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.9) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fHS90Nominal200[i];
  }

  if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fPU02Nominal200[i];
  }
  else if (m_pueff == PU && fabs(m_fEff - 0.05) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fPU05Nominal200[i];
  }
  else if (m_pueff == PU && fabs(m_fEff - 0.1) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fPU10Nominal200[i];
  }else{
    std::cout << " WARNING :: UpgradePerformanceFunctions::initializePileupTemplates:: You haven't provided working point consistent with PU::Gold " << std::endl;
    return;
  }

  // mu 140

  if (m_pueff == HS && fabs(m_fEff - 0.7) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fHS70Nominal140[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.8) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fHS80Nominal140[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.9) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fHS90Nominal140[i];
  }

  if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fPU02Nominal140[i];
  }
  if (m_pueff == PU && fabs(m_fEff - 0.05) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fPU05Nominal140[i];
  }
  if (m_pueff == PU && fabs(m_fEff - 0.1) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffNominalArray[i] = fPU10Nominal140[i];
  }

  // medium region

  // mu 200

  if (m_pueff == HS && fabs(m_fEff - 0.7) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fHS70Medium200[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.8) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fHS80Medium200[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.9) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fHS90Medium200[i];
  }

  if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fPU02Medium200[i];
  }
  else if (m_pueff == PU && fabs(m_fEff - 0.05) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fPU05Medium200[i];
  }
  else if (m_pueff == PU && fabs(m_fEff - 0.1) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fPU10Medium200[i];
  }else{
    std::cout << " WARNING :: UpgradePerformanceFunctions::initializePileupTemplates:: You haven't provided working point consistent with PU::Gold " << std::endl;
    return;
  }

  // mu 140

  if (m_pueff == HS && fabs(m_fEff - 0.7) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fHS70Medium140[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.8) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fHS80Medium140[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.9) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fHS90Medium140[i];
  }

  if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fPU02Medium140[i];
  }
  if (m_pueff == PU && fabs(m_fEff - 0.05) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fPU05Medium140[i];
  }
  if (m_pueff == PU && fabs(m_fEff - 0.1) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffMediumArray[i] = fPU10Medium140[i];
  }

  // extended region

  // mu 200

  if (m_pueff == HS && fabs(m_fEff - 0.7) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fHS70Extended200[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.8) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fHS80Extended200[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.9) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fHS90Extended200[i];
  }

  if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fPU02Extended200[i];
  }
  else if (m_pueff == PU && fabs(m_fEff - 0.05) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fPU05Extended200[i];
  }
  else if (m_pueff == PU && fabs(m_fEff - 0.1) < 0.001 && fabs( m_avgMu - 200)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fPU10Extended200[i];
  }else{
    std::cout << " WARNING :: UpgradePerformanceFunctions::initializePileupTemplates:: You haven't provided working point consistent with PU::Gold " << std::endl;
    return;
  }

  // mu 140

  if (m_pueff == HS && fabs(m_fEff - 0.7) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fHS70Extended140[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.8) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fHS80Extended140[i];
  }
  if (m_pueff == HS && fabs(m_fEff - 0.9) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fHS90Extended140[i];
  }

  if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fPU02Extended140[i];
  }
  if (m_pueff == PU && fabs(m_fEff - 0.05) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fPU05Extended140[i];
  }
  if (m_pueff == PU && fabs(m_fEff - 0.1) < 0.001 && fabs( m_avgMu - 140)  < 0.1){
    for (int i = 0; i < m_nPUPtBins; i++)
      fEffExtendedArray[i] = fPU10Extended140[i];
  }

  }

  // as of ITk Step 1.6 we have different parametrisation: pol3 below and above 50 GeV

  // pt < 50 GeV
  float pars_ITK_20_lt50[4] = {0.865632, -0.048153, 0.0411527, -0.0130769};     // ITk 2% PU
  float pars_ITKHGTD_20_lt50[4]={1.00826, -0.171095, 0.0667378, -0.00937643};   // ITk+HGTD 2% PU
  float pars_ITK_10_lt50[4]={0.977724, -0.310333, 0.160202, -0.0318497};        // ITk 1% PU
  float pars_ITKHGTD_10_lt50[4]={1.0473, -0.340747, 0.165846, -0.0265715};      // ITk+HGTD 1% PU
  float pars_ITK_05_lt50[4]={0.903501, -0.306444, 0.146269, -0.0300982};        // ITk 0.5% PU
  float pars_ITKHGTD_05_lt50[4]={1.0122, -0.311178, 0.145689, -0.024726};       // ITk+HGTD 0.5% PU
 
  // pt > 50 GeV
  float pars_ITK_20_gt50[4] ={1.00589, -0.0941386, 0.0655066, -0.0158565};      // ITk 2% PU
  float pars_ITKHGTD_20_gt50[4]={1.01645, -0.0630882, 0.0235701, -0.00364137};  // ITk+HGTD 2% PU
  float pars_ITK_10_gt50[4]={0.917016, -0.0202391, 0.0501683, -0.0201117};      // ITk 1% PU
  float pars_ITKHGTD_10_gt50[4]={0.996236, -0.0552977, 0.028086, -0.00617803};  // ITk+HGTD 1% PU
  float pars_ITK_05_gt50[4]={0.743145, 0.201092, -0.118611, 0.00935206};        // ITk 0.5% PU
  float pars_ITKHGTD_05_gt50[4]={1.04009, -0.117583, 0.0502864, -0.0101477};    // ITk+HGTD 0.5% PU

  func_TC_lt50 = new TF1("func_TC_lt50","[0]+[1]*x + [2]*x*x + [3]*x*x*x",1.2,3.8); // default ITk only
  func_TC_gt50 = new TF1("func_TC_gt50","[0]+[1]*x + [2]*x*x + [3]*x*x*x",1.2,3.8);
  func_IH_lt50 = new TF1("func_IH_lt50","[0]+[1]*x + [2]*x*x + [3]*x*x*x",1.2,3.8); // IH = ITk + HGTD
  func_IH_gt50 = new TF1("func_IH_gt50","[0]+[1]*x + [2]*x*x + [3]*x*x*x",1.2,3.8);

  if ( m_layout==Step1p6 ){
      if (m_pueff == PU && fabs(m_fEff - 0.02) < 0.001 && fabs( m_avgMu - 200)  < 0.1){  // "standard setting"
	for (int i = 0; i < 4; i++){
	  func_TC_lt50->SetParameter(i,pars_ITK_20_lt50[i]);
	  func_TC_gt50->SetParameter(i,pars_ITK_20_gt50[i]);
	  func_IH_lt50->SetParameter(i,pars_ITKHGTD_20_lt50[i]);
	  func_IH_gt50->SetParameter(i,pars_ITKHGTD_20_gt50[i]);
	}
      }
      else if (m_pueff == PU && fabs(m_fEff - 0.01) < 0.001 && fabs( m_avgMu - 200)  < 0.1){  // PU 1% ITk only
	for (int i = 0; i < 4; i++){
	  func_TC_lt50->SetParameter(i,pars_ITK_10_lt50[i]);
	  func_TC_gt50->SetParameter(i,pars_ITK_10_gt50[i]);
	  func_IH_lt50->SetParameter(i,pars_ITKHGTD_10_lt50[i]);
	  func_IH_gt50->SetParameter(i,pars_ITKHGTD_10_gt50[i]);
	}
      }
      else if (m_pueff == PU && fabs(m_fEff - 0.005) < 0.0001 && fabs( m_avgMu - 200)  < 0.1){  // PU 0.5% ITk only
	for (int i = 0; i < 4; i++){
	  func_TC_lt50->SetParameter(i,pars_ITK_05_lt50[i]);
	  func_TC_gt50->SetParameter(i,pars_ITK_05_gt50[i]);
	  func_IH_lt50->SetParameter(i,pars_ITKHGTD_05_lt50[i]);
	  func_IH_gt50->SetParameter(i,pars_ITKHGTD_05_gt50[i]);
	}
      }
      else{
	std::cout << " WARNING :: UpgradePerformanceFunctions::initializePileupTemplates:: You haven't provided working point consistent with ITk 1.6 " << std::endl;
	return;
      }
    
  } else {
    // in older than Step 1.6 version you won't use the new parametrisation however the function needs to be initialised -> dummy
    for (int i = 0; i < 4; i++){
      func_TC_lt50->SetParameter(i,100.);
      func_TC_gt50->SetParameter(i,100.);
      func_IH_lt50->SetParameter(i,100.);
      func_IH_gt50->SetParameter(i,100.);
    }
  }
  

  return;
}

std::vector<TLorentzVector> UpgradePerformanceFunctions::getPileupJets() {
  std::vector<TLorentzVector> pujets;
  
  int iNevts = m_pileupTree30->GetEntries();
  int indx = m_pileupRandom.Integer(iNevts);
  
  m_pileupTree30->GetEntry(indx);
  int nj = m_pileupEta30->size();
  
  if (nj > 0) { // loop over number of actual PU jets in the event
    for (int ia = 0; ia < nj; ia++) {
      TLorentzVector jet;

      float pt = m_pileupPt30->at(ia); 
      // pt cut - added
      if (pt > getPileupJetPtThresholdMeV())
	{
	  jet.SetPtEtaPhiE(m_pileupPt30->at(ia),m_pileupEta30->at(ia),
			   m_pileupPhi30->at(ia),m_pileupE30->at(ia));
	  pujets.push_back(jet);
	}
    }
  }
  return pujets;
}

float UpgradePerformanceFunctions::getTrackJetConfirmEff(float ptMeV, float eta, TString jetType) {
  float ptGeV = ptMeV/1000.;
  //  float m_fPileupTCMaxPtGeV = m_fPileupTCMaxPtMeV/1000.;
  float tceff = 1.0; // jet survival probability

  // if TC is OFF, return 1.00 for everything

  if (!m_bUseTrackConf){
    tceff = 1.0;
    return tceff;
  }

  if (!jetType.Contains("HS") && !jetType.Contains("PU")) {
    std::cout << "WARNING: jet type " << jetType 
	      << " not supported.  Allowed types are HS and PU." << std::endl;
    tceff = 1.0;
  }
  
  if (ptGeV<20.) {
    std::cout << "WARNING : track-jet confirmation not implemented for pT<20 GeV !" << std::endl;
    tceff = 1.0;
  }

  // completely new approach

  // estimate the pT bin 
  int nPt = -1;
  if ( 20. <= ptGeV && ptGeV < 40. )
    nPt = 0;
  if ( 40. <= ptGeV && ptGeV < 50. )
    nPt = 1;
  if ( 50. <= ptGeV && ptGeV < 70. )
    nPt = 2;
  if ( 70. <= ptGeV && ptGeV <= 100. )
    nPt = 3;

  bool bHS = false;

  if( jetType.Contains("HS") ) // jet type, not Efficiency Scheme!!
    bHS = true;
  if( jetType.Contains("PU") ) // jet type, not Efficiency Scheme!!
    bHS = false;

  // apply only in correct pt range

  // now implement correctly the Step 1.6

  if (m_layout!=Step1p6){
    
    // std::cout << " Entering !Step1p6 " << std::endl;

    if (nPt > -1){

      // now clearly distinguish all three scenarios

      // ----------- Bronze ------------

      if ( m_layout == bronze ){

	if (fabs(eta)> m_BronzeEtaMax)
	  tceff = 1.00; // nothing changes above 2.4
	else{

	  if (m_pueff == HS){       // HS Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = m_fEff;
	    } else { // PU jet
	      tceff = fEffNominalArray[nPt];
	    }
	  }

	  if (m_pueff == PU){       // PU Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = fEffNominalArray[nPt];
	    } else { // PU jet
	      tceff = m_fEff;
	    }
	  }

	}
      }
      // ----------- Silver ------------

      if ( m_layout == silver ){
	if (fabs(eta)> m_SilverEtaMax)
	  tceff = 1.00; // nothing changes above 3.2
	else if (fabs(eta)> m_BronzeEtaMax)  { // the actual silver part

	  if (m_pueff == HS){       // HS Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = m_fEff;
	    } else { // PU jet
	      tceff = fEffMediumArray[nPt];
	    }
	  }

	  if (m_pueff == PU){       // PU Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = fEffMediumArray[nPt];
	    } else { // PU jet
	      tceff = m_fEff;
	    }
	  }
	} else { // nominal part stays the same

	  if (m_pueff == HS){       // HS Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = m_fEff;
	    } else { // PU jet
	      tceff = fEffNominalArray[nPt];
	    }
	  }

	  if (m_pueff == PU){       // PU Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = fEffNominalArray[nPt];
	    } else { // PU jet
	      tceff = m_fEff;
	    }
	  }
	}
      }

      // ----------- Gold ------------

      if ( m_layout == gold ){
	if (fabs(eta)> m_GoldEtaMax)
	  tceff = 1.00; // nothing changes above 3.8
    
	else if (fabs(eta)> m_SilverEtaMax)  { // the actual gold part

	  if (m_pueff == HS){       // HS Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = m_fEff;
	    } else { // PU jet
	      tceff = fEffExtendedArray[nPt];
	    }
	  }

	  if (m_pueff == PU){       // PU Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = fEffExtendedArray[nPt];
	    } else { // PU jet
	      tceff = m_fEff;
	    }
	  }
	}

	else if (fabs(eta)> m_BronzeEtaMax)  { // medium part stays the same

	  if (m_pueff == HS){       // HS Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = m_fEff;
	    } else { // PU jet
	      tceff = fEffMediumArray[nPt];
	    }
	  }

	  if (m_pueff == PU){       // PU Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = fEffMediumArray[nPt];
	    } else { // PU jet
	      tceff = m_fEff;
	    }
	  }
	} else { // nominal part stays the same

	  if (m_pueff == HS){       // HS Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = m_fEff;
	    } else { // PU jet
	      tceff = fEffNominalArray[nPt];
	    }
	  }

	  if (m_pueff == PU){       // PU Efficiency Scheme
	    if (bHS){ // HS jet
	      tceff = fEffNominalArray[nPt];
	    } else { // PU jet
	      tceff = m_fEff;
	    }
	  }
	}
      }
    }

    //  std::cout  << " m_bUseHGTD0: " << m_bUseHGTD0 << " m_bUseHGTD1: " << m_bUseHGTD1 << std::endl;
    // HGTD add-on two scenarios which surely could be done nicer
    if  (m_bUseHGTD0 || m_bUseHGTD1){

      //      std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff::Warning You are using an obsolete HGTD implementation, for correct one, initialise with layout=Step1p6" << std::endl;

      float minHGTD = 0.;
      if (m_bUseHGTD0)
	minHGTD =  m_HGTD0min;
      if (m_bUseHGTD1)
	minHGTD =  m_HGTD1min;

      if (fabs(eta)> m_HGTDmax){
	tceff = 1.00; // nothing changes above 4.3
      } 
      else if ( fabs(eta) > minHGTD ) {
	// simple scenario for the start - flat in pt (20-100 GeV) and eta, only affecting the HS efficiency --> assumes 2%PU scenario
	//      if (ptGeV > 30. && ptGeV < 100.){
	if (ptGeV > 20. && ptGeV < 100.){
	  float tceff1 = tceff;
	  if (!bHS){
	    if (fabs(eta) > 2.4)
	      tceff = tceff1*0.5;
	    else
	      tceff = tceff1;
	  }
	}
      }
      // else we keep what is in the central region
    }
  } else{

    // new implementation of the track confirmation

    // PU scheme only - 2%, 1%, 0.5%

    if (m_pueff == PU){       // PU Efficiency Scheme

      if (!bHS && ptGeV < 100.) // flat efficiency for PU
	tceff = m_fEff;
      else{ // HS jet
	// pt bin 

	if (ptGeV > 20. && ptGeV < 50.){
      
	  if (m_bUseHGTD0 || m_bUseHGTD1){

	    if (fabs(eta) > 2.4 && fabs(eta) < m_GoldEtaMax) {// forward region - always HGTD once at least one ON
	      tceff = func_IH_lt50->Eval(fabs(eta));
	    }
	    else if ( fabs(eta) > 1.2 && fabs(eta) < 2.4 ){
	    
	      if (m_bUseHGTD0) // nominal IDR HGTD
		tceff = func_TC_lt50->Eval(fabs(eta)); // below 2.4 us ITk only
	      else if (m_bUseHGTD1) // nominal IDR HGTD
		tceff = func_IH_lt50->Eval(fabs(eta)); // below 2.4 us ITk+"HGTD" only
	      else
		std::cout << " This should never happen " << std::endl;
	    } 
	    else if  ( fabs(eta) <= 1.2 ){

	      if (m_bUseHGTD0) // nominal IDR HGTD
		tceff = func_TC_lt50->Eval(1.2); // below 2.4 us ITk only
	      else if (m_bUseHGTD1) // nominal IDR HGTD
		tceff = func_IH_lt50->Eval(1.2); // below 2.4 us ITk+"HGTD" only
	      else
		std::cout << " This should never happen " << std::endl;
	    }
	    else{
	      std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff::Warning You are using eta range outside of the generally adopted areng (<3.8), your eta is = " << eta << std::endl;
	    }
	      
	  }else{ // ITk only

	    if (fabs(eta) > 1.2 && fabs(eta) < m_GoldEtaMax) {// forward region - always HGTD once at least one ON
	      tceff = func_TC_lt50->Eval(fabs(eta));
	    }
	    else if  ( fabs(eta) <= 1.2 ){
	      tceff = func_TC_lt50->Eval(1.2); // below 2.4 us ITk only
	    } 
	    else{
	      std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff::Warning You are using eta range outside of the generally adopted areng (<3.8), your eta is = " << eta << std::endl;
	    }
	  } // end of ITk vs HGTDx

	} // end of low pt bin
	else if (ptGeV > 50. &&  ptGeV < 100.){
	  if (m_bUseHGTD0 || m_bUseHGTD1){

	    if (fabs(eta) > 2.4 && fabs(eta) < m_GoldEtaMax) {// forward region - always HGTD once at least one ON
	      tceff = func_IH_gt50->Eval(fabs(eta));
	    }
	    else if ( fabs(eta) > 1.2 && fabs(eta) < 2.4 ){
	    
	      if (m_bUseHGTD0) // nominal IDR HGTD
		tceff = func_TC_gt50->Eval(fabs(eta)); // below 2.4 us ITk only
	      else if (m_bUseHGTD1) // nominal IDR HGTD
		tceff = func_IH_gt50->Eval(fabs(eta)); // below 2.4 us ITk+"HGTD" only
	      else
		std::cout << " This should never happen " << std::endl;
	    } 
	    else if  ( fabs(eta) <= 1.2 ){

	      if (m_bUseHGTD0) // nominal IDR HGTD
		tceff = func_TC_gt50->Eval(1.2); // below 2.4 us ITk only
	      else if (m_bUseHGTD1) // nominal IDR HGTD
		tceff = func_IH_gt50->Eval(1.2); // below 2.4 us ITk+"HGTD" only
	      else
		std::cout << " This should never happen " << std::endl;
	    }
	    else{
	      std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff::Warning You are using eta range outside of the generally adopted range (<3.8), your eta is = " << eta << std::endl;
	    }
	      
	  }else{ // ITk only

	    if (fabs(eta) > 1.2 && fabs(eta) < m_GoldEtaMax) {// forward region - always HGTD once at least one ON
	      tceff = func_TC_gt50->Eval(fabs(eta));
	    }
	    else if  ( fabs(eta) <= 1.2 ){
	      tceff = func_TC_gt50->Eval(1.2); // below 2.4 us ITk only
	    } 
	    else{
	      std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff::Warning You are using eta range outside of the generally adopted range (<3.8), your eta is = " << eta << std::endl;
	    }
	  } // end of ITk vs HGTDx
	} // end of high pT bin
      }
    }
    else{
      std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff::Warning You are using an unsupported HGTD implementation (HS scheme)!" << std::endl;
      std::cout << " -> for correct one, initialise with  m_upgrade->setPileupEfficiencyScheme(UpgradePerformanceFunctions::PileupEff::PU) " << std::endl;
      tceff = 1.0;
    }
  } // end of Step 1.6 branch

  //  std::cout << "UpgradePerformanceFunctions::getTrackJetConfirmEff:: bHS: " << bHS <<" pt: " << ptGeV << " eta: " << eta << " tc: " << tceff << std::endl;
  
  return tceff;
}

void UpgradePerformanceFunctions::setUseHGTD0(bool usehgtd0){
  m_bUseHGTD0 = usehgtd0;
  if (m_ExtHGTDTrig){
    m_JetTrigMaxEta = m_HGTDMaxTrigEta;
    m_JetTrig4jMin  = m_HGTD0MinExtTrigPt;
  } else { // max trigger eta stays default 3.2
    m_JetTrig4jMin  = m_HGTD0MinTrigPt;
  }
  std::cout << "UpgradePerformanceFunctions::setUseHGTD0 is set to " << m_bUseHGTD0 << std::endl;

}

void UpgradePerformanceFunctions::setUseHGTD1(bool usehgtd1){
  m_bUseHGTD1 = usehgtd1;
  if (m_ExtHGTDTrig){
    m_JetTrigMaxEta = m_HGTDMaxTrigEta;
    m_JetTrig4jMin  = m_HGTD1MinExtTrigPt;
  } else { // max trigger eta stays default 3.2
    m_JetTrig4jMin  = m_HGTD1MinTrigPt;
  }

  std::cout << "UpgradePerformanceFunctions::setUseHGTD1 is set to " << m_bUseHGTD1 << std::endl;

}

void UpgradePerformanceFunctions::extendJetTrigger(bool exttrig){
  m_ExtHGTDTrig = exttrig;
}


float UpgradePerformanceFunctions::getHGTDMinEta(){

  float mineta = -1000.;

  if(m_bUseHGTD0)
    mineta =  m_HGTD0min;
  if(m_bUseHGTD1)
    mineta =  m_HGTD1min;

  return mineta;
}
float UpgradePerformanceFunctions::getHGTDMaxEta(){
   return m_HGTDmax;
}



// void UpgradePerformanceFunctions::setHGTDHSEff(float hgtdhseff){
//   m_HGTDHSEff = hgtdhseff;
// } 
// void UpgradePerformanceFunctions::setHGTDPUEff(float hgtdpueff){
//   m_HGTDPUEff = hgtdpueff;
// } 



#endif
