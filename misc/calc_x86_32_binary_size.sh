#!/bin/bash

cd `dirname $0`/..

sudo docker buildx build --platform linux/386 -t libcsuit_nm1_32 -f misc/dockerfile/nm1_32.Dockerfile .
sudo docker run --platform linux/386 -t libcsuit_nm1_32 > misc/result_size_x86_32.txt
