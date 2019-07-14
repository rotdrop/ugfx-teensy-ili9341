#!/bin/bash

# DRY=echo

top_srcdir=$(dirname $(realpath $0))

cd $top_srcdir

function createExternal ()
{
    local LOCALPATH=$1
    local BRANCH=$2
    local REMOTE=$3
    local TYPE=$4

    if [ "$TYPE" = subtree ]; then
        $DRY git subtree add --prefix $LOCALPATH $REMOTE $BRANCH --squash
    else
        $DRY git submodule add -b $BRANCH $REMOTE $LOCALPATH
    fi
}

while IFS=\; read LOCALPATH BRANCH REMOTE TYPE ; do
    if [ "$LOCALPATH" = "PATH" ]; then
        continue
    fi
    echo "$LOCALPATH;$BRANCH;$REMOTE;$TYPE"
    if [ -d $top_srcdir/$LOCALPATH ]; then
        echo $LOCALPATH exists
    else
        createExternal $LOCALPATH $BRANCH $REMOTE $TYPE
    fi
done < EXTERNALS.csv
