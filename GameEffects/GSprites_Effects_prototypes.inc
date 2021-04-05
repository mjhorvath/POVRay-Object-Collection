// Video game special effects collection for POV-Ray v2.00
// *******************************************************
// Created: 2006-03-01 by Michael Horvath
// Last updated: 2008-07-05
// Website: http://www.geocities.com/Area51/Quadrant/3864/povray.htm
// Original work is licensed under the BSD license.
// Portions of this file are copyrighted by their original authors.

#include "GSprites_Effects_textures.inc"


// -------------------------------------------------------------
// Macros for the various floor prototypes

// Energy explosion - requires "LENS.INC" by Chris Colefax
// Has some problems with the "spots" that need to be removed. 
// Alternately, you can reposition the effect so that is lies 
// in the center of the scene and then crop the resulting image 
// accordingly. This is what I did when I actually rendered the 
// sprite for a game. The scaling is unpredictable when moving 
// the effect in relation to the camera. Better to keep it at 
// the center, but I'll include it as-is, here.
// Finally, this effect interferes with some of the other 
// objects. Disable this one while rendering the others.
#macro GSprites_Explosion_Effects_Prototype_C(Tiles_Width,Tiles_Height,Tiles_Translate)
	#local Effect_Scale = 1/256;
	#local TClock = clock * 8/10 + 1/10;
	#local TCutoff = 1;
	#local camera_location = GSprites_Camera_Location;
	#local camera_look_at = GSprites_Camera_LookAt;
	#local effect_location = y * Tiles_Height + Tiles_Translate;
	#local effect_always_on_top = false;
	#local effect_intensity = 1 - TClock;
	#local effect_brightness = 2;
	//#local effect_type = "Camera2";	// I like this one better, but it has spots when the camera isn't aligned right
	#local effect_type = "Star";
	#local effect_spin = 1;
	#local spots_contract_to_centre = true;
	#switch (clock)
		#range (0, TCutoff)
			#local effect_scale = TClock * Effect_Scale;
		#break
		#range (TCutoff, 1)
			#local TReverse = (1 - ((TClock - TCutoff)/(1 - TCutoff))) * TCutoff;
			#local effect_scale = TReverse * Effect_Scale;
		#break
	#end
	#include "LENS.INC"
	GSprites_Standard_Camera()
#end

// Another explosion - by John (evilsnack) at http://www.geocities.com/evilsnack/
#macro GSprites_Explosion_Effects_Prototype_D(Tiles_Width,Tiles_Height)
	#local TClock = clock * 6/10 + 4/10;
	#local PClock = pow(1 - clock * 9/10, 2);
	//#local PClock = 1 - clock * 9/10;
	#local Radius = Tiles_Width/2;
	intersection
	{
		sphere
		{
			0,2 hollow
			texture {GSprites_Plain_Clear}
			interior
			{
				media
				{
					absorption 8/Radius * PClock emission 8/Radius * PClock method 3 samples 30,30 intervals 1
					density
					{
						spherical
						warp {turbulence TClock * 0.3 + .05 lambda 2.75}
						density_map
						{
							[1 - 0.99 * TClock rgb <0,0,0,>]
							[1 - 0.99 * TClock rgb <max(0, 1 - TClock * 3/2),max(0, 1 - TClock * 9/2),max(0, 1 - TClock * 6),>]
							[1 - 0.00 rgb <3/4,1/4,0,>]
						}
					}
				}
				media
				{
					absorption 3/Radius * PClock scattering {1 0.3/Radius * PClock} method 3 samples 30,30 intervals 1
					density
					{
						spherical
						warp {turbulence TClock * 0.3 + .05 lambda 2.75}
						density_map
						{
							[1 - 0.99 * TClock rgb <0,0,0,>]
							[1 - 0.99 * TClock rgb <1,1,1,>]
							[1 - 0.49 * TClock rgb <1,1,1,>]
							[1 - 0.49 * TClock rgb <0,0,0,>]
						}
					}
				}
			}
			scale TClock * Radius
			translate y * Tiles_Height
		}
		plane {-y,0}
		texture {GSprites_Plain_Clear}
	}
