#!/bin/bash

cd `dirname $0`
cd ..

sudo docker build -f min0.Dockerfile -t libcsuit_min0 .
sudo docker build -f min1.Dockerfile -t libcsuit_min1 .
sudo docker build -f min2.Dockerfile -t libcsuit_min2 .
sudo docker build -f min3.Dockerfile -t libcsuit_min3 .
sudo docker build -f min4.Dockerfile -t libcsuit_min4 .
sudo docker build -f min5.Dockerfile -t libcsuit_min5 .

sudo docker run -t libcsuit_min0
sudo docker run -t libcsuit_min1
sudo docker run -t libcsuit_min2
sudo docker run -t libcsuit_min3
sudo docker run -t libcsuit_min4
sudo docker run -t libcsuit_min5
