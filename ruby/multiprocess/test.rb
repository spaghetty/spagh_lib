require 'thread'
mutex = Mutex.new

a = Thread.new do
  sleep 2
  puts "i'm the main thread"
end

b = Thread.new do
  sleep 3
  puts "i'm the second thread"
end

c = Thread.new do
  sleep 1
  puts "i'm the third thread"
end

d = Thread.new do
  puts "i'm the forth thread"
end

e = Thread.new do
  sleep 9
  puts "i'm the fifth thread"
end

Thread.new do
  a.join
  b.join
  c.join
  d.join
  e.join
  puts "i'm the last one"
end

sleep 10
