FROM nginx:1.26.1

RUN apt-get update && \
    apt-get install -y telnet vim

EXPOSE 8080

RUN mkdir -p /data/www
COPY html/ /data/www/

COPY test/config/conf_file/ /etc/nginx/
COPY test/config/check.sh /var/check.sh
