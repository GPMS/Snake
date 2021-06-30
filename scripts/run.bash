#!/bin/bash

ExitFailure()
{
    popd
    exit 1
}

dir=$(echo "$(dirname "$(realpath "$BASH_SOURCE[0]")")")
configFile="scripts/config.txt"

pushd $dir
    cd ..

    # Pick right excutable
    if ! test -e "$configFile"; then
        echo "ERROR: No config file!"
        ExitFailure
    fi
    config=$(cat "$configFile")
    case $config in
        "Debug" | "Release")
            exec="build/$config/Snake"
            ;;
        *)
            echo "Unknown build type"
            ExitFailure
            ;;
    esac

    # Run
    if test -x "$exec"; then
        echo "NOTIFICATION: Running in $config mode"
        "$exec"
    else
        echo "ERROR: No executable!"
        ExitFailure
    fi
popd
