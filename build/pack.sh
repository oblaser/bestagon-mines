#!/bin/bash

# author        Oliver Blaser
# date          02.02.2022
# copyright     OLC-3 - Copyright (c) 2022 Oliver Blaser



source dep_globals.sh

exeName=bestagon-mines
platform=$(uname -m)
packedDir="./packed"
outDirName="${exeName}_linux_$platform"
outDir="$packedDir/$outDirName"
archive="$packedDir/${exeName}_linux_${platform}_v$versionstr.tar.gz"

rm -rf $outDir

mkdir -p $outDir/$exeName/assets

cp ../assets/smiley.png $outDir/$exeName/assets
cp ../assets/smiley_click.png $outDir/$exeName/assets
cp ../assets/smiley_expl.png $outDir/$exeName/assets
cp ../assets/smiley_hover2.png $outDir/$exeName/assets
cp ../assets/smiley_won.png $outDir/$exeName/assets

cp ./cmake/$exeName $outDir/$exeName/
cp ../assets/icon.png $outDir/$exeName/$exeName.png

cp ../license.txt $outDir/$exeName/license.txt
cp dep_readme.txt $outDir/$exeName/readme.txt

rm -f $archive
tar -czf $archive -C $outDir $exeName
