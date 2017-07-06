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

  printf "${BLUE}Checking output files ...${NC}\n"
  
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
    -HGTD)
    HGTD=true ;;
  esac
  shift   
done


if [[ "$smearing" == false ]] ; then
  base_dir=/usatlas/u/$USER/tHFramework/OutputRootFiles/mu0
else
  if [[ "$trackConfirm" == false ]] ; then
    base_dir=/usatlas/u/$USER/tHFramework/OutputRootFiles/mu200_noTC
  else
    base_dir=/usatlas/u/$USER/tHFramework/OutputRootFiles/mu200_TC
  fi 
  if [[ "$HGTD" == true ]] ; then 
    base_dir=$base_dir'_HGTD'
  fi
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

