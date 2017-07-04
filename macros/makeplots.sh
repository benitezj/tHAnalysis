export INPUT=/data/tHAnalysis/July4
export OUTPUT=./Plots
rm -f $OUTPUT/*
mkdir $OUTPUT

######before preselection
#root -b -q tHAnalysis/macros/plotSamples.C\(\"NLep_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
#root -b -q tHAnalysis/macros/plotSamples.C\(\"jet_pt_nocuts\",\"$INPUT\",\"$OUTPUT\"\)

#####after preselection
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Nbjets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_eta\",\"$INPUT\",\"$OUTPUT\"\)

root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m1\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m2\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3\",\"$INPUT\",\"$OUTPUT\"\)


####3B signal regions
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Eta_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)

root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m1_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt1_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m2_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt2_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRB3\",\"$INPUT\",\"$OUTPUT\"\)

#root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_Higgs_fwdJet_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_fwdJets_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_leadJets_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB3\",\"$INPUT\",\"$OUTPUT\"\)


####4B signal region
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Eta_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet1Pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m1_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt1_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m2_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt2_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_m3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Higgs_pt3_SRB4\",\"$INPUT\",\"$OUTPUT\"\)

#root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_Higgs_fwdJet_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_fwdJets_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"deltaEta_leadJets_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"top_m_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
