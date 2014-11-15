// Author: DarkTigrus
//
// Speedometer.cpp
//
// implementation of CHudSpeedometer class
//

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "parsemsg.h"

DECLARE_MESSAGE(m_Speedometer, Velocity)

int CHudSpeedometer::Init(void)
{
	HOOK_MESSAGE(Velocity);

	m_iSpeed = 0;

	gHUD.AddHudElem(this);

	CVAR_CREATE("hud_speedometer", "1", FCVAR_ARCHIVE);

	return 1;
};

int CHudSpeedometer::VidInit(void)
{
	return 1;
};

int CHudSpeedometer::MsgFunc_Velocity(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	m_iVelocity.x = READ_COORD();
	m_iVelocity.y = READ_COORD();
	m_iVelocity.z = READ_COORD();

	m_iFlags |= HUD_ACTIVE;

	return 1;
}

int CHudSpeedometer::Draw(float flTime)
{
	int r, g, b;
	int x, y;

	if (!CVAR_GET_FLOAT("hud_speedometer"))
		return 1;

	UnpackRGB(r, g, b, RGB_YELLOWISH);
	ScaleColors(r, g, b, MIN_ALPHA);

	x = ScreenWidth / 2;
	y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;

	m_iSpeed = (int)m_iVelocity.Length2D();

	gHUD.DrawHudNumber(x, y, 1, m_iSpeed, r, g, b);

	return 1;
}
