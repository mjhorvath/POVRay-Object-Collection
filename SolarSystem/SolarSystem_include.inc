// RANDOM SOLAR SYSTEM GENERATOR FOR POV-Ray v1.2
// **********************************************
// Authors: Michael Horvath
// Created: 2005-03-01
// Updated: 2021-03-10
// Website: http://isometricland.net
// This file is licensed under the terms of the CC-LGPL.
//
// DESCRIPTION
// ***********
// Randomly generates a solar system, complete with sun, moons, planets and asteroid belts.
//
// INSTRUCTIONS
// ************
// If rendering an orthographic image, then the size of each tile will be equal to the height of the output image divided by sqrt(SSys_NumberPlanets).
// Else, if rendering a spherical image, then the output image should be twice as wide as it is tall.
// Note that the camera and a light source are already included.
// You can copy the Global Variables section, below, to a new file and then include this file if you want.
//
// CREDITS
// *******
// The sun halo code was adapted from Chris Colefax's Galaxy include.
//
// NOTES/FORMULAS
// **************
// By default, 1AU is roughly 1000 units; the sun's diameter is 10 units; and, the maximum possible planet diameter is 1 unit.
// Mean planetary distances are currently calculated using the Titius-Bode law, which is an approximation of our own solar system.
//
// Ellipse cartesian formula:
//	1 = x^2 / a^2 + y^2 / b^2
// or:
//	1 = x^2 / a^2 + y^2 / (a^2 - c^2)
//	if 0 < a^2 < c^2
// Ellipse parametric formula:
//	x = a * cos(theta)
//	y = b * sin(theta)
// where:
//	0 <= theta <= 2 * pi
//	a is the length of the semi-major axis (mean distance)
//	b is the length of the semi-minor axis
//	c is the distance from the origin to a focus
//	e is the eccentricity
//	b = sqrt(a^2 - c^2)
//	b = sin(acos(c / a)) * a
//	c = sqrt(a^2 - b^2)
//	c = cos(asin(b / a)) * a
//	e = c / a
// Ellipse area formula:
//	A = pi * a * b
//
// Titius-Bode Law:
//	D = pow(1.93, n - 3.15) * 680 + 0.3
// where:
//	n = the number in the list of planets
//	the result is in AUs
//
//
// ISSUES/TO DO
// ************
// The generated planets don't look realistic.
// It may be better to use texture maps (e.g. shiny textures for oceans and rough textures for land masses) in place of color maps.
// Mapping heightfields to spheres would be nice.
// Planetary mean distances need to be randomized.
// Would like to use a hyperbolic function to calculate mean planetary distances, instead of parabolic.
// Should planetary distances be a product of the sun's mass?
// Make it so that the camera is always facing the object, unless CameraAzimuth and CameraAltitude are explicitely set.
// Have to make sure that the number of calls to the random function is balanced across the various options.
// Global absorbtion media blots out the sun at large distances.
// The sun is ugly. The sun is not visible at large distances, while the other stars are.
// The fog conflicts with the Galaxy Include File if it is not the same color as the light source.
// Make elliptical paths for moons, as well.
// Add orbital/rotation velocities, and synchronize them with the clock so that the solar system can be animated.
// Should add a regular orthographic rendering mode.
// Need to fix orthographic tiled mode so that the number of planets doesn't have to be the square of an integer.
// Not sure how to properly scale the sun's halo.
// Would be neat to add man-made objects, like space stations and Dyson spheres.
// Need to rotate comets so that their tails point away from the sun.
// Rings are maybe too dark.
//
//
// GLOBAL VARIABLES
// ****************
// Copy, uncomment and adjust the following variables to modify your generated scene.

/*============================== BEGIN COPY ====================================//

// VERSION NUMBER
#version 3.8;

// INCLUDES
#include "colors.inc"
#include "strings.inc"
#include "math.inc"
#include "rand.inc"
#include "textures.inc"
#include "stones.inc"
#include "transforms.inc"
#include "CIE.inc"				// http://www.ignorancia.org/en/index.php?page=Lightsys
#include "lightsys.inc"			// http://www.ignorancia.org/en/index.php?page=Lightsys

// Set this to equal "true" if Chris Colefax's "GALAXY.INC" and "Lens.inc" exist somewhere within POV-Ray's library path.
#declare SSys_DependenciesExist = 0;	// boolean, 0 = disable; 1 = enable

// ... seed for planets
#declare SSys_S1 = seed(342117);		// float
// ...seed for moons
#declare SSys_S2 = seed(09822334834);		// float
// ...seed for rings
#declare SSys_S3 = seed(12);			// float
// ...seed for camera & lights
#declare SSys_S4 = seed(2212);			// float
// ...seed for starry background
#declare SSys_S5 = seed(212);			// float
// ...seed for asteroid fields
#declare SSys_S6 = seed(212);			// float
// ...seed for lens flares
#declare SSys_S7 = seed(88934);			// float
// ...seed for planet materials
#declare SSys_M1 = seed(88934);			// float
// ...seed for moon materials
#declare SSys_M2 = seed(12);			// float
// ...seed for ring materials
#declare SSys_M3 = seed(88934);			// float
// ...seed for camera & light materials
#declare SSys_M4 = seed(88934);			// float
// ...seed for asteroid field materials
#declare SSys_M6 = seed(88934);			// float

// focus the camera on this planet (or, only render this planet if SSys_RenderMode = 1), counting from bottom to top, starting on the left and going to the right
// set this to zero (0) to focus on the sun (or, render all planets if SSys_RenderMode = 1)
#declare SSys_RenderOnly = 1;			// integer, from 0 to SSys_NumberPlanets
// the method of texturing the planets
#declare SSys_RenderStyle = 0;			// integer, 0 = plain, with high detail maps; 1 = shiny and slow, like glass or metal; 2 = smooth bright pastels, like ice cream
// the method of rendering the scene
#declare SSys_RenderMode = 1;			// integer, 0 = perspective, random orbits; 1 = orthographic, tiled; 2 = spherical, random orbits
// render the starry background? (requires Chris Colefax's Galaxy Include File)
#declare SSys_RenderStarfield = 0;		// boolean, 0 = disable; 1 = enable
// render the galactic background? (requires Chris Colefax's Galaxy Include File) (slow)
#declare SSys_RenderBackground = 0;		// boolean, 0 = disable; 1 = enable
// render the galactic objects? (requires Chris Colefax's Galaxy Include File)
#declare SSys_RenderObjects = 0;		// boolean, 0 = disable; 1 = enable
// render the lens effects? (requires Chris Colefax's Lens Effects Include File)
#declare SSys_RenderLensEffects = 0;	// boolean, 0 = disable; 1 = enable
// should fog be added to the scene? interferes with galaxy include
#declare SSys_RenderFog = 0;			// boolean, 0 = disable; 1 = enable
// surround the sun in a sphere of solar wind; interferes with galaxy include
#declare SSys_RenderSunWind = 1;		// boolean, 0 = disable; 1 = enable
// show a halo around the sun
#declare SSys_RenderSunHalo = 1;		// boolean, 0 = disable; 1 = enable
// render the planets' orbital paths
#declare SSys_RenderOrbits = 0;			// boolean, 0 = disable; 1 = enable
// render a numbered label above each planet
#declare SSys_RenderLabels = on;		// boolean, 0 = disable; 1 = enable
// turn radiosity on/off in the scene
#declare SSys_RenderRadiosity = 1;		// boolean, 0 = disable; 1 = enable

// the number of planetary to generate
#declare SSys_NumberPlanets = 36;		// integer
// the maximum number of moons a planet may have
#declare SSys_NumberMoons = <1, 10>;	// integer
// the maximum number of rings a planet may have
#declare SSys_NumberRings = <1, 10>;	// integer
// the maximum number of unique asteroids to generate and store in memory (these will get reused over and over fill the field)
#declare SSys_NumberAsteroids = 16;		// integer
// the maximum number of galactic objects
#declare SSys_NumberObjects = 16;		// integer
// the statistical chance that a planet has moons
#declare SSys_ChanceMoons = 0.5;		// float, 0 to 1
// the statistical chance that a planet has rings
#declare SSys_ChanceRings = 0.5;		// float, 0 to 1
// the statistical chance that a planet has malformed into an asteroid field (slow)
#declare SSys_ChanceAsteroids = 0.1;	// float, 0 to 1
// the maximum planet radius
#declare SSys_RadiusPlanets = 1;		// float
// the maximum distance of a planet from the center of the sun (not implemented currently)
#declare SSys_DistancePlanets = 1000;	// float
// the maximum distance of a moon from the planet it orbits. note that planets and moons follow circular orbits
#declare SSys_DistanceMoons = 10;		// float
// the maximum density of an asteroid field (asteroids per unit of area, independent of height)
#declare SSys_DensityAsteroids = 0.001;	// float, 0 to 1

// the maximum height of a planet's atmosphere, relative to the planet's radius (recommend you use higher values for orthographic projections)
#declare SSys_AtmosFactor = 1/18;		// float, 0 to 1
// enables scattering-type media for the planet atmospheres (very slow, but more realistic)
#declare SSys_AtmosScatter = 0;			// boolean, 0 = disable; 1 = enable
// strength of the fog
#declare SSys_FogStrength = 1/4;		// float, low values
// the color of the fog
#declare SSys_FogColor = <pow(rand(SSys_S4),1/SSys_FogStrength), pow(rand(SSys_S4),1/SSys_FogStrength), pow(rand(SSys_S4),1/SSys_FogStrength), pow(rand(SSys_S4),SSys_FogStrength)>;	// color vector4, typically low values
// the relative luminence/opacity of the background scene
#declare SSys_GalaxyIntensity = 0.1;	// float, 0 to 1

// the radius of the light source (the sun)
#declare SSys_RadiusSun = 5;			// float
// the radius of the light source halo (the sun corona)
#declare SSys_RadiusHalo = 500;			// float
#declare SSys_Light_Point_Lumens = 2;
#declare SSys_Light_Point_Temp = Daylight(5800);
#declare SSys_Light_Point_Color = Light_Color(SSys_Light_Point_Temp,SSys_Light_Point_Lumens);
#declare SSys_Light_Area_Radius = SSys_RadiusSun;
#declare SSys_Light_Area_Theta_Num = 4;		// was 6
#declare SSys_Light_Area_Phi_Num = 4;		// was 6
#declare SSys_Light_Area_Lumens = SSys_Light_Point_Lumens/SSys_Light_Area_Theta_Num/SSys_Light_Area_Phi_Num;
#declare SSys_Light_Area_Temp = Daylight(5800);
#declare SSys_Light_Area_Color = Light_Color(SSys_Light_Area_Temp,SSys_Light_Area_Lumens);
// the distance of the light source from the center of the scene (only applies if SSys_RenderMode = 1)
#declare SSys_DistanceSun = 100;		// float
// the lattitude and longitude of the light source (only applies if SSys_RenderMode = 1)
#declare SSys_OrientationSun = <-10, -100>;	// vector (degrees)

// the coordinates of the camera (only applies if SSys_RenderMode != 1 and SSys_RenderOnly = 0)
#declare SSys_CameraLocation = <0, 0, 10>;	// vector
// the coordinates that the camera will point toward (only applies if SSys_RenderMode != 1 and SSys_RenderOnly = 0)
#declare SSys_CameraLookAt = <0, 0, 0>;	// vector
// the distance between the camera and the object it is orbiting (only applies if SSys_RenderMode != 1 and SSys_RenderOnly != 0)
#declare SSys_CameraDistance = 2;		// float
// the lattitude and longitude of the camera, with repect to the object it is orbiting (only applies if SSys_RenderMode != 1)
// the camera is pointed along the Z axis, by default.
#declare SSys_CameraPosition = <-15, -120>;	// vector (degrees)
// the altitude and azimuth of the camera (only applies if SSys_RenderMode != 1)
#declare SSys_CameraDirection = <0, 60>;	// vector (degrees)

//================================= END COPY ===================================*/

