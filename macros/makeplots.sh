export INPUT=/data/tHAnalysis/July6
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
root -b -q tHAnalysis/macros/plotSamples.C\(\"NLightjets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_eta\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadJetPt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadBJetPt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"sumAllPt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_pt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_jfwd_b1\",\"$INPUT\",\"$OUTPUT\"\)


################################################################
####3B tags
################################################################
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"sumAllPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"NLightjets_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadJetPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadBJetPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEta_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_jfwd_b1_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_H3_jfwd_SRB3\",\"$INPUT\",\"$OUTPUT\"\)

####3B + mH cut 
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEtaH3_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPtH3_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEtaH3_jfwd_b1_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_H3_jfwd_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)




#############################################################
#####4B tags
#############################################################
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"sumAllPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"NLightjets_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadJetPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"leadBJetPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEta_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_jfwd_b1_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_H3_jfwd_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

#### + mH cut 
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetEtaH3_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJetPtH3_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEtaH3_jfwd_b1_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_H3_jfwd_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

