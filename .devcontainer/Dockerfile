FROM ubuntu:20.04

# 非対話的モードを設定
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    git=1:2.25.1-1ubuntu3.13 \
    python3=3.8.2-0ubuntu2 \
    python3-pip=20.0.2-5ubuntu1.10 \
    build-essential \
    curl=7.68.0-1ubuntu2.22 \
    vim-common=2:8.1.2269-1ubuntu5.23 vim-runtime=2:8.1.2269-1ubuntu5.23 vim=2:8.1.2269-1ubuntu5.23 \
    clang-format=1:10.0-50~exp1 \
    valgrind=1:3.15.0-1ubuntu9 \
    cppcheck=1.90-4build1 \
    tzdata=2024a-0ubuntu0.20.04 --no-install-recommends

# ここでないとできない
RUN apt-get update && apt-get install -y python3.8-dev

WORKDIR /workspaces/webserv

COPY / /workspaces/webserv/
RUN pip install --upgrade pip && \
    pip install --no-cache-dir pip-tools==7.4.1 && \
    pip-compile /workspaces/webserv/requirements.in --strip-extras && \
    pip-sync

# この書き方だとここ(pip系の後)でないとできない
RUN apt-get update && \
    apt-get install -y lsb-release wget software-properties-common gnupg && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 14 && \
    apt update && \
    apt install clang-format-14 && \
    ln -sf /usr/bin/clang-format-14 /usr/local/bin/clang-format

RUN rm -rf /var/lib/apt/lists/*

VOLUME [ "/workspaces" ]

ENTRYPOINT [ "/bin/bash" ]
