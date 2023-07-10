#!/bin/bash

cd `dirname $0`/..

FROM_IMAGE="debian:latest"
ARCH=`arch`
case "${ARCH}" in
  armv7*)
    FROM_IMAGE="arm32v7/ubuntu:18.04"
    ;;
esac

SEQ="5 4 2 1"
for N in ${SEQ}; do
  echo "docker build ${N}"
  sudo docker build -f ./misc/dockerfile/min${N}.Dockerfile -t libcsuit_min${N} --build-arg FROM_IMAGE=$FROM_IMAGE . > /dev/null
  sudo docker build -f ./misc/dockerfile/nm${N}.Dockerfile -t libcsuit_nm${N} . > /dev/null
done

OUTPUT_FILE=misc/result_size_${ARCH}.txt
> $OUTPUT_FILE
for N in ${SEQ}; do
  sed -n 3p ./misc/dockerfile/nm${N}.Dockerfile >> $OUTPUT_FILE
  NAME=`sed -n 3p ./misc/dockerfile/nm${N}.Dockerfile | cut -c 3-`
  echo "|                 | ${NAME} |" >> $OUTPUT_FILE
  sudo docker run -t libcsuit_nm${N} >> $OUTPUT_FILE
done
cat $OUTPUT_FILE

./misc/calc_table.py $ARCH
