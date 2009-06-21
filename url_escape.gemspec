# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{url_escape}
  s.version = "2009.06.21"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Evan Phoenix", "TJ Vanderpoel", "Michael Fellinger"]
  s.date = %q{2009-06-21}
  s.email = %q{manveru@rubyists.com}
  s.extensions = ["ext/extconf.rb"]
  s.files = ["AUTHORS", "CHANGELOG", "MANIFEST", "README", "Rakefile", "ext/escape.c", "ext/extconf.rb", "tasks/authors.rake", "tasks/bacon.rake", "tasks/changelog.rake", "tasks/copyright.rake", "tasks/gem.rake", "tasks/manifest.rake", "tasks/release.rake", "tasks/reversion.rake", "url_escape.gemspec"]
  s.homepage = %q{http://github.com/bougyman/seedling}
  s.require_paths = ["ext"]
  s.rubygems_version = %q{1.3.4}
  s.summary = %q{Fast url_escape library written in C}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
