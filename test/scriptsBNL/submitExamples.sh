#!/bin/bash
if [[ "$#" != 1 ]] ; then
  echo "Usage: ./submitExamples.sh submit|merge"
  exit 1
fi

if [[ "$1" == "submit" ]] ; then
  command=./submit.sh
elif [[ "$1" == "merge" ]] ; then
  command=./mergeBatchOutput.sh
else
  echo "Usage: ./ubmitExamples.sh submit|merge"
  exit 2
fi

####
# Baseline
####

# mu=0 - truth level stuff - no smearing
$command -n

# mu=200 no Pileup Jets - smearing with track confirmation const PU efficiency 2% (ITk only) => BASELINE
$command -s -noPUJets -n


####
# All options below are with PU jets
####

# mu=200 track confirmation const PU efficiency 2% (ITk only)
$command -s -n

# mu=200 no track confirmation
$command -s -noTC

# mu=200 track confirmation const PU efficiency 2% + HGTD
$command -s -HGTD -PU 0.02

# mu=200 track confirmation const PU efficiency 5% + HGTD
$command -s -HGTD -PU 0.05

# mu=200 track confirmation const PU efficiency 10% + HGTD
$command -s -HGTD -PU 0.10

# mu=200 track confirmation const HS efficiency 70% + HGTD
$command -s -HGTD -HS 0.70

# mu=200 track confirmation const HS efficiency 80% + HGTD
$command -s -HGTD -HS 0.80

# mu=200 track confirmation const HS efficiency 90% + HGTD
$command -s -HGTD -HS 0.90


####
# All options below are for testing the improvement in l-jet rejection with the HGTD
####
$command -s -HGTD -HGTDbtag -PU 0.02

$command -s -HGTD -HGTDbtag -HS 0.90

exit 0
