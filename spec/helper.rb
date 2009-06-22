require "rubygems"
require "bacon"

Bacon.summary_at_exit


require "pathname"

Dir.chdir File.expand_path("../../ext", __FILE__)
puts %x{make clean}
File.unlink("Makefile") if File.file?("Makefile")
puts %x{ruby extconf.rb}
puts %x{make}
$LOAD_PATH.unshift(File.expand_path("."))
require "url_escape"
