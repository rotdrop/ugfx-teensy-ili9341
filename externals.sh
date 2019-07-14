#!/bin/bash

while read PATH BRANCH REMOTE TYPE ; do
    echo "$PATH;$BRANCH;$REMOTE;$TYPE"
done < EXTERNALS.csv