#end

// Fiery explosion - can't remember the author
#macro GSprites_Explosion_Effects_Prototype_E(Tiles_Width,Tiles_Height,Tiles_Seed)
	#local Scale_All = Tiles_Width/4;
	#local IClock = 1 - (clock * 9/10);
	intersection
	{
		union
		{
			// Gaswolke  - expanding gas
			sphere
			{
				0,1 hollow
				texture {GSprites_Plain_Clear}
				interior
				{
					media
					{
						emission 4/pow(clock * 2 + 0.1, 2.5)/Scale_All*IClock
						absorption 2.5/pow(clock * 2 + 0.1, 2.5)/Scale_All*IClock
						density
						{
							spherical
							color_map
							{
								[0.0 color rgb 0]
								[0.2 color rgb <1, 0.5, 0.2,> * 0.5]
								[0.3 color rgb <1, 0.7, 0.3,> * 0.3]
								[0.5 color rgb 0]
								[0.8 color rgb 0]
//								[0.95 color rgb <0.4, 0.4, 0.5,> * 0.5]
								[0.95 color rgb <4, 4, 5,> * 0.5]
								[1.0 color rgb 5]
							}
						}
						density
						{
						
							granite 
							color_map
							{
								[0.0 color rgb <1, 0.7, 0.7,> * 0.3]
								[0.25 color rgb <0.7, 0.7, 1> * 0.4]
								[0.35 color rgb 1]
								[1.0 color rgb 1]
							}
							scale 1
						}
					}
				}
				scale 1.5
				scale 1.5 * pow(clock, 0.85) + 0.001
			}
			// Blitz - Flash
			sphere
			{
				0,1 hollow
				texture {GSprites_Plain_Clear}
				interior
				{
					media
					{
						emission 2/pow(clock*2+1,4)/Scale_All*IClock
						density
						{
							spherical
							color_map
							{
								[0.0 color rgb 0]
								[0.25 color rgb 1/6]
								[0.50 color rgb 1/5]
								[0.75 color rgb 1/2]
								[1.00 color rgb 1]
							}
						}
					}
				}
				scale 1.5
				scale 1-pow(abs(clock-.11),.3)
			}
			// Partikel - particles
			#if(clock>.05)
				#local i=0;
				#while(i < floor(100 * IClock))
					sphere
					{
						0,.01
						scale <5,1,1>
						pigment{color rgb .3} finish{ambient .5}
						translate x*2*clock+.001*(rand(Tiles_Seed)*.5+.5)
						rotate z*180*(rand(Tiles_Seed)-.5)
						rotate y*360*rand(Tiles_Seed)
					}
					#local i = i + 1;
				#end	
			#end
			scale Scale_All
			translate y * Tiles_Height
		}
		plane {-y,0}
		texture {GSprites_Plain_Clear}
	}
#end

