export INPUT=/data/tHAnalysis/July1
export OUTPUT=./Plots
rm -f $OUTPUT/*
mkdir $OUTPUT

#####before preselection
root -b -q tHAnalysis/macros/plotSamples.C\(\"NLep_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET_nocuts\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"jet_pt_nocuts\",\"$INPUT\",\"$OUTPUT\"\)

#####after preselection
root -b -q tHAnalysis/macros/plotSamples.C\(\"MET\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Njets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Nbjets\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"Hbjets_m\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_pt\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"lep_eta\",\"$INPUT\",\"$OUTPUT\"\)

###signal regions
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet0Eta_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet0Pt_SRB3\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet0Eta_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
root -b -q tHAnalysis/macros/plotSamples.C\(\"fwdJet0Pt_SRB4\",\"$INPUT\",\"$OUTPUT\"\)
