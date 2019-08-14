#!/sur/bin/env bash

## Parameter testing!
LIB=$1
if [[ ! -f $LIB || -d $LIB ]]; then
    echo "Parameter has to be the shared library (.so / .dll / .dylib)!"
    exit 1
fi

## Test if "strip" is installed
command -v strip &>/dev/null
if [[ $? -ne 0 ]]; then
    echo "'strip' command not found. Has to be installed!"
    exit 1
fi

echo "Discarding unneccessary symbols from library..."

## Strip symbols
strip -x $LIB
if [[ $? -ne 0 ]]; then
    echo "Cannot use 'strip -x $LIB' ?"
    exit 1
fi

## Verify if you want (nm + objdump)
read -n 1 -p "Verify -> nm + objdump (y|n): " ANSWER
case $ANSWER in
	  N|n)	printf "\nDone!\n"
          exit 1 ;;
	  *)		;;
esac


command -v nm &>/dev/null
if [[ $? -ne 0 ]]; then
    exit 1
fi
printf "\nRunning: nm -Ca $LIB | grep GLCM\n"
nm -Ca $LIB | grep GLCM

command -v objdump &>/dev/null
if [[ $? -ne 0 ]]; then
    exit 1
fi
printf "\nRunning: objdump -t $LIB | grep GLCM\n"
objdump -t $LIB | grep GLCM