// DEPENDENCIES
#ifndef (SSys_DependenciesExist) #declare SSys_DependenciesExist = 0; #end	// boolean

// RANDOM SEEDS
#ifndef (SSys_S1) #declare SSys_S1 = seed(56353); #end				// float
#ifndef (SSys_S2) #declare SSys_S2 = seed(26036); #end				// float
#ifndef (SSys_S3) #declare SSys_S3 = seed(35027); #end				// float
#ifndef (SSys_S4) #declare SSys_S4 = seed(44018); #end				// float
#ifndef (SSys_S5) #declare SSys_S5 = seed(00298); #end				// float
#ifndef (SSys_S6) #declare SSys_S6 = seed(56353); #end				// float
#ifndef (SSys_S7) #declare SSys_S7 = seed(56353); #end				// float
#ifndef (SSys_M1) #declare SSys_M1 = seed(80874); #end				// float
#ifndef (SSys_M2) #declare SSys_M2 = seed(62090); #end				// float
#ifndef (SSys_M3) #declare SSys_M3 = seed(71081); #end				// float
#ifndef (SSys_M4) #declare SSys_M4 = seed(71081); #end				// float
#ifndef (SSys_M6) #declare SSys_M6 = seed(71081); #end				// float

// OBJECT PARAMETERS
#ifndef (SSys_NumberPlanets) #declare SSys_NumberPlanets = 9; #end		// integer
#ifndef (SSys_NumberMoons) #declare SSys_NumberMoons = <1, 4>; #end		// integer
#ifndef (SSys_NumberRings) #declare SSys_NumberRings = <1, 4>; #end		// integer
#ifndef (SSys_NumberAsteroids) #declare SSys_NumberAsteroids = 16; #end		// integer
#ifndef (SSys_NumberObjects) #declare SSys_NumberObjects = 16; #end		// integer
#ifndef (SSys_ChanceMoons) #declare SSys_ChanceMoons = 2/3; #end		// float, 0 to 1
#ifndef (SSys_ChanceRings) #declare SSys_ChanceRings = 1/3; #end		// float, 0 to 1
#ifndef (SSys_ChanceAsteroids) #declare SSys_ChanceAsteroids = 0.1; #end	// float, 0 to 1
#ifndef (SSys_RadiusPlanets) #declare SSys_RadiusPlanets = 1; #end		// float
#ifndef (SSys_DistancePlanets) #declare SSys_DistancePlanets = 1000; #end	// float
#ifndef (SSys_DistanceMoons) #declare SSys_DistanceMoons = 10; #end		// float
#ifndef (SSys_DensityAsteroids) #declare SSys_DensityAsteroids = 0.0001; #end	// float

// RENDER OPTIONS
#ifndef (SSys_RenderOnly) #declare SSys_RenderOnly = 0; #end			// integer
#ifndef (SSys_RenderStyle) #declare SSys_RenderStyle = 0; #end			// integer
#ifndef (SSys_RenderMode) #declare SSys_RenderMode = 1; #end			// integer
#ifndef (SSys_RenderStarfield) #declare SSys_RenderStarfield = 0; #end		// boolean
#ifndef (SSys_RenderBackground) #declare SSys_RenderBackground = 0; #end	// boolean
#ifndef (SSys_RenderObjects) #declare SSys_RenderObjects = 0; #end		// boolean
#ifndef (SSys_RenderLensEffects) #declare SSys_RenderLensEffects = 0; #end	// boolean
#ifndef (SSys_RenderFog) #declare SSys_RenderFog = 0; #end			// boolean
#ifndef (SSys_RenderSunWind) #declare SSys_RenderSunWind = 0; #end			// boolean
#ifndef (SSys_RenderSunHalo) #declare SSys_RenderSunHalo = 0; #end		// boolean
#ifndef (SSys_RenderOrbits) #declare SSys_RenderOrbits = 0; #end		// boolean
#ifndef (SSys_RenderLabels) #declare SSys_RenderLabels = 0; #end		// boolean
#ifndef (SSys_RenderRadiosity) #declare SSys_RenderRadiosity = 1; #end	// boolean

// CAMERA MODIFIERS
#ifndef (SSys_CameraLookAt) #declare SSys_CameraLookAt = <0, 0, 0>; #end	// vector
#ifndef (SSys_CameraLocation) #declare SSys_CameraLocation = <0, 0, SSys_DistancePlanets / 2>; #end		// vector
#ifndef (SSys_CameraDistance) #declare SSys_CameraDistance = 2; #end		// float
#ifndef (SSys_CameraDirection) #declare SSys_CameraDirection = <0, 0>; #end	// float (degrees)
#ifndef (SSys_CameraPosition) #declare SSys_CameraPosition = <0, 0>; #end	// float (degrees)

// GASSY STUFF
#ifndef (SSys_FogStrength) #declare SSys_FogStrength = 1/4; #end	// float
#ifndef (SSys_FogColor) #declare SSys_FogColor = 0.5; #end			// color vector
#ifndef (SSys_AtmosFactor) #declare SSys_AtmosFactor = 1/16; #end		// float, 0 to 1
#ifndef (SSys_AtmosScatter) #declare SSys_AtmosScatter = 0; #end		// boolean
#ifndef (SSys_GalaxyIntensity) #declare SSys_GalaxyIntensity = 0.1; #end		// float, 0 to 1

// LIGHT SOURCE MODIFIERS
#ifndef (SSys_RadiusSun) #declare SSys_RadiusSun = 5;	#end		// float
#ifndef (SSys_RadiusHalo) #declare SSys_RadiusHalo = 500;	#end			// float
#ifndef (SSys_DistanceSun) #declare SSys_DistanceSun = 100; #end		// float
#ifndef (SSys_OrientationSun) #declare SSys_OrientationSun = <-10, 80>; #end	// float (degrees)
#switch (SSys_RenderMode)
	#case (1)
		#ifndef (SSys_LightLocation) #declare SSys_LightLocation = vrotate(<0, 0, SSys_DistanceSun>, <SSys_OrientationSun.x, SSys_OrientationSun.y, 0>); #end			// vector3
	#break
	#case (0)
	#case (2)
	#case (3)
		#ifndef (SSys_LightLocation) #declare SSys_LightLocation = <0, 0, 0>; #end			// vector3
	#break
#end
#ifndef (SSys_Light_Point_Lumens) #declare SSys_Light_Point_Lumens = 2; #end
#ifndef (SSys_Light_Point_Temp) #declare SSys_Light_Point_Temp = Daylight(5800); #end
#ifndef (SSys_Light_Point_Color) #declare SSys_Light_Point_Color = Light_Color(SSys_Light_Point_Temp,SSys_Light_Point_Lumens); #end
#ifndef (SSys_Light_Area_Radius) #declare SSys_Light_Area_Radius = SSys_RadiusSun; #end
#ifndef (SSys_Light_Area_Theta_Num) #declare SSys_Light_Area_Theta_Num = 4; #end		// was 6
#ifndef (SSys_Light_Area_Phi_Num) #declare SSys_Light_Area_Phi_Num = 4; #end		// was 6
#ifndef (SSys_Light_Area_Lumens) #declare SSys_Light_Area_Lumens = SSys_Light_Point_Lumens/SSys_Light_Area_Theta_Num/SSys_Light_Area_Phi_Num; #end
#ifndef (SSys_Light_Area_Temp) #declare SSys_Light_Area_Temp = Daylight(5800); #end
#ifndef (SSys_Light_Area_Color) #declare SSys_Light_Area_Color = Light_Color(SSys_Light_Area_Temp,SSys_Light_Area_Lumens); #end

// TEXTURE MODIFIERS, FORMAT IS <MIN, MAX>
#ifndef (SSys_TexScale) #declare SSys_TexScale = <0, 2>; #end			// float, 0 to 1, default 1
#ifndef (SSys_TexNormHeight) #declare SSys_TexNormHeight = <0, 2>; #end	// float, any, default 0
#ifndef (SSys_TexAmbient) #declare SSys_TexAmbient = <0, 0.5>; #end		// float, 0 to 1, default 1
#ifndef (SSys_TexDiffuse) #declare SSys_TexDiffuse = <0, 1>; #end		// float, 0 to 1, default 0.6
#ifndef (SSys_TexCrand) #declare SSys_TexCrand = <0, 1>; #end			// float, typically 0.01 to 0.5, default 0
#ifndef (SSys_TexBrilliance) #declare SSys_TexBrilliance = <0, 1>; #end	// float, any, default 1
#ifndef (SSys_TexReflection) #declare SSys_TexReflection = <0, 1>; #end	// float, 0 to 1, default 0
#ifndef (SSys_TexSpecular) #declare SSys_TexSpecular = <0, 1>; #end		// float, typically 0 to 1, default 0
#ifndef (SSys_TexRoughness) #declare SSys_TexRoughness = <0.001, 1>; #end	// float, typically 1 to almost 0, default 0.05
#ifndef (SSys_TexPhong) #declare SSys_TexPhong = <0, 1>; #end			// float, typically 0 to 1, default 0
#ifndef (SSys_TexPhongSize) #declare SSys_TexPhongSize = <1, 250>; #end	// float, typically 1 to 250, default 40
#ifndef (SSys_TexMetallic) #declare SSys_TexMetallic = <0, 5>; #end		// float, any, default 0
#ifndef (SSys_TexFrequency) #declare SSys_TexFrequency = <0, 1>; #end		// integer, any, default 1
#ifndef (SSys_TexWarpNumber) #declare SSys_TexWarpNumber = <0, 5>; #end	// integer, any, default 5
#ifndef (SSys_TexWarpFalloff) #declare SSys_TexWarpFalloff = <1, 5>; #end	// float, low values greater than 1, default 2
#ifndef (SSys_TexWarpStrength) #declare SSys_TexWarpStrength = <0, 5>; #end	// float, low values, default 1
#ifndef (SSys_TexPhase) #declare SSys_TexPhase = <0, 0>; #end			// float, 0 to 1, default 0
#ifndef (SSys_TexTurbulence) #declare SSys_TexTurbulence = <1, 1, 1>; #end	// vector, typically 0 to 1 or more, default 0
#ifndef (SSys_TexOctaves) #declare SSys_TexOctaves = <1, 1>; #end		// integer, 1 to 9, default 6
#ifndef (SSys_TexLambda) #declare SSys_TexLambda = <0, 1>; #end		// float, typically low values, default 2
#ifndef (SSys_TexOmega) #declare SSys_TexOmega = <0, 1>; #end			// float, default 0.5
#ifndef (SSys_TexIrid) #declare SSys_TexIrid = <0, 0.5>; #end			// float, typically 0.25 or less or 0.1 to 0.5, default ?
#ifndef (SSys_TexIridThick) #declare SSys_TexIridThick = <0, 1>; #end		// float, typically 0.25 to 1, default 0
#ifndef (SSys_TexIridTurb) #declare SSys_TexIridTurb = <0, 1>; #end		// float, typically 0.25 to 1, default ?

// INTERIOR MEDIA MODIFIERS, FORMAT IS <MIN, MAX>
#ifndef (SSys_MedIor) #declare SSys_MedIor = <1, 1>; #end			// float, typically 5 or less, default 1
#ifndef (SSys_MedFadePower) #declare SSys_MedFadePower = <1, 2>; #end		// float, typically 1 or 2, default 0
#ifndef (SSys_MedDispersion) #declare SSys_MedDispersion = <1.01, 1.1>; #end	// float, typically 1.01 to 1.1
#ifndef (SSys_MedDispSamples) #declare SSys_MedDispSamples = <7, 7>; #end	// integer, minimum 2, default 7
#ifndef (SSys_MedNormDepth) #declare SSys_MedNormDepth = <0, 1>; #end		// float, typically 0 to 1, default 0.5
#ifndef (SSys_MedEmission) #declare SSys_MedEmission = <0, 1>; #end		// float, 0 to 1
#ifndef (SSys_MedAbsorbtion) #declare SSys_MedAbsorbtion = <0, 1>; #end	// float, 0 to 1
#ifndef (SSys_MedExtinction) #declare SSys_MedExtinction = <0, 1>; #end	// float, typically 1, default 1