// Flame - requires Rune's Particle System
#macro GSprites_Fire_Effects_Prototype(Tiles_Width,Tiles_Height)
	#local Flame_Obj = sphere
	{
		0, 2.0
		texture {GSprites_Flame_Effects_Texture}
		scale y * 1/10
		translate y * 1/2
	}
	union
	{
		// Rune's Particle System
		// Sample animation scene file
		// 
		// Suggested command line settings (for 25 frames per second):
		// +kff50 +kf2 +kc

		// Include particle system include file
		// ************************************

		#include "particle.inc"

		// Clock settings
		// **************

		#local particle_start  = 0.0;
		#local particle_end    = 2.0;
		#local particle_cyclic = on;
		#local particle_steps  = 100;

		// General particle settings
		// *************************

		#local particle_frequency = 40;
		#local particle_life      = 0.7;
		#local particle_lifeturb  = 0.3;
		#local particle_seed      = 123;
		//#declare particle_maxnumber = 100;

		// Environment settings
		// ********************

		#local particle_drag      = 0.0;
		#local particle_transfer  = 0.0;

		#macro particle_gravity  (Clock,Point) 40 * y	#end
		#macro particle_wind     (Clock,Point) <0,0,0,>	#end

		// Emitter settings
		// ****************

		#macro particle_emitter  (Clock) <0,0,0>	#end
		#macro particle_emitting (Clock) on		#end
		#macro particle_emitvect (Clock) <0,0,0>	#end
		#macro particle_emitturb (Clock) 1.5		#end
		#macro particle_emitobj  (Clock) Flame_Obj	#end
		#macro particle_emitobjn (Clock) 0.0		#end

		// Collision settings
		// ******************

		// (Collision settings not used)

		// Call particle system
		// ********************

		particle_system("fire")

		// Load system
		// ***********

		#include "fire.inc"

		#local fire_method     = 1;
		#local fire_color      = <0.85,0.45,0.15>;
		#local fire_intensity  = 0.25;
		#local fire_highlight  = 2.0;
		#local fire_samples    = 5;
		#local fire_turbulence = 1.0;
		#local fire_size       = 1.0;
		#local fire_stretch    = 0.1;

		fire_create("fire")

		// Scene setup
		// ***********

		light_source
		{
			fire_center, color 0.25 * fire_light * <1.0,0.7,0.4>
			fade_power 2 fade_distance 10
			area_light 5*x, 5*z, 5, 5 jitter adaptive 0 circular orient
		}

		scale <Tiles_Width/12,Tiles_Height/12,Tiles_Width/12,>
	}
#end

