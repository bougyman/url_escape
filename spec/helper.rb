require 'rbconfig'

unless RUBY_PLATFORM.match(/java/)
  ext = File.expand_path("../../ext", __FILE__)
  exec_format = RbConfig::CONFIG['ruby_install_name'].sub(/^ruby.*$/, '%s') rescue '%s'

  system(exec_format % 'rake', 'build')

  require File.join(ext, "url_escape")
else
  lib = File.expand_path("../../lib", __FILE__)
  require File.join(lib, "url_escape")
end

  require "rubygems"
  require "bacon"

Bacon.summary_at_exit
