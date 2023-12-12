#!/bin/bash

patch_commiter='PatchCommitter.sh'
tar_name='patches.tar.gz'
config='./config.json'

patch_destination=$(grep -o '"patch_destination": "[^"]*' $config | grep -o '[^"]*$')

function gen {
    wait $(./GarbageCollector)
}

function make_tar {
    wait $(gen)
    cp $patch_commiter $patch_destination
    tar -czvf $tar_name $patch_destination
    mv $tar_name $destination
}

function apply_tar {
    mv $tar_path ./
    fname=$(basename $tar_path)
    tar -xzvf $fname
    sh $patch_destination/$patch_commiter
}

function gen_and_run {
    wait $(gen)
    cp $patch_commiter $patch_destination
    pushd $patch_destination
    sh $patch_commiter
    popd
}

function set_regex {
    sed -i '/^    "pack_regexs":/c\    "pack_regexs": ['"$regex"'],' $config
}

function set_verbose {
    sed -i '/^    "is_quiet":/c\    "is_quiet": false' $config
}

function set_quiet {
    sed -i '/^    "is_quiet":/c\    "is_quiet": true' $config
}

function help {
   # Display Help
   echo "Add description of the script functions here."
   echo
   echo "Usage: $(basename \$0) [-c] [-t tar_destination] [-r] [-a tar_path] [-s regex] [-q] [-h]"
   echo "options:"
   echo "c     Open and edit config."
   echo "t     Generates patches to destionation."
   echo "r     Generates and applies patch."
   echo "a     Applies patch from tar."
   echo "s     Set regex for packages with delemeter ','."
   echo "q     Quiet mode."
   echo "h     Print this Help."
   echo
}

rm -rf $patch_destination
mkdir $patch_destination
set_verbose

while getopts 'ctrasqh:' OPTION; do
  case "$OPTION" in
    c)
      vim $config
      ;;
    t)
      destination="$OPTARG"
      echo "Generating patches.tar.gz to $OPTARG"
      make_tar
      ;;
    r)
      echo "Generating and applying patch"
      gen_and_run
      ;;
    a)
      tar_path="$OPTARG"
      echo "Applying patches from $OPTARG"
      apply_tar
      ;;
    s)
      regex="$OPTARG"
      set_regex
      ;;
    q)
      set_quiet
      ;;
    h)
      help
      exit 0
      ;;
    ?)
      echo "script usage: $(basename \$0) [-t tar_destination] [-r] [-a tar_path] [-s regex] [-q]" >&2
      exit 1
      ;;
  esac
done
shift "$(($OPTIND -1))"