As performance boosts are about speed, we'll start with the
benchmarks.  Here is a run of spec/bench.rb, from the url\_escape
source tree.
  
<strong>Escape</strong>
------
<table>
<tr>
  <th>-</th>
  <th>user</th>
  <th>system</th>
  <th>total</th>
  <th>real</th>
</tr>
<tr>
  <td>URLEscape::escape</td>
  <td>0.200000</td>
  <td>0.000000</td>
  <td>0.200000</td>
  <td>(  0.196100)</td>
</tr>
<tr>
  <td>CGI::escape</td>         
  <td>3.830000 </td>
  <td>0.010000</td>
  <td>3.840000</td>
  <td>(  3.828438)</td>
</tr>
<tr>
  <td>Rack::Utils::escape</td>
   <td>3.880000</td>
   <td>0.010000</td>   
   <td>3.890000</td>
   <td>(  3.880745)</td>
</tr>
</table>
<br />
<strong>Unescape</strong>
--------
<table>
  <tr>
    <th>-</th>
    <th>user</th>
    <th>system</th>
    <th>total</th>
    <th>real</th>
  </tr>
  <tr>
    <td>URLEscape::unescape</td>
   <td>0.090000</td>   
   <td>0.000000</td>
   <td>0.090000</td> 
   <td>(  0.089190)</td>
  </tr>
  <tr>
    <td>CGI::unescape</td>
    <td>2.820000</td>   
    <td>0.000000</td>  
    <td>2.820000</td> 
     <td>(  2.816234)</td>
  </tr>
  <tr>
    <td>Rack::Utils::unescape</td>
    <td>3.140000</td>
    <td>0.000000</td>  
    <td> 3.140000</td> 
    <td>(  3.137291)</td>
  </tr>
</table>
<br />
<strong>URLEscape</strong> provides these two methods as a C extension, suitable for
use on ruby 1.8.6-8 and 1.9.1+; tested on linux, XP, and Vista.
<br />
The jruby version uses the java stdlib's java.net.URLEncoder and
URLDecoder.  We only see a 200-700% increase with this change, and would like to improve
on those numbers.

### Why?

We came across a bottleneck when regression testing FXC (a web app
which serves configuration information to the FreeSWITCH softswitch)
where requests were being delayed in our rack middleware, which parses
the POST data sent by FreeSWITCH and routes requests to the ramaze
application for processing.  The delay was noticeable under loads of
only 50 req/second; where rack became the bottleneck, not ramaze, the
db, or any other factor.  Adding the above library (on linux, with
ruby 1.9.1) removed the delay in rack, pushing the work back to
to the web app (or database) where it's free to be as slow as it must. 
Optimally we'd like to perform at a speed equal to the database, making it
the final bottleneck in a dynamic application.

### Installation and Usage

#### To use URLEscape standalone
Install with one of the following methods: 
 * gem install url\_escape
 * get the tarball from [RubyForge](http://url-escape.rubyforge.org)
 * get the source from [GitHub](http://github.com/bougyman/url\_escape) and rake install in the source top-level.

Then simply require "url\_escape" and you have access to URLEscape.escape(string) and URLEscape.unescape(string)

#### To use URLEscape's escape/unescape in place of CGI or Rack::Utils versions

> gem install rack\_fast\_escape

or

> gem install cgi\_fast\_escape

This will install url\_escape if it's not already installed, as well.

You can optionally install [rack\_fast\_escape](http://github.com/bougyman/rack_fast_escape) or [cgi\_fast\_escape](http://github.com/bougyman/cgi\_fast\_escape) from rubyforge's tarball or the github source (rake install as with url_escape).  If you use tarball or source rake install, *you will have to manually install url\_escape first.*

Once installed, simply use

> require "rack_fast_escape"

to replace Rack::Utils version, or 

> require "cgi_fast_escape"

to replace the CGI version.  

### What else?

The ability of large posts to slow down a web application cannot be
removed by just speeding up the POST parser.  In order to alleviate the
risk of such large POSTs being used to deny a service, firewall or
web server throttling or limiting is a more reliable protection to
enable.  Here are a few examples:

 * Lighttpd: http://lighttpd.net
   * Offers mod_evasive which limits connections per ip, as well as
     the ability to limit the data rate per connection.

 * Nginx: http://nginx.org
   * Flexible limiting system, per vhost, per user, per connection.

 * Netfilter/QoS (linux): http://l7-filter.sourceforge.net/
   * Allow classifiation of HTTP packets so iptables/tc or whatever
     utility you'd like can have the info it needs about the HTTP
     protocol to make limiting/dropping/queueing decisions

* Others: Apache, Squid, Litespeed, many others will have various methods
   of limiting size and frequency of requests.

### Side Note
When speccing these libraries, a few implementation differences came to light which we'll highlight here.

 1. Rack::Utils and CGI both throw errors on a mixed ASCII and Unicode string in ruby 1.9.1 and above
 1. Java's URLDecoder and URLEncoder do not escape or unescape mixed ASCII/Unicode properly.

URLEscape (the C version) handles these cases properly, though we don't expect you'd see them much
in proper requests.

### Thanks
To Evan Phoenix, Trey Dempsey, Jayson Vaughn, Michael Fellinger, Kevin Berry, and all the other contributors
of ideas and support who made this product a reality.

### License
Nothing to fear, it's MIT
