FROM ubuntu:20.04

# 非対話的モードを設定
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    python3=3.8.2-0ubuntu2 \
    python3-pip=20.0.2-5ubuntu1.10 \
    build-essential \
    curl=7.68.0-1ubuntu2.22 \
    vim-common=2:8.1.2269-1ubuntu5.23 vim-runtime=2:8.1.2269-1ubuntu5.23 vim=2:8.1.2269-1ubuntu5.23 \
    clang-format=1:10.0-50~exp1 \
    valgrind=1:3.15.0-1ubuntu9 \
    cppcheck=1.90-4build1 \
    tzdata=2024a-0ubuntu0.20.04 --no-install-recommends \
    git=1:2.25.1-1ubuntu3.13

WORKDIR /webserv

COPY requirements.in /webserv/requirements.in
RUN pip install --upgrade pip && \
    pip install --no-cache-dir pip-tools && \
    pip-compile /webserv/requirements.in --strip-extras && \
    pip-sync

COPY srcs /webserv/srcs
COPY html /webserv/html
COPY Makefile /webserv/Makefile
COPY docker.mk /webserv/docker.mk

RUN rm -rf /var/lib/apt/lists/*

CMD ["make", "run"]
