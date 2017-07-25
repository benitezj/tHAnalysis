

####
# Baseline
####

# mu=0 - truth level stuff - no smearing
./submit.sh -n

# mu=200 no Pileup Jets - smearing with track confirmation const PU efficiency 2% (ITk only) => BASELINE
./submit.sh -s -noPUJets -n


####
# All options below are with PU jets
####

# mu=200 track confirmation const PU efficiency 2% (ITk only)
./submit.sh -s -n

# mu=200 no track confirmation
./submit.sh -s -noTC

# mu=200 track confirmation const PU efficiency 2% + HGTD
./submit.sh -s -HGTD -PU 0.02

# mu=200 track confirmation const PU efficiency 5% + HGTD
./submit.sh -s -HGTD -PU 0.05

# mu=200 track confirmation const PU efficiency 10% + HGTD
./submit.sh -s -HGTD -PU 0.10

# mu=200 track confirmation const HS efficiency 70% + HGTD
./submit.sh -s -HGTD -HS 0.70

# mu=200 track confirmation const HS efficiency 80% + HGTD
./submit.sh -s -HGTD -HS 0.80

# mu=200 track confirmation const HS efficiency 90% + HGTD
./submit.sh -s -HGTD -HS 0.90


####
# All options below are for testing the improvement in l-jet rejection with the HGTD
####
./submit.sh -s -HGTD -HGTDbtag -PU 0.02

./submit.sh -s -HGTD -HGTDbtag -HS 0.90
