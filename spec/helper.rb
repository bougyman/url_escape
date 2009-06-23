ext = File.expand_path("../../ext", __FILE__)

Dir.chdir ext do
  puts %x{make clean}
  File.unlink("Makefile") if File.file?("Makefile")
  puts %x{ruby extconf.rb}
  puts %x{make}
end

require File.join(ext, "url_escape")

require "rubygems"
require "bacon"

Bacon.summary_at_exit
