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

DECLARE_MESSAGE(m_Speedometer, Speed)

int CHudSpeedometer::Init(void)
{
	HOOK_MESSAGE(Speed);

	m_iSpeed = 0;

	gHUD.AddHudElem(this);

	CVAR_CREATE("hud_speedometer", "1", FCVAR_ARCHIVE);

	return 1;
};

int CHudSpeedometer::VidInit(void)
{
	return 1;
};

int CHudSpeedometer::MsgFunc_Speed(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	m_iSpeed = READ_SHORT();

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

	gHUD.DrawHudNumber(x, y, 1, m_iSpeed, r, g, b);

	return 1;
}
