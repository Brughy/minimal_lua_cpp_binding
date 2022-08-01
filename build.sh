#!/bin/bash
  
  build_dir="cmake-build-debug"
  NUMTEST=1
  
function _clean () {
  LUAVER=$1
  echo "Cleaning..."  
  \rm -rf lua/lua-${LUAVER}
  \rm -rf lua/lua-${LUAVER}.tar.gz    
  \rm -rf ${build_dir}  
  \rm -rf examples/mylib/libmylib.so
  for ((i=0; i<${NUMTEST}; i++)); do
       (cd examples/dir${i}/ ; rm -rf ../../${build_dir}/examples/dir${i}/libmylib.so)
	(cd examples/dir${i}/ ; rm -rf ../../${build_dir}/examples/dir${i}/main)
  done
}
 
function _download () {
  LUAVER=$1
  if [ ! -f lua/lua-${LUAVER}.tar.gz ]; then
	echo "Downloading lua-${LUAVER}..."  
	./scripts/lua_download.sh ${LUAVER} lua
  fi 
}  

function _build () {
  SHARED=$1
  LUAVER=$2
  ARCH=$3
  mkdir -p ${build_dir}
  (
  echo "Building project..."
  cd ${build_dir} || exit
  cmake -Wdev -Wdeprecated --debug-output -DCMAKE_BUILD_TYPE=Debug -GNinja -DLUA_ENABLE_SHARED=${SHARED} -DLUAVER=${LUAVER} -DNO_FETCH=OFF -S .. -B .
  #-DARCH=${ARCH}
  echo "Building apps..."
  ninja
  )
}

function _examples () {
  echo "Run examples..."
  for ((i=0; i<${NUMTEST}; i++)); do
  	echo "## ---------- EXAMPLE ${i} ---------- "
	(\cp ./${build_dir}/examples/mylib/libmylib.so examples/dir${i}/ )
	(cd examples/dir${i}/ ; ../../${build_dir}/examples/dir${i}/main)
  done
}

function _test () {
  echo "Google Testing apps... to be done"...
}

function _deploy () {
  tar_source_dir="${build_dir}/cpp-lua-bridges_lib"
  version=$(git describe --dirty --tag --always --first-parent --match ver[0-9]* | sed -e 's|^ver||' -e 's|-|.|g')
  tar_name="cpp-lua-bridges_lib-${version}.tar.gz"
  (
  cd "${tar_source_dir}" || exit
  tar -czvf "${tar_name}" *
  mv "${tar_name}" ..
  )
  curl -u $A_U_ID:$A_API -X PUT "https://repo.xxx.com/artifactory/${tar_name}" -T "${build_dir}/${tar_name}"
}

# Set some default values:
USAGE=""
CLEAR=""
BUILD=""
TEST=""
EXAMPLES=""
DEPLOY=""
SHARED=0
ARCH=x86_64

LUAVER='5.4.4'

usage()
{
  echo "Usage: ${0}
  -h | --help
		print this help
	-c | --clear
		clear build
	-b | --build
		to build the applications
	-e | --examples
		to run examples		
	-t | --test
		to test the applications		
	-d | --deploy
		to deploy the applications
	-s | --shared
		to do share lib (Default. static .a)		
	-a | --arch
		select architecture. arm-32, arm-64, x86_64, x86_32
		Default x86_64
		
		
	Example:	./build.sh -c -b
			./build.sh -e	
			./build.sh -t	
		"
  exit 2
}

check_parameters() {
  if [ -z "${2}" ]; then
    echo "Error: Argument for ${1} is missing" >&2
    exit 1
  fi
}

if [[ $1 == "" ]]; then
    usage "${0}"
    exit 0
fi

PARSED_ARGUMENTS=$(getopt -n "${0}" -o "hcbetdsa:" --long "help,clear,build,test,deploy,arch:" -- "$@")
VALID_ARGUMENTS=$?
if [ "$VALID_ARGUMENTS" != "0" ]; then
  usage "${0}"
fi

echo "PARSED_ARGUMENTS is $PARSED_ARGUMENTS"
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "${1}" in
     -h | --help)
      USAGE="1";
      shift
      ;;
    -c | --clear)
      CLEAR="1";
      shift
      ;;
    -b | --build)
      BUILD="1";
      shift
      ;;
    -e | --examples)
      EXAMPLES="1";
      shift
      ;;        
    -t | --test)
      TEST="1";
      shift
      ;;      
    -d | --deploy)
      DEPLOY="1";
      shift
      ;;
    -s | --shared)
      SHARED=1;
      shift
      ;;      
     -a | --arch)
      ARCH=${2};
      shift
      shift
      ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift;
        break
        ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: ${1} - this should not happen."
       usage "${0}"
       ;;
  esac
done

echo "CLEAR    : ${CLEAR}"
echo "BUILD    : ${BUILD}"
echo "EXAMPLES : ${EXAMPLES}"
echo "TEST     : ${TEST}"
echo "DEPLOY   : ${DEPLOY}"
echo "SHARED   : ${SHARED}"
echo "ARCH     : ${ARCH}"

if [[ ${USAGE} != "" ]]; then
    usage;
fi
if [[ ${CLEAR} != "" ]]; then
    _clean ${LUAVER}
fi
if [[ ${BUILD} != "" ]]; then
    _download ${LUAVER} 
    _build ${SHARED} ${LUAVER} ${ARCH}
fi
if [[ ${EXAMPLES} != "" ]]; then
    _examples
fi
if [[ ${TEST} != "" ]]; then
    _test
fi
if [[ ${DEPLOY} != "" ]]; then
    _deploy
fi
