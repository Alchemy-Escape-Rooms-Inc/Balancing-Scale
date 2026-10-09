// ============================================================
// Mermaid's Tale - Standing Gauge Instrument + ESP32 Enclosure
// Half-moon YAY/NAY gauge recessed in upper front face.
// ESP32-S3 protoboard stands VERTICALLY in lower body.
// Magnetic bottom access panel. Rear cable grommet.
// Units mm.
// ============================================================
$fn = 96;

// ---------- MEASURED COMPONENTS ----------
gauge_w      = 130;   // half-moon flat-edge width
gauge_h      = 56;    // flat edge to arc top
gauge_depth  = 41;    // dial face to back of servo (servo incl.)
board_l      = 71;    // protoboard length  (stands vertical)
board_w      = 49;    // protoboard width
board_th     = 21;    // board+component thickness (-> depth when vertical)

// ---------- ENCLOSURE PARAMETERS ----------
wall         = 3;
clear        = 2;
floor_t      = 3;
pocket_clear = 1.0;   // slack around half-moon dial
servo_clear  = 4;     // depth behind dial for servo + wiring
mag_d        = 6.2;   // 6mm magnet pocket
mag_t        = 3.2;   // 3mm magnet depth
grommet_d    = 10;

// ---------- DERIVED ----------
// depth now driven by gauge (board is vertical, only 21 deep)
body_depth   = max(gauge_depth + servo_clear, board_th + 2*clear) + 2*wall;
body_w       = gauge_w + 2*wall + 2*clear;
gauge_zone_h = gauge_h + 2*wall;
board_zone_h = board_l + 2*clear + floor_t;   // board stands on long edge
body_h       = gauge_zone_h + board_zone_h;
echo(str("BODY  W=",body_w,"  D=",body_depth,"  H=",body_h));

module rounded_box(w,d,h,r=4){
    hull() for(x=[r,w-r],y=[r,d-r]) translate([x,y,0]) cylinder(r=r,h=h);
}
module half_moon(w,h,depth){
    intersection(){
        translate([0,-h,0]) cube([w,2*h,depth]);
        translate([w/2,0,0]) scale([1,(2*h)/w,1]) cylinder(d=w,h=depth);
    }
}

module body(){
    difference(){
        rounded_box(body_w, body_depth, body_h, r=5);
        // interior cavity (board zone)
        translate([wall,wall,floor_t])
            rounded_box(body_w-2*wall, body_depth-2*wall, board_zone_h, r=3);
        // gauge pocket + servo cavity (single cut -> manifold-clean)
        gx=(body_w-(gauge_w+2*pocket_clear))/2;
        translate([gx,-0.1,board_zone_h+wall])
            half_moon(gauge_w+2*pocket_clear, gauge_h+pocket_clear,
                      wall+gauge_depth+servo_clear+0.1);
        // needle shaft clearance
        translate([body_w/2,-1.1,board_zone_h+wall+6])
            rotate([-90,0,0]) cylinder(d=12,h=wall+1.2);
        // rear cable grommet
        translate([body_w/2,body_depth+0.1,floor_t+12])
            rotate([90,0,0]) cylinder(d=grommet_d,h=wall+1);
        // bottom magnet pockets
        for(x=[10,body_w-10],y=[10,body_depth-10])
            translate([x,y,-0.1]) cylinder(d=mag_d,h=mag_t);
    }
    // vertical board slot rails (board slides down between them)
    slot = board_th + 1.0;
    bx = (body_w-board_w)/2;
    for(side=[0,1])
        translate([bx + side*(board_w) , (body_depth-slot)/2, floor_t])
            cube([2, slot, board_zone_h-floor_t-4]);
}

module bottom_panel(){
    difference(){
        rounded_box(body_w, body_depth, floor_t, r=5);
        for(x=[10,body_w-10],y=[10,body_depth-10])
            translate([x,y,floor_t-mag_t+0.1]) cylinder(d=mag_d,h=mag_t);
    }
}

body();
