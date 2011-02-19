require 'rubygems'
require 'eventmachine'
require 'singleton'
require 'socket'

class ServerPool
  include EM::Deferrable
  
  def initialize(request)
    @fname = request[0]
    @sock_num = request[1].to_i
    @ip_set = request[2..-1]
    @port_set = []
    begin
      (0..@sock_num-1).each do | count |
        @port_set[count] = rand(65000)
      end
      succeed(@port_set)
    rescue Exception => e 
      fail e
    end
  end

  def startPoll(request)
    
  end
end

module MultipathServ
  def post_init
    puts "start here"
    @buff = ""
    @buff_limit = 256
  end

  def receive_data(data)
    @buff << data
    if @buff =~ /START.+END/m
      request = @buff.gsub(/\s/,'').split('|')[1..-2]
      @buff = ""
      rport, rip = Socket.unpack_sockaddr_in(get_peername())
      puts "transfer req from #{rip} on #{rport} with #{request[1]} sockets"
      send_data "ACK#{request[1]}\n"
      defer = ServerPool.new(request)
      defer.callback { | ps | send_data "START|"+ps.join("|")+"|END\n" }
    else
      if @buff[0..4] == 'START' and @buff.bytesize<@buff_limit
        puts "this is just a portion of the data"
      else
        rport, rip = Socket.unpack_sockaddr_in(get_peername())
        puts "spam form #{rip} sending size #{@buff.bytesize} closing socket"
        close_connection
      end
    end
  end

end

EventMachine::run {
  EventMachine.epoll
  EventMachine::start_server("192.168.2.108", 8000, MultipathServ)
  puts "Listening ...."
}
