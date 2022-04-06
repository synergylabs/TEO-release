#!/usr/bin/env bash
# Setup for install Motion camera on Raspberry Pi

sudo apt update
sudo apt upgrade -y

sudo apt-get install -y autoconf automake autopoint build-essential pkgconf libtool libzip-dev libjpeg-dev git libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libavdevice-dev libwebp-dev gettext libmicrohttpd-dev

cd ~
#git clone https://github.com/Motion-Project/motion.git
git clone https://github.com/zhanghan177/motion.git
cd motion
autoreconf -fiv
./configure
make -j$(nproc)
sudo make install

# Add motion to global path
echo "export TOT_SOURCE_PATH=${PWD}" >> ~/.bashrc
echo "export TOT_MOTION_LOG_PATH=${HOME}/motion.log" >> ~/.bashrc
echo "export TOT_MOTION_CONF_PATH=${HOME}/motion.conf" >> ~/.bashrc
source ~/.bashrc

# Post installation
# Link motion configuration file
cd ~
ln -vis "${TOT_SOURCE_PATH}/cases/motion/motion.conf" "${TOT_MOTION_CONF_PATH}"


## See config file in <project_root>/bin/motion.conf for more details
