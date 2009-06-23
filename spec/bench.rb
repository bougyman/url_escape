require File.expand_path("../helper", __FILE__)
require 'benchmark'
require 'rack'
require 'cgi'


describe "URLEscape benchmark" do
  def benchit(obj, meth, s = nil)
    n = 100
    s ||= "%03a" * 10000
    bm = Benchmark.bmbm { |b|
      b.report("#{obj.name}::#{meth}") { n.times{ obj.send(meth, s) } }
    }
  end

  it "runs faster than standard CGI.unescape" do
    one = benchit(URLEscape, :unescape).first
    two = benchit(CGI, :unescape).first
    two.real.should > 25.0 * one.real
  end

  it "runs faster than Rack::Utils.unescape" do
    one = benchit(URLEscape, :unescape).first
    two = benchit(Rack::Utils, :unescape).first
    two.real.should > 25.0 * one.real
  end

  it "runs faster than standard CGI.escape" do
    one = benchit(URLEscape, :escape).first
    two = benchit(CGI, :escape).first
    two.real.should > 15.0 * one.real
  end

  it "runs faster than Rack::Utils.escape" do
    one = benchit(URLEscape, :escape).first
    two = benchit(Rack::Utils, :escape).first
    two.real.should > 15.0 * one.real
  end

end
