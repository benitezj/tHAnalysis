#!/bin/bash

source ~/scripts/colors.sh

checkFinished() {

  printf "${BLUE}Checking output files ...${NC}\n"
  
  samples=(ttbar ttH_dilep ttH_semilep tH tWH)
  samplesdir=/usatlas/u/sargyrop/tHFramework/tHAnalysis/data/PathsBNL/TRUTH3

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

# Get the options
while [[ $# > 0 ]] ; do
  arg="$1"
  case $arg in 
    -check|-c)
    checkonly=true ;;
  esac
  shift   
done

base_dir=/usatlas/u/sargyrop/tHFramework/OutputRootFiles
cd $base_dir

# Flag to test if all files have been produced
allok=true

# Check if all files for all samples have been produced
checkFinished

# Now merge the files
if [[ "$allok" == true ]] ; then
  mergeFiles
fi

exit 0

