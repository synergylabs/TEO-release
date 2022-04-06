#!/usr/bin/env bash

sudo apt update -qq
sudo apt install -y pigpio


git clone https://github.com/zhanghan177/Smartphone-Doorlock.git

cd Smartphone-Doorlock
npm ci 
