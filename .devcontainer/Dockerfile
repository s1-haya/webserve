FROM ubuntu:22.04

# 非対話的モードを設定
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y tzdata

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:deadsnakes/ppa && \
    apt-get update && \
    apt-get install -y \
    python3.9 python3.9-distutils \
    build-essential \
    git \
    telnet \
    valgrind \
    clang-format \
    vim \
    curl

RUN curl -sS https://bootstrap.pypa.io/get-pip.py | python3.9
COPY requirements.txt /workspaces/webserv/requirements.txt
RUN pip install --upgrade pip && pip install -r /workspaces/webserv/requirements.txt

RUN rm -rf /var/lib/apt/lists/*

VOLUME [ "/workspaces" ]

WORKDIR /workspaces/webserv

CMD [ "/bin/bash" ]