// Smoke column - requires Rune's Particle System
#macro GSprites_Smoke_Effects_Prototype(Tiles_Width,Tiles_Height,ShowCloud,ShowSmoke,ShowDust,ShowDebris)
	union
	{
		// Rune's Particle System
		// Sample animation scene file
		// 
		// Suggested command line settings (for 25 frames per second):
		// +kff500 +ki-0.5 +kf1.5 -kc
	
		#local exp_test = no; // show solid spheres instead of ones with fire and smoke textures
		#local FocusSpline = spline
		{
			natural_spline
			-0.50,<0,0,5>,
			+0.10,<0,0,8>,
			+0.50,<0,0,1>,
			+0.80,<0,0,1>,
			+1.30,<0,0,0>,
		}
		#local FlashSpline = spline
		{
			linear_spline
			0.00,0.00,
			0.03,1.00,
			0.10,0.30,
		}
		light_source
		{
			<0,1.5,-0.2>,
			color <1.0,0.4,0.1>*25*FlashSpline(clock)
			fade_power 2 fade_distance 1
			//shadowless
		}
		#local Ground_Plane = plane {y,0}
		#local camera_location = GSprites_Camera_Location;
	
		//object {BuildingAndGround}
		
		// Include particle system include file
		// ************************************
	
		#include "particle.inc"
		
		// ...and particle element include file.
		#include "expl.inc"
	
		
		// FIRST PARTICLE SYSTEM - EXPLOSION A
		// ***********************************
		
		// Clock options
		// *************
		
		#local particle_start  = 0;
		#local particle_end    = 1.0;
		#local particle_cyclic = on;
		#local particle_steps  = 50;
		
		// General particle options
		// ************************
		
		#local particle_frequency = 150;
		#local particle_life      = 0.5;
		#local particle_lifeturb  = 0.5;
		#local particle_seed      = 234;
		//#declare particle_maxnumber = 100;
		
		// Environment options
		// *******************
		
		#local particle_drag      = 0.3;
		#local particle_transfer  = 0.0;
		#macro particle_gravity  (Clock,Point) 32.2*Tiles_Height/64*y	#end
		#macro particle_wind     (Clock,Point) 5*x	#end
		
		// Emitter options
		// ***************
		
		#macro particle_emitter  (Clock) <0,1.3,0>	#end
		#macro particle_emitting (Clock) (Clock<0.05)	#end
		#macro particle_emitvect (Clock) <0,0,0>	#end
		#macro particle_emitturb (Clock) 30.0		#end
		#undef particle_emitobj
		//#macro particle_emitobj  (Clock) 		#end
		
		// Collision options
		// *****************
		
		#local particle_blockobj     = Ground_Plane
		#local particle_bounce       = 1.0;
		#local particle_bounceturb   = 0.0;
		#local particle_friction     = 0.0;
		#local particle_bounceoffset = 0.01;
		//#macro particle_killobj    (Clock) object {}    #end
		
		// Particle element macro (optional)
		// *********************************
		
		#local exp_size       = 4.0;
		#local exp_opacity    = 1.0;
		#local exp_emission   = 2.5;
		#local exp_smokeshade = 0.4;
		#local exp_smokeglow  = 1.5;
		#local exp_smokestart = 0.0;
		#local exp_fireend    = 0.0;
		#local exp_ambient    = 0.5;
		#macro particle_element() exp_element() #end
		
		// Call particle system
		// ********************
		
		#if (ShowCloud)
			particle_system("explosion_a")
		#end
		
		
		// SECOND PARTICLE SYSTEM - EXPLOSION B
		// ************************************
		
		#local particle_frequency = 60;
		#macro particle_emitting (Clock) on #end
		//#macro particle_emitvect (Clock) <0,0,0> #end
		#macro particle_emitturb (Clock) max(7+5,20-20*Clock) #end
		#local exp_size       = 2.5;	//2.5
		#local exp_opacity    = 1.0;	//1.0
		#local exp_emission   = 2.5;	//2.5
		#local exp_smokeshade = 0.4;	//0.4
		#local exp_smokeglow  = 1.5;	//1.5
		#local exp_smokestart = 0.0;	//0.14
		#local exp_fireend    = 0.0;	//0.23
		#macro particle_element() exp_element() #end
		
		// Call particle system
		// ********************
		
		#if (ShowSmoke)
			particle_system ("explosion_b")
		#end
		
		
		// THIRD PARTICLE SYSTEM - DUST
		// ****************************
		
		#local particle_frequency = 150;
		#local particle_life      = 2.5*1.5;
		#macro particle_emitter  (Clock) <0,0.7,0> #end
		#macro particle_emitting (Clock) (Clock<0.05) #end
		//#macro particle_emitvect (Clock) <0,0,0> #end
		#macro particle_emitturb (Clock) 35 #end
		#macro particle_gravity  (Clock,Point) -10*y #end
		#local particle_blockobj    = Ground_Plane
		#local exp_size       = 6.0;
		#local exp_opacity    = 0.3;
		#local exp_emission   = 0.0;
		#local exp_smokeshade = 1.0;
		#local exp_smokeglow  = 0.0;
		#local exp_smokestart = 0.0;
		#local exp_fireend    = 0.0;
		#macro particle_element() exp_element() #end
		
		// Call particle system
		// ********************
		
		#if (ShowDust)
			particle_system("dust")
		#end
		
		scale <Tiles_Width/12,Tiles_Height/12,Tiles_Width/12,>
	}
#end

