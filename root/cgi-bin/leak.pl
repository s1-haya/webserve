test/apache/cgi/leak.pl#!/usr/bin/perl

my @array;
while (1) {
    push @array, "This will consume memory.";
}
