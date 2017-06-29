#!/bin/bash

file=$1
sample=$2
filename=$(echo $file | awk 'BEGIN {FS="/"} ; {print $NF}')
echo "Processing: $file ..."
echo "Filename = $filename"


HOME=/direct/usatlas+u/sargyrop/tHFramework
cd $HOME

OUTPUTDIR=$HOME/OutputRootFiles/$sample

# setup envionment
source rcSetup.sh

TMDPDIR=$(mktemp -p)
cd $TMPDIR

# copy file
cp $file .

# run simpleAnalysis
echo "Running on file: $file"
simpleAnalysis -a tHAnalysis $file

ls

# copy to output directory
if [ -f tHAnalysis.root ] ; then
  outputname=$(echo $filename | awk 'BEGIN {FS=".pool"} ; {print $1}')
  mv tHAnalysis.root $OUTPUTDIR/$outputname.root
  cd $HOME
else
  echo "ERROR: tHAnalysis.root has not been created"
  cd $HOME
  exit 5
fi

exit 0
