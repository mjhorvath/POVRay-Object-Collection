// Caption: Cube Fonts macro
// Version: 1.0
// Authors: Michael Horvath
// Website: http://isometricland.net
// Created: 2009-10-29
// Updated: 2021-03-11
// This file is licensed under the terms of the CC-LGPL.


//------------------------------
// Includes

#include "CubeFonts_include.inc"
#include "math.inc"


//------------------------------
// Scenery

global_settings
{
	assumed_gamma	1.0
	ambient_light	0.3
}

background {rgb 1/8}
/*
light_source
{
	-x * 100
	color rgb 1
	shadowless
	parallel
	point_at	<0,0,0>
	rotate		-z * 60
	rotate		-y * 30
}

light_source
{
	-x * 100
	color rgb 1
//	shadowless
	parallel
	point_at	<0,0,0>
	rotate		-z * 60
	rotate		+y * 60
}
*/
// R
light_source
{
	-x * 100
	color rgb x
	parallel
	point_at	<0,0,0>
}

// G
light_source
{
	+y * 100
	color rgb y
	parallel
	point_at	<0,0,0>
}
// B
light_source
{
	-z * 100
	color rgb z
	parallel
	point_at	<0,0,0>
}

// Iso
camera
{
	#local CameraDistance =	10;
	#local AspectRatio =	image_width/image_height;
	orthographic
	location	-z * CameraDistance
	direction	+z
	up			+y * 5
	right		+x * 5 * AspectRatio
	rotate		+x * asind(tand(30))
	rotate		+y * 45
}
/*
union
{
	sphere
	{
		0,1/100
		pigment{color rgb 0}
	}
	cylinder
	{
		0,x*5,1/100
		pigment{color srgb x}
	}
	cylinder
	{
		0,y*5,1/100
		pigment{color srgb y}
	}
	cylinder
	{
		0,z*5,1/100
		pigment{color srgb z}
	}
}
*/

//------------------------------
// Macro examples

#declare Font_Name =		"visitor1.ttf";		// Visitor by Ænigma
#declare Text_Direction =	7;
#declare Char_Distance =	1;
#declare Char_Scale =		<1,1,1>;
#declare String_Array = array[3]
{
	"I",
	"S",
	"O"
}
object
{
	CBF_String_Macro(String_Array,Font_Name,Text_Direction,Char_Distance,Char_Scale)
	pigment {color srgb 1}
	finish {ambient 0 diffuse 1}
	translate <-0.5,-0.5,-0.5>
}

// R
polygon
{
	4, <-1,-1>, <+1,-1>, <+1,+1>, <-1,+1>
	pigment {color srgb 1}
	finish {ambient 0 diffuse 1}
	translate <0,0,+2>
	rotate +y * 90
}

// G
polygon
{
	4, <-1,-1>, <+1,-1>, <+1,+1>, <-1,+1>
	pigment {color srgb 1}
	finish {ambient 0 diffuse 1}
	translate <0,0,+2>
	rotate +x * 90
}

// B
polygon
{
	4, <-1,-1>, <+1,-1>, <+1,+1>, <-1,+1>
	pigment {color srgb 1}
	finish {ambient 0 diffuse 1}
	translate <0,0,+2>
	rotate +z * 0
}
