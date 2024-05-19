FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y software-properties-common

RUN add-apt-repository ppa:longsleep/golang-backports && \
    apt-get update && \
    apt-get install -y \
    golang-1.22 \
    build-essential \
    git \
    curl \
    telnet \
    valgrind \
    clang-format

RUN ln -s /usr/lib/go-1.22/bin/go /usr/local/bin/go

