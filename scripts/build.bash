#!/bin/bash

ExitFailure()
{
    popd
    exit 1
}

dir=$(echo "$(dirname "$(realpath "$BASH_SOURCE[0]")")")
configFile="scripts/config.txt"
THREADS=4

pushd $dir
    cd ..

    if ! test -e "$configFile"; then
        echo "ERROR: No config file!"
        ExitFailure
    fi

    config=$(cat "$configFile")

    if ! test -e "CMakeLists.txt"; then
        echo "ERROR: No CMakeLists.txt!"
        ExitFailure
    fi

    case $config in
        "Debug" | "Release")
            echo "NOTIFICATION: Building in $config mode"

            # Create build directory
            buildDir="build/$config"
            ! test -d $buildDir && mkdir -p $buildDir

            # Generate Makefile
            cmake -S . -B $buildDir -G "Unix Makefiles" -D CMAKE_BUILD_TYPE="$config"

            # Build
            cd $buildDir
            if test -e "Makefile"; then
                make -j$THREADS config=$(echo $config | tr [:upper:] [:lower:])
            else
                echo "ERROR: No Makefile"
            fi
            ;;
        *)
            echo "ERROR: Unknown build type"
        ;;
    esac
popd
