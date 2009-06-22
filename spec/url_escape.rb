# * Encoding: UTF-8

require File.expand_path("../helper", __FILE__)

describe "URLEscape" do
  describe '#escape' do
    def escape(input)
      URLEscape.escape input
    end

    it "should escape a standard ascii string" do
      s = "A standard ascii string."
      escape(s).should == "A+standard+ascii+string." # This should have spaces escaped
      escape('a & b').should == "a+%26+b" # This should have spaces escaped
    end

    it 'escapes an escaped uri fragment' do
      escape("%C3%87").should == '%25C3%2587'
    end

    it 'escapes unicode' do
      escape('ルビイスと').should == '%e3%83%ab%e3%83%93%e3%82%a4%e3%82%b9%e3%81%a8'
    end
  end

  describe '#unescape' do

    def unescape(input)
      URLEscape.unescape input
    end

    it "should unescape a url" do
      s = "http://a.simple.url/foo_bar.php?hey=you%20me"
      unescape(s).should == "http://a.simple.url/foo_bar.php?hey=you me" # This should have spaces expanded
    end

    it 'should unescape unicode' do
      unescape('%E3%83%AB%E3%83%93%E3%82%A4%E3%82%B9%E3%81%A8').should == 'ルビイスと'
    end
  end
end
