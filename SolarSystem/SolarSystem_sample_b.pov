// RANDOM SOLAR SYSTEM GENERATOR FOR POV-Ray v1.2
// **********************************************
// Authors: Michael Horvath
// Created: 2005-03-01
// Updated: 2021-03-10
// Website: http://isometricland.net
// This file is licensed under the terms of the CC-LGPL.
// Equirectangular (spherical) panorama. Render one frame at 2:1 aspect ratio.

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

#declare SSys_DependenciesExist = false;
#declare SSys_NumberPlanets = 9;
#declare SSys_RenderMode = 2;
#declare SSys_RenderOnly = 0;
#declare SSys_RenderOrbits = on;
#declare SSys_RenderLabels = on;
#declare SSys_CameraLocation = <0, 100, 0,>;
#declare SSys_CameraLookAt = <0, 100, 100,>;

#include "SolarSystem_include.inc"
