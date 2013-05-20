--
-- Scale a KiCAD module file, created with Bitmap2Component
-- use: lua scale_logo.lua logo.mod > logo_scales.mod
--
-- returns the 'basedir', 'basename' and 'extention' 
function split_path(filename) 
 return  string.match(filename, "(.-)([^\\/]-%.?([^%.\\/]*))$");
end 

s = tonumber( arg[2] or 0.5);

-- For all lines, search for the "Dl [x] [y]" line and scale the two coordinates
for l in io.lines(arg[1]) do
  if l:find("Dl") ~= nil then -- "Dl" field (line)
    x,y = l:match("Dl ([%+%-]*%d+) ([%+%-]*%d+)"); -- Extract the coordinates
    x = math.floor( x * s );
    y = math.floor( y * s);
	  print("Dl " .. x .. " " .. y);
  else 
    if l:find("LOGO") ~= nil then
      p,n,e = split_path(arg[1]);
      l = string.gsub(l, "LOGO", n);
      l = string.gsub(l, ".mod", "");
    end;
    print(l);
  end;
end;
