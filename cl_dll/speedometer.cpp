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

int giPrevJump;

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
	int x = READ_BYTE();

	m_iFlags |= HUD_ACTIVE;

	m_iSpeed = (int)m_iVelocity.Length2D();

	if (x)
	{
		giPrevJump = m_iJump;
		m_iJump = m_iSpeed;
		m_fFade = FADE_TIME;
	}

	return 1;
}

void CHudSpeedometer::GetJumpColor(int &r, int &g, int &b, float t)
{
	if (m_iJump > giPrevJump)
	{
		r = 255 - (255 * t) / FADE_TIME;
	}
	else
	{
		g = 160 - (160 * t) / FADE_TIME;
	} 
}

int CHudSpeedometer::Draw(float flTime)
{
	int r, g, b;
	int a = 0, x, y;

	if (!CVAR_GET_FLOAT("hud_speedometer"))
		return 1;

	UnpackRGB(r, g, b, RGB_YELLOWISH);
	ScaleColors(r, g, b, MIN_ALPHA);

	x = ScreenWidth / 2;
	y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;

	gHUD.DrawHudNumber(x, y, DHN_3DIGITS | DHN_DRAWZERO, m_iSpeed, r, g, b);

	if (m_fFade)
	{
		m_fFade -= (gHUD.m_flTimeDelta * MIN_ALPHA);
		if (m_fFade <= 0)
		{
			a = MIN_ALPHA;
			m_fFade = 0;
		}

		a = MIN_ALPHA + (m_fFade / FADE_TIME) * 128;

	}
	else
		a = MIN_ALPHA;

	UnpackRGB(r, g, b, RGB_YELLOWISH);
	GetJumpColor(r, g, b, m_fFade);
	ScaleColors(r, g, b, a);

	y -= gHUD.m_iFontHeight + gHUD.m_iFontHeight / 2;

	gHUD.DrawHudNumber(x, y, DHN_3DIGITS | DHN_DRAWZERO, m_iJump, r, g, b);

	return 1;
}
