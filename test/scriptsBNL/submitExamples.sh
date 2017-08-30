#!/bin/bash
if [[ "$#" != 1 ]] ; then
  echo "Usage: ./submitExamples.sh submit|merge|check"
  exit 1
fi

if [[ "$1" == "submit" ]] ; then
  command=./submit.sh
elif [[ "$1" == "merge" ]] ; then
  command=./mergeBatchOutput.sh
elif [[ "$1" == "check" ]] ; then
  command="./mergeBatchOutput.sh -c" 
else
  echo "Usage: ./submitExamples.sh submit|merge|check"
  exit 2
fi

####
# Baseline
####

# mu=0 - truth level stuff - no smearing
#$command 

# mu=200 no Pileup Jets - smearing with track confirmation const PU efficiency 2% (ITk only) => BASELINE
$command -s -noPUJets 


####
# All options below are with PU jets
####

# mu=200 no track confirmation
$command -s -noTC
#
## mu=200 track confirmation const PU efficiency 2% 
$command -s -PU 0.02
#
## mu=200 track confirmation const PU efficiency 1% 
$command -s -PU 0.01
#
## mu=200 track confirmation const PU efficiency 0.5% 
$command -s -PU 0.005
#
## mu=200 track confirmation const HS efficiency 70% 
#$command -s -HS 0.70
#
## mu=200 track confirmation const HS efficiency 80% 
#$command -s -HS 0.80
#
## mu=200 track confirmation const HS efficiency 90% 
#$command -s -HS 0.90

####
# Add HGTD, -purej = pileup rejection * 2
####

# mu=200 track confirmation const PU efficiency 2% + HGTD
#$command -s -HGTD -PU 0.02
$command -s -HGTD -purej -PU 0.02

# mu=200 track confirmation const PU efficiency 1% + HGTD
#$command -s -HGTD -PU 0.05
$command -s -HGTD -purej -PU 0.01

# mu=200 track confirmation const PU efficiency 0.05% + HGTD
#$command -s -HGTD -PU 0.10
$command -s -HGTD -purej -PU 0.005

# mu=200 track confirmation const HS efficiency 70% + HGTD
#$command -s -HGTD -HS 0.70
#$command -s -HGTD -purej -HS 0.70

# mu=200 track confirmation const HS efficiency 80% + HGTD
#$command -s -HGTD -HS 0.80
#$command -s -HGTD -purej -HS 0.80

# mu=200 track confirmation const HS efficiency 90% + HGTD
#$command -s -HGTD -HS 0.90
#$command -s -HGTD -purej -HS 0.90

####
# Add HGTD, -purej = pileup rejection * 2 with 85% WP
####

# mu=200 track confirmation const PU efficiency 5% + HGTD
#$command -s -HGTD -PU 0.05
#$command -s -HGTD -purej -PU 0.02 -btagOP 85

####
# All options below are for testing the improvement in l-jet/c-jet rejection or b-efficiency with the HGTD
####

#$command -s -HGTD -HGTDbtag lrej -purej -PU 0.02

#$command -s -HGTD -HGTDbtag crej -purej -PU 0.02

#$command -s -HGTD -HGTDbtag beff -purej -PU 0.02

####
# All options below are for testing the improvement in l-jet/c-jet rejection or b-efficiency with the HGTD w/ 85% WP
####

#$command -s -HGTD -HGTDbtag lrej -purej -PU 0.02 -btagOP 85 

#$command -s -HGTD -HGTDbtag crej -purej -PU 0.02 -btagOP 85

#$command -s -HGTD -HGTDbtag beff -purej -PU 0.02 -btagOP 85


exit 0
