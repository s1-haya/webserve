FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    git \
    curl \
    telnet \
    valgrind \
    clang-format

RUN add-apt-repository ppa:longsleep/golang-backports && \
    apt update && \
    apt install -y \
    golang-1.17 \
    siege
