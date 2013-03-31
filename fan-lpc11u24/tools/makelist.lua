--
-- Make list of components in kiCAD drawing (.sch file)
-- use: lua makelist.lua < fanbot.sch > farnell.lst
--
list = {};

-- For all lines, search for the Farnell fields and extract the text
for l in io.lines() do
  if l:find("Farnell") ~= nil then -- farnell field
    part = l:match("\"[0-9 ]*\""); -- Extract the quoted string containing the part number ("000000")
	part = part:gsub("[\" ]", ""); -- remove quotes and spaces
    list[ part ] = (list[part] or 0 ) + 1 ;
	-- print(part, l);
  end;
end;

order = 5;

-- Print partnr and order quantity (order * parts/board)
for k,v in pairs(list) do
  print(k .. ", " .. order * v);
end;