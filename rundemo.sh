#!/bin/bash

MEMORY_SIZE=FFFF # Ammount of memory for the emulator to create.
DEMO_DIR=demos # Directory where demos are found.

# Ensure CLI emulator is built:
cd emulator/build/
cmake ..
make cli
cd ../../

if [ "$1" ] # Check if command-line argument supplied.
  then
    DEMO_SRC="$DEMO_DIR/$1.asm"
    DEMO_BUILD="$DEMO_DIR/$1.demo"

    if [ -r $DEMO_SRC ]
      then
        # Build demo if it has not already been built previously:
        if [ ! -x $DEMO_BUILD ]
          then
            nasm $DEMO_SRC -f bin -o $DEMO_BUILD
        fi

        # Run the demo program on the emulator:
        ./emulator/build/cli $MEMORY_SIZE $DEMO_BUILD
    else
        echo "Demo specified does not exist: $DEMO_SRC"
    fi
else
    echo "Please specify a demo to run as a command-line argument."
fi