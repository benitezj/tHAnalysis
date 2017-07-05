#!/bin/bash

#samples=(ttbar tWH tH ttH_dilep ttH_semilep)
samples=(tH)
smearing=false
trackConfirm=true
HGTD=false

while [[ $# > 0 ]] ; do
  arg="$1"
  case $arg in 
    -smear|-s)
    smearing=true ;;
    -noTC)
    trackConfirm=false ;;
    -HGTD)
    HGTD=true ;;
  esac
  shift   
done

# Set up directory structure - done automatically for BNL and uiowapc
BASEDIR=$(echo $PWD | awk 'BEGIN {FS="/tHAnalysis"} ; {print $1}')

if [[ $BASEDIR == *"usatlas"* ]] ; then 
  SCRIPTDIR=$BASEDIR/tHAnalysis/test/scriptsBNL
  LOGDIR=$SCRIPTDIR/logs
else
  SCRIPTDIR=$BASEDIR/tHAnalysis/test/scriptsIowa
  LOGDIR=$SCRIPTDIR/logs
fi

# Create directory to store log files if missing
if [ ! -d $LOGDIR ] ; then
  mkdir $LOGDIR
fi

# move to submission directory
cd $SCRIPTDIR


# Loop over samples
for sample in "${samples[@]}" ; do
 
  if [[ $BASEDIR == *"usatlas"* ]] ; then 
    pathFile=../../data/PathsBNL/TRUTH1/$sample.txt
  else
    pathFile=../../data/PathsIowa/$sample.txt 
  fi  
  
  # Get missing files
  files=($(./mergeBatchOutput.sh -cM $sample))
  
  # Loop over input files and submit jobs
  job=0
  for file in "${files[@]}" ; do 
    sed "s|FWPATH|$SCRIPTDIR|g" template.sub > temp.sub
    sed -i "s|SAMPLE|$sample|g" temp.sub
    sed -i "s|FILE|$file|g" temp.sub
    sed -i "s|USER|$USER|g" temp.sub
    sed -i "s|LOGPATH|$LOGDIR|g" temp.sub
    sed -i "s|JOB|$job|g" temp.sub
    sed -i "s|SMEAR|$smearing|g" temp.sub
    sed -i "s|TC|$trackConfirm|g" temp.sub
    sed -i "s|HGTD|$HGTD|g" temp.sub
    condor_submit temp.sub
    rm -f temp.sub
    let job=$job+1
  done
  
done

exit 0
