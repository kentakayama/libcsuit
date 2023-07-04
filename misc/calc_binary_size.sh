#!/bin/bash

cd `dirname $0`/..

for N in 0 1 2 3 4 5; do
  sudo docker build -f min${N}.Dockerfile -t libcsuit_min${N} .
done

for N in 0 1 2 3 4 5; do
  sudo docker run -t libcsuit_min${N}
done
