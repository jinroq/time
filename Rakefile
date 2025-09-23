require "bundler/gem_tasks"
require "rake/testtask"
require "rake/extensiontask"

extask = Rake::ExtensionTask.new("time") do |ext|
  ext.name = "time_core"
  ext.lib_dir.sub!(%r[(?=/|\z)], "/#{RUBY_VERSION}/#{ext.platform}")
end

Rake::TestTask.new(:test) do |t|
  t.libs << "test/lib"
  t.ruby_opts << "-rhelper"
  t.test_files = FileList["test/**/test_*.rb"]
end

task compile: "ext/time/zonetab.h"
file "ext/time/zonetab.h" => "ext/time/zonetab.list" do |t|
  dir, hdr = File.split(t.name)
  make_program_name =
    ENV['MAKE'] || ENV['make'] ||
    RbConfig::CONFIG['configure_args'][/with-make-prog\=\K\w+/] ||
    (/mswin/ =~ RUBY_PLATFORM ? 'nmake' : 'make')
  make_program = Shellwords.split(make_program_name)
  sh(*make_program, "-f", "prereq.mk", "top_srcdir=.."+"/.."*dir.count("/"),
     hdr, chdir: dir)
end

task :default => :test
