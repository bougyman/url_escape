require File.expand_path("../helper", __FILE__)
require 'benchmark'
require 'rack'
require 'cgi'

describe "Long running benchmarks" do
  def benchit(obj, meth, s = nil)
    n = 100
    s ||= "%03a" 
    s = s * 1000
    bm = Benchmark.bmbm { |b|
      b.report("#{obj.name}::#{meth}") { n.times{ obj.send(meth, s) } }
    }
  end

  it "is more efficient on larger encoded sets" do
    fs_post = %q{hostname=foo&section=directory&tag_name=domain&key_name=name&key_value=foo.example.com&Event-Name=REQUEST_PARAMS&Core-UUID=30f4a606-5896-11de-a226-fbfc74fd2199&FreeSWITCH-Hostname=foo&FreeSWITCH-IPv4=164.5.172.224&FreeSWITCH-IPv6=%3A%3A1&Event-Date-Local=2009-06-23%2019%3A02%3A09&Event-Date-GMT=Tue,%2023%20Jun%202009%2023%3A02%3A09%20GMT&Event-Date-Timestamp=1245798129959360&Event-Calling-File=sofia_reg.c&Event-Calling-Function=sofia_reg_parse_auth&Event-Calling-Line-Number=1689&action=sip_auth&sip_profile=internal&sip_user_agent=Aastra%2057iCT/2.4.0.96&sip_auth_username=2600&sip_auth_realm=164.5.172.224&sip_auth_nonce=e15fb75a-6049-11de-a226-fbfc74fd2199&sip_auth_uri=sip%3A0%40164.5.172.224%3A5060&sip_contact_user=2600&sip_contact_host=191.111.208.25&sip_to_user=0&sip_to_host=164.5.172.224&sip_to_port=5060&sip_from_user=2600&sip_from_host=164.5.172.224&sip_from_port=5060&sip_request_user=0&sip_request_host=164.5.172.224&sip_request_port=5060&sip_auth_qop=auth&sip_auth_cnonce=52b8c341&sip_auth_nc=00000001&sip_auth_response=debe14710073800831189e05242744ed&sip_auth_method=INVITE&key=id&user=2600&domain=foo.example.com&ip=191.111.208.25HTTP/1.1}

    one = benchit(URLEscape, :unescape, fs_post).first
    two = benchit(Rack::Utils, :unescape, fs_post).first
    two.real.should > one.real

    three = benchit(CGI, :unescape, fs_post).first
    three.real.should > one.real
    p one
    p two
    p three
  end
end


