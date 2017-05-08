Dint=25.5;   // Octogone interne
DPlexi=Dint-6;
HPlexi=3;

// space for the plexy
projection(cut=true) { 
    translate([0,0,-1]) cylinder(d=DPlexi,h=2);
}