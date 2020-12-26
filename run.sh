#!/bin/bash
make
#./mayerhofer_rankings Teams/Teams_2020 Scores/Scores_2020
Teams="Teams/Teams_${1}"
Scores="Scores/Scores_${1}"

if [ "$2" != "" ]; then
    ./mayerhofer_rankings $Teams $Scores $2
else
    ./mayerhofer_rankings $Teams $Scores
fi