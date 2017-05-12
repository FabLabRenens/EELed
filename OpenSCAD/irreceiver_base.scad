$fn = 48;

DledExt=16;

IRReceiverRingInnerDiameter = DledExt+0.5;
IRReceiverRingOuterDiameter = IRReceiverRingInnerDiameter + 4;
IRReceiverRingHeight = 5;
IRReceiverModuleSideLength = 20;
IRReceiverBaseSideLength = IRReceiverModuleSideLength + 3;
IRReceiverBaseHeight = 2;
IRReceiverBaseFixationDiameter = 4.5;

difference() {
    union() {
        translate([0,0,0])cylinder(d=IRReceiverRingOuterDiameter,h=IRReceiverRingHeight); 
        translate([(IRReceiverRingOuterDiameter/2+IRReceiverModuleSideLength)/2,0,IRReceiverBaseHeight/2]) cube(size=[IRReceiverRingOuterDiameter/2+IRReceiverModuleSideLength,IRReceiverModuleSideLength, IRReceiverBaseHeight], center=true);
    }
    translate([0,0,IRReceiverRingOffsetHeight])cylinder(d=IRReceiverRingInnerDiameter,h=IRReceiverRingHeight);   
}


// TODO: space on the platform for the ir module bottom pins


difference(){
    translate([IRReceiverRingOuterDiameter/2+IRReceiverModuleSideLength,0,0])cylinder(d=IRReceiverBaseFixationDiameter,h=4);
    translate([IRReceiverRingOuterDiameter/2+IRReceiverModuleSideLength+IRReceiverBaseFixationDiameter/2,0,IRReceiverBaseFixationDiameter/2])cube(center=true,size=[IRReceiverBaseFixationDiameter,IRReceiverBaseFixationDiameter,IRReceiverBaseFixationDiameter]);
}

translate([IRReceiverRingOuterDiameter/2,0,0])cylinder(d=IRReceiverBaseFixationDiameter,h=4);