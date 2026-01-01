#!/usr/bin/bash

# check if XDG_DATA_HOME is set already?
if [[ -z "$XDG_DATA_HOME" ]]; then
    if [[ -z "$HOME" ]]; then
        echo "ERROR: HOME environment variable is not set. Cannot set XDG_DATA_HOME."
        exit 1
    elif [[ $HOME == "/root" || $USER == "root" || -z "$USER" ]]; then
        export XDG_DATA_HOME="/home/.local/share"
    else
        export XDG_DATA_HOME="$HOME/.local/share"
    fi
fi

# export DISPLAY=:0.0
mkdir -pv "$XDG_DATA_HOME/medict/"
cp /work/data/test.json "$XDG_DATA_HOME/medict/"

./build/medict-dev -v
