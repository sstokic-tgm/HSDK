#pragma once

#include "CRC.hpp"
#include "In_Buttons.hpp"
#include "Util_Vector.hpp"

class UserCMD
{
public:

	virtual ~UserCMD() {};

	CRC32_t GetChecksum(void) const;

	int command_number;
	int tick_count;
	vec3_t viewangles;
	vec3_t aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	char pad_0x4C[0x18];

};

class VerifiedUserCMD
{
public:

	UserCMD  m_cmd;
	CRC32_t   m_crc;
};

class Input
{
public:

	void *pvftable;
	bool m_fTrackIRAvailable;
	bool m_fMouseInitialized;
	bool m_fMouseActive;
	bool m_fJoystickAdvancedInit;
	char pad_0x08[0x2C];
	void *m_pKeys;
	char pad_0x38[0x64];
	bool m_fCameraInterceptingMouse;
	bool m_fCameraInThirdPerson;
	bool m_fCameraMovingWithMouse;
	vec3_t m_vecCameraOffset;
	bool m_fCameraDistanceMove;
	int m_nCameraOldX;
	int m_nCameraOldY;
	int m_nCameraX;
	int m_nCameraY;
	bool m_CameraIsOrthographic;
	vec3_t m_angPreviousViewAngles;
	vec3_t m_angPreviousViewAnglesTilt;
	float m_flLastForwardMove;
	int m_nClearInputState;
	char pad_0xE4[0x8];
	UserCMD *m_pCommands;
	VerifiedUserCMD *m_pVerifiedCommands;
};

extern Input *p_Input;