// LOCAL VARIABLES
#local SSys_NumberSqrt = floor(sqrt(SSys_NumberPlanets) + 0.5);
#local SSys_DistanceRoot = pow(SSys_DistancePlanets - SSys_RadiusSun, 1/SSys_NumberPlanets);
#local SSys_CameraIsSet = 0;

// MACROS
#macro VRand4D(Stream)
	<rand(Stream), rand(Stream), rand(Stream), rand(Stream)>
#end
#macro RandSign(Stream)
	#if (rand(Stream) < 0.5)
		#local TempVar = -1;
	#else
		#local TempVar = 1;
	#end
	TempVar
#end

// ARRAYS
#local SSys_PatTotal = 18;	// 7 + 11
#local SSys_PatArray = array[SSys_PatTotal]
{
	"bozo", "marble", "agate", "granite", "leopard", "spotted", "wood",
	"crackle", "gradient y", "mandel 50", "julia <rand(SSys_M1), rand(SSys_M1)>, 50", "ripples", "waves", "wrinkles", "magnet 1 mandel 50", "magnet 2 mandel 50", "magnet 1 julia <rand(SSys_M1), rand(SSys_M1)>, 50", "magnet 2 julia <rand(SSys_M1), rand(SSys_M1)>, 50",
}
#local SSys_TexTotal = 30;
#local SSys_TexArray = array[SSys_TexTotal]
{
	"T_Grnt0", "T_Grnt1", "T_Grnt2", "T_Grnt3", "T_Grnt4", "T_Grnt5", "T_Grnt6", "T_Grnt7", "T_Grnt8", "T_Grnt9", "T_Grnt10", "T_Grnt11", "T_Grnt12", "T_Grnt13", "T_Grnt14", "T_Grnt15", "T_Grnt16", "T_Grnt17", "T_Grnt18", "T_Grnt19", "T_Grnt20", "T_Grnt21", "T_Grnt22", "T_Grnt23", "T_Grnt24", "T_Grnt25", "T_Grnt26", "T_Grnt27", "T_Grnt28", "T_Grnt29", 
}
#local SSys_PlanetsPos = array[SSys_NumberPlanets]


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// MISC GRAPHICAL SETTINGS

global_settings
{
	#if (SSys_RenderRadiosity = 1)
		ambient_light	0
		radiosity
		{
			pretrace_start	0.08
			pretrace_end	0.02
			count			20
			error_bound		1
			recursion_limit	1
			normal			on
			brightness		0.001
			always_sample	no
			gray_threshold	0.8
			media			on
		}
	#else
		ambient_light	0.001
	#end
	charset			utf8
	assumed_gamma	1
	max_trace_level	8		// default is 5
}

background {color srgb 0}

sky_sphere
{
	pigment
	{
		bozo
		color_map
		{
			[0.0 color srgbt <3,3,3,0>]
			[0.2 color srgbt <0,0,0,1>]
			[1.0 color srgbt <0,0,0,1>]
		}
		scale 0.001
	}
}


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// OBJECT GENERATION

