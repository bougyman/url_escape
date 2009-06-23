require "rubygems"
require "bacon"

Bacon.summary_at_exit


Dir.chdir ext = File.expand_path("../../ext", __FILE__)
puts %x{make clean}
File.unlink("Makefile") if File.file?("Makefile")
puts %x{ruby extconf.rb}
puts %x{make}
require File.join(ext, "url_escape.so")
