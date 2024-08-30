#!/usr/bin/perl

while (1) {
    open my $fh, '>', '/dev/null';
    print $fh "This will exhaust file descriptors.\n";
}
