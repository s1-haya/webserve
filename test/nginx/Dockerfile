FROM nginx:1.26.1

RUN apt-get update && apt-get install -y vim

RUN mkdir -p /var/www/dav && chown www-data:www-data /var/www/dav
RUN chown www-data:www-data /usr/share/nginx/html/
COPY /test/nginx/dav.conf /etc/nginx/conf.d/default.conf

RUN mkdir -p /var/www/dav/upload && chmod 777 /var/www/dav/upload
RUN chmod 777 /usr/share/nginx/html

EXPOSE 80
