#!/usr/bin/ruby -w

require 'rubygems'
require 'optparse'

def run cmd
  puts "Running: #{cmd}" if $options[:verbose]
  `#{cmd}`
end

def parse! args
  $options = {
    :verbosity => 0,    # Kind of a "log level"
    :debug => false,    # Whether to run in gdb for debugging.
    :target => nil,     # Default is "all"
    :builddir => './',  # Root directory where *.o are.
  }

  OptionParser.new do |opts|
    opts.banner = "Usage: #$0 [options]"

    opts.on('-v', '--verbose', 'Run verbosely') do |v|
      $options[:verbosity] += 1
    end
    opts.on('-d', '--debug TARGET', 'Run in debug mode (via gdb)') do |target|
      $options[:debug] = target
    end
    opts.on('-b', '--builddir DIR', 'Set build dir.') do |builddir|
      $options[:builddir] = builddir
    end
  end.parse!
end

def test args
  parse! args

  if $options[:debug] then
    run(%{CK_FORK=no gdb #{$options[:target]} -ex run})
  elsif $options[:target]
    run(File.join($options[:builddir], $options[:target]))
  else
    STDERR.puts 'No command line args given!'
  end
end

test ARGV if $0 == __FILE__
