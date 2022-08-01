#/bin/bash
ver=$1
dir=$2
curl -X GET  https://www.lua.org/ftp/lua-${ver}.tar.gz --output ${dir}/lua-5.4.4.tar.gz
(cd ${dir} ; tar -xf lua-${ver}.tar.gz)
