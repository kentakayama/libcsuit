#!/bin/bash

cd `dirname $0`/..

SEQ="5 4 3 2 1"
for N in ${SEQ}; do
  echo "docker build ${N}"
  sudo docker build -f ./misc/dockerfile/min${N}.Dockerfile -t libcsuit_min${N} . > /dev/null
  sudo docker build -f ./misc/dockerfile/nm${N}.Dockerfile -t libcsuit_nm${N} . > /dev/null
done

echo "===================================="

for N in ${SEQ}; do
  sed -n 3p ./misc/dockerfile/nm${N}.Dockerfile
  NAME=`sed -n 3p ./misc/dockerfile/nm${N}.Dockerfile | cut -c 3-`
  echo "|                 | ${NAME} |"
  sudo docker run -t libcsuit_nm${N}
done
