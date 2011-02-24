require 'rubygems'
require 'eventmachine'
require 'tempfile'
require 'singleton'
require 'socket'

class MultipathServ < EM::Connection
  def post_init
    puts "start here"
    @f = nil
  end

  def receive_data(data)
    if data.size==30
      puts data
      tmp = data
      @f.close() if @f!=nil
      puts tmp
      @f = File.new(tmp.gsub(/0/,'')[0..-2],"w")
      send_data "OK\n"
    else
      @f.write(data)
    end
  end

end

EventMachine::run {
  EventMachine.epoll
  EventMachine::start_server("192.168.2.108", 8000, MultipathServ)
  puts "Listening ...."
}
