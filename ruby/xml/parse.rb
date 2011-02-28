require 'libxml'
require 'xml'

class XMLNode
  
  attr_reader :statistics, :connections
  
  def self.instance(xmlNode)
    type = xmlNode.attributes.get_attribute("Type").value.capitalize
    return Kernel.const_get(type).new(xmlNode)
  end

  def initialize(xmlNode)
    @node = xmlNode
    @attribs = @node.attributes
    @type = @attribs.get_attribute("Type").value
    @id = @attribs.get_attribute("ID").value
    @name = @attribs.get_attribute("Name").value
    @description = @attribs.get_attribute("Description").value
    set_statistics
    set_connections
  end

  def set_statistics
    @statistics = {}
    @node.find_first("Statistics").children.each do | elem |
      attribs = elem.attributes
      begin
        @statistics[attribs.get_attribute("ID").value] = attribs.get_attribute("Flag").value
      rescue
      end
    end
  end

  def set_connections
    @connections = {}
    @node.find_first("Connections").children.each do | elem |
      attribs = elem.attributes
      begin
        @connections[attribs.get_attribute("ID").value] = attribs.get_attribute("NextBlock").value
      rescue
      end
    end
  end
end


class Begin < XMLNode
end

#reader = XML::Reader.file('example.xml')
#while reader.read
#  if reader.node_type == XML::Reader::TYPE_COMMENT or reader.node_type == XML::Reader::TYPE_SIGNIFICANT_WHITESPACE
#    next
#  end
#  if reader.node_type == XML::Reader::TYPE_END_ELEMENT
#    puts "X -> #{reader.name}"
#  else
#    print "->",reader.name, " ", reader.node_type, " "
#    if reader.has_attributes?
#      if reader.move_to_attribute("Name")==1
#        print reader.value
#      end
#    end
#    print "\n"
#  end
#end

xmlParser = XML::Parser.file('example.xml')
xmlDocument = xmlParser.parse()
t = xmlDocument.find_first("//Block[@Type='BEGIN']")

t.find_first("Connections").children.each do |elem|
  attribs = elem.attributes
  begin
    p attribs.get_attribute("ID").value
    p attribs.get_attribute("NextBlock").value
  rescue
  end
end

puts "DONE"

z =  XMLNode.instance(t)
p z
