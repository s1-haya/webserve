FROM ubuntu:20.04

# 非対話的モードを設定
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y tzdata --no-install-recommends

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:deadsnakes/ppa && \
    apt-get update && \
    apt-get install -y \
    git=1:2.25.1-1ubuntu3.12 \
    python3=3.8.2-0ubuntu2 \
    python3-pip=20.0.2-5ubuntu1.10 \
    build-essential \
    curl=7.68.0-1ubuntu2.22 \
    vim-common=2:8.1.2269-1ubuntu5.23 vim-runtime=2:8.1.2269-1ubuntu5.23 vim=2:8.1.2269-1ubuntu5.23 \
    clang-format=1:10.0-50~exp1 \
    valgrind=1:3.15.0-1ubuntu9 \

WORKDIR /webserv

COPY requirements.txt /webserv/requirements.txt
RUN pip install --upgrade pip && pip install --no-cache-dir -r /webserv/requirements.txt

COPY srcs /webserv/srcs
COPY html /webserv/html
COPY Makefile /webserv/Makefile

RUN rm -rf /var/lib/apt/lists/*

CMD ["make", "-C", "/webserv", "run"]
