require 'rbconfig'

ext = File.expand_path("../../ext", __FILE__)
exec_format = RbConfig::CONFIG['ruby_install_name'].sub('ruby', '%s') rescue '%s'

system(exec_format % 'rake', 'build')

require File.join(ext, "url_escape")

require "rubygems"
require "bacon"

Bacon.summary_at_exit
