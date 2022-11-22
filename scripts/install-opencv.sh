#!/bin/bash

if (( $EUID > 0 )); then
	echo " - Please run as root"
	exit
fi

# Version of opencv to install
VERSION='4.5.5'
CUDA=true

echo "- Create libs folder"
mkdir -p ../libs
cd ../libs

echo " - Update aptitude"
apt update

echo " - Install Build tools"
apt install -y cmake gcc g++ make git unzip curl

echo " - Installing Dependencies"
apt install -y libavcodec-dev libavformat-dev libswscale-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev libgtk-3-dev libpng-dev libjpeg-dev libopenexr-dev libtiff-dev libwebp-dev libxvidcore-dev x264 libx264-dev libfaac-dev libmp3lame-dev libtheora-dev libvorbis-dev libtbb-dev libatlas-base-dev gfortran

echo " - Install OpenCV Contrib Modules"
echo "    - Clone repository"
git clone https://github.com/opencv/opencv_contrib.git

echo "    - Checkout version $VERSION branch"
cd opencv_contrib
git fetch
git checkout $VERSION

cd ..

echo " - Install OpenCV"
echo "    - Clone repository"
git clone https://github.com/opencv/opencv.git

echo "    - Checkout version $VERSION branch"
cd opencv
git fetch
git checkout $VERSION

echo "    - Generate makefile"
mkdir build
cd build
cmake -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules/ -D BUILD_EXAMPLES=OFF -D WITH_V4L=ON -D WITH_TBB=ON -D OPENCV_ENABLE_NONFREE=1 -D ENABLE_FAST_MATH=1  .. #-D WITH_CUDA=ON -D WITH_CUDNN=ON -D OPENCV_DNN_CUDA=ON -D CUDA_FAST_MATH=1 -D CUDA_ARCH_BIN=7.5 .. 

echo "    - Build and Install"
make install -j8

echo " - Done!"