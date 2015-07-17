#!/usr/bin/ruby -w
#
#

require 'rubygems'
require 'optparse'


def run(cmd)
  puts "Running: #{cmd}" if $options[:verbose]
  `#{cmd}`
end

def parse! args
  $options = {
    :verbose => false,
    :indir => nil,
    :outdir => nil,
  }
  OptionParser.new do |opts|
    opts.banner = "Usage: #$0 [-o <dir>|--output[=]<dir>]"

    opts.on('-v', '--verbose', 'Run verbosely.') do |v|
      $options[:verbose] = v
    end
    opts.on('-i', '--indir DIR', 'Set input directory') do |dir|
      $options[:indir] = dir
    end
    opts.on('-o', '--outdir DIR', 'Set output directory') do |dir|
      $options[:outdir] = dir
    end
  end.parse!
end

def build args
  parse! args

  dirs=run(%{for d in `cat #{$options[:indir]}/Doxyfile | grep '\<INPUT\\s\+=' | gawk -F= '{print $2}'` ; do printf "${srcdir}/$$d " ; done})
	run(%{(cat #{$options[:indir]}/Doxyfile | sed -r 's/(^INPUT\\s+=\\s+).*/\\1#{dirs}/' ; echo "INPUT = #{dirs}") | doxygen -})
end

build ARGV if $0 == __FILE__