#local SSys_PlanetsCnt = 0;
#local SSys_XPosCnt = 0;
#while (SSys_XPosCnt < SSys_NumberPlanets)
	#local SSys_YPosCnt = 0;
	#while (SSys_YPosCnt < SSys_NumberSqrt)

        //------4-------6-------8-------10------12------14------16------18------20------22------
		// LOCAL VARIABLES
		#local PlanetRadius = rand(SSys_S1) * 1/2;
		#local AtmosRadius = rand(SSys_S1) * SSys_AtmosFactor * PlanetRadius + PlanetRadius;
		#local HazeRadius = rand(SSys_S1) * SSys_AtmosFactor * AtmosRadius + AtmosRadius;
		#local HoleRadius = rand(SSys_S1) * 6/36 + 2/36 + HazeRadius;	
		#local RingRadius = rand(SSys_S3) * 9/36 + HoleRadius;
		#local PlanetString = SSys_PatArray[floor(rand(SSys_M1) * (SSys_PatTotal - 1) + 0.5)];
		#local AtmosString = SSys_PatArray[floor(rand(SSys_M1) * (SSys_PatTotal - 1) + 0.5)];
		#local AsteroidString = SSys_TexArray[floor(rand(SSys_M6) * (SSys_TexTotal - 1) + 0.5)];
		#local MeanDistance = pow(1.93, SSys_PlanetsCnt - 3.15) * 680 + 0.3;
		#local Eccentricity = Rand_Exp(40/3, SSys_S1);	// 40/3 or 20
		#local FocalDistance = MeanDistance * Eccentricity;
		#local SemiMinorAxis = MeanDistance * sin(acos(Eccentricity));
		#local Obliquity = Rand_Exp(0.02, SSys_S1);
		#local Inclination = Rand_Exp(0.2, SSys_S1);
		#local Direction = rand(SSys_S1) * 360;
		#local AxisRotation = <Inclination, Direction, 0>;
		#local Theta = rand(SSys_S1) * 2 * pi;
		#local CoordX = cos(Theta) * MeanDistance - FocalDistance;
		#local CoordZ = sin(Theta) * SemiMinorAxis;
		#local PlanetPosition = vrotate(<CoordX, 0, CoordZ>, AxisRotation);
		#local SSys_PlanetsPos[SSys_PlanetsCnt] = PlanetPosition;
		#local SSys_PlanetsCnt = SSys_PlanetsCnt + 1;
		#debug concat("\n//Debug - Number: ", str(SSys_PlanetsCnt, 0, 0))
		#debug concat("\n//Debug - Mean Distance: ", str(MeanDistance, 0, -1))

        //------4-------6-------8-------10------12------14------16------18------20------22------
		// ORBITAL PATHS
		#local OrbitThickness = MeanDistance * 0.01;
		#local OrbitPath = intersection
		{
			difference
			{
				sphere
				{
					0, 1.0
					scale <MeanDistance + OrbitThickness, 1, SemiMinorAxis + OrbitThickness>
				}
				sphere
				{
					0, 1.0
					scale <MeanDistance - OrbitThickness, 1, SemiMinorAxis - OrbitThickness>
				}
			}
			plane
			{
				y, 0
			}
			plane
			{
				-y, 0
			}
			pigment
			{
				rgbt <0, 0, 1, 0.5>
			}
			finish
			{
				emission 1
			}
			hollow
			translate -x * FocalDistance
			rotate AxisRotation
		}

        //------4-------6-------8-------10------12------14------16------18------20------22------
		// PLANETARY SYSTEMS
		#local PlanetSystem = union
		{
			// PLANET SURFACE LAYER
			#local GradeMargin = rand(SSys_M1);
			#local GradeDiff = 1 - GradeMargin;
			#local GradeCount = 0;
			#local SetScale = RRand(SSys_TexScale.x, SSys_TexScale.y, SSys_M1) / 2;
			#switch (SSys_RenderStyle)
				#case (0)
					#local GradeMax = ceil(rand(SSys_M1) * 255);
					#local SetWarpNumber = floor(RRand(SSys_TexWarpNumber.x, SSys_TexWarpNumber.y, SSys_M1) + 0.5);
					#local SetWarpFalloff = RRand(SSys_TexWarpFalloff.x, SSys_TexWarpFalloff.y, SSys_M1);
					#local SetWarpStrength = RRand(SSys_TexWarpStrength.x, SSys_TexWarpStrength.y, SSys_M1);
					#local SetFrequency = ceil(rand(SSys_M1) * 1);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = 6;
					#local SetLambda = 2;
					#local SetOmega = 0.5;
					#local SetNormHeight = 0;
					#local SetAmbient = 0.1;
					#local SetDiffuse = 0.6;
					#local SetCrand = 0;
					#local SetBrilliance = 1;
					#local SetReflection = 0;
					#local SetSpecular = 0;
					#local SetRoughness = 0.05;
					#local SetPhong = 0;
					#local SetPhongSize = 40;
					#local SetMetallic = 0;
					#local SetIrid = 0;
					#local SetIridThick = 0;
					#local SetIridTurb = 0;
				#break
				#case (1)
					#local GradeMax = ceil(rand(SSys_M1) * 2);
					#local SetWarpNumber = floor(RRand(SSys_TexWarpNumber.x, SSys_TexWarpNumber.y, SSys_M1) + 0.5);
					#local SetWarpFalloff = RRand(SSys_TexWarpFalloff.x, SSys_TexWarpFalloff.y, SSys_M1);
					#local SetWarpStrength = RRand(SSys_TexWarpStrength.x, SSys_TexWarpStrength.y, SSys_M1);
					#local SetFrequency = ceil(rand(SSys_M1) * 5);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = floor(RRand(SSys_TexOctaves.x, SSys_TexOctaves.y, SSys_M1) + 0.5);
					#local SetLambda = RRand(SSys_TexLambda.x, SSys_TexLambda.y, SSys_M1);
					#local SetOmega = RRand(SSys_TexOmega.x, SSys_TexOmega.y, SSys_M1);
					#local SetNormHeight = RRand(SSys_TexNormHeight.x, SSys_TexNormHeight.y, SSys_M1);
					#local SetAmbient = RRand(SSys_TexAmbient.x, SSys_TexAmbient.y, SSys_M1);
					#local SetDiffuse = RRand(SSys_TexDiffuse.x, SSys_TexDiffuse.y, SSys_M1);
					#local SetCrand = RRand(SSys_TexCrand.x, SSys_TexCrand.y, SSys_M1);
					#local SetBrilliance = RRand(SSys_TexBrilliance.x, SSys_TexBrilliance.y, SSys_M1);
					#local SetReflection = RRand(SSys_TexReflection.x, SSys_TexReflection.y, SSys_M1);
					#local SetSpecular = RRand(SSys_TexSpecular.x, SSys_TexSpecular.y, SSys_M1);
					#local SetRoughness = RRand(SSys_TexRoughness.x, SSys_TexRoughness.y, SSys_M1);
					#local SetPhong = RRand(SSys_TexPhong.x, SSys_TexPhong.y, SSys_M1);
					#local SetPhongSize = RRand(SSys_TexPhongSize.x, SSys_TexPhongSize.y, SSys_M1);
					#local SetMetallic = RRand(SSys_TexMetallic.x, SSys_TexMetallic.y, SSys_M1);
					#local SetIrid = RRand(SSys_TexIrid.x, SSys_TexIrid.y, SSys_M1);
					#local SetIridThick = RRand(SSys_TexIridThick.x, SSys_TexIridThick.y, SSys_M1);
					#local SetIridTurb = RRand(SSys_TexIridTurb.x, SSys_TexIridTurb.y, SSys_M1);
				#break
				#case (2)
					#local GradeMax = ceil(rand(SSys_M1) * 5);
					#local SetWarpNumber = floor(RRand(SSys_TexWarpNumber.x, SSys_TexWarpNumber.y, SSys_M1) + 0.5);
					#local SetWarpFalloff = RRand(SSys_TexWarpFalloff.x, SSys_TexWarpFalloff.y, SSys_M1);
					#local SetWarpStrength = RRand(SSys_TexWarpStrength.x, SSys_TexWarpStrength.y, SSys_M1);
					#local SetFrequency = ceil(rand(SSys_M1) * 1);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = 6;
					#local SetLambda = 2;
					#local SetOmega = 0.5;
					#local SetNormHeight = 0;
					#local SetAmbient = 0.1;
					#local SetDiffuse = 0.6;
					#local SetCrand = 0;
					#local SetBrilliance = 1;
					#local SetReflection = 0;
					#local SetSpecular = 0;
					#local SetRoughness = 0.05;
					#local SetPhong = 0;
					#local SetPhongSize = 40;
					#local SetMetallic = 0;
					#local SetIrid = 0;
					#local SetIridThick = 0;
					#local SetIridTurb = 0;
				#break
			#end
			#if (SetWarpNumber > 0)
				#local WarpTable1 = array[SetWarpNumber]
				#local WarpTable2 = array[SetWarpNumber]
			#end

			sphere
			{
				<0, 0, 0>
				PlanetRadius
				material
				{
				  	texture
					{
						pigment
						{
							Parse_String(PlanetString)
							#local WarpCount = 0;
							#while (WarpCount < SetWarpNumber)
								#local WarpTable1[WarpCount] = VRand(SSys_M1);
								#local WarpTable2[WarpCount] = rand(SSys_M1);
								warp
								{
									black_hole vrotate(<0, 0, PlanetRadius>, WarpTable1[WarpCount] * 360), WarpTable2[WarpCount] * PlanetRadius / 10
									falloff SetWarpFalloff
									strength SetWarpStrength
									inverse
								}
								#local WarpCount = WarpCount + 1;
							#end

							frequency SetFrequency
							noise_generator SetNoise
							warp
							{
								turbulence SetTurbulence
								octaves SetOctaves
								omega SetOmega
								lambda SetLambda
							}
							color_map
							{
								#local GradePos = 0;
								#while (GradeCount < GradeMax)
									#switch (SSys_RenderStyle)
										#case (0)
//											#local GradeMargin = rand(SSys_M1);
//											#local GradeDiff = 1 - GradeMargin;
//											#local SetPigColor = VRand(SSys_M1) * GradeMargin / 2 + VRand(SSys_M1) * GradeDiff / 2 + (GradeMargin + GradeDiff) / 2;
											#local SetPigColor = VRand(SSys_M1) * GradeMargin + VRand(SSys_M1) * GradeDiff;
//											#local SetPigColor = VRand(SSys_M1) * GradeMargin / 2 + GradeMargin / 2;
//											#local SetPigColor = VRand(SSys_M1);
											[GradePos color srgbt SetPigColor]
										#break
										#case (1)
											[GradeCount/GradeMax color srgbt VRand(SSys_M1)]
										#break
										#case (2)
											[GradeCount/GradeMax color srgbt VRand(SSys_M1) * GradeMargin + GradeDiff]
										#break
									#end
									#local GradePos = rand(SSys_M1) * (1 - GradePos) + GradePos;
									#local GradeCount = GradeCount + 1;
								#end
							}
						}
						normal
						{
							Parse_String(PlanetString) SetNormHeight
							frequency SetFrequency
							noise_generator SetNoise
							warp
							{
								turbulence SetTurbulence
								octaves SetOctaves
								omega SetOmega
								lambda SetLambda
							}
/*
							#local WarpCount = 0;
							#while (WarpCount < SetWarpNumber)
								warp
								{
									black_hole vrotate(<0, 0, PlanetRadius>, WarpTable1[WarpCount] * 360), WarpTable2[WarpCount] * PlanetRadius / 10
									falloff SetWarpFalloff
									strength SetWarpStrength
									inverse
								}
								#local WarpCount = WarpCount + 1;
							#end
*/
						}
						finish
						{
							ambient SetAmbient
							diffuse SetDiffuse
							crand SetCrand
							brilliance SetBrilliance
							irid
							{
								SetIrid
								thickness SetIridThick
								turbulence SetIridTurb
							}
							reflection
							{
								SetReflection
								fresnel
							}
							specular SetSpecular
							roughness SetRoughness
							phong SetPhong
							phong_size SetPhongSize
							metallic SetMetallic
						}
						scale SetScale
					}
				}
			}

            //--4-------6-------8-------10------12------14------16------18------20------22------
			// PLANET CLOUD LAYER
			#local GradeMargin = rand(SSys_M1);
			#local GradeDiff = 1 - GradeMargin;
			#local GradeCount = 0;
			#local SetScale = RRand(SSys_TexScale.x, SSys_TexScale.y, SSys_M1) / 2;
			#switch (SSys_RenderStyle)
				#case (0)
					#local GradeMax = ceil(rand(SSys_M1) * 255);
					#local SetFrequency = ceil(rand(SSys_M1) * 1);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = 6;
					#local SetLambda = 2;
					#local SetOmega = 0.5;
					#local SetNormHeight = 0;
					#local SetMediaColor = VRand(SSys_M1);
					#local SetIor = RRand(SSys_MedIor.x, SSys_MedIor.y, SSys_M1);
					#local SetFadeDist = AtmosRadius;	// - PlanetRadius;
					#local SetFadePower = RRand(SSys_MedFadePower.x, SSys_MedFadePower.y, SSys_M1);
					#local SetAbsorbtion = RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M1) * 1024 / 100 / SSys_RadiusPlanets;
					#local SetEmission = RRand(SSys_MedEmission.x, SSys_MedEmission.y, SSys_M1) * 1024 / 100 / SSys_RadiusPlanets;
					#local SetScatColor = SetMediaColor * 1024 / 100 / SSys_RadiusPlanets;
					#local SetScatType = floor(RRand(0, 4, SSys_M1) + 0.5);
					#local SetExtinction = RRand(SSys_MedExtinction.x, SSys_MedExtinction.y, SSys_M1);
					#local SetDispersion = RRand(SSys_MedDispersion.x, SSys_MedDispersion.y, SSys_M1);
					#local SetDispSamples = floor(RRand(SSys_MedDispSamples.x, SSys_MedDispSamples.y, SSys_M1) + 0.5);
				#break
				#case (1)
					#local GradeMax = ceil(rand(SSys_M1) * 10);
					#local SetFrequency = ceil(rand(SSys_M1) * 5);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = floor(RRand(SSys_TexOctaves.x, SSys_TexOctaves.y, SSys_M1) + 0.5);
					#local SetLambda = RRand(SSys_TexLambda.x, SSys_TexLambda.y, SSys_M1);
					#local SetOmega = RRand(SSys_TexOmega.x, SSys_TexOmega.y, SSys_M1);
					#local SetNormHeight = RRand(SSys_TexNormHeight.x, SSys_TexNormHeight.y, SSys_M1);
					#local SetMediaColor = VRand(SSys_M1);
					#local SetIor = RRand(SSys_MedIor.x, SSys_MedIor.y, SSys_M1);
					#local SetFadeDist = AtmosRadius;	// - PlanetRadius;
					#local SetFadePower = RRand(SSys_MedFadePower.x, SSys_MedFadePower.y, SSys_M1);
					#local SetAbsorbtion = RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetEmission = RRand(SSys_MedEmission.x, SSys_MedEmission.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetScatColor = SetMediaColor * 768 / 100 / SSys_RadiusPlanets;
					#local SetScatType = floor(RRand(0, 4, SSys_M1) + 0.5);
					#local SetExtinction = RRand(SSys_MedExtinction.x, SSys_MedExtinction.y, SSys_M1);
					#local SetDispersion = RRand(SSys_MedDispersion.x, SSys_MedDispersion.y, SSys_M1);
					#local SetDispSamples = floor(RRand(SSys_MedDispSamples.x, SSys_MedDispSamples.y, SSys_M1) + 0.5);
				#break
				#case (2)
					#local GradeMax = ceil(rand(SSys_M1) * 10);
					#local SetFrequency = ceil(rand(SSys_M1) * 5);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = 6;
					#local SetLambda = 2;
					#local SetOmega = 0.5;
					#local SetNormHeight = 0;
					#local SetMediaColor = VRand(SSys_M1);
					#local SetIor = RRand(SSys_MedIor.x, SSys_MedIor.y, SSys_M1);
					#local SetFadeDist = AtmosRadius;	// - PlanetRadius;
					#local SetFadePower = RRand(SSys_MedFadePower.x, SSys_MedFadePower.y, SSys_M1);
					#local SetAbsorbtion = RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetEmission = RRand(SSys_MedEmission.x, SSys_MedEmission.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetScatColor = SetMediaColor * 768 / 100 / SSys_RadiusPlanets;
					#local SetScatType = floor(RRand(0, 4, SSys_M1) + 0.5);
					#local SetExtinction = RRand(SSys_MedExtinction.x, SSys_MedExtinction.y, SSys_M1);
					#local SetDispersion = RRand(SSys_MedDispersion.x, SSys_MedDispersion.y, SSys_M1);
					#local SetDispSamples = floor(RRand(SSys_MedDispSamples.x, SSys_MedDispSamples.y, SSys_M1) + 0.5);
				#break
			#end

			sphere
			{
				<0, 0, 0>
				AtmosRadius
				hollow
				photons
				{
					target
					refraction on
					reflection on
					collect off
				}
				material
				{
					interior
					{
						ior SetIor
						fade_distance SetFadeDist
						fade_power SetFadePower
						#if (SSys_AtmosScatter = 1)
							media
							{
								scattering
								{
									SetScatType,
									rgbt SetScatColor
									extinction SetExtinction
								}
							}
						#else
							dispersion SetDispersion
							dispersion_samples SetDispSamples
							media
							{
								absorption SetAbsorbtion
								emission SetEmission
								density
								{
									spherical
									density_map
									{
										[0 color rgbt 0]
										[1 color rgbt SetMediaColor]
									}
								}
							}
						#end
					}
				  	texture
					{
						pigment
						{
							Parse_String(AtmosString)
							frequency SetFrequency
							noise_generator SetNoise
							warp
							{
								turbulence SetTurbulence
								octaves SetOctaves
								omega SetOmega
								lambda SetLambda
							}
							color_map
							{
								#local GradePos = 0;
								#while (GradeCount < GradeMax)
									#switch (SSys_RenderStyle)
										#case (0)
//											#local GradeMargin = rand(SSys_M1);
//											#local GradeDiff = 1 - GradeMargin;
//											[GradePos color srgbt VRand(SSys_M1) * GradeMargin / 2 + VRand(SSys_M1) * GradeDiff / 2 + (GradeMargin + GradeDiff) / 2 + <0,0,0,rand(SSys_M1)>]
											[GradePos color srgbt VRand4D(SSys_M1) * GradeMargin + VRand4D(SSys_M1) * GradeDiff + <0,0,0,rand(SSys_M1)>]
//											[GradePos color srgbt VRand(SSys_M1) * GradeMargin / 2 + GradeMargin / 2 + <0,0,0,rand(SSys_M1)>]
//											[GradePos color srgbt VRand4D(SSys_M1)]
										#break
										#case (1)
											[GradeCount/GradeMax color srgbt VRand4D(SSys_M1)]
										#break
										#case (2)
											[GradeCount/GradeMax color srgbt VRand4D(SSys_M1) * GradeMargin + GradeDiff]
										#break
									#end
									#local GradePos = GradePos + rand(SSys_M1) * (1 - GradePos);
									#local GradeCount = GradeCount + 1;
								#end
							}
						}
						finish
						{
							ambient SetAmbient
						}
						scale SetScale
					}
				}
			}

            //--4-------6-------8-------10------12------14------16------18------20------22------
			// PLANET HAZE LAYER
			#local SetScale = RRand(SSys_TexScale.x, SSys_TexScale.y, SSys_M1) / 2;
			#switch (SSys_RenderStyle)
				#case (0)
					#local SetFrequency = ceil(rand(SSys_M1) * 1);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = 6;
					#local SetLambda = 2;
					#local SetOmega = 0.5;
					#local SetNormHeight = 0;
					#local SetAmbient = 0.1;
					#local SetDiffuse = 0.6;
					#local SetCrand = 0;
					#local SetBrilliance = 1;
					#local SetReflection = 0;
					#local SetSpecular = 0;
					#local SetRoughness = 0.05;
					#local SetPhong = 0;
					#local SetPhongSize = 40;
					#local SetMetallic = 0;
					#local SetIrid = 0;
					#local SetIridThick = 0;
					#local SetIridTurb = 0;
					#local SetMediaColor = VRand(SSys_M1);
					#local SetIor = RRand(SSys_MedIor.x, SSys_MedIor.y, SSys_M1);
					#local SetFadeDist = HazeRadius;	// - AtmosRadius;
					#local SetFadePower = RRand(SSys_MedFadePower.x, SSys_MedFadePower.y, SSys_M1);
					#local SetAbsorbtion = RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M1) * 1024 / 100 / SSys_RadiusPlanets;
					#local SetEmission = RRand(SSys_MedEmission.x, SSys_MedEmission.y, SSys_M1) * 1024 / 100 / SSys_RadiusPlanets;
					#local SetScatColor = SetMediaColor * 1024 / 100 / SSys_RadiusPlanets;
					#local SetScatType = floor(RRand(1, 5, SSys_M1) + 0.5);
					#local SetExtinction = RRand(SSys_MedExtinction.x, SSys_MedExtinction.y, SSys_M1);
					#local SetDispersion = RRand(SSys_MedDispersion.x, SSys_MedDispersion.y, SSys_M1);
					#local SetDispSamples = floor(RRand(SSys_MedDispSamples.x, SSys_MedDispSamples.y, SSys_M1) + 0.5);
				#break
				#case (1)
					#local SetFrequency = ceil(rand(SSys_M1) * 5);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = floor(RRand(SSys_TexOctaves.x, SSys_TexOctaves.y, SSys_M1) + 0.5);
					#local SetLambda = RRand(SSys_TexLambda.x, SSys_TexLambda.y, SSys_M1);
					#local SetOmega = RRand(SSys_TexOmega.x, SSys_TexOmega.y, SSys_M1);
					#local SetNormHeight = RRand(SSys_TexNormHeight.x, SSys_TexNormHeight.y, SSys_M1);
					#local SetAmbient = RRand(SSys_TexAmbient.x, SSys_TexAmbient.y, SSys_M1);
					#local SetDiffuse = RRand(SSys_TexDiffuse.x, SSys_TexDiffuse.y, SSys_M1);
					#local SetCrand = RRand(SSys_TexCrand.x, SSys_TexCrand.y, SSys_M1);
					#local SetBrilliance = RRand(SSys_TexBrilliance.x, SSys_TexBrilliance.y, SSys_M1);
					#local SetReflection = RRand(SSys_TexReflection.x, SSys_TexReflection.y, SSys_M1);
					#local SetSpecular = RRand(SSys_TexSpecular.x, SSys_TexSpecular.y, SSys_M1);
					#local SetRoughness = RRand(SSys_TexRoughness.x, SSys_TexRoughness.y, SSys_M1);
					#local SetPhong = RRand(SSys_TexPhong.x, SSys_TexPhong.y, SSys_M1);
					#local SetPhongSize = RRand(SSys_TexPhongSize.x, SSys_TexPhongSize.y, SSys_M1);
					#local SetMetallic = RRand(SSys_TexMetallic.x, SSys_TexMetallic.y, SSys_M1);
					#local SetIrid = RRand(SSys_TexIrid.x, SSys_TexIrid.y, SSys_M1);
					#local SetIridThick = RRand(SSys_TexIridThick.x, SSys_TexIridThick.y, SSys_M1);
					#local SetIridTurb = RRand(SSys_TexIridTurb.x, SSys_TexIridTurb.y, SSys_M1);
					#local SetIor = RRand(SSys_MedIor.x, SSys_MedIor.y, SSys_M1);
					#local SetMediaColor = VRand(SSys_M1);
					#local SetFadeDist = HazeRadius;	// - AtmosRadius;
					#local SetFadePower = RRand(SSys_MedFadePower.x, SSys_MedFadePower.y, SSys_M1);
					#local SetAbsorbtion = RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetEmission = RRand(SSys_MedEmission.x, SSys_MedEmission.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetScatColor = SetMediaColor * 768 / 100 / SSys_RadiusPlanets;
					#local SetScatType = floor(RRand(1, 5, SSys_M1) + 0.5);
					#local SetExtinction = RRand(SSys_MedExtinction.x, SSys_MedExtinction.y, SSys_M1);
					#local SetDispersion = RRand(SSys_MedDispersion.x, SSys_MedDispersion.y, SSys_M1);
					#local SetDispSamples = floor(RRand(SSys_MedDispSamples.x, SSys_MedDispSamples.y, SSys_M1) + 0.5);
				#break
					#local SetFrequency = ceil(rand(SSys_M1) * 5);
					#local SetNoise = ceil(rand(SSys_M1) * 3);
					#local SetTurbulence = rand(SSys_M1) * SSys_TexTurbulence;	//VRand(SSys_M1) * SSys_TexTurbulence;
					#local SetOctaves = 6;
					#local SetLambda = 2;
					#local SetOmega = 0.5;
					#local SetNormHeight = 0;
					#local SetAmbient = 0.1;
					#local SetDiffuse = 0.6;
					#local SetCrand = 0;
					#local SetBrilliance = 1;
					#local SetReflection = 0;
					#local SetSpecular = 0;
					#local SetRoughness = 0.05;
					#local SetPhong = 0;
					#local SetPhongSize = 40;
					#local SetMetallic = 0;
					#local SetIrid = 0;
					#local SetIridThick = 0;
					#local SetIridTurb = 0;
					#local SetMediaColor = VRand(SSys_M1);
					#local SetIor = RRand(SSys_MedIor.x, SSys_MedIor.y, SSys_M1);
					#local SetFadeDist = HazeRadius;	// - AtmosRadius;
					#local SetFadePower = RRand(SSys_MedFadePower.x, SSys_MedFadePower.y, SSys_M1);
					#local SetAbsorbtion = RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetEmission = RRand(SSys_MedEmission.x, SSys_MedEmission.y, SSys_M1) * 768 / 320 / SSys_RadiusPlanets;
					#local SetScatColor = SetMediaColor * 768 / 100 / SSys_RadiusPlanets;
					#local SetScatType = floor(RRand(1, 5, SSys_M1) + 0.5);
					#local SetExtinction = RRand(SSys_MedExtinction.x, SSys_MedExtinction.y, SSys_M1);
					#local SetDispersion = RRand(SSys_MedDispersion.x, SSys_MedDispersion.y, SSys_M1);
					#local SetDispSamples = floor(RRand(SSys_MedDispSamples.x, SSys_MedDispSamples.y, SSys_M1) + 0.5);
				#break
			#end

			sphere
			{
				<0, 0, 0>
				HazeRadius
				hollow
				photons
				{
					target
					refraction on
					reflection on
					collect off
				}
				material
				{
					interior
					{
						ior SetIor
						fade_distance SetFadeDist
						fade_power SetFadePower
						#if (SSys_AtmosScatter = 1)
							media
							{
								scattering
								{
									SetScatType,
									rgbt SetScatColor
									extinction SetExtinction
								}
							}
						#else
							dispersion SetDispersion
							dispersion_samples SetDispSamples
							media
							{
								absorption SetAbsorbtion
								emission SetEmission
								density
								{
									spherical
									density_map
									{
										[0 color rgbt 0]
										[1 color rgbt SetMediaColor]
									}
								}
							}
						#end
					}
				  	texture
					{
						pigment
						{
							transmit 1
						}

						normal
						{
							Parse_String(AtmosString) SetNormHeight
							frequency SetFrequency
							noise_generator SetNoise
							warp
							{
								turbulence SetTurbulence
								octaves SetOctaves
								omega SetOmega
								lambda SetLambda
							}
						}

						finish
						{
							ambient SetAmbient
							diffuse SetDiffuse
//							crand SetCrand
							brilliance SetBrilliance
							irid
							{
								SetIrid
								thickness SetIridThick
								turbulence SetIridTurb
							}
							reflection
							{
								SetReflection
								fresnel
							}
							specular SetSpecular
							roughness SetRoughness
							phong SetPhong
							phong_size SetPhongSize
							metallic SetMetallic
						}
						scale SetScale
					}
				}
			}

            //--4-------6-------8-------10------12------14------16------18------20------22------
			// RINGS
			#local OuterRadius = RingRadius;
			#local InnerRadius = HoleRadius;
			#local CountRings = 0;
			#local RollRings = rand(SSys_S3);
			#if (RollRings <= SSys_ChanceRings)
				#local TotalRings = floor(RRand(SSys_NumberRings.x, SSys_NumberRings.y, SSys_S3) + 0.5);
			#else
				#local TotalRings = 0;
			#end
			#debug concat("\n//Debug - Total Rings: ", str(TotalRings, 0, 0))
			#while (CountRings < TotalRings)
				#local OuterRadius = RRand(InnerRadius, RingRadius, SSys_S3);
				#local GradeMargin = sqrt(rand(SSys_M3));
				#local GradeDiff = 1 - GradeMargin;
				#local GradeCount = 0;
				#local SetScale = RingRadius - HoleRadius;
				#local SetPhase = 1 - (RingRadius / (RingRadius - HoleRadius));
				#switch (SSys_RenderStyle)
					#case (0)
						#local GradeMax = ceil(rand(SSys_M3) * 255);
						#local SetFrequency = 1;
						#local SetNoise = ceil(rand(SSys_M3) * 3);
						#local SetTurbulence = rand(SSys_M3) * 0.001;	//VRand(SSys_M3) * SSys_TexTurbulence;
						#local SetOctaves = 6;
						#local SetLambda = 2;
						#local SetOmega = 0.5;
						#local SetNormHeight = 0;
						#local SetAmbient = 0.1;
						#local SetDiffuse = 0.6;
						#local SetCrand = 0;
						#local SetBrilliance = 1;
						#local SetReflection = 0;
						#local SetSpecular = 0;
						#local SetRoughness = 0.05;
						#local SetPhong = 0;
						#local SetPhongSize = 40;
						#local SetMetallic = 0;
						#local SetIrid = 0;
						#local SetIridThick = 0;
						#local SetIridTurb = 0;
					#break
					#case (1)
						#local GradeMax = ceil(rand(SSys_M3) * 5);
						#local SetFrequency = 1;
						#local SetNoise = ceil(rand(SSys_M3) * 3);
						#local SetTurbulence = rand(SSys_M3) * 0.001;	//VRand(SSys_M3) * SSys_TexTurbulence;
						#local SetOctaves = floor(RRand(SSys_TexOctaves.x, SSys_TexOctaves.y, SSys_M3) + 0.5);
						#local SetLambda = RRand(SSys_TexLambda.x, SSys_TexLambda.y, SSys_M3);
						#local SetOmega = RRand(SSys_TexOmega.x, SSys_TexOmega.y, SSys_M3);
						#local SetNormHeight = RRand(SSys_TexNormHeight.x, SSys_TexNormHeight.y, SSys_M3);
						#local SetAmbient = RRand(SSys_TexAmbient.x, SSys_TexAmbient.y, SSys_M3);
						#local SetDiffuse = RRand(SSys_TexDiffuse.x, SSys_TexDiffuse.y, SSys_M3);
						#local SetCrand = RRand(SSys_TexCrand.x, SSys_TexCrand.y, SSys_M3);
						#local SetBrilliance = RRand(SSys_TexBrilliance.x, SSys_TexBrilliance.y, SSys_M3);
						#local SetReflection = RRand(SSys_TexReflection.x, SSys_TexReflection.y, SSys_M3);
						#local SetSpecular = RRand(SSys_TexSpecular.x, SSys_TexSpecular.y, SSys_M3);
						#local SetRoughness = RRand(SSys_TexRoughness.x, SSys_TexRoughness.y, SSys_M3);
						#local SetPhong = RRand(SSys_TexPhong.x, SSys_TexPhong.y, SSys_M3);
						#local SetPhongSize = RRand(SSys_TexPhongSize.x, SSys_TexPhongSize.y, SSys_M3);
						#local SetMetallic = RRand(SSys_TexMetallic.x, SSys_TexMetallic.y, SSys_M3);
						#local SetIrid = RRand(SSys_TexIrid.x, SSys_TexIrid.y, SSys_M3);
						#local SetIridThick = RRand(SSys_TexIridThick.x, SSys_TexIridThick.y, SSys_M3);
						#local SetIridTurb = RRand(SSys_TexIridTurb.x, SSys_TexIridTurb.y, SSys_M3);
					#break
						#local GradeMax = ceil(rand(SSys_M3) * 5);
						#local SetFrequency = 1;
						#local SetNoise = ceil(rand(SSys_M3) * 3);
						#local SetTurbulence = rand(SSys_M3) * 0.001;	//VRand(SSys_M3) * SSys_TexTurbulence;
						#local SetOctaves = 6;
						#local SetLambda = 2;
						#local SetOmega = 0.5;
						#local SetNormHeight = 0;
						#local SetAmbient = 0.1;
						#local SetDiffuse = 0.6;
						#local SetCrand = 0;
						#local SetBrilliance = 1;
						#local SetReflection = 0;
						#local SetSpecular = 0;
						#local SetRoughness = 0.05;
						#local SetPhong = 0;
						#local SetPhongSize = 40;
						#local SetMetallic = 0;
						#local SetIrid = 0;
						#local SetIridThick = 0;
						#local SetIridTurb = 0;
					#break
				#end

				disc
				{
					<0, 0, 0>,
					y,
					OuterRadius,
					InnerRadius
					material
					{
						texture
						{
							pigment
							{
								onion
								frequency SetFrequency
								warp
								{
									turbulence SetTurbulence
									octaves SetOctaves
									omega SetOmega
									lambda SetLambda
								}
								color_map
								{
									#local GradePos = 0;
									#while (GradeCount < GradeMax)
										#switch (SSys_RenderStyle)
											#case (0)
												#local GradeMargin = rand(SSys_M2);
												#local GradeDiff = (1 - GradeMargin) / 12;
												[GradePos color srgbt VRand4D(SSys_M3) * GradeDiff + GradeMargin]
//												[GradePos color srgbt VRand4D(SSys_M3) * GradeMargin + VRand(SSys_M3) * GradeDiff + <0,0,0,rand(SSys_M3)>]
//												[GradePos color srgbt VRand4D(SSys_M3)]
											#break
											#case (1)
												[GradePos color srgbt VRand4D(SSys_M3)]
											#break
											#case (2)
												[GradePos color srgbt VRand4D(SSys_M3) * GradeMargin + GradeDiff + <0,0,0,rand(SSys_M3)>]
											#break
										#end
										#local GradePos = rand(SSys_M3) * (1 - GradePos) + GradePos;
										#local GradeCount = GradeCount + 1;
									#end
								}
								scale SetScale
								phase SetPhase
							}
							normal
							{
								onion
								frequency SetFrequency
								warp
								{
									turbulence SetTurbulence
									octaves SetOctaves
									omega SetOmega
									lambda SetLambda
								}
								scale SetScale
								phase SetPhase
							}
							finish
							{
								ambient SetAmbient
								diffuse SetDiffuse
								crand SetCrand
								brilliance SetBrilliance
								irid
								{
									SetIrid
									thickness SetIridThick
									turbulence SetIridTurb
								}
								reflection
								{
									SetReflection
									fresnel
								}
								specular SetSpecular
								roughness SetRoughness
								phong SetPhong
								phong_size SetPhongSize
								metallic
							}
						}
					}
				}
				#local InnerRadius = RRand(OuterRadius, RingRadius, SSys_S3);
				#local CountRings = CountRings + 1;
			#end

            //--4-------6-------8-------10------12------14------16------18------20------22------
			// MOONS
			#local RollMoons = rand(SSys_S2);
			#if (RollMoons <= SSys_ChanceMoons)
				#local TotalMoons = floor(RRand(SSys_NumberMoons.x, SSys_NumberMoons.y, SSys_S2) + 0.5);
			#else
				#local TotalMoons = 0;
			#end
			#debug concat("\n//Debug - Total Moons: ", str(TotalMoons, 0, 0))
			#for (CountMoons, 1, TotalMoons)
				#local MoonRadius = rand(SSys_S2) * PlanetRadius / 4;
				#local MoonPosition = vrotate(<0, 0, rand(SSys_S2) * (1 - PlanetRadius - MoonRadius) + (PlanetRadius + MoonRadius)>, <rand(SSys_S2) * 20 - 10, rand(SSys_S2) * 360, 0>);
				#local MoonString = SSys_PatArray[floor(rand(SSys_M2) * (SSys_PatTotal - 1) + 0.5)];
				#local GradeMargin = rand(SSys_M2);
				#local GradeDiff = (1 - GradeMargin) / 12;
				#local GradeCount = 0;
				#local SetScale = RRand(SSys_TexScale.x, SSys_TexScale.y, SSys_M2) / 2;
				#switch (SSys_RenderStyle)
					#case (0)
						#local GradeMax = ceil(rand(SSys_M2) * 255);
						#local SetFrequency = 1;
						#local SetNoise = ceil(rand(SSys_M2) * 3);
						#local SetTurbulence = rand(SSys_M2) * 0.001;	//VRand(SSys_M3) * SSys_TexTurbulence;
						#local SetOctaves = 6;
						#local SetLambda = 2;
						#local SetOmega = 0.5;
						#local SetNormHeight = 0;
						#local SetAmbient = 0.1;
						#local SetDiffuse = 0.6;
						#local SetCrand = 0;
						#local SetBrilliance = 1;
						#local SetReflection = 0;
						#local SetSpecular = 0;
						#local SetRoughness = 0.05;
						#local SetPhong = 0;
						#local SetPhongSize = 40;
						#local SetMetallic = 0;
						#local SetIrid = 0;
						#local SetIridThick = 0;
						#local SetIridTurb = 0;
					#break
					#case (1)
						#local GradeMax = ceil(rand(SSys_M2) * 5);
						#local SetFrequency = 1;
						#local SetNoise = ceil(rand(SSys_M2) * 3);
						#local SetTurbulence = rand(SSys_M2) * 0.001;	//VRand(SSys_M2) * SSys_TexTurbulence;
						#local SetOctaves = floor(RRand(SSys_TexOctaves.x, SSys_TexOctaves.y, SSys_M2) + 0.5);
						#local SetLambda = RRand(SSys_TexLambda.x, SSys_TexLambda.y, SSys_M2);
						#local SetOmega = RRand(SSys_TexOmega.x, SSys_TexOmega.y, SSys_M2);
						#local SetNormHeight = RRand(SSys_TexNormHeight.x, SSys_TexNormHeight.y, SSys_M2);
						#local SetAmbient = RRand(SSys_TexAmbient.x, SSys_TexAmbient.y, SSys_M2);
						#local SetDiffuse = RRand(SSys_TexDiffuse.x, SSys_TexDiffuse.y, SSys_M2);
						#local SetCrand = RRand(SSys_TexCrand.x, SSys_TexCrand.y, SSys_M2);
						#local SetBrilliance = RRand(SSys_TexBrilliance.x, SSys_TexBrilliance.y, SSys_M2);
						#local SetReflection = RRand(SSys_TexReflection.x, SSys_TexReflection.y, SSys_M2);
						#local SetSpecular = RRand(SSys_TexSpecular.x, SSys_TexSpecular.y, SSys_M2);
						#local SetRoughness = RRand(SSys_TexRoughness.x, SSys_TexRoughness.y, SSys_M2);
						#local SetPhong = RRand(SSys_TexPhong.x, SSys_TexPhong.y, SSys_M2);
						#local SetPhongSize = RRand(SSys_TexPhongSize.x, SSys_TexPhongSize.y, SSys_M2);
						#local SetMetallic = RRand(SSys_TexMetallic.x, SSys_TexMetallic.y, SSys_M2);
						#local SetIrid = RRand(SSys_TexIrid.x, SSys_TexIrid.y, SSys_M2);
						#local SetIridThick = RRand(SSys_TexIridThick.x, SSys_TexIridThick.y, SSys_M2);
						#local SetIridTurb = RRand(SSys_TexIridTurb.x, SSys_TexIridTurb.y, SSys_M2);
					#break
						#local GradeMax = ceil(rand(SSys_M2) * 5);
						#local SetFrequency = 1;
						#local SetNoise = ceil(rand(SSys_M2) * 3);
						#local SetTurbulence = rand(SSys_M2) * 0.001;	//VRand(SSys_M3) * SSys_TexTurbulence;
						#local SetOctaves = 6;
						#local SetLambda = 2;
						#local SetOmega = 0.5;
						#local SetNormHeight = 0;
						#local SetAmbient = 0.1;
						#local SetDiffuse = 0.6;
						#local SetCrand = 0;
						#local SetBrilliance = 1;
						#local SetReflection = 0;
						#local SetSpecular = 0;
						#local SetRoughness = 0.05;
						#local SetPhong = 0;
						#local SetPhongSize = 40;
						#local SetMetallic = 0;
						#local SetIrid = 0;
						#local SetIridThick = 0;
						#local SetIridTurb = 0;
					#break
				#end

				sphere
				{
					<0, 0, 0>
					MoonRadius
					material
					{
					  	texture
						{
							pigment
							{
								Parse_String(MoonString)
								frequency SetFrequency
								noise_generator SetNoise
								warp
								{
									turbulence SetTurbulence
									octaves SetOctaves
									omega SetOmega
									lambda SetLambda
								}
								color_map
								{
									#local GradePos = 0;
									#while (GradeCount < GradeMax)
										#switch (SSys_RenderStyle)
											#case (0)
												#local GradeMargin = rand(SSys_M2);
												#local GradeDiff = (1 - GradeMargin) / 12;
//												[GradePos color srgbt VRand(SSys_M2) * GradeMargin / 2 + VRand(SSys_M2) * GradeDiff / 2 + (GradeMargin + GradeDiff) / 2]
												[GradePos color srgbt VRand(SSys_M2) * GradeDiff + GradeMargin]
//												[GradePos color srgbt VRand(SSys_M2)]
											#break
											#case (1)
												[GradeCount/GradeMax color srgbt VRand(SSys_M2) * GradeMargin + VRand(SSys_M2) * GradeDiff]
											#break
											#case (2)
												[GradePos color srgbt VRand(SSys_M2) * GradeDiff + GradeMargin]
											#break
										#end
										#local GradePos = rand(SSys_M2) * (1 - GradePos) + GradePos;
										#local GradeCount = GradeCount + 1;
									#end
								}
							}
							normal
							{
								dents SetNormHeight
							}
							finish
							{
								ambient SetAmbient
								diffuse SetDiffuse
								crand SetCrand
								brilliance SetBrilliance
								irid
								{
									SetIrid
									thickness SetIridThick
									turbulence SetIridTurb
								}
								reflection
								{
									SetReflection
									fresnel
								}
								specular SetSpecular
								roughness SetRoughness
								phong SetPhong
								phong_size SetPhongSize
								metallic SetMetallic
							}
							scale SetScale
						}
					}
					#if (SSys_RenderMode = 1)
						translate MoonPosition
					#else
						translate MoonPosition * SSys_DistanceMoons
					#end
				}
			#end
			rotate <rand(SSys_S1) * 180 - 90, rand(SSys_S1) * 360 - 180, 0>
		}

        //------4-------6-------8-------10------12------14------16------18------20------22------
		// ASTEROID FIELDS
		#local OuterEdgeX = MeanDistance * 1.01;
		#local InnerEdgeX = MeanDistance * 0.99;
		#local OuterEdgeZ = SemiMinorAxis * 1.01;
		#local InnerEdgeZ = SemiMinorAxis * 0.99;
		#local OuterArea = pi * OuterEdgeX * OuterEdgeZ;
		#local InnerArea = pi * InnerEdgeX * InnerEdgeZ;
		#local CountAsteroids = 0;
		#local RollAsteroids = rand(SSys_S1);
		#if (RollAsteroids <= SSys_ChanceAsteroids)
			#if (SSys_RenderMode = 1)
				#local TotalAsteroids = 64;
			#else
				#local TotalAsteroids = (OuterArea - InnerArea) * SSys_DensityAsteroids;
			#end
		#else
			#local TotalAsteroids = 0;
		#end
		#debug concat("\n//Debug - Total Asteroids: ", str(TotalAsteroids, 0, 0))
		#local AsteroidsTable = array[SSys_NumberAsteroids]
		#while (CountAsteroids < SSys_NumberAsteroids)
			#local AsteroidsTable[CountAsteroids] = blob
			{
				threshold rand(SSys_S6)
				#local CountFields = 0;
				#local MaxFields = ceil(rand(SSys_S6) * 10);
				#while (CountFields < MaxFields)
					sphere { vrotate(<0, 0, rand(SSys_S6) / 2>, VRand(SSys_S6) * 360), rand(SSys_S6) * 1/2 + 1/2, rand(SSys_S6) * 1/2 + 1/2}
					#local CountFields = CountFields + 1;
				#end
				#local CountFields = 0;
				#local MaxFields = ceil(rand(SSys_S6) * 10);
				#while (CountFields < MaxFields)
					sphere { vrotate(<0, 0, rand(SSys_S6) / 2>, VRand(SSys_S6) * 360), rand(SSys_S6) * 1/4 + 1/4, rand(SSys_S6) * -1/4 - 1/4}
					#local CountFields = CountFields + 1;
				#end
				#local CountFields = 0;
				#local MaxFields = ceil(rand(SSys_S6) * 80);
				#while (CountFields < MaxFields)
					sphere { vrotate(<0, 0, rand(SSys_S6) * 1>, VRand(SSys_S6) * 360), rand(SSys_S6) * 1/8 + 1/8, rand(SSys_S6) * -1/4 - 1/4}
					#local CountFields = CountFields + 1;
				#end
				#local CountFields = 0;
				#local MaxFields = ceil(rand(SSys_S6) * 160);
				#while (CountFields < MaxFields)
					sphere { vrotate(<0, 0, rand(SSys_S6) * 1>, VRand(SSys_S6) * 360), rand(SSys_S6) * 1/16 + 1/16, rand(SSys_S6) * -1/4 - 1/4}
					#local CountFields = CountFields + 1;
				#end
				hollow
				material
				{
					texture
					{
						Parse_String(AsteroidString)
						normal
						{
							average
							normal_map
							{
								[dents 5 scale 1]
								[granite 1 scale 1/10]
							}
						}
						finish
						{
							diffuse 1
							crand 0.01
						}
						scale 1/5
					}
				}
				scale rand(SSys_S6)
			}
			#local CountAsteroids = CountAsteroids + 1;
		#end

        //------4-------6-------8-------10------12------14------16------18------20------22------
		// CAMERA POSITION
		#if (SSys_RenderMode != 1)
			#if (SSys_RenderOrbits = 1)
				object { OrbitPath }
			#end
			#switch (SSys_RenderOnly)
				#case (0)
					#if (SSys_CameraIsSet = 0)
						#local TempVector1 = vnormalize(SSys_CameraLocation - SSys_CameraLookAt);
						#local TempVector2 = vrotate(TempVector1, <SSys_CameraPosition.x, SSys_CameraPosition.y, 0>);
						#local TempVector3 = vrotate(TempVector2 * -1, <SSys_CameraDirection.x, SSys_CameraDirection.y, 0>);
						#local TempDistance = VDist(SSys_CameraLocation, SSys_CameraLookAt);
						#local SSys_CameraLocation = SSys_CameraLookAt + TempDistance * TempVector2;
						#local SSys_CameraLookAt = SSys_CameraLocation + TempDistance * TempVector3;
						#local SSys_CameraIsSet = 1;
					#end
				#break
				#case (SSys_PlanetsCnt)
					#local TempVector1 = vrotate(z, <SSys_CameraPosition.x, SSys_CameraPosition.y, 0>);
					#local TempVector2 = vrotate(TempVector1 * -1, <SSys_CameraDirection.x, SSys_CameraDirection.y, 0>);
					#local SSys_CameraLocation = PlanetPosition + SSys_CameraDistance * TempVector1;
					#local SSys_CameraLookAt = SSys_CameraLocation + TempVector2;
				#break
			#else
			#end
		#else
		#end

        //------4-------6-------8-------10------12------14------16------18------20------22------
		// OBJECT PLACEMENT
		#if (TotalAsteroids = 0)
			object
			{
				PlanetSystem
				#if (SSys_RenderMode = 1)
					#switch (SSys_RenderOnly)
						#case (0)
							translate <SSys_XPosCnt * 2 - SSys_NumberSqrt + 1, SSys_YPosCnt * 2 - SSys_NumberSqrt + 1, 0>
						#break
						#case (SSys_PlanetsCnt)
							translate <0, 0, 0>
						#break
					#else
						no_image
						no_shadow
						no_reflection
					#end
				#else
					scale SSys_RadiusPlanets
					translate PlanetPosition
				#end
			}
		#else
			#for (CountAsteroids, 1, TotalAsteroids)
				object
				{
					AsteroidsTable[floor(rand(SSys_S6) * SSys_NumberAsteroids)]
					#if (SSys_RenderMode = 1)
						#local Theta = rand(SSys_S6) * 2 * pi;
						#local Phi = rand(SSys_S6) * pi;
						#local Radius = rand(SSys_S6) * 2 / 3;
						#local CoordX = cos(Theta) * sin(Phi) * Radius;
						#local CoordZ = sin(Theta) * sin(Phi) * Radius;
						#local CoordY = cos(Phi) * Radius;
						#local AsteroidPosition = <CoordX, CoordY, CoordZ>;
						#switch (SSys_RenderOnly)
							#case (0)
								#local AsteroidPosition = AsteroidPosition + <SSys_XPosCnt * 2 - SSys_NumberSqrt + 1, SSys_YPosCnt * 2 - SSys_NumberSqrt + 1, 2>;
							#break
							#case (SSys_PlanetsCnt)
								#local AsteroidPosition = AsteroidPosition + <0, 0, 2>;
							#break
						#else
							no_image
							no_shadow
							no_reflection
						#end
						scale 1 / 32
						rotate VRand(SSys_S6) * 90
						translate AsteroidPosition
					#else
						#local Theta = rand(SSys_S6) * 2 * pi;
						#local CoordX = cos(Theta) * rand(SSys_S6) * MeanDistance * 0.02 + InnerEdgeX - FocalDistance;
						#local CoordZ = sin(Theta) * rand(SSys_S6) * SemiMinorAxis* 0.02 + InnerEdgeZ;
						#local CoordY = Rand_Normal(MeanDistance * 0.01, 1, SSys_S6) * RandSign(SSys_S6);
						#local AsteroidPosition = <CoordX, CoordY, CoordZ>;
						scale SSys_RadiusPlanets / 32
						translate AsteroidPosition
						translate -x * FocalDistance
						rotate AxisRotation
					#end
				}
			#end
		#end
		#if (SSys_PlanetsCnt = SSys_NumberPlanets)
			#local SSys_XPosCnt = 1000;
			#local SSys_YPosCnt = 1000;
		#end
		#local SSys_YPosCnt = SSys_YPosCnt + 1;
	#end
	#local SSys_XPosCnt = SSys_XPosCnt + 1;
