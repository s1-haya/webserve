FROM nginx:1.26.1

RUN apt-get update && apt-get install -y vim

COPY /test/nginx/nginx.conf /etc/nginx/conf.d/default.conf

RUN mkdir -p /var/www/dav && chown www-data:www-data /var/www/dav
COPY /test/nginx/dav.conf /etc/nginx/conf.d/dav.conf

EXPOSE 80
