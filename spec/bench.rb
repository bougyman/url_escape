require File.expand_path("../helper", __FILE__)
require 'benchmark'
require 'rack'
require 'cgi'

describe "URLEscape benchmark" do
  def benchit(obj)
    bm = Benchmark.bmbm { |b|
      n = 100
      s = "%%%%" * 10000
      b.report("#{obj.name}::escape"){ n.times{ obj.escape(s) }}
    }
  end

  it "runs faster than standard CGI.escape" do
    one = benchit(URLEscape).first
    two = benchit(CGI).first
    one.real.should < 20.0 * two.real
  end

  it "runs faster than Rack::Utils.escape" do
    one = benchit(URLEscape).first
    two = benchit(Rack::Utils).first
    one.real.should < 20.0 * two.real
  end
end
