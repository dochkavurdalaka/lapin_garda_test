# Use Ubuntu 24.04 as the base image
FROM ubuntu:24.04

LABEL maintainer="Lapin Dmitriy <dmitriylapin2001@gmail.com>"
COPY . /lapin_garda_test

#-y is for accepting yes when the system asked us for installing the package
RUN apt-get update && apt-get install -y \
    curl \
    cmake \
    gcc-12\
    ninja-build \
    build-essential \
    && rm -rf /var/lib/apt/lists/*


