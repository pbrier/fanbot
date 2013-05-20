w=42;
h=24;
frames = 1000;

for f=1,frames do
  newframe = "^";
  for y=1,h do
    s = "";
    for  x = 1,w do
      if ( (x+f) % 2 == 0 ) then
	    s = s .. "*";
	  else
	   s = s .. ".";
	  end;
    end
    print(newframe .. s);
	newframe = "";
  end;
end;