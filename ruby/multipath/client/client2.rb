require 'rubygems'
require 'socket'
require 'eventmachine'
require 'fileutils'


$ip_stuff = [
             "192.168.122.167",
             "10.168.100.163"
           ]

$base_key = nil
$dir_name = "data_soruce"
$file_list = []
$split_size = "50MB"

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
      size = File.size @fname
      puts @fname
      send_data "#{@fname.split("/")[-1].ljust(39,'0')}\n"
      send_data "#{size.to_s.rjust(39,'-')}\n"
    end
  end

  def send_file()
      streamer = EventMachine::FileStreamer.new(self,@fname)
      streamer.callback{
        send_fine
      }
  end
  
  def send_fine()
    puts "Time: #{$start_time - Time.now.to_i}"
    send_data '\n'
    if not $file_list.empty?
      close_connection(after_writing=true)
      EventMachine::bind_connect(@ip,
                                 0,
                                 '192.168.2.108', 
                                 8000, 
                                 Multipath)
    else
      3.times { send_data "#{"".ljust(39,'0')}\n" }
      close_connection(after_writing=true)
    end
  end

  def receive_data(data)
    if data =~ /OK/
      send_file
    end
  end
end

def do_connection(ip)
  EventMachine::bind_connect(ip,
                             0,
                             '192.168.2.108', 
                             8000, 
                             Multipath)
end



if __FILE__ == $0
  if ARGV.size!=1
    puts "FILE NAME MISSED"
    exit(0)
  end
  if not File.exists? ARGV[0]
    puts "SPECIFIED FILE DO NOT EXISTS"
    exit(0)
  end

  if File.directory? $dir_name
    %x[rm -rf #{$dir_name}/*]
  else
    %x[mkdir #{$dir_name}]
  end

  %x[split -a 4 -b #{$split_size} #{ARGV[0]} #{$dir_name}/pending_#{ARGV[0]}__]
  

  Dir.foreach("#{$dir_name}") do | elem |
    if elem =~ /pending_.*/
      $file_list << "#{$dir_name}/"+elem
    end
  end
  
  puts "chit: #{$file_list}"

  EventMachine::run do
    $start_time = Time.now.to_i
    $ip_stuff.each do | ip |
      do_connection(ip)
    end
  end
end
