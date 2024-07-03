#!/usr/bin/perl
use strict;
use warnings;

print "Content-type: text/plain\n\n";

my $content_length = $ENV{'CONTENT_LENGTH'};
if (defined $content_length) {
    read(STDIN, my $post_data, $content_length);
    print "POST data: $post_data\n";
} else {
    print "No POST data received.\n";
}
