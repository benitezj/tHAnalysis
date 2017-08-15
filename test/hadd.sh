export PROD=/nfs/uiowapc/data/tHAnalysis/July20

rm -f  $PROD/tH2017_tHqb.root
rm -f  $PROD/tH2017_tWH.root 
rm -f  $PROD/tH2017_ttH_semilep.root 
rm -f  $PROD/tH2017_ttH_dilep.root
rm -f  $PROD/tH2017_ttbar.root

hadd $PROD/tH2017_tHqb.root $PROD/tH/*.root
hadd $PROD/tH2017_tWH.root $PROD/tWH/*.root 
hadd $PROD/tH2017_ttH_semilep.root $PROD/mc15_13TeV.343366.aMcAtNloPythia8EvtGen_A14_NNPDF23_NNPDF30ME_ttH125_semilep.merge.DAOD_TRUTH3.e4706_p2949/*.root
hadd $PROD/tH2017_ttH_dilep.root $PROD/mc15_13TeV.343365.aMcAtNloPythia8EvtGen_A14_NNPDF23_NNPDF30ME_ttH125_dilep.merge.DAOD_TRUTH3.e4706_p2949/*.root
hadd $PROD/tH2017_ttbar.root $PROD/mc15_13TeV.410501.PowhegPythia8EvtGen_A14_ttbar_hdamp258p75_nonallhad.merge.DAOD_TRUTH3.e5458_p2949/*.root
