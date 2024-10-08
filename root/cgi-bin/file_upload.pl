#!/usr/bin/perl
use strict;
use warnings;

print "Content-Type: text/html\r\n\r\n";

if ($ENV{'REQUEST_METHOD'} eq 'POST') {
    my $boundary = $ENV{'CONTENT_TYPE'} =~ /boundary=(.*)$/ ? $1 : '';
    my $content_length = $ENV{'CONTENT_LENGTH'};
    if ($boundary && $content_length) {
        read(STDIN, my $post_data, $content_length);
        if ($post_data =~ /filename="([^"]+)".*?\r\n\r\n(.*?)\r\n--$boundary/s) {
            my ($filename, $file_content) = ($1, $2);
            open my $fh, '>', "/tmp/$filename" or die "Cannot open file: $!";
            print $fh $file_content;
            close $fh;
            print "<html><body><h1>File uploaded successfully</h1></body></html>";
        } else {
            print "<html><body><h1>Failed to parse file upload</h1></body></html>";
        }
    } else {
        print "<html><body><h1>Invalid POST data</h1></body></html>";
    }
} else {
    print <<'END_HTML';
<html>
<body>
<h1>File Upload (upload to /tmp)</h1>
<form method="POST" enctype="multipart/form-data">
    <input type="file" name="file"><br>
    <input type="submit" value="Upload">
</form>
</body>
</html>
END_HTML
}