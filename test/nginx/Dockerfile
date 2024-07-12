FROM nginx:1.26.1

RUN apt-get update && apt-get install -y vim

COPY /test/nginx/nginx.conf /etc/nginx/conf.d/default.conf

EXPOSE 80
