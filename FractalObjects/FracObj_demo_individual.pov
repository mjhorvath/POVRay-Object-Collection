// Title: Fractal Objects Include v1.11
// Authors: Michael Horvath, http://isometricland.net
// Created: 2008/11/26
// Updated: 2019/05/30
// This file is licensed under the terms of the CC-LGPL.

#version 3.7

#include "math.inc"
#include "FracObj_include.inc"
#include "rad_def.inc"

//------------------------------------------------------------------------------Scenery

global_settings
{
	assumed_gamma	1.0
	adc_bailout		0.005
	max_trace_level	50
	charset			utf8
	ambient_light	0
	radiosity
	{
		Rad_Settings(Radiosity_Final, off, off)
	}
}

background {srgb 1/4}

sphere
{
	0, 1e+6
	inverse
	texture
	{
		pigment {color srgb 1}
		finish {emission 1}
	}
	no_image
}

light_source
{
	-x*100
	color rgb 1
//	shadowless
	parallel
	point_at	0
	rotate		-z*60
	rotate		-y*30
}

light_source
{
	-x*100
	color rgb 1
//	shadowless
	parallel
	point_at	0
	rotate		-z*60
	rotate		+y*60
}

camera
{
	#local CameraDistance	= 10;
	#local ScreenArea		= cosd(45) * 2.1;
	#local AspectRatio		= image_width/image_height;
//	orthographic
	location	-z*CameraDistance
	direction	+z*CameraDistance
	right		+x*ScreenArea*AspectRatio
	up			+y*ScreenArea
	rotate		+x*asind(tand(30))
	rotate		+y*45
}


//------------------------------------------------------------------------------CSG objects

#local Default_Texture = texture
{
	pigment {color srgb <1,1/2,0>}
//	finish {ambient 1/2}
}
/*
// Menger Sponge
object
{
	FracObj_Menger_Sponge(2, <1,1,1,>, 0.01)
	rotate		+y * 180
	scale		2
//	translate	-x * 2
	texture {Default_Texture}
}
*/

// Sierpinski Pyramid
// The pyramid is 1 unit high by default; if you want the faces to be equilateral triangles, then scale the y-axis by FracObj_Sierpinski_Pyramid_Eql_Hgh.
// The pyramid is 1 unit high by default; if you want the faces to look like equilateral triangles when viewed from the side along the x or z axes, then scale the y-axis by FracObj_Sierpinski_Pyramid_Eql2D_Hgh.
// Scaling by the same amount again also produces interesting results.
object
{
	FracObj_Sierpinski_Pyramid(2, <1,1,1,>, 0)
	rotate		+y * 180
	translate	(+x+z) * 1/4
	texture {Default_Texture}
//	no_shadow
}

/*
// Sierpinski Tetrahedron
// The tetrahedron is 1 unit high by default; if you want the faces to be equilateral triangles, then scale the y-axis by FracObj_Sierpinski_Tetrahedron_Eql_Hgh.
// Scaling by the same amount again also produces interesting results.
object
{
	FracObj_Sierpinski_Tetrahedron(2, <1,FracObj_Sierpinski_Tetrahedron_Eql_Hgh,1,>, 0.01)
	rotate		+y * 180
	scale		3
	translate	+x
//	translate	-x * 2
//	translate	(+x-z) * 2
//	scale		+y * FracObj_Eql_Hgh
	texture {Default_Texture}
}
*/