// Cuncussive explosion - requires Object Exploder by Chris Colefax
#macro GSprites_Explosion_Effects_Prototype_A(Tiles_Width,Tiles_Height,Scale_Factor)
	#local Wid = Tiles_Width/2;
	#local Center = y * Tiles_Height;
	#local Radius = Tiles_Width/16;
	#local Puff_Radius = Tiles_Width/2;	//(sin(pi/3)-sin(pi/6))/cos(pi/4)
	#local Smoke_Ball = sphere {Center,Radius texture {Smoke_Texture}}
	union
	{
		#local explode_start =		-01/8;
		#local explode_life =		+10/8;
		#local explode_object =		object {Smoke_Ball}
		#local object_centre =		Center;
		#local object_size =		Radius * 2;
		#local object_hollow =		true;
		#local particle_object =	sphere {0,1024}
		#local particle_res =		<6,1,6,>;
		#local particle_texture =	Smoke_Texture;
		#local exp_location =		Center;
		#local exp_strength =		32.2/Scale_Factor;
		#local exp_gravity =		32.2/Scale_Factor;
		//#local exp_spin =		1;
		//#local exp_seed =		87632;
		//#local exp_turb =		1;
		#local disintegration =		1;
		//#local scale_turb =		1/2;
		//#local rotate_turb =		1;
		//#local vel_turb =		3/10;
		//#local dir_turb =		4/10;
		//#local spin_turb =		3/10;
		#local ground_plane =		true;
		#include "Explode.inc"
		clipped_by {plane {-y,0}}
	}
#end

// Volatile explosion - requires Object Exploder by Chris Colefax
#macro GSprites_Explosion_Effects_Prototype_B(Tiles_Width,Tiles_Height,Scale_Factor)
	#local Wid = Tiles_Width/2;
	#local Center = y * Tiles_Height;
	#local Radius = Tiles_Width/32;
	#local Shrapnel_Ball = sphere {Center,Radius texture {Shrapnel_Texture}}
	union
	{
		#local explode_start =		-01/8;
		#local explode_life =		+10/8;
		#local explode_object =		object {Shrapnel_Ball}
		#local object_centre =		Center;
		#local object_size =		Radius * 2;
		#local object_hollow =		false;
		#local particle_object =	sphere {0,3/4}
		#local particle_res =		<4,4,4>;
		#local particle_texture =	Shrapnel_Texture;
		#local exp_location =		Center;
		#local exp_strength =		32.2/Scale_Factor;
		#local exp_gravity =		32.2/Scale_Factor;
		#local exp_spin =		1;
		#local exp_seed =		87632;
		#local exp_turb =		1;
		#local disintegration =		1/2;
		#local scale_turb =		1/2;
		#local rotate_turb =		1;
		#local vel_turb =		3/10;
		#local dir_turb =		4/10;
		#local spin_turb =		3/10;
		#local ground_plane =		true;
		#include "Explode.inc"
		disc {0,GSprites_Camera_Location,1 texture {Fire_Texture} scale clock * Wid + Wid translate Center}
		clipped_by {plane {-y,0}}
	}
#end

// Bullet projectile - requires "LENS.INC" by Chris Colefax
// Note: this effect will only appear as it is supposed to if 
// the "Energy explosion" effect is commented out, above.
#macro GSprites_Bullet_Effects_Prototype(Tiles_Width,Tiles_Height,Burst_Fire,Translate_Vector)
	#local Rotate_Y =		clock * 360 - 45;
	#local camera_location =	vaxis_rotate(<+Tiles_Width,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
	#local camera_look_at =		vaxis_rotate(<+00000000000,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
	#local effect_location =	vaxis_rotate(<+00000000000,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
	#if (clock = 2/8)
		#local camera_location = <+Tiles_Width*6/8,Tiles_Height,0,> + Translate_Vector;		//*5/8
		#local camera_sky = z;
	#end
	#if (clock = 6/8)
		#local camera_location = <-Tiles_Width*6/8,Tiles_Height,0,> + Translate_Vector;		//*5/8
		#local camera_sky = z;
	#end
	#include "LNSEFCTS.INC"
	#if (Burst_Fire = true)
		#local camera_location =	vaxis_rotate(<+Tiles_Width*10/8,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#local camera_look_at =		vaxis_rotate(<+Tiles_Width*02/8,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#local effect_location =	vaxis_rotate(<+Tiles_Width*02/8,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#if (clock = 2/8)
			#local camera_location = <+Tiles_Width*6/8,Tiles_Height,0,> + Translate_Vector;
			#local camera_sky = z;
		#end
		#if (clock = 6/8)
			#local camera_location = <-Tiles_Width*6/8,Tiles_Height,0,> + Translate_Vector;
			#local camera_sky = z;
		#end
		#include "LNSEFCTS.INC"
		#local camera_location =	vaxis_rotate(<-Tiles_Width*10/8,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#local camera_look_at =		vaxis_rotate(<-Tiles_Width*02/8,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#local effect_location =	vaxis_rotate(<-Tiles_Width*02/8,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#if (clock = 2/8)
			#local camera_location = <-Tiles_Width*6/8,Tiles_Height,0,> + Translate_Vector;
			#local camera_sky = z;
		#end
		#if (clock = 6/8)
			#local camera_location = <+Tiles_Width*6/8,Tiles_Height,0,> + Translate_Vector;
			#local camera_sky = z;
		#end
		#include "LNSEFCTS.INC"
	#end
	GSprites_Standard_Camera()
