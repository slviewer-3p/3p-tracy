#!/usr/bin/env bash

cd "$(dirname "$0")" 

# turn on verbose debugging output for parabuild logs.
exec 4>&1; export BASH_XTRACEFD=4; set -x
# make errors fatal
set -e
# complain about unset env variables
set -u

# Check autobuild is around or fail
if [ -z "$AUTOBUILD" ] ; then 
    exit 1
fi
if [ "$OSTYPE" = "cygwin" ] ; then
    autobuild="$(cygpath -u $AUTOBUILD)"
else
    autobuild="$AUTOBUILD"
fi

top="$(pwd)"
stage="/tmp/3p-tracy-stage"

# Load autobuild provided shell functions and variables
source_environment_tempfile="$top/stage/source_environment.sh"
"$autobuild" source_environment > "$source_environment_tempfile"
. "$source_environment_tempfile"

TRACY_VERSION="$(sed -n -E 's/(v[0-9]+\.[0-9]+\.[0-9]+) \(.+\)/\1/p' NEWS | head -1)"

build=${AUTOBUILD_BUILD_ID:=0}
echo "${TRACY_VERSION}.${build}" > "$top/stage/VERSION.txt"

mkdir -p "$stage/include/tracy"
cp -r $top/* "$stage/include/tracy"

cp -rf $stage $top/
rm -rf $stage

rm -rf $top/stage/.git
rm $top/stage/autobuild.xml
rm $top/stage/build-cmd.sh
rm $top/stage/BuildParams


