#!/usr/bin/perl -W

use File::Find;
use strict;

my @ directory_list = ('../src/web/');
my $files_processed = 0;

my $cprogramheader = <<END
#include "resources.h"

END
;

my $addroutes = "";
my $functions = "";

sub get_file_content {
   my ($filename) = @_;

   my $ret = "";
   open(IN, $filename) or die "Can't open $filename";
   while(<IN>) {
      chomp;
      my $line = $_;
      $line =~ s/\\/\\\\/g;
      $line =~ s/\"/\\\"/g;
      $line =~ s/\%/\%\%/g;
      $ret .= '"' . $line . '"' . "\n";
   }
   close(IN);
   return $ret;
}

#-----------------------------------------------------------------------
#Process a single file in a directory
#-----------------------------------------------------------------------
sub process_file {
    my $filename = $_;
    my $filename_full = $File::Find::name;
    my $filename_web = $File::Find::name;
    my $directory = $File::Find::dir;

    if($filename_full =~ /\.(.{2,4})$/) {
       my $type = $1;
       print "$filename - type: $type\n";
       if($type eq "css" || 
          $type eq "js" || 
          $type eq "html") {
          $filename_web =~ s/..\/src\/web//;
          my $function_name = $filename_web;
          $function_name =~ s/[\/\.\-]/\_/g;
          $addroutes .= "   add_route(\"$filename_web\",$function_name);\n";  

          ###  put file content in one string
          my $content = get_file_content($filename);

          my $one_function = <<END 
int $function_name(struct mg_connection *conn)
{
    mg_send_header(conn, "Content-Type", "text/$type");
    mg_send_header(conn, "Cache-Control", "public, max-age=604800");
    mg_printf_data(conn, $content);
    return MG_TRUE;
}

END
;
          $functions .= $one_function;
          $files_processed++;
       }
    }
    return;
}

find(\&process_file, @directory_list);
print "\nfiles processed : $files_processed\n";

open(OUT, ">../src/resources.c");
print OUT $cprogramheader;
print OUT $functions;
print OUT "\nvoid init_resources_table() {\n";
print OUT $addroutes;
print OUT "}\n\n";
close OUT;

