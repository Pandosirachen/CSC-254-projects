require 'enumerator' 
directory_name = "HTML"
Dir.mkdir(directory_name) unless File.exists?(directory_name)
data={}
f = false
testprogram = ARGV[0]
table = {}
if testprogram == nil then
    puts("No program")
    exit
end
objdump = "objdump -d '#{testprogram}'"
dwarfdump = "llvm-dwarfdump --debug-line '#{testprogram}'"
obj = `#{objdump}`
dwarf = `#{dwarfdump}`
html = "<!DOCTYPE html> 
    <html>
    <head>
        <style>
            html {background-color:#B7A9A1; }
	    #over {
		background-color:#B7A9A1;
		padding-top:10px;
            }
            h1 { 
                    text-align: center;
                    color: #403B3B;
                    background-color:#DBD9D2;
                    font-size: 42px; 
                }
            h2 {text-align: center;color: #8C8C8C; font-size: 18px;}
            div
            {
                width : 80%;
                max-width : 960px;
                display: grid;
                margin-left: auto;
                margin-right: auto;
            }
            th{
                padding-left: 20px;
                padding-right: 20px;
                background-color: #DBD9D2;
                color: #403B3B;
                font-size: 23px;
            }
            td {
    padding-left: 20px;
                padding-right: 20px;
                background-color: #DBD9D2;
                color: #403B3B;
    font-size: 14px;
    text-align: left;
            }
    
        </style>    
  </head>
        <h1>CSC 254 Project 4<h1>
        <h2>Sichen Pan</h2>
  <body>
            <div>
    <table>
          <tr>
            <th>Address & Assembly</th>
            <th>Source Code</th>
          </tr>"
fn = []
tail = "</div>
        </body>
    </html>"
tmp=1

dwarf.each_line do |line|
    if tmp == 1
      if line.match(/file_names/)
      elsif line.match(/dir_index:/)
      elsif line.match(/mod_time:/)
      elsif line.match(/length:/)     
      elsif line.match(/name:/)
        file_name = line.scan(/"[^"]*"/).to_s.gsub('"','').gsub("\\",'')
        fn.push(file_name)
      elsif line.match(/Address/)
        tmp = 0
      end 
    end
    if tmp==0 
      if line.match("0x")
        info = line.split(" ")
        address = info[0]
        line = info[1]
        column = info[2]
        file = info[3]
        isa = info[4]
        discriminator = info[5]
        flags = info[6]
        file = fn[file.to_i-1].gsub("[", '').gsub("]", '')
        if table.has_key?(file)
          value = table[file]
          value[address] = line
          table[file] = value
          n = {}
          value.each do |f, source|          
            code = IO.readlines(file)[source.to_i-1]
            l = value.select{|a, b| b == source}.keys
            if l.length > 1
              n[l] = code
            else
              n[f] = code
            end
          end
          data[file] = n
        else
          table[file] = {address => line}
        end
      end 
    end 
end
obj.each_line do |line|
  over=true
  if line.length<=1
	html = html +"<tr>
                            <td id='over'></td>
                            <td id='over'></td>
                        </tr>"
    over=false
  end
  tmp = line.strip
  line =line.gsub("<", "&lt").gsub(">", "&gt")
  infoarray= line.gsub(/\s/m, ' ').strip.split(" ")
  addr = infoarray[0]
  if !addr.nil? == true && addr.match(/:/) && over==true
    addr = addr.gsub(/:/, '')
  data.each do |key, value|
    value.keys.each do |addr1|
      if addr1.kind_of?(Array)
        if addr1[0].match(/#{addr}/)
          html = html +"<tr>
                            <td>#{tmp}</td>
                            <td id='#{addr}'>#{value[addr1]}</td>
                        </tr>"
          f = true
        end
      else
        if addr1.match(/#{addr}/)
          html = html +"<tr>
                              <td>#{tmp}</td>
                              <td id='#{addr}'>#{value[addr1]}</td>
                        </tr>"
          f = true
        end
      end
    end
  end
  end
 if f == false && over == true
	flag=0
   	   if line.match(/callq/)
		 flag=2
   		 positions = line.enum_for(:scan, /callq/).map { Regexp.last_match.begin(0) }  
		 index = positions.to_s.gsub("[", '').gsub("]", '')
		 pre = line[0,index.to_i+6]
		 func = line[index.to_i+7,6]
		 after=" "
		 after = line[index.to_i+13,line.length-index.to_i+13]
		 if func.match? /\A\d+\z/
			flag=1
			m= Integer(func) rescue nil
		 end
	   end
   	if flag == 2
   		html = html +"<tr><td  id='#{addr}'>" + pre + func + after+ "</td><td id='over'></td>
</tr>"
	elsif flag == 1
      		html = html +"<tr><td  id='#{addr}'>" + pre + "<a href='#"+m.to_s+"'>" + m.to_s + "</a>" + after+ "</td><td id='over'></td>
</tr>"
	else html = html +"<tr><td  id='#{addr}'>" + line + "</td><td id='over'></td>
</tr>"
	end
 end
 f = false
end
htmlFile = File.new("HTML/index.html", "w+")
html = html + tail
htmlFile.puts(html)
puts "Make HTML success"

