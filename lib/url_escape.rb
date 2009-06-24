# Copyright © 2009 Evan Phoenix and The Rubyists, LLC
# Distributed under the terms of the MIT license.
# See the LICENSE file which accompanies this software for the full text
require 'java'

module URLEscape
  include_class 'java.net.URLEncoder'
  include_class 'java.net.URLDecoder'

  def self.escape(str)
    URLEncoder.encode(str, "UTF-8")
  end

  def self.unescape(escaped_str)
    URLDecoder.decode(escaped_str, "UTF-8")
  end
end
