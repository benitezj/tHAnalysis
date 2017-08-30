#export INPUT=/data/tHAnalysis/August7/mu0
#export INPUT=/data/tHAnalysis/August7/mu200_TC_HS0.70
#export INPUT=/data/tHAnalysis/August7/mu200_TC_HS0.70_HGTD
export INPUT=/data/tHAnalysis/August7/mu200_TC_PU0.05
#export INPUT=/data/tHAnalysis/August7/mu200_TC_PU0.05_HGTD_HGTDpuRejx2
export OUTPUT=./Plots
rm -f $OUTPUT/*
mkdir $OUTPUT

######before preselection
#root -b -q tHAnalysis/macros/plotSamples.C\(\"NLep_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jet_pt_nocuts\",\"$INPUT\",\"$OUTPUT\"\)

######Preselection
#root -b -q tHAnalysis/macros/plotSamples.C\(\"MET\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"Nbjets\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"NLightjets\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_eta\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"j1_pt\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"b1_pt\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"HT\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_m\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_pt\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"top_pt\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_jfwd_b1\",\"$INPUT\",\"$OUTPUT\"\)


################################################################
####3B tags1-lep. 3 b-tag 1-2 light-jets
################################################################
#root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"HT_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"NLightjets_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"j1_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"b1_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_eta_SRB3\",\"$INPUT\",\"$OUTPUT\",\"1-lep,\ 3\ b-tag\,\ 1-2\ light-jets\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_jfwd_b1_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_m_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_H3_jfwd_SRB3\",\"$INPUT\",\"$OUTPUT\"\)

#####3B + mH cut 
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_eta_SRMbbH3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_pt_SRMbbH3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_pt_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_H3_jfwd_SRMbb_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_jfwd_b1_SRMbbH3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)



#############################################################
#####4B tags
#############################################################
#root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"HT_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"NLightjets_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"j1_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"b1_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_eta_SRB4\",\"$INPUT\",\"$OUTPUT\",\"1-lep,\ 4\ b-tag\,\ 1-2\ light-jets\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_jfwd_b1_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_m_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_H3_jfwd_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

##### + mH cut 
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_eta_SRMbbH3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jfwd_pt_SRMbbH3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"H3_pt_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_H3_jfwd_SRMbb_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"dEta_jfwd_b1_SRMbbH3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
