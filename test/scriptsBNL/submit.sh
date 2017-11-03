#!/bin/bash

# Sample declaration
samples=(ttbar tWH tH ttH_dilep ttH_semilep)
#samples=(tH)

# Default options
smearing=false
addPUJets=true
trackConfirm=true
HGTD=false
HGTDscheme=0 # 0 is for forward only , 1 is for central+forward
HGTDBTAG=false
btagscheme="lrej"
purej=true
ntup=""
effScheme="PU"
puEff=0.02
btagOP=70
useFlatEff=false
flatLEff=0.0033

while [[ $# > 0 ]] ; do
  arg="$1"
  case $arg in 
    -smear|-s)
    smearing=true ;;
    -noTC)
    trackConfirm=false ;;
    -HGTD)
    HGTD=true 
    HGTDscheme=$2
    shift ;;
    -HGTDbtag)
    HGTDBTAG=true
    btagscheme=$2
    shift ;;
    -noPUJets)
    addPUJets=false;;
    -n)
    ntup="-n" ;;
    -purej)
    purej=true ;;
    -HS)
    effScheme="HS"
    puEff=$2
    shift ;;
    -PU)
    effScheme="PU"
    puEff=$2
    shift ;;
    -btagOP)
    btagOP=$2
    shift ;;
    -useFlatEff)
    useFlatEff=true
    flatLEff=$2
    shift ;;    
    *)
    echo "Unknown option: $arg" 
    exit 1 ;;
  esac
  shift   
done

# Check whether supported HS and PU schemes have been provided
if [[ "$smearing" == true ]] ; then
  if [[ "$effScheme" == "PU" && "$puEff" != "0.02" && "$puEff" != "0.01" && "$puEff" != "0.005" ]] ; then
    echo "Unsupported PU scheme: $puEff"
    echo "Supported values: 0.02, 0.01, 0.005"
    exit 2
  fi
  if [[ "$effScheme" == "HS" && "$puEff" != "0.70" && "$puEff" != "0.80" && "$puEff" != "0.90" ]] ; then
    echo "Unsupported HS scheme: $puEff"
    echo "Supported values: 0.70, 0.80, 0.90"
    exit 3
  fi
  if [[ "$btagOP" != "70" && "$btagOP" != "85" ]] ; then
    echo "Unsupported b-tag OP: $btagOP"
    echo "Supported values: 70 85"
    exit 4
  fi
fi


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
    if [[ "$smearing" == false ]] ; then 
	files=($(./mergeBatchOutput.sh -cM $sample))
    else
	smearString="-s"
	if [[ "$addPUJets" == false ]] ; then
	    smearString+=" -noPUJets"
	fi   
	if [[ "$trackConfirm" == false ]] ; then
	    smearString+=" -noTC"
	fi
	if [[ "$HGTD" == true ]] ; then
	    smearString+=" -HGTD $HGTDscheme"

	    if [[ "$HGTDBTAG" == true ]] ; then 
		smearString+=" -HGTDbtag $btagscheme"
	    fi
	    if [[ "$purej" == true ]] ; then 
		smearString+=" -purej"
	    fi
	fi
	if [[ "$effScheme" == "HS" ]] ; then
	    smearString+=" -HS $puEff"
	elif [[ "$effScheme" == "PU" ]] ; then
	    smearString+=" -PU $puEff"  
	fi
	if [[ "$btagOP" == "85" ]] ; then
	    smearString+=" -btagOP 85"
	fi    
	if [[ "$useFlatEff" == true ]] ; then
	    smearString+=" -useFlatEff $flatLEff"
	fi    
	files=($(./mergeBatchOutput.sh -cM $sample $smearString))
    fi
      
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
    sed -i "s|PUJETS|$addPUJets|g" temp.sub
    sed -i "s|TC|$trackConfirm|g" temp.sub
    sed -i "s|HGTD|$HGTD|" temp.sub
    sed -i "s|HGTDSCHEME|$HGTDscheme|" temp.sub
    sed -i "s|BTAGHGTD|$HGTDBTAG|g" temp.sub
    sed -i "s|BTAGSCHEME|$btagscheme|g" temp.sub
    sed -i "s|NTUPLE|$ntup|g" temp.sub
    sed -i "s|EFFSCHEME|$effScheme|g" temp.sub
    sed -i "s|PUEFF|$puEff|g" temp.sub
    sed -i "s|PUREJ|$purej|g" temp.sub
    sed -i "s|BTAGOP|$btagOP|g" temp.sub
    sed -i "s|USEFLATEFF|$useFlatEff|g" temp.sub
    sed -i "s|FLATLEFF|$flatLEff|g" temp.sub
    condor_submit temp.sub
    rm -f temp.sub
    let job=$job+1
  done
  
done

exit 0
