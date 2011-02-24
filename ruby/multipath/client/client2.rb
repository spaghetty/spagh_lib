require 'rubygems'
require 'socket'
require 'eventmachine'

$ip_stuff = [
             "192.168.122.167",
             "10.168.100.163"
           ]

$base_key = nil

$file_list = []

class Multipath < EM::Connection
  def post_init
    puts "done"
    @ip = Socket.unpack_sockaddr_in(get_sockname)[1]
    send_name
  end
  
  def send_name()
    puts "s_name"
    if not $file_list.empty?
      @fname = $file_list.pop
      puts @fname
      send_data "#{@fname.split("/")[-1].ljust(29,'0')}\n"
    end
  end

  def send_file()
      streamer = EventMachine::FileStreamer.new(self,@fname)
      streamer.callback{
        send_fine
      }
  end
  
  def send_fine()
    close_connection(after_writing=true)
    puts "Time: #{$start_time - Time.now.to_i}"
    if not $file_list.empty?
      EventMachine::bind_connect(@ip,
                                 0,
                                 '192.168.2.108', 
                                 8000, 
                                 Multipath)
    end
  end

  def receive_data(data)
    if data =~ /OK/
      send_file
    end
  end
end

Dir.foreach("data_source") do | elem |
  if elem =~ /pending_.*/
    $file_list << "data_source/"+elem
  end
end

def do_connection(ip)
  EventMachine::bind_connect(ip,
                             0,
                             '192.168.2.108', 
                             8000, 
                             Multipath)
end

EventMachine::run do
  $start_time = Time.now.to_i
  $ip_stuff.each do | ip |
    do_connection(ip)
  end
end
