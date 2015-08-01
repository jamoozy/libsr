#!/usr/bin/ruby -w
#
#

require 'rubygems'
require 'optparse'


def run(cmd)
  puts %{Running: "#{cmd}"} if $options[:verbose]
  `#{cmd}`
end

def parse! args
  # Default values.
  $options = {
    :verbose => false,
    :indir => nil,
    :outdir => nil,
    :subst => '',
  }
  OptionParser.new do |opts|
    opts.banner = "Usage: #$0 [options]"

    opts.on('-v', '--verbose', 'Run verbosely.') do |v|
      $options[:verbose] = v
    end
    opts.on('-i', '--indir DIR', 'Set input directory') do |dir|
      $options[:indir] = dir
    end
    opts.on('-o', '--outdir DIR', 'Set output directory') do |dir|
      $options[:outdir] = dir
    end
    opts.on('-s', '--subst S', 'List of comma-separated "k=v" pairs') do |s|
      $options[:subst] = s.split(',').map{|e| e.split('=')}
    end
  end.parse!
end

def build args
  parse! args

  # Find the directory with a "Doxyfile" in it.
  base_dir = File.dirname(__FILE__)
  while !File.file?(File.join(base_dir, 'Doxyfile'))
    base_dir = File.dirname(base_dir)
  end

  # Get all the variables from the Doxyfile as a Hash.
  vals = Hash[
    IO.readlines(File.join(base_dir, 'Doxyfile').reject do |line|
      line =~ /^\s*#/ or line =~ /^\s*$/
    end.map do |line|
      line.split('=').map{|e| e.strip}
    end.flatten
  ]

  # Update

  Dir.chdir(base_dir) do
    cmd = 'doxygen'
    cmd = "#{} #{cmd}"
    run(cmd)
  end

  dirs = run(%{for d in `cat #{$options[:indir]}/Doxyfile | grep '\<INPUT\\s\+=' | gawk -F= '{print $2}'` ; do printf "${srcdir}/$$d " ; done})
	run(%{(cat #{$options[:indir]}/Doxyfile | sed -r 's/(^INPUT\\s+=\\s+).*/\\1#{dirs}/' ; echo "INPUT = #{dirs}") | doxygen -})
end

build ARGV if $0 == __FILE__
