FROM --platform=linux/x86_64 ubuntu:22.04

# 非対話的モードを設定
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    git=1:2.34.1-1ubuntu1.11 \
    python3=3.10.6-1~22.04.1 \
    python3-pip=22.0.2+dfsg-1ubuntu0.4 \
    build-essential=12.9ubuntu3 \
    curl=7.81.0-1ubuntu1.18 \
    vim=2:8.2.3995-1ubuntu2.19 \
    clang-format=1:14.0-55~exp2 \
    valgrind=1:3.18.1-1ubuntu2 \
    cppcheck=2.7-1 \
    tzdata=2024a-0ubuntu0.22.04.1 \
    siege=4.0.7-1build3 \
    strace=5.16-0ubuntu3 \
    net-tools=1.60+git20181103.0eebece-1ubuntu5 \
    iputils-ping=3:20211215-1 --no-install-recommends

# ここでないとできない
RUN apt-get update && apt-get install -y python3.8-dev

WORKDIR /workspaces/webserv

COPY / /workspaces/webserv/
RUN pip install --upgrade pip && \
    pip install --no-cache-dir pip-tools==7.4.1 && \
    pip-compile /workspaces/webserv/requirements.in --strip-extras && \
    pip-sync

RUN rm -rf /var/lib/apt/lists/*

# 新しいユーザーを作成。この時点ではuserはroot
RUN useradd -ms /bin/bash user_webserv
# root権限が必要なinstall等を終えたら、作成したユーザーに切り替え
USER user_webserv

VOLUME [ "/workspaces" ]

ENTRYPOINT [ "/bin/bash" ]
