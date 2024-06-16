FROM nginx:1.26.1

RUN apt-get update && \
    apt-get install -y telnet

EXPOSE 8080

COPY config/nginx.conf /etc/nginx/nginx.conf
