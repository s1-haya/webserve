FROM nginx:1.26.1

RUN apt-get update && \
    apt-get install -y telnet --no-install-recommends && \
    rm -rf /var/lib/apt/lists/*

EXPOSE 80
