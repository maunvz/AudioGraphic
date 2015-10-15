rm Resources/icon.icns
rm -rf AG.app
iconutil -c icns Resources/icon.iconset
mkdir AG.app
cd AG.app
mkdir Contents
cd Contents
cp ../../Resources/Info.plist Info.plist
mkdir MacOS
cp ../../ag MacOS/ag
mkdir Resources
cp ../../Resources/icon.icns Resources/AppIcon.icns
