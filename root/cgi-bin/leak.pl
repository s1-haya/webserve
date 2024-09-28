#!/usr/bin/perl

my @array;
while (1) {
    push @array, "This will consume memory.";
    sleep(0.1);  # 100ミリ秒の遅延を追加
}
