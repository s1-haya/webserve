#!/usr/bin/perl
use strict;
use warnings;

print "Content-type: text/plain\n\n";
if ($ENV{'REQUEST_METHOD'} eq 'POST') {
    my $content_length = $ENV{'CONTENT_LENGTH'};
    if (defined $content_length) {
        if (read(STDIN, my $post_data, $content_length)) {
            print "POST data: $post_data\n";
        } else {
            print "Error: Faild to read, POST data received.\n";
        }
    } else {
        print "No POST data received.\n";
    }
} else {
    print "This script only handles POST requests.\n";
}

