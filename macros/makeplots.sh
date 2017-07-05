export INPUT=/data/tHAnalysis/July5
export OUTPUT=./Plots
rm -f $OUTPUT/*
mkdir $OUTPUT

######before preselection
#root -b -q tHAnalysis/macros/plotSamples.C\(\"NLep_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jet_pt_nocuts\",\"$INPUT\",\"$OUTPUT\"\)

#####Preselection
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Nbjets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"NAntiBjets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_eta\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadJetPt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadBJetPt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"sumAllPt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m1\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m2\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt1\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt2\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_pt\",\"$INPUT\",\"$OUTPUT\"\)

################################################################
####3B tags
################################################################
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"sumAllPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"NAntiBjets_SRBX\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadJetPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadBJetPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Eta_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)

root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m1_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt1_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m2_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt2_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)

####3B + mH cut 
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEtaH2_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPtH2_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEtaH3_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPtH3_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)



#############################################################
#####4B tags
#############################################################
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"sumAllPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"NAntiBjets_SRBX\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadJetPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadBJetPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Eta_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m1_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt1_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m2_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt2_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

#### + mH cut 
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEtaH2_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPtH2_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEtaH3_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPtH3_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

