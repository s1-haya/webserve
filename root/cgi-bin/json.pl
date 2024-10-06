#!/usr/bin/perl
use strict;
use warnings;

print "Content-Type: application/json\r\n\r\n";

my %response = (
    status => 'success',
    message => 'Hello, world!',
);

my $json_response = "{\n";
$json_response .= '  "status": "' . $response{status} . "\",\n";
$json_response .= '  "message": "' . $response{message} . "\"\n";
$json_response .= "}";

print $json_response;
