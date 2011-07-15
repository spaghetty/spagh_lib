require 'rubygems'
require 'eventmachine'
require 'tempfile'
require 'singleton'
require 'socket'

class MultipathServ < EM::Connection
  def post_init
    puts "start here"
    @f = nil
    @counter=0
    @f_size=0
    @data_count=0
  end

  def receive_data(data)
    if @counter<2      
      tmp = data.split("\n")
      puts tmp
      tmp.each do |dat|
        if @counter==0
          puts "this is the file name #{dat}"
          @f = File.new(dat.gsub(/0/,''),"w")
        end
        if @counter==1
          puts "this is the file size #{dat.gsub(/-/,'')}"
          @f_size=dat.gsub(/-/,'').to_i
          puts "fsize #{@f_size}"
        end
        @counter=@counter+1
      end
      send_data "OK\n"
    elsif @counter>4
      @f.close() if @f!=nil
      puts "close everithing"
    else
      if @data_count+data.size <= @f_size
        @f.write(data)
        @data_count=@data_count+data.size
      else
        tmp_size= @f_size-@data_count
        @f.write(data[0..tmp_size-1])
        @counter=5
        puts "gestire gli spezzoni #{@data_count+data.size} != #{@f_size}"
      end
    end
  end

end

EventMachine::run {
  EventMachine.epoll
  EventMachine::start_server("192.168.2.108", 8000, MultipathServ)
  puts "Listening ...."
}
