
class HostDescriptor
  attr_accessor :ip, :uname, :path, :type
  def initialize(type, ip, uname)
    @type = type.strip()
    @ip = ip.strip()
    @uname = uname.strip()
    has_key = false
  end
  
  def has_key?
    return @has_key
  end

  def need_sudo?
    return (not @uname=="root")
  end
  
  def has_key=(val)
    @has_key = val
  end

  def get_ssh_string()
    res = "#{@uname}@#{@ip}"
    if @has_key
      res = "-i #{GIT_REPO}/#{ip}/.ssh_key #{@uname}@#{@ip}" 
    end
    return res
  end

  def get_key_file()
    return "#{GIT_REPO}/#{ip}/.ssh_key"
  end

  def self.find_by_ip(ip)
    found = nil
    if File.exists? "#{GIT_REPO}/#{ip}/.ecatonchiro"
      f = File.open("#{GIT_REPO}/#{ip}/.ecatonchiro", "r")
      desc = f.gets()
      el = desc.split(":")
      found = HostDescriptor.new(el[1], el[0], el[2])
      if File.exists? "#{GIT_REPO}/#{ip}/.ssh_key"
        found.has_key = true
      end
    end
    return found
  end
end
