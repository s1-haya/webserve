#!/usr/bin/perl

sub infinite_recursion {
    infinite_recursion();
}

infinite_recursion();
