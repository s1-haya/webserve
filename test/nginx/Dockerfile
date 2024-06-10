FROM nginx:1.26.1

RUN apt-get update && \
    apt-get install -y telnet=6.0 --no-install-recommends && \
    rm -rf /var/lib/apt/lists/*

EXPOSE 8080

COPY config/nginx.conf /etc/nginx/nginx.conf
