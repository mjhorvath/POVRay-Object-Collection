// Caption: Grid & Axes Include File v1.8
// Authors: Michael Horvath
// Website: http://isometricland.net
// Created: 2004/07/14
// Updated: 2019/12/13
// This file is licensed under the terms of the CC-LGPL.

#include "axes_macro.inc"
#include "math.inc"
#include "finish.inc"
#include "transforms.inc"

global_settings
{
	assumed_gamma 1.0
//	radiosity
//	{
//		brightness 0.3
//	}
}

camera
{
	orthographic
	location	-z*1000
	direction	+z*1000
	up			+y*1*image_height/image_width
	right		+x*1
	rotate		+x*asind(tand(30))
	rotate		+y*45
}

sky_sphere
{
	pigment
	{
		gradient y
		color_map
		{
			[0.0 rgb <0.6,0.7,1.0>]		//153, 178.5, 255	//150, 240, 192
			[0.7 rgb <0.0,0.1,0.8>]		//  0,  25.5, 204	//155, 240, 96
		}
		scale 2
		translate 1
	}
}

light_source
{
	<0, 0, -100>            // light's position (translated below)
	color rgb <1, 1, 1>  // light's color
	rotate <60,30,0>
	parallel
	shadowless
}

// the coordinate grid and axes

#declare Axes_Color = 3/4;		// Axes_color: 1/4 for dark, 3/4 for light
#declare Axes_Infinite = false;	// Axes_Infinite: are the planes infinite in every direction?

Axes_Macro
(
	2.5,	// Axes_axesSize:	The distance from the origin to one of the grid's edges.	(float)
	2.5/3,	// Axes_majUnit:	The size of each large-unit square.	(float)
	5,		// Axes_minUnit:	The number of small-unit squares that make up a large-unit square.	(integer)
	0.005,	// Axes_thickRatio:	The thickness of the grid lines (as a factor of axesSize).	(float)
	on,		// Axes_aBool:		Turns the axes on/off. (boolian)
	on,		// Axes_mBool:		Turns the minor units on/off. (boolian)
	off,	// Axes_xBool:		Turns the plane perpendicular to the x-axis on/off.	(boolian)
	on,		// Axes_yBool:		Turns the plane perpendicular to the y-axis on/off.	(boolian)
	off		// Axes_zBool:		Turns the plane perpendicular to the z-axis on/off.	(boolian)
)

object
{
	Axes_Object
}
