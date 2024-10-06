#!/usr/bin/perl
use strict;
use warnings;

print "Content-Type: text/html\r\n\r\n";

if ($ENV{'REQUEST_METHOD'} eq 'POST') {
    my $content_length = $ENV{'CONTENT_LENGTH'};
    if (defined $content_length) {
        if (read(STDIN, my $post_data, $content_length)) {
            print "<html><body><h1>POST data received</h1><p>$post_data</p></body></html>";
        } else {
            print "<html><body><h1>Error</h1><p>Failed to read POST data.</p></body></html>";
        }
    } else {
        print "<html><body><h1>No POST data received</h1></body></html>";
    }
} else {
    print <<'END_HTML';
<html>
<body>
<h1>Simple Form</h1>
<form method="POST" action="">
    Name: <input type="text" name="name"><br>
    <input type="submit" value="Submit">
</form>
</body>
</html>
END_HTML
}
