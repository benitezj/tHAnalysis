#!/bin/bash

file=$1
sample=$2
smear=$3
TC=$4
HGTD=$5

filename=$(echo $file | awk 'BEGIN {FS="/"} ; {print $NF}')
echo "Processing: $file ..."
echo "Filename = $filename"


HOME=/direct/usatlas+u/$USER/tHFramework
cd $HOME

OUTPUTDIR=$HOME/OutputRootFiles

if [[ "$smear" == false ]] ; then 
  OUTPUTDIR=$OUTPUTDIR/mu0/$sample
else
  if [[ "$TC" == false ]] ; then
    OUTPUTDIR=$OUTPUTDIR/mu200_noTC/$sample
  else
    if [[ "$HGTD" == true ]] ; then
      OUTPUTDIR=$OUTPUTDIR/mu200_TC_HGTD/$sample
    else
      OUTPUTDIR=$OUTPUTDIR/mu200_TC/$sample
    fi
  fi
fi

if [ ! -d $OUTPUTDIR ] ; then
  mkdir $OUTPUTDIR
fi

# setup envionment
source rcSetup.sh

TMDPDIR=$(mktemp -p)
cd $TMPDIR

# copy file
cp $file .

# run simpleAnalysis
echo "Running on file: $file"

if [[ "$smear" == false ]] ; then 
  simpleAnalysis -a tH2017 $file
else
  if [[ "$TC" == false ]] ; then
    simpleAnalysis -s mu=200,noTrackConfirm,addPileupJets -a tH2017 $file
  else
    if [[ "$HGTD" == true ]] ; then
      simpleAnalysis -s mu=200,useHGTD0,addPileupJets -a tH2017 $file
    else
      simpleAnalysis -s mu=200,addPileupJets -a tH2017 $file
    fi
  fi
fi

# copy to output directory
if [ -f tH2017.root ] ; then
  outputname=$(echo $filename | awk 'BEGIN {FS=".pool"} ; {print $1}')
  mv tH2017.root $OUTPUTDIR/$outputname.root
  cd $HOME
else
  echo "ERROR: tH2017.root has not been created"
  cd $HOME
  exit 5
fi

exit 0
