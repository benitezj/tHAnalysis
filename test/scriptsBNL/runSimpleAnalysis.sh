#!/bin/bash

file=$1
sample=$2
smear=$3
TC=$4
HGTD=$5
EFFSCHEME=$6
PUEFF=$7
NTUPLE=$8

filename=$(echo $file | awk 'BEGIN {FS="/"} ; {print $NF}')
echo "Processing: $file ..."
echo "Filename = $filename"


HOME=/direct/usatlas+u/$USER/tHFramework
cd $HOME

OUTPUTDIR=$HOME/OutputRootFiles

if [[ "$smear" == false ]] ; then 
  OUTPUTDIR=$OUTPUTDIR/mu0/$sample
else
  dirname="mu200"
  if [[ "$TC" == false ]] ; then
    dirname+="_noTC"
  else
    dirname+="_TC"
    if [[ "$EFFSCHEME" = "PU" ]] ; then
      dirname+="_PU$PUEFF"
    elif [[ "$EFFSCHEME" = "HS" ]] ; then
      dirname+="_HS$PUEFF"
    fi
  fi 
  if [[ "$HGTD" == true ]] ; then 
    dirname+="_HGTD"
  fi
  OUTPUTDIR=$OUTPUTDIR/$dirname/$sample
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

# build smearing string
if [[ "$smear" == true ]] ; then 
  # Create submission command based on options provided
  smearString="-s mu=200,addPileupJets"  
  if [[ "$TC" == false ]] ; then
    smearString+=",noTrackConfirm"
  fi
  if [[ "$HGTD" == true ]] ; then
    smearString+=",useHGTD0"
  fi
  if [[ "$EFFSCHEME" == "HS" ]] ; then
    smearString+=",HSeff=$PUEFF"
  elif [[ "$EFFSCHEME" == "PU" ]] ; then
    smearString+=",PUeff=$PUEFF"
  fi
fi

# buld command string
commandString="$smearString -a tH2017 $NTUPLE $file"
echo "==> Running simpleAnalysis $commandString"
simpleAnalysis $commandString

# copy to output directory
if [ -f tH2017.root ] ; then
  outputname=$(echo $filename | awk 'BEGIN {FS=".pool"} ; {print $1}')
  echo "Copy file to: $OUTPUTDIR"
  mv tH2017.root $OUTPUTDIR/$outputname.root
  cd $HOME
else
  echo "ERROR: tH2017.root has not been created"
  cd $HOME
  exit 5
fi

exit 0
