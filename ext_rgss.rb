# encoding: utf-8
require_so "ExtRgss"
$bmp = Bitmap.new("Graphics/Titles1/Plain.png")
Graphics.init($bmp)

# generate stub and show TODO
[:Graphics].each do |name|
  old_name = "Old#{name}".intern
  method_list = Object.const_get(old_name).methods(false) - Object.const_get(name).methods(false)
  method_list.each do |method_name|
    eval "def #{name}.#{method_name}(*args); #{old_name}.send(:#{method_name}, *args); end"
  end
  puts "* #{name} TODO:\n  methods - #{method_list.join(', ')}"
end
