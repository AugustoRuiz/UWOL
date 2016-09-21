#!/bin/bash

APPNAME=UWOL
APPBUNDLE=bundle/${APPNAME}.app

# bundle

rm -rf ${APPBUNDLE}
mkdir -p ${APPBUNDLE}/Contents
mkdir -p ${APPBUNDLE}/Contents/{MacOS,Resources,Frameworks}

cp macosx/Info.plist ${APPBUNDLE}/Contents/
cp macosx/PkgInfo ${APPBUNDLE}/Contents/
cp bin/uwol ${APPBUNDLE}/Contents/MacOS/${APPNAME}
cp bin/UWOL.pak ${APPBUNDLE}/Contents/Resources/
cp -R /Library/Frameworks/SDL*.framework ${APPBUNDLE}/Contents/Frameworks/

# icons

rm -rf ${APPNAME}.iconset
mkdir ${APPNAME}.iconset
sips -z 16 16     macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_16x16.png
sips -z 32 32     macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_16x16@2x.png
sips -z 32 32     macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_32x32.png
sips -z 64 64     macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_32x32@2x.png
sips -z 128 128   macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_128x128.png
sips -z 256 256   macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_128x128@2x.png
sips -z 256 256   macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_256x256.png
sips -z 512 512   macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_256x256@2x.png
sips -z 512 512   macosx/${APPNAME}Icon.png --out ${APPNAME}.iconset/icon_512x512.png
cp macosx/${APPNAME}Icon.png ${APPNAME}.iconset/icon_512x512@2x.png
iconutil -c icns -o ${APPBUNDLE}/Contents/Resources/${APPNAME}.icns ${APPNAME}.iconset
rm -r ${APPNAME}.iconset

# Frameworks
install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2 @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 ${APPBUNDLE}/Contents/MacOS/${APPNAME}
install_name_tool -change @rpath/SDL2_image.framework/Versions/A/SDL2_image @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image ${APPBUNDLE}/Contents/MacOS/${APPNAME}
install_name_tool -change @rpath/SDL2_mixer.framework/Versions/A/SDL2_mixer @executable_path/../Frameworks/SDL2_mixer.framework/Versions/A/SDL2_mixer ${APPBUNDLE}/Contents/MacOS/${APPNAME}