#end


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// LABELS & ORBITS

#local SSys_PlanetsCnt = 0;
#local SSys_XPosCnt = 0;
#while (SSys_XPosCnt < SSys_NumberPlanets)
	#local SSys_YPosCnt = 0;
	#while (SSys_YPosCnt < SSys_NumberSqrt)
		#local PlanetPosition = SSys_PlanetsPos[SSys_PlanetsCnt];
		#local SSys_PlanetsCnt = SSys_PlanetsCnt + 1;

		// LABEL TEXT
		#local LabelText = text
		{
			ttf "cyrvetic.ttf"
			str(SSys_PlanetsCnt, 0, 0)
			0, 0
			hollow
			pigment { rgbt <1, 0, 0, 0.5> }
			finish { emission 1 }
			scale 1/8
		}
	
		// LABEL POINTS
		#local LabelPoint = sphere
		{
			0, 1
			hollow
			pigment { rgbt <1, 0, 0, 0.5> }
			finish { emission 1 }
			scale 1/128
		}
	
		#if (SSys_RenderMode = 1)
			#local LabelText = object
			{
				LabelText
				rotate <0, 180, 0>
				translate <SSys_XPosCnt * 2 - SSys_NumberSqrt + 1, SSys_YPosCnt * 2 - SSys_NumberSqrt + 1, 5>
			}
			#local LabelPoint = object
			{
				LabelPoint
				no_image
				no_shadow
				no_reflection
			}
		#else
			#local TempVector1 = vnormalize(PlanetPosition - SSys_CameraLocation);
			#local TempVector2 = PlanetPosition - TempVector1 * SSys_RadiusPlanets;
			#local TempFloat1 = VDist(TempVector2, SSys_CameraLocation);
			#local LabelText = object
			{
				LabelText
				Reorient_Trans(z, TempVector1)
				scale TempFloat1
				translate TempVector2
			}
			#local LabelPoint = object
			{
				LabelPoint
				scale TempFloat1
				translate PlanetPosition
			}
		#end
	
		#if (SSys_RenderLabels = on)
			object { LabelText }
			object { LabelPoint }
		#end

		#if (SSys_PlanetsCnt = SSys_NumberPlanets)
			#local SSys_XPosCnt = 1000;
			#local SSys_YPosCnt = 1000;
		#end

		#local SSys_YPosCnt = SSys_YPosCnt + 1;
	#end
	#local SSys_XPosCnt = SSys_XPosCnt + 1;
