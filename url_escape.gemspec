# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{url_escape}
  s.version = "1.8.6"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Evan Phoenix", "TJ Vanderpoel", "Michael Fellinger", "Trey Dempsey"]
  s.date = %q{2009-06-23}
  s.description = %q{Fast replacement for CGI.escape and Rack::Utils.escape}
  s.email = %q{manveru@rubyists.com}
  s.extensions = ["ext/extconf.rb"]
  s.files = ["AUTHORS", "CHANGELOG", "MANIFEST", "README", "Rakefile", "ext/escape.c", "ext/extconf.rb", "spec/bench.rb", "spec/helper.rb", "spec/url_escape.rb", "tasks/authors.rake", "tasks/bacon.rake", "tasks/build.rake", "tasks/changelog.rake", "tasks/clean.rake", "tasks/copyright.rake", "tasks/gem.rake", "tasks/gem_installer.rake", "tasks/manifest.rake", "tasks/release.rake", "tasks/reversion.rake", "tasks/setup.rake"]
  s.homepage = %q{http://github.com/bougyman/seedling}
  s.require_paths = ["ext"]
  s.rubygems_version = %q{1.3.4}
  s.summary = %q{Fast url_escape library written in C}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<rack>, [">= 0"])
    else
      s.add_dependency(%q<rack>, [">= 0"])
    end
  else
    s.add_dependency(%q<rack>, [">= 0"])
  end
end
