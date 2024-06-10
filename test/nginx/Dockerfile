FROM nginx:latest

RUN apt-get update && \
    apt-get install -y telnet

EXPOSE 80

COPY config/nginx.conf /etc/nginx/nginx.conf
