#!/bin/bash

source ~/scripts/colors.sh

checkMissing() {
  sample=$1
  samplesdir=/usatlas/u/$USER/tHFramework/tHAnalysis/data/PathsBNL/TRUTH1
  # Loop over input files
  for i in `cat $samplesdir/$sample.txt` ; do
    inname=$(echo $i | awk 'BEGIN {FS="/"} ; {print $NF}' | awk 'BEGIN {FS=".pool"} ; {print $1}')
    outname=$inname.root
    if [ ! -f $base_dir/$sample/$outname ] ; then
      echo $i
    fi
  done
}

checkFinished() {

  outputdir=$(echo $base_dir | awk 'BEGIN {FS="/"} ; {print $NF}')

  printf "${BLUE}Checking output files: $outputdir ...${NC}\n"
  
  samples=(ttbar ttH_dilep ttH_semilep tH tWH)
  samplesdir=/usatlas/u/$USER/tHFramework/tHAnalysis/data/PathsBNL/TRUTH1

  # Loop over samples and check if number of output files is as expected
  for sample in "${samples[@]}" ; do 
    # calculate number of input files
    ninput=$(wc -l $samplesdir/$sample.txt | awk '{print $1}')
    # calculate number of output files
    noutput=$(ls $base_dir/$sample/*.root 2> /dev/null | wc -l)
    # If numbers don't agree print them
    if (( noutput != ninput )) ; then
      printf "${RED}%20s :\t Finished:%3d\t\tExpected:%3d ${NC}\n" $sample $noutput $ninput
      allok=false
    fi
  done
  
  if [[ "$allok" == true ]] ; then
    echo -e "...${GREEN}All OK ${NC}"
  fi
}


mergeFiles() {
  samples=(ttbar ttH_dilep ttH_semilep tH tWH)
  for sample in "${samples[@]}" ; do 
    cd $base_dir/$sample
    echo -e "${BLUE}Merging files - Output: $sample.root ${NC}\n"
    rm -f $sample.root
    hadd $sample.root *.root
  done
  echo -e "${GREEN}DONE\n${NC}"
}

# Main script
checkonly=false
checkMissing=false
smearing=false
trackConfirm=true
HGTD=false
HGTDBTAG=false
btagscheme=""
purej=false
addPUJets=true
effScheme="PU"
puEff=0.02
btagOP=70

# Get the options
while [[ $# > 0 ]] ; do
  arg="$1"
  case $arg in 
    -check|-c)
    checkonly=true ;;
    -checkMissing|-cm|-cM)
    checkmissing=true
    sampleCheck=$2
    shift ;;
    -smear|-s)
    smearing=true ;;
    -noTC)
    trackConfirm=false ;;
    -noPUJets)
    addPUJets=false;;
    -HGTD)
    HGTD=true ;;
    -HGTDbtag)
    HGTDBTAG=true
    btagscheme=$2
    shift ;;
    -purej)
    purej=true;;
    -n)
    ntup="-n" ;;
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
    *)
    echo "Unknown option: $arg" 
    exit 1 ;;
  esac
  shift   
done


if [[ "$smearing" == false ]] ; then
  base_dir=/usatlas/u/$USER/tHFramework/OutputRootFiles/mu0
else
  # build directory name from options
  dirname="mu200"
  if [[ "$addPUJets" == false ]] ; then
    dirname+="_noPUJets"
  fi  
  if [[ "$trackConfirm" == false ]] ; then
    dirname+="_noTC"
  else
    dirname+="_TC"
    if [[ "$effScheme" = "PU" ]] ; then
      dirname+="_PU$puEff"
    elif [[ "$effScheme" = "HS" ]] ; then
      dirname+="_HS$puEff"
    fi
  fi 
  if [[ "$HGTD" == true ]] ; then 
    dirname+="_HGTD"
    if [[ "$HGTDBTAG" == true ]] ; then 
      dirname+="_HGTDbtag"
      if [[ "$btagscheme" != "" ]] ; then
	  dirname+="_$btagscheme"
      fi
    fi
    if [[ "$purej" == true ]] ; then 
      dirname+="_purej"
    fi
  fi
  if [[ "$btagOP" == "85" ]] ; then
    dirname+="_btagOP85"
  fi  
  base_dir=/usatlas/u/$USER/tHFramework/OutputRootFiles/$dirname
fi

# Create directory if it doesn't exist
if [ ! -d $base_dir ] ; then
  mkdir $base_dir
fi
cd $base_dir

# Flag to test if all files have been produced
allok=true

if [[ "$checkmissing" == true ]] ; then
  checkMissing $sampleCheck
  exit 0
fi

# Check if all files for all samples have been produced
if [[ "$checkonly" == true ]] ; then
  checkFinished
  exit 0
fi

# Now merge the files
checkFinished
if [[ "$allok" == true ]] ; then
  mergeFiles
fi

exit 0

