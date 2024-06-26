FROM httpd:2.4

RUN apt-get update && apt-get install -y \
	vim \
	perl

COPY test/apache/config/httpd.conf /usr/local/apache2/conf/httpd.conf
COPY test/apache/first.pl /usr/local/apache2/cgi-bin/first.pl

RUN chmod a+x /usr/local/apache2/cgi-bin/first.pl

EXPOSE 4242
