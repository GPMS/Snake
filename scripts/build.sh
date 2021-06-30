#!/bin/sh

config=`cat Scripts/config.txt`

if [ ! "$config" = "Debug" -a ! "$config" = "Release" ]; then
    echo "Unknown build type"
    exit 1
fi
echo "Building in $config mode"

buildDir="build/$config"

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$config -B $buildDir
mv $buildDir/compile_commands.json .
cp -R resources $buildDir
cd $buildDir && make