#end


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// CAMERA

#switch (SSys_RenderMode)
	#declare SSys_CameraAspect = image_width / image_height;
	// standard perspective view
	#case (0)
		#declare SSys_CameraUp = +y;
		#declare SSys_CameraRight = +x * SSys_CameraAspect;
		#declare SSys_CameraDirection = SSys_CameraLookAt - SSys_CameraLocation;
		#declare Camera_Object = camera
		{
			location	SSys_CameraLocation
			look_at		SSys_CameraLookAt
			up			SSys_CameraUp
			right		SSys_CameraRight
		}
	#break
	// orthographic spritesheet
	#case (1)
		#if (SSys_RenderOnly = 0)
			#declare SSys_CameraUp		= +y * 2 * SSys_NumberSqrt / SSys_CameraAspect;
			#declare SSys_CameraRight	= +x * 2 * SSys_NumberSqrt;
		#else
			#declare SSys_CameraUp		= +y * 2 / SSys_CameraAspect;
			#declare SSys_CameraRight	= +x * 2;
		#end
		#declare SSys_CameraLocation = <0,0,image_width>;
		#declare SSys_CameraLookAt = <0, 0, 0>;
		#declare SSys_CameraDirection = SSys_CameraLookAt - SSys_CameraLocation;
		#declare Camera_Object = camera
		{
			orthographic
			location	SSys_CameraLocation
			look_at		SSys_CameraLookAt
			up			SSys_CameraUp
			right		SSys_CameraRight
		}
	#break
	// spherical panorama
	#case (2)
		#declare Camera_Origin		= <0,0,0>;
		#declare Camera_Up			= +y;
		#declare Camera_Right		= +x;
		#declare Camera_Direction	= +z/2;
		#declare Camera_Location	= SSys_CameraLocation;
		#declare Camera_Translate	= <0,0,0>;
		#declare Camera_Scale		= <1,1,1>;
		#declare Camera_Transform = transform
		{
			Reorient_Trans(Camera_Direction, SSys_LightLocation-Camera_Location)
			translate	Camera_Location
			translate	Camera_Translate
			scale		Camera_Scale
		}
		#declare Camera_Object = camera
		{
			spherical
			up			Camera_Up
			right		Camera_Right
			direction	Camera_Direction
			location	Camera_Origin
			transform {Camera_Transform}
		}
		#declare Camera_Location		= vtransform(Camera_Origin,Camera_Transform);
		#declare Camera_LookAt			= vtransform(Camera_Direction,Camera_Transform);
		#declare SSys_CameraLocation	= Camera_Location;
		#declare SSys_CameraLookAt		= Camera_LookAt;
		#declare SSys_CameraDirection	= Camera_LookAt - Camera_Location;
		#declare SSys_CameraUp			= Camera_Up;
		#declare SSys_CameraRight		= Camera_Right;
	#break
	// cube map panorama
	// should really have both a "pre" rotation and a "post" rotatation
	#case (3)
		#declare Camera_Origin		= <0,0,0>;
		#declare Camera_Up			= +y;
		#declare Camera_Right		= +x;
		#declare Camera_Direction	= +z/2;
		#declare Camera_Location	= SSys_CameraLocation;
		#declare Camera_Translate	= <0,0,0>;
		#declare Camera_Scale		= <1,1,1>;
		#ifndef (Camera_CubeFace)
			#declare Camera_CubeFace = frame_number;
		#end
		#switch (Camera_CubeFace)
			#case (0)
				#declare Camera_Rotate	= <0,000,0>;
			#break
			#case (1)
				#declare Camera_Rotate	= <0,090,0>;
			#break
			#case (2)
				#declare Camera_Rotate	= <0,180,0>;
			#break
			#case (3)
				#declare Camera_Rotate	= <0,270,0>;
			#break
			#case (4)
				#declare Camera_Rotate	= <270,0,0>;
			#break
			#case (5)
				#declare Camera_Rotate	= <090,0,0>;
			#break
		#end
		#declare Camera_Transform = transform
		{
			rotate		Camera_Rotate
			Reorient_Trans(Camera_Direction, SSys_LightLocation-Camera_Location)
			translate	Camera_Location
			translate	Camera_Translate
			scale		Camera_Scale
		}
		#declare Camera_Object = camera
		{
			up			Camera_Up
			right		Camera_Right
			direction	Camera_Direction
			location	Camera_Origin
			transform {Camera_Transform}
		}
		#declare Camera_Location		= vtransform(Camera_Origin,Camera_Transform);
		#declare Camera_LookAt			= vtransform(Camera_Direction,Camera_Transform);
		#declare SSys_CameraLocation	= Camera_Location;
		#declare SSys_CameraLookAt		= Camera_LookAt;
		#declare SSys_CameraDirection	= Camera_LookAt - Camera_Location;
		#declare SSys_CameraUp			= Camera_Up;
		#declare SSys_CameraRight		= Camera_Right;
		#debug concat("\nCamera_CubeFace = ", str(Camera_CubeFace, 0, 0))
		#debug concat("\nCamera_Rotate = <", vstr(3, Camera_Rotate, ",", 0, -1), ">")
	#break
