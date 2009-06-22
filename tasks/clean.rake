desc "Clean compiled remnants"
task :clean do
  %w{ext/url_escape.bundle ext/Makefile ext/escape.o ext/url_escape.so}.each { |file| File.unlink(file) if File.file?(file) }
end
