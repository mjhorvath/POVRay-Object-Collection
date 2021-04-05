// Video game special effects collection for POV-Ray v2.00
// *******************************************************
// Created: 2006-03-01 by Michael Horvath
// Last updated: 2008-07-05
// Website: http://www.geocities.com/Area51/Quadrant/3864/povray.htm
// Original work is licensed under the BSD license.
// Portions of this file are copyrighted by their original authors.


// -------------------------------------------------------------

#declare GSprites_Exhaust_Effects_Texture_A = texture
{
	#local TClock = clock * 8/10 + 1/10;
	pigment
	{
		bozo
		color_map {[0 color rgbt <1,1,1,TClock,>][1 color rgbt <1,1,1,1,>]}
	}
	finish {GSprites_Default_Finish}
}

#declare GSprites_Exhaust_Effects_Texture_B = texture
{
	pigment
	{
		bozo
		color_map {[1/4 color rgbt <1,1,1,0,>][4/4 color rgbt <1,1,1,1,>]}
	}
	finish {GSprites_Default_Finish}
	scale 4
}

#declare GSprites_Exhaust_Effects_Texture_C = texture
{
	pigment
	{
		gradient -y
		color_map {[0/4 rgbt <2/2,2/2,1/2,0/2,>][1/4 rgbt <2/2,1/2,0/2,2/2,>]}
	}
	normal {bumps 4}
	finish {GSprites_Default_Finish}
	scale 64
}

#declare GSprites_Exhaust_Effects_Mixture_A = texture
{
	average
	texture_map {[1 GSprites_Exhaust_Effects_Texture_B][1 GSprites_Exhaust_Effects_Texture_C]}
}

#declare GSprites_Exhaust_Effects_Texture_D = texture
{
	pigment {color rgbt <1,1/2,0,0,>}
	finish {GSprites_Default_Finish ambient 1}
}

#declare GSprites_Explosion_Effects_Texture_A = texture
{
	pigment
	{
		bozo
		color_map {[0 rgb 4/10][1 rgb 8/10]}
		scale 3/10
	}
	finish {GSprites_Default_Finish}
	normal {bumps 2 scale .1}
}

#declare GSprites_Explosion_Effects_Texture_B = texture
{
	#local TClock = clock * 8/10 + 1/10;
	pigment {color rgbt (TClock + 3)/4}
	finish {GSprites_Default_Finish}
	normal {bumps 2 scale .1}
}

#declare GSprites_Explosion_Effects_Texture_C = texture
{
	pigment
	{
		bozo
		color_map {[0/4 rgbt <0/1,2/2,1/2,1/2,>][1/4 rgbt <2/2,1/2,0/2,2/2,>]}
		scale 4/10
	}
	finish {GSprites_Default_Finish}
}

#declare GSprites_Flame_Effects_Texture = texture
{
	pigment {color rgb <0.85,0.45,0.15>}
	finish {GSprites_Default_Finish}
}

#declare GSprites_FireBall_Effects_Texture = texture
{
	#local TClock = clock * 8/10 + 1/10;
	pigment
	{
		onion
		color_map
		{
			[0/4 rgbt <2/2 - TClock/2,	2/2 - TClock/2,	1/2,		1/2 + TClock/4,>	* <1/2,1/2,1/2,2/2,>]
			[1/4 rgbt <2/2 - TClock/2,	1/2,		TClock/2,	2/2,>			* <1/2,1/2,1/2,2/2,>]
		}
		scale 2
		warp
		{
			turbulence -TClock * 1/2
			octaves 3
			lambda 3
		}
		rotate clock * 30
	}
	finish {ambient 3 diffuse 0}
}
