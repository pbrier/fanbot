
module connector()
{
difference()
{
   translate([0,0.5,0])  cube([6.5,6.5,16], center=true);
   //cylinder(r=3,h=20, center=true, $fn=30);
    # union() 
    {
      translate([0,0,0]) cylinder(r=1.5,h=20, center=true, $fn=30, center=true);
      translate([0,3,-4]) rotate([90,0,0]) cylinder(r=2.4,h=2.1, $fn=30, center=true); // hole for servo
      translate([0,0,-4]) rotate([90,0,0]) cylinder(r=1,h=12.1, $fn=30, center=true);
      translate([0,-1.6,-4]) rotate([90,0,0]) cylinder(r=2.2,h=5.1, $fn=30, center=true);
    }
  }
}

// translate([0,2,-5]) rotate([90,0,0]) cylinder(r=1,h=1, $fn=30, center=true);

single = 1;

if ( single == 0 )
for ( x = [0 : 3] )
  for ( y = [0 : 3] )
    for ( z = [0 : 0] )
    translate([7*x, 7*y, 20.1*z])  connector();

if ( single == 1 )
 connector();