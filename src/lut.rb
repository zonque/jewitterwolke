#!/bin/ruby

length = 256

length.times do |i|
  v = Math::exp(i.to_f / length.to_f)
  # v *= length

  print "0x%02x, " % v.to_i
  print "\n" if i % 8 == 7
end