#end
#debug concat("\nSSys_CameraLocation = <", vstr(3, SSys_CameraLocation, ",", 0, -1), ">")

camera {Camera_Object}


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// SUN

#declare SSys_SunObject = sphere
{ 
	0, SSys_RadiusSun
	hollow
	material
	{
		texture
		{
			pigment {rgbt 1}
		}
		interior
		{
			media
			{
				emission 20*SSys_Light_Area_Color/SSys_RadiusSun
				density
				{
					function {pow(f_spherical(x,y,z),2)}
					density_map
					{
						[0.00 rgb 0]
						[0.01 rgb 1]
						[1.00 rgb 1]
					}
					scale SSys_RadiusSun
				}
			}
		}
	}
}

#declare SSys_HaloObject = sphere
{ 
	0, SSys_RadiusHalo
	hollow
	material
	{
		texture
		{
			pigment {rgbt 1}
		}
		interior
		{
			media
			{
				scattering {1, 20*SSys_Light_Area_Color/SSys_RadiusHalo}
				density
				{
					function {pow(f_spherical(x,y,z),10)}
					density_map
					{
						[0.00 rgb 0]
						[1.00 rgb 1]
					}
					scale SSys_RadiusHalo
				}
			}
		}
	}
}

#declare SSys_WindObject = sphere
{ 
	0, 500			// SSys_DistancePlanets
	hollow
	material
	{
		texture
		{
			pigment {rgbt 1}
		}
		interior
		{
			media
			{
				scattering {1, 20*SSys_Light_Area_Color/SSys_DistancePlanets}
				density
				{
					function {pow(f_spherical(x,y,z),2)}
					density_map
					{
						[0.00 rgb 0]
						[1.00 rgb 1]
					}
					scale SSys_DistancePlanets
				}
			}
		}
	}
}

