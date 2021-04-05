RANDOM SOLAR SYSTEM GENERATOR FOR POV-Ray v1.2
**********************************************
Authors: Michael Horvath
Created: 2005-03-01
Updated: 2021-03-11
Website: http://isometricland.net


DESCRIPTION
***********
This include file will randomly generate a solar system, complete with sun, 
moons, planets and asteroid belts. There are options to render a single planet 
at a time, or render the whole system from a bird's-eye view with all orbits 
drawn and planet positions marked and numbered. Another option is to generate 
an orthographic tiled image suitable for splitting apart and inserting into a 
video game as sprites.

Three sample scenes are included to demonstrate the different scene-rendering 
options.

The include file is set up to utilize Chris Colefax's "GALAXY.INC" and 
"Lens.inc" if they exist somewhere in POV-Ray's library path. To turn on this 
support, simply set the "SSys_DependenciesExist" variable to equal "true". Note 
that, since Chris Colefax's include files aren't compliant with the Object 
Collection's variable naming and abbreviation rules, you may experience 
conflicts within your scene if one of your own variables happens to share an 
identical name with one of Chris Colefax's. Turning this support off eliminates 
this problem.


INSTRUCTIONS
************
If rendering an orthographic image, then the size of each tile will be equal to 
the height of the output image divided by sqrt(SSys_NumberPlanets). Else, if 
rendering a spherical image, then the output image should be twice as wide as 
it is tall. Note that the camera and a light source are already included. You 
can copy the Global Variables section, below, to a new file and then include 
this file if you want.


NOTES
*****
By default, 1AU is roughly equal to 1000 POV-Ray units. The sun's diameter is 
10 units, and the maximum possible planet diameter is 1 unit. Mean planetary 
distances are currently calculated using the Titius-Bode law, which is an 
(outdated) formula created in the 19th century to approximate our own solar 
system.


ISSUES/TO DO
************
* The generated planets don't look realistic enough.
* It may be better to use texture maps (e.g. shiny textures for oceans and 
  rough textures for land masses) in place of color maps.
* Mapping heightfields to spheres would be nice.
* Planetary mean distances need to be randomized.
* Would like to use a hyperbolic function to calculate mean planetary 
  distances, instead of parabolic.
* Should planetary distances be a product of the sun's mass?
* Make it so that the camera is always facing the object, unless CameraAzimuth 
  and CameraAltitude are explicitely set.
* Have to make sure that the numbers of calls to the random function are 
  balanced across the various options.
* Global absorbtion media blots out the sun at large distances.
* The sun is ugly. The sun is not visible at large distances, while the other 
  stars are.
* Fog conflicts with the Galaxy and Lens Effects include files.
* Create elliptical paths for moons, as well.
* Add orbital/rotation velocities, and synchronize them with the clock so that 
  the solar system can be animated.
* Should add a regular orthographic rendering mode.
* Need to fix orthographic tiled mode so that the number of planets doesn't 
  have to be the square of an integer.
* Would be neat to add man-made objects, like space stations, ring worlds, 
  Dyson spheres, etc.
* Need to rotate comets so that their tails point away from the sun.
* Planet rings are too dark.
* Should the planets be flattened vertically a little bit?


GLOBAL VARIABLES
****************
Copy, uncomment and adjust the following variables to modify the parameters of 
the include file within your own scene.


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


KEYWORDS
********

outer
space
solar
system
planet
asteroid
sun
star
random
generator
