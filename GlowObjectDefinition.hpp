#pragma once

struct GlowObjectDefinition
{
	unsigned long pEntity;
	float r;
	float g;
	float b;
	float a;
	unsigned char unk0[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	unsigned char unk1[14];
};