#declare SSys_SunUnion = union
{
	object	{ SSys_SunObject }
	#if (SSys_RenderSunHalo = 1)
		object	{ SSys_HaloObject }
	#end
	#if (SSys_RenderSunWind = 1)
//		object	{ SSys_WindObject }
	#end
}


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// LIGHTS

#debug concat("\nSSys_LightLocation = <", vstr(3, SSys_LightLocation, ",", 0, -1), ">")
#debug concat("\nSSys_RadiusSun = ", str(SSys_RadiusSun, 0, -1))
light_source
{
	#switch (SSys_RenderMode)
		#case (1)
			SSys_LightLocation
			SSys_Light_Point_Color
			fade_power 2
			fade_distance SSys_DistancePlanets
//			fade_power 1/3
//			fade_distance SSys_RadiusSun
//			fade_power 1
//			fade_distance sqrt(SSys_DistancePlanets * SSys_RadiusSun)
			parallel
		#break
		#case (0)
		#case (2)
		#case (3)
			SSys_LightLocation
			SSys_Light_Area_Color
			area_light
			+x * SSys_RadiusSun * 2, +y * SSys_RadiusSun * 2		// lights spread out across this distance (x * z)
			SSys_Light_Area_Theta_Num, SSys_Light_Area_Phi_Num													// total number of lights in grid (4x*4z = 16 lights)
			adaptive 1												// 0,1,2,3...
			jitter													// adds random softening of light
			circular												// make the shape of the light circular
			orient													// orient light
			fade_power 2
			fade_distance SSys_DistancePlanets
//			fade_power 1/3
//			fade_distance SSys_RadiusSun
//			fade_power 1
//			fade_distance sqrt(SSys_DistancePlanets * SSys_RadiusSun)
			looks_like {SSys_SunUnion}
		#break
	#end
}


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// FOG (INTERFERES WITH GALAXY INCLUDE AND LENS EFFECTS)

#if (SSys_RenderFog = 1)

	fog
	{
		1
		distance SSys_DistancePlanets
		color srgbt SSys_FogColor
	}
/*
	media
	{
		#if (SSys_RenderStyle = 0)
			absorption RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M4) * 2024 / 100 / SSys_DistancePlanets
		#else
			absorption RRand(SSys_MedAbsorbtion.x, SSys_MedAbsorbtion.y, SSys_M4) * 768 / 320 / SSys_DistancePlanets
		#end
		density
		{
			spherical
			density_map
			{
				[0 color rgb 0]
				[1 color rgbt (1 - SSys_FogColor)]
			}
		}
		scale SSys_DistancePlanets
	}
*/
#end


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// GALAXY INCLUDE OPTIONS

#if ((SSys_RenderMode != 1) & (SSys_DependenciesExist = 1))
	#declare galaxy_intensity = SSys_GalaxyIntensity;
	#declare galaxy_distance = SSys_DistancePlanets * 2;
	#declare galaxy_seed = SSys_S5;
	#local SSys_GalSeedSF = seed(rand(SSys_S5));
	#local SSys_GalSeedBG = seed(rand(SSys_S5));
	#local SSys_GalSeedOJ = seed(rand(SSys_S5));
	#if (SSys_RenderStarfield = 1)
		//#declare galaxy_intensity = SSys_GalaxyIntensity;
		#declare star_count = 2000;
		#declare star_scale = 0.5;
		#include "GALAXY.SF"
		
		#declare star_count = 500;
		#declare star_type = 3;
		#declare star_colour = <1, .9, .7>;
		#declare star_scale = 1;
		#include "GALAXY.SF"
	#end
	#if (SSys_RenderBackground = 1)
//		#declare galaxy_intensity = SSys_GalaxyIntensity;
		#declare galaxy_starfield = 0;
		#declare galaxy_bg = 1;
		#declare galaxy_objects = 0;
		#include "GALAXY.INC"
	#end
	#debug concat("\n//Debug - SSys_RenderObjects: ", str(SSys_RenderObjects, 0, 0))
	#if (SSys_RenderObjects = 1)
//		#declare galaxy_intensity = 1/2;
		#declare SSys_GalObjectTable = array[12]
		{
			"N1", "N2", "N3", "N4", "N5", "N6", "G1", "G2", "G3", "G4", "G5", ""
		}
		#declare SSys_GalClusterTable = array[10]
		{
			"S1", "S2", "S3", "S4", "C1", "C2", "C3", "M1", "M2", ""
		}
		#for (SSys_GalCountObj, 1, SSys_NumberObjects)
			#declare galaxy_colour1 = VRand(SSys_GalSeedOJ);
			#declare galaxy_colour2 = VRand(SSys_GalSeedOJ);
			#declare galaxy_colour3 = VRand(SSys_GalSeedOJ);
			#declare galaxy_object_name = SSys_GalObjectTable[floor(rand(SSys_GalSeedOJ) * 12)];
			#declare galaxy_object_position = <rand(SSys_GalSeedOJ) * 180 - 90, rand(SSys_GalSeedOJ) * 360 - 180, 0>;
			#declare galaxy_object_scale = rand(SSys_GalSeedOJ) * 5;
			#declare galaxy_cluster_name = SSys_GalClusterTable[floor(rand(SSys_GalSeedOJ) * 10)];
			#declare galaxy_cluster_objects = floor(rand(SSys_GalSeedOJ) * rand(SSys_GalSeedOJ) * SSys_NumberObjects);
			#declare galaxy_cluster_scale = rand(SSys_GalSeedOJ);
			#declare galaxy_cluster_spread = rand(SSys_GalSeedOJ) * rand(SSys_GalSeedOJ) * 180;
			#include "GALAXY.OBJ"
		#end
	#end
#end


//------2-------4-------6-------8-------10------12------14------16------18------20------22------
// LENS INCLUDE OPTIONS
// Try to keep the effect at the center of the camera since perspective distortions mess things up.

#if ((SSys_DependenciesExist = 1) & (SSys_RenderLensEffects = 1))
	#declare effect_seed			= SSys_S7;
	#declare rays_seed				= SSys_S7;
	#declare streak_seed			= SSys_S7;
	#declare spots_seed				= SSys_S7;
	//#declare effect_always_on_top	= false;
	#declare effect_scale			= 1/3;
	#declare effect_intensity		= 1/3;
	#declare effect_type			= "RAINBOW";
	#declare camera_off				= true;		// custom flag
	#declare camera_sky				= SSys_CameraUp;
	#declare camera_direction		= SSys_CameraDirection;
	#declare camera_location		= SSys_CameraLocation;
	#declare camera_look_at			= SSys_CameraLookAt;
	//#declare camera_angle			= 90;
	#declare effect_location		= SSys_LightLocation;
	#declare effect_always_visible	= false;		// custom flag
	//#declare effect_rotate		= 30;
	#declare spots_contract_to_centre	= true;
	#declare spots_spacing			= 0.9;		// has no effect here
	#include "Lens.inc"			// http://www.oocities.org/ccolefax/lenseffects.html
#end
