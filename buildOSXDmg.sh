#!/bin/bash

# delete previous image
rm bundle/UWOL.dmg

# create files
cp macosx/uwol.json bundle/
cp macosx/background.png bundle/
cp Ficha.txt bundle/README.txt
mkdir -p bundle/Licenses
cp bin/*.txt bundle/Licenses
# todo add readme file
cd bundle/
appdmg uwol.json UWOL.dmg

# remove temp files
rm uwol.json
rm background.png
rm README.txt
rm -rf Licenses
