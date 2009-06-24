require 'java'

class URLEscape
  include_class 'java.net.URLEncoder'
  include_class 'java.net.URLDecoder'

  def self.escape(str)
    URLEncoder.encode(str, "UTF-8")
  end

  def self.unescape(escaped_str)
    URLDecoder.decode(escaped_str, "UTF-8")
  end
end
