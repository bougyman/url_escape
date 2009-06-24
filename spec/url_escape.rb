# * Encoding: UTF-8

require File.expand_path("../helper", __FILE__)
require "rack"
require "cgi"

describe "URLEscape" do
  describe '#escape' do
    def escape(input)
      URLEscape.escape input
    end

    it "should escape a standard ascii string" do
      s = "A standard ascii string."
      escape(s).should == Rack::Utils.escape(s) # This should have spaces escaped
      escape(s).should == CGI.escape(s) # This should have spaces escaped
      escape(s).should == 'A+standard+ascii+string.' # This should have spaces escaped
      s1 = 'a & b'
      escape(s1).should == Rack::Utils.escape(s1) # This should have spaces escaped and the amp encoded
      escape(s1).should == CGI.escape(s1) # This should have spaces escaped and the amp encoded
      escape(s1).should == 'a+%26+b' # This should have spaces escaped and the amp encoded
    end

    it "should escape a very hairy ascii string" do
      s = %q{oi&%$#@!;laksdf\"';:<>?\\}
      escape(s).should == "oi%26%25%24%23%40%21%3Blaksdf%5C%22%27%3B%3A%3C%3E%3F%5C" # This should have lots of escapes
      escape(s).should == CGI.escape(s)
      escape(s).should == Rack::Utils.escape(s)
    end

    it 'escapes an escaped uri fragment' do
      escape(s = "%C3%87").should == '%25C3%2587'
      escape(s).should == CGI.escape(s)
      escape(s).should == Rack::Utils.escape(s)
    end

    it 'escapes unicode' do
      escape(s = 'ルビイスと').should == '%E3%83%AB%E3%83%93%E3%82%A4%E3%82%B9%E3%81%A8'
      escape(s).should == CGI.escape(s)
      escape(s).should == Rack::Utils.escape(s) unless /^1\.9/ === RUBY_VERSION
    end

    it 'escapes mixed ascii and unicode' do
      escape(s = "oidfu㈤").should == "oidfu%E3%88%A4"
      escape(s).should == CGI.escape(s)
      escape(s).should == Rack::Utils.escape(s) unless /^1\.9/ === RUBY_VERSION
    end

    it 'handles high bit ascii strings' do
      s = "\xA1oidfu\xB5"
      if /java/i === RUBY_PLATFORM
        escape(s).should == "%EF%BF%BDoidfu%EF%BF%BD"
      else
        escape(s).should == CGI.escape(s) unless /^1\.9/ === RUBY_VERSION
        escape(s).should == Rack::Utils.escape(s) unless /^1\.9/ === RUBY_VERSION
        escape(s).should == "%A1oidfu%B5"
      end
    end

    it 'treats strings ending in a unicode starting byte as high bit ascii' do
      s = "oidfu\xD5"
      s1 = "\xE1\xB1"
      if /java/i === RUBY_PLATFORM
        escape(s).should == "oidfu%EF%BF%BD"
        escape(s1).should == "%EF%BF%BD" 
      else
        escape(s).should == CGI.escape(s) unless /^1\.9/ === RUBY_VERSION
        escape(s).should == Rack::Utils.escape(s) unless /^1\.9/ === RUBY_VERSION
        escape(s).should == "oidfu%D5"

        escape(s1).should == CGI.escape(s1) unless /^1\.9/ === RUBY_VERSION
        escape(s1).should == Rack::Utils.escape(s1) unless /^1\.9/ === RUBY_VERSION
        escape(s1).should == "%E1%B1" 
      end
    end
  end

  describe '#unescape' do
    def unescape(input)
      URLEscape.unescape input
    end

    it "should unescape a url" do
      s = "http://a.simple.url/foo_bar.php?hey=you%20me"
      unescape(s).should == "http://a.simple.url/foo_bar.php?hey=you me" # This should have spaces expanded
      unescape(s).should == CGI.unescape(s)
      unescape(s).should == Rack::Utils.unescape(s)
    end

    it 'should unescape unicode' do
      s = 'ルビイスと'
      if s.respond_to?(:force_encoding)
        unescape('%E3%83%AB%E3%83%93%E3%82%A4%E3%82%B9%E3%81%A8').
          should == s.force_encoding("ASCII-8BIT")
        unescape(s).should == CGI.unescape(s)
        unescape(s).should == Rack::Utils.unescape(s)
      else
        unescape('%E3%83%AB%E3%83%93%E3%82%A4%E3%82%B9%E3%81%A8').
          should == s
        unescape(s).should == CGI.unescape(s)
        unescape(s).should == Rack::Utils.unescape(s)
      end
    end
  end
end
