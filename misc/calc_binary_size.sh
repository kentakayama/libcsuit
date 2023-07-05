#!/bin/bash

cd `dirname $0`/..

SEQ="0 1 2 3 4 5"
for N in ${SEQ}; do
  echo "docker build -f ./misc/dockerfile/min${N}.Dockerfile -t libcsuit_min${N} ."
  sudo docker build -f ./misc/dockerfile/min${N}.Dockerfile -t libcsuit_min${N} . > /dev/null
done

echo "===================================="
sudo docker run -t libcsuit_min0

SEQ="1 2 3 4 5"
for N in ${SEQ}; do
  sed -n 3p ./misc/dockerfile/min${N}.Dockerfile
  sudo docker run -t libcsuit_min${N}
  echo
done
