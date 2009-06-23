desc "Clean compiled remnants"
task :clean do
  make = RUBY_PLATFORM.match(/mswin/) ? "nmake" : "make"
  Dir.chdir("ext") do
    sh "#{make} distclean" rescue nil
    %w{url_escape.bundle url_escape.so.manifest Makefile escape.o rl_escape.so}.each { |file| File.unlink(file) if File.file?(file) }
  end
end