#end

// Energy projectile - requires "LENS.INC" by Chris Colefax
// Note: this effect will only appear as it is supposed to if 
// the "Energy explosion" effect is commented out, above.
#macro GSprites_Sparkle_Effects_Prototype(Tiles_Width,Tiles_Height,Burst_Fire,Translate_Vector)
	#local Rotate_Y = clock * 360 - 45;
	#local camera_location =	<0,Tiles_Height,0,> + Translate_Vector;
	#local camera_look_at =		Translate_Vector;
	#local effect_location =	Translate_Vector;
	#local camera_sky =		y;
	#include "LNSEFCTS.INC"
	#if (Burst_Fire = true)
		#local camera_location =	vaxis_rotate(<+Tiles_Width/4,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#include "LNSEFCTS.INC"
		#local camera_location =	vaxis_rotate(<-Tiles_Width/4,Tiles_Height,0,>, y, Rotate_Y) + Translate_Vector;
		#include "LNSEFCTS.INC"
	#end
	GSprites_Standard_Camera()
#end

// Flamethrower - by H.E. Day
#macro GSprites_Flamethrower_Effects_Prototype(Tiles_Width,Tiles_Height)
	#local Vector = z;
	#local Length = 20;
	#local DensMap = density
	{
	        radial
	        rotate y * 90
	        translate Vector * -Length * 1/4
	        translate Vector * -Length * 3/4 * clock
	        warp {turbulence 0.8 octaves 3 lambda 2.5}
	        translate Vector * +Length * 3/4 * clock
	        colour_map
	        {
	                [0.490 rgb 0]
	                [0.495 rgb 1]
	                [0.500 rgb 4]
	                [0.505 rgb 1]
	                [0.510 rgb 0]
	        }
	}
	#local ColMap = density
	{
	        radial
	        rotate y*90
	        translate Vector * -Length * 1/4
	        translate Vector * -Length * 3/4 * clock
	        warp {turbulence 1.2 octaves 3 lambda 2.5}
	        translate Vector * +Length * 3/4 * clock
	        colour_map
	        {
	                [0.495 rgb <1,1,0,>]
	                [0.500 rgb 1]
	                [0.505 rgb <1,1,0,>]
	        }
	}
	#local DensEnding = density
	{
	        spherical
	        scale 10
	        colour_map
	        {
	                [0.000 rgb 0]
	                [0.500 rgb <1,1/2,0,>/2]
	                [0.750 rgb <1,1,0,>] 
	                [0.875 rgb 1] 
	                [1.000 rgb <-1,0,1,>]
	        }
	}
	#local DensMap2 = density
	{
	        radial
	        rotate y*90
	        translate Vector * -Length * 1/7
	        translate Vector * -Length * 5/4 * clock
	        warp {turbulence 1.3 octaves 3 }
	        translate Vector * +Length * 5/4 * clock
	        colour_map
	        {
	                [0.45 rgb 0.0]
	                [0.48 rgb 0.4]
	                [0.50 rgb 1.0]
	                [0.52 rgb 0.4]
	                [0.55 rgb 0.0]
	        }
	}
	#local ColMap2 = density
	{
	        radial
	        rotate y*90
	        translate Vector * -Length * 1/7
	        translate Vector * -Length * 5/4 * clock
	        warp {turbulence 1.3 octaves 3 }
	        translate Vector * +Length * 5/4 * clock
	        colour_map
	        {
	                [0.47 rgb <1,0,0,>]
	                [0.50 rgb <1,1/2,0,>]
	                [0.53 rgb <1,0,0,>]
	        }
	}
	#local DensEnding2 = density
	{
	        spherical
	        scale 5
	        scale Vector*2
	        translate Vector * -Length * 3/4 * clock
	        warp {turbulence 0.5}
	        translate Vector * +Length * 3/4 * clock
	        colour_map
	        {
	                [0.000 rgb 0]
	                [0.100 rgb 1]
	                [0.875 rgb 1] 
	                [1.000 rgb <-1,-1,+1,>]
	        }
	}
	union
	{
		cylinder
		{
		        0, Vector * Length/2, Length/7
		        pigment {Clear}
		        hollow
		        interior
		        {
		                media
		                {
		                        emission 1/2
		                        density {ColMap}
		                        density {DensMap}
		                        density {DensMap rotate Vector * 90}
		                        density {DensEnding}
		                	scale 0.8
		                }
		                media
		                {
		                        emission 0.75/2
		                        density {ColMap2}
		                        density {DensMap2}
		                        density {DensMap2 rotate Vector * 90}
		                        density {DensEnding2}
		                	scale 0.8
		                }
		        }
		        scale 3/.8
		}
		scale Tiles_Width/32
		translate -z * Tiles_Width/2
		translate y * Tiles_Height
		rotate y * (360 * clock - 45)
	}
