require 'rbconfig'

task :build do
  if(RUBY_PLATFORM.match(/mswin/))
    build_mswin
  else
    build_unix
  end
end

def build_unix
  Dir.chdir 'ext' do
    sh 'make distclean' rescue nil
    ruby 'extconf.rb'
    sh 'make'
  end
end

def build_mswin
  mswin_gotchas
  Dir.chdir 'ext' do
    sh 'nmake distclean' rescue nil
    File.unlink('url_escape.so.manifest') rescue nil
    ruby 'extconf.rb'
    sh 'nmake'
    sh 'mt -manifest url_escape.so.manifest -outputresource:url_escape.so;#2'
  end
end

def mswin_gotchas
  if(ENV['VCINSTALLDIR'].nil?)
    puts "\nWARNING: Your development environment may not be correctly configured.  Run vcvars32 first to setup the environment\n\n"
  end

  config_h = File.join(RbConfig::CONFIG["archdir"], "config.h")
  if(File.exists?(config_h))
    File.open(config_h, "r") do |f|
      f.each do |l|
        if(l.match(/#error MSC version unmatch/))
          puts "\nWARNING: Ruby 1.8.6 single click installer's config.h checks for Microsoft VC 6.0.  Remove the following lines to allow compiling on later editions."
          puts "Remove #if _MSC_VER != 1200"
          puts "       #error MSC version unmatch"
          puts "       #endif"
          puts "From #{config_h}\n\n"
          break
        end
      end
    end
  end
end
