#!/usr/bin/perl
use strict;
use warnings;

print "Content-Type: text/plain\r\n\r\n";

if ($ENV{'REQUEST_METHOD'} eq 'GET') {
    my $path_info = $ENV{'PATH_INFO'};
    if ($path_info) {
        print "PathInfo string: $path_info\n";
    } else {
        print "No path info received.\n";
    }
} else {
    print "This script only handles GET requests.\n";
}
