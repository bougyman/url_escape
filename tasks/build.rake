task :build do
  Dir.chdir 'ext' do
    sh 'make clean' rescue nil
    rm_f 'Makefile'
    ruby 'extconf.rb'
    sh 'make'
  end
end


