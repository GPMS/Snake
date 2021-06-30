#!/bin/sh

config=`cat Scripts/config.txt`
runDirectory=""

if [ ! "$config" = "Debug" -a ! "$config" = "Release" ]; then
    echo "Unknown build type"
    exit 1
fi
echo "Running in $config mode"

runDir="build/$config"

cp -R resources $runDir
cd $runDir && ./Snake
