FROM ubuntu:22.04

# 非対話的モードを設定
ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /webserv

RUN apt-get update && \
    apt-get install -y \
    build-essential --no-install-recommends

COPY srcs /webserv/srcs
COPY root /webserv/root
COPY Makefile /webserv/Makefile
COPY Dockerfile /webserv/Dockerfile
COPY config /webserv/config

RUN rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["make", "run"]
