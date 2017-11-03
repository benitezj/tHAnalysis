#!/bin/bash

file=$1
sample=$2
smear=$3
PUJets=$4
TC=$5
HGTD=$6
HGTDscheme=$7
HGTDBTAG=$8
BTAGSCHEME=$9
EFFSCHEME=${10}
PUEFF=${11}
BTAGOP=${12}
PUREJ=${13}
USEFLATEFF=${14}
FLATLEFF=${15}
NTUPLE=${16}


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
    if [[ "$PUJets" == false ]] ; then
	dirname+="_noPUJets"
    fi  
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
	dirname+="_HGTD$HGTDscheme"

	if [[ "$HGTDBTAG" == true ]] ; then
	    dirname+="_HGTDbtag_$BTAGSCHEME"
	fi

	if [[ "$PUREJ" == true ]] ; then 
	    dirname+="_purej"
	fi
    fi
    if [[ "$BTAGOP" == "85" ]] ; then
      dirname+="_btagOP85"
    fi  
    if [[ "$USEFLATEFF" == true ]] ; then
      dirname+="_flatLEff$FLATLEFF"
    fi  
    OUTPUTDIR=$OUTPUTDIR/$dirname/$sample
fi

if [ ! -d $OUTPUTDIR ] ; then
    mkdir $OUTPUTDIR
fi

# setup envionment
source "rcSetup.sh"

TMDPDIR=$(mktemp -p)
cd $TMPDIR

# copy file
cp $file .

# run simpleAnalysis
echo "Running on file: $file"

# build smearing string
if [[ "$smear" == true ]] ; then 
  # Create submission command based on options provided
    smearString="-s mu=200"
    if [[ "$PUJets" == true ]] ; then
	smearString+=",addPileupJets"  
    fi
    if [[ "$TC" == false ]] ; then
	smearString+=",noTrackConfirm"
    fi
    if [[ "$HGTD" == true ]] ; then
	smearString+=",useHGTD$HGTDscheme"
	if [[ "$HGTDBTAG" == true ]] ; then 
	    smearString+=",useHGTDbtag"
	fi

	if [[ "$BTAGSCHEME" == "lrej" ]] ; then 
	    smearString+=",btagScheme=lrej"  
	elif [[ "$BTAGSCHEME" == "crej" ]] ; then 
	    smearString+=",btagScheme=crej"	    
	elif [[ "$BTAGSCHEME" == "beff" ]] ; then 
	    smearString+=",btagScheme=beff"
	fi

	if [[ "$PUREJ" == true ]] ; then 
	    smearString+=",useHGTD_PUrejx2"
	fi
    fi
    if [[ "$EFFSCHEME" == "HS" ]] ; then
	smearString+=",HSeff=$PUEFF"
    elif [[ "$EFFSCHEME" == "PU" ]] ; then
	smearString+=",PUeff=$PUEFF"
    fi
    if [[ "$BTAGOP" == "85" ]] ; then
	smearString+=",btagOP=85"
    fi	
    if [[ "$USEFLATEFF" == true ]] ; then
	smearString+=",useFlatEff=$FLATLEFF"
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
