// Video game special effects collection for POV-Ray v2.00
// *******************************************************
// Created: 2006-03-01 by Michael Horvath
// Last updated: 2008-07-05
// Website: http://www.geocities.com/Area51/Quadrant/3864/povray.htm
// Original work is licensed under the BSD license.
// Portions of this file are copyrighted by their original authors.

// Notes: Special effects are intended to be rendered as 
// 8-frame animations. However, the number of frames really 
// only matters for projectiles. The others may use as many 
// frames as you wish.
// +ki0 +kf1 +kfi0 +kff7 +kc
// +k0.5

#declare GSprites_Burst_Mode = on;
#include "GSprites_Scene_settings.inc"			// Source of the camera and lighting code
#include "GSprites_Effects_prototypes.inc"		// Source of the object components
background {color rgb 0}


// -------------------------------------------------------------

// Another explosion - by John (evilsnack) at http://www.geocities.com/evilsnack/
object
{
	GSprites_Explosion_Effects_Object_D(GSprites_Width,GSprites_Height/4)
	translate GSprites_Left_Vector*+3/2
}

// Fiery explosion - can't remember the author
object
{
	GSprites_Explosion_Effects_Object_E(GSprites_Width,GSprites_Height/4,GSprites_Seed)
	translate GSprites_Left_Vector*+1/2
}

// Cuncussive explosion - requires Object Exploder by Chris Colefax
object
{
	GSprites_Cuncussion_Effects_Object(GSprites_Width,GSprites_Height/4,1)
	translate GSprites_Left_Vector*-1/2
}

// Volatile explosion - requires Object Exploder by Chris Colefax
object
{
	GSprites_Volatile_Effects_Object(GSprites_Width,GSprites_Height/4,1)
	translate GSprites_Left_Vector*-3/2
}

// Flame - requires Rune's Particle System
object
{
	GSprites_Fire_Effects_Object(GSprites_Width,GSprites_Height)
	translate GSprites_Left_Vector*+3/2
	translate GSprites_Up_Vector
}

// Smoke column - requires Rune's Particle System
object
{
	GSprites_Smoke_Effects_Object(GSprites_Width,GSprites_Height)
	translate GSprites_Left_Vector*+1/2
	translate GSprites_Up_Vector
}

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
//GSprites_Explosion_Effects_Object_C(GSprites_Width,GSprites_Height/4,(GSprites_Left_Vector*-1/2+GSprites_Up_Vector))


//Mushroom cloud goes here.


// Bullet projectile - requires "LENS.INC" by Chris Colefax
// Note: this effect will only appear as it is supposed to if 
// the "Energy explosion" effect is commented out, above.
GSprites_Bullet_Effects_Object(GSprites_Width,GSprites_Height/4,GSprites_Burst_Mode,(GSprites_Left_Vector*+3/2+GSprites_Up_Vector*2))

// Energy projectile - requires "LENS.INC" by Chris Colefax
// Note: this effect will only appear as it is supposed to if 
// the "Energy explosion" effect is commented out, above.
GSprites_Sparkle_Effects_Object(GSprites_Width,GSprites_Height/4,GSprites_Burst_Mode,(GSprites_Left_Vector*+1/2+GSprites_Up_Vector*2))

// Flamethrower - by H.E. Day
object
{
	GSprites_Flamethrower_Effects_Object(GSprites_Width,GSprites_Height/4)
	translate GSprites_Left_Vector*-1/2
	translate GSprites_Up_Vector*2
}

// Missile
object
{
	GSprites_Missile_Effects_Object(GSprites_Width,GSprites_Height/4,GSprites_Burst_Mode)
	translate GSprites_Left_Vector*-3/2
	translate GSprites_Up_Vector*2
}