#end

// Missile
#macro GSprites_Missile_Effects_Prototype(Tiles_Width,Tiles_Height,Burst_Fire)
	#declare Missile = union
	{
		cylinder
		{
			<0,2,0,>,<0,12,0,>,1
			texture {GSprites_Plain_White}
		}
		cone
		{
			<0,12,0,>,1,<0,16,0,>,0
			texture {GSprites_Plain_White}
		}
		cone
		{
			<0,2,0,>,2,<0,4,0,>,0
			texture {GSprites_Plain_Black}
		}
		cone
		{
			<0,-2,0,>,0,<0,2,0,>,1
			texture {GSprites_Exhaust_Effects_Texture_D}
		}
		cone
		{
			<0,-64,0,>,8,<0,0,0,>,2
			open
			texture {GSprites_Exhaust_Effects_Mixture_A}
		}
		translate y * 24
		rotate x * 90
		scale <1/2,1/2,1,>
		scale Tiles_Width/64
	}
	union
	{
		object {Missile translate y * Tiles_Height}
		#if (Burst_Fire = 1)
			object {Missile translate x * Tiles_Width/4 translate y * Tiles_Height}
			object {Missile translate x * -Tiles_Width/4 translate y * Tiles_Height}
		#end
		rotate y * (clock * 360 - 45)
	}
#end


// -------------------------------------------------------------
// Definitions for the various pre-defined floor types

// Energy explosion - requires "LENS.INC" by Chris Colefax
// Has some problems with the "spots" that need to be removed. 
// Alternately, you can reposition the effect so that is lies 
// in the center of the scene and then crop the resulting image 
// accordingly. This is what I did when I actually rendered the 
// sprite for a game. The scaling is unpredictable when moving 
// the effect in relation to the camera. Better to keep it at 
// the center, but I'll include it as-is, here.
// Finally, this effect interferes with some of the other 
// objects. Disable this one while rendering the others.
#macro GSprites_Explosion_Effects_Object_C(Tiles_Width,Tiles_Height,Tiles_Translate)
	GSprites_Explosion_Effects_Prototype_C(Tiles_Width,Tiles_Height,Tiles_Translate)
#end

