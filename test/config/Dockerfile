FROM nginx:1.26.1

RUN apt-get update && \
    apt-get install -y telnet vim

EXPOSE 8080

RUN mkdir -p /data/www
COPY html/ /data/www/

RUN mkdir -p /etc/nginx/conf_file
COPY test/config/check.sh /var/check.sh
