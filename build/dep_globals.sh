# author        Oliver Blaser
# date          01.02.2022
# copyright     OLC-3 - Copyright (c) 2022 Oliver Blaser

versionstr="0.1.0-alpha"

function ptintTitle()
{
    if [ "$2" = "" ]
    then
        echo "  --=====#   $1   #=====--"
    else
        echo -e "\033[3$2m  --=====#   \033[9$2m$1\033[3$2m   #=====--\033[39m"
    fi
}
