# Copyright © 2009 Evan Phoenix and The Rubyists, LLC
# Distributed under the terms of the MIT license.
# See the LICENSE file which accompanies this software for the full text
require 'rbconfig'

unless RUBY_PLATFORM.match(/java/)
  ext = File.expand_path("../../ext", __FILE__)
  ruby = File.join(RbConfig::CONFIG['bindir'], RbConfig::CONFIG['ruby_install_name'])
  %x{#{ruby} -e "begin; require 'rake'; rescue LoadError; require 'rubygems'; require 'rake'; end; Rake.application.run" build}

  require File.join(ext, "url_escape")
else
  lib = File.expand_path("../../lib", __FILE__)
  require File.join(lib, "url_escape")
end

  require "rubygems"
  require "bacon"

Bacon.summary_at_exit
