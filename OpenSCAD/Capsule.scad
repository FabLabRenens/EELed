Dext=27;   //Octogone externe
Dint=25.5;   // Octogone interne
DledInt=13.5;
DledExt=16;
Dsp=22; 
Dal=20;  //alésage base
Hal=3;
Dfil=3;
Hled=40;
Hdome=8;
$fn=48;
PosVertAnneauBlocageLED=19;
HAnneauBlocageLED=1.5;
DIntAnneauBlocageLED=DledInt-0.8;
EpaissDome=1.6;
DExtra=1;


difference() {
    union() {
        #translate([0,0,0])cylinder(d=Dext,h=2,$fn=8); // octogone inf
        #translate([0,0,2])cylinder(d=Dint,h=4,$fn=8); // octone sup
         
        translate([0,0,5])scale([1,1,1.2])sphere(d=Dsp); // dome
        translate([0,0,2])cylinder(d=DledExt,h=Hled-4); // tour LED
    }
       translate([0,0,5])scale([1,1,1.2])sphere(d=Dsp-EpaissDome); // dome   
       translate([0,0,-Hled])cylinder(d=DledInt,h=Hled*2); // cavité LED
       translate([0,0,-1])cylinder(d=Dal,h=Hal+1);
       translate([0,0,-100])cylinder(d=Dext,h=100);
   
}
difference() {
    translate([0,0,PosVertAnneauBlocageLED])cylinder(d=DledInt,h=HAnneauBlocageLED);
    union(){
        translate([0,0,PosVertAnneauBlocageLED])cylinder(d1=DledInt,d2=DIntAnneauBlocageLED,h=HAnneauBlocageLED/3);
        translate([0,0,PosVertAnneauBlocageLED+HAnneauBlocageLED/3])cylinder(d=DIntAnneauBlocageLED,h=HAnneauBlocageLED/3);
        translate([0,0,PosVertAnneauBlocageLED+HAnneauBlocageLED/3*2])cylinder(d1=DIntAnneauBlocageLED,d2=DledInt,h=HAnneauBlocageLED/3);
    }
}


//base

translate([0,0,0])cylinder(d=Dint-1,h=2);
