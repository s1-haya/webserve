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

# デフォルトのPythonバージョンをPython 3.9に変更
RUN update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.9 1
RUN update-alternatives --set python3 /usr/bin/python3.9

WORKDIR /webserv

COPY srcs /webserv/srcs
COPY html /webserv/html
COPY Makefile /webserv/Makefile
COPY requirements.txt /webserv/requirements.txt
RUN pip install --upgrade pip && pip install -r /webserv/requirements.txt

RUN rm -rf /var/lib/apt/lists/*

CMD ["make", "-C", "/webserv", "run"]
