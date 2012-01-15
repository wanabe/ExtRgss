# encoding: utf-8
#__END__
require_so "ExtRgss"
Graphics.init

# generate stub and show TODO
[:Graphics, :Sprite, :Window].each do |name|
  old_name = "Old#{name}".intern
  method_list = Object.const_get(old_name).methods(false) - Object.const_get(name).methods(false)
  method_list.sort!
  method_list.each do |method_name|
    eval "def #{name}.#{method_name}(*args); #{old_name}.send(:#{method_name}, *args); end"
  end
  imethod_list = Object.const_get(old_name).instance_methods(false) - Object.const_get(name).instance_methods(false)
  imethod_list.sort!
  imethod_list.each do |method_name|
    eval "class #{name}; def #{method_name}(*args); @old.send(:#{method_name}, *args); end; end"
  end
  unless imethod_list.empty?
    eval <<-EOS
    class #{name}
      alias old_initialize initialize
      def initialize(*args)
        @old = #{old_name}.send(:new, *args)
        send(:old_initialize, *args)
      end
    end
    EOS
  end
  puts "* #{name} TODO:",
       "  + methods - #{method_list.join(', ')}",
       "  + instance methods - #{imethod_list.join(', ')}"
end
