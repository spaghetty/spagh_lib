require 'libxml'
require 'xml'

reader = XML::Reader.file('example.xml')
while reader.read
  if reader.node_type == XML::Reader::TYPE_COMMENT or reader.node_type == XML::Reader::TYPE_SIGNIFICANT_WHITESPACE
    next
  end
  if reader.node_type == XML::Reader::TYPE_END_ELEMENT
    puts "X -> #{reader.name}"
  else
    print "->",reader.name, " ", reader.node_type, " "
    if reader.has_attributes?
      if reader.move_to_attribute("Name")==1
        print reader.value
      end
    end
    print "\n"
  end
end
puts "DONE"
