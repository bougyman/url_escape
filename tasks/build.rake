task :build do
  if(RUBY_PLATFORM.match(/mswin/))
    build_windows
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

def build_windows
  Dir.chdir 'ext' do
    sh 'nmake distclean' rescue nil
    File.unlink('url_escape.so.manifest') rescue nil
    ruby 'extconf.rb'
    sh 'nmake'
    sh 'mt -manifest url_escape.so.manifest -outputresource:url_escape.so;#2'
  end
end
