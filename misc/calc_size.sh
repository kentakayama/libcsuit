#!/bin/sh
cd `dirname $0`
cd ..

docker build -f min0.Dockerfile -t libcsuit_min0 .
docker build -f min1.Dockerfile -t libcsuit_min1 .
docker build -f min2.Dockerfile -t libcsuit_min2 .
docker build -f min3.Dockerfile -t libcsuit_min3 .
docker build -f min4.Dockerfile -t libcsuit_min4 .

docker run -t libcsuit_min0
docker run -t libcsuit_min1
docker run -t libcsuit_min2
docker run -t libcsuit_min3
docker run -t libcsuit_min4
