# Copyright © 2009 Evan Phoenix and The Rubyists, LLC
# Distributed under the terms of the MIT license.
# See the LICENSE file which accompanies this software for the full text
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