// Another explosion - by John (evilsnack) at http://www.geocities.com/evilsnack/
#macro GSprites_Explosion_Effects_Object_D(Tiles_Width,Tiles_Height)
	GSprites_Explosion_Effects_Prototype_D(Tiles_Width,Tiles_Height)
#end

// Fiery explosion - can't remember the author
#macro GSprites_Explosion_Effects_Object_E(Tiles_Width,Tiles_Height,Tiles_Seed)
	GSprites_Explosion_Effects_Prototype_E(Tiles_Width,Tiles_Height,Tiles_Seed)
#end

// Flame - requires Rune's Particle System
#macro GSprites_Fire_Effects_Object(Tiles_Width,Tiles_Height)
	GSprites_Fire_Effects_Prototype(Tiles_Width,Tiles_Height)
#end

// Smoke column - requires Rune's Particle System
#macro GSprites_Smoke_Effects_Object(Tiles_Width,Tiles_Height)
	GSprites_Smoke_Effects_Prototype(Tiles_Width,Tiles_Height,no,yes,no,no)
#end

// Cuncussive explosion - requires Object Exploder by Chris Colefax
#macro GSprites_Cuncussion_Effects_Object(Tiles_Width,Tiles_Height,Scale_Factor)
	#local Smoke_Texture = GSprites_Explosion_Effects_Texture_B;
	#local Fire_Texture = GSprites_FireBall_Effects_Texture;
	GSprites_Explosion_Effects_Prototype_A(Tiles_Width,Tiles_Height,Scale_Factor)
#end

// Volatile explosion - requires Object Exploder by Chris Colefax
#macro GSprites_Volatile_Effects_Object(Tiles_Width,Tiles_Height,Scale_Factor)
	#local Shrapnel_Texture = GSprites_Plain_DarkGray;
	#local Fire_Texture = GSprites_FireBall_Effects_Texture;
	GSprites_Explosion_Effects_Prototype_B(Tiles_Width,Tiles_Height,Scale_Factor)
#end

// Bullet projectile - requires "LENS.INC" by Chris Colefax
// Note: this effect will only appear as it is supposed to if 
// the "Energy explosion" effect is commented out, above.
#macro GSprites_Bullet_Effects_Object(Tiles_Width,Tiles_Height,Burst_Fire,Translate_Vector)
	#local effect_colour = <1,1/2,0,>;
	#local effect_always_on_top = false;
	#local effect_scale = 1/2;
	#local effect_intensity = 4;
	#local streak_type = 7;
	#local effect_type = "Headlight";
	#local source_color = <1/2,1/2,1/2,>;
	GSprites_Bullet_Effects_Prototype(Tiles_Width,Tiles_Height,Burst_Fire,Translate_Vector)
#end

// Energy projectile - requires "LENS.INC" by Chris Colefax
// Note: this effect will only appear as it is supposed to if 
// the "Energy explosion" effect is commented out, above.
#macro GSprites_Sparkle_Effects_Object(Tiles_Width,Tiles_Height,Burst_Fire,Translate_Vector)
	#local effect_colour =		<0.9, 1.0, 1.1>;
	#local source_color =		<1.1, 1.2, 1.3>;
	#local effect_scale =		pow(Tiles_Width,2);		// 10000 // Width
	#local effect_spin =		1;
	//#local effect_flicker =	1;
	//#local effect_intensity =	2;
	//#local effect_brightness =	1;
	#local streak_type =		3;
	GSprites_Sparkle_Effects_Prototype(Tiles_Width,Tiles_Height,Burst_Fire,Translate_Vector)
#end

// Flamethrower - by H.E. Day
#macro GSprites_Flamethrower_Effects_Object(Tiles_Width,Tiles_Height)
	GSprites_Flamethrower_Effects_Prototype(Tiles_Width,Tiles_Height)
#end

// Missile
#macro GSprites_Missile_Effects_Object(Tiles_Width,Tiles_Height,Burst_Fire)
	GSprites_Missile_Effects_Prototype(Tiles_Width,Tiles_Height,Burst_Fire)
#end
