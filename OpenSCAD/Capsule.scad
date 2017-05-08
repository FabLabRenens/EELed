Dext=27;   //Octogone externe
Dint=25;   // Octogone interne
Dled=5.8;  
Dsp=22; 
Dal=14.5;  //alésage base
Hal=3.3;
Dfil=3;
Hled=10;
Hdome=8;
$fn=48;

difference() {
    union() {
        translate([0,0,0])cylinder(d=Dext,h=2,$fn=8);
        translate([0,0,2])cylinder(d=Dint,h=3,$fn=8);
        translate([0,0,5])scale([1,1,0.6])sphere(d=Dsp);
    }
       translate([0,0,-1])cylinder(d=Dled,h=Hled);
       translate([6,0,-1])rotate([0,32,0])cylinder(d=Dfil,h=Hled*3);
       translate([0,0,-2.5])cylinder(d=Dal,h=Hal);
}
