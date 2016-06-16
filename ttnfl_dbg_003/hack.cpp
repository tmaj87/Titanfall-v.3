#include "header.h"

float hack::getDist(Vector& myCoords, Vector& enemyCoords)
{
	return (float)sqrt(
		pow(enemyCoords[0] - myCoords[0], 2.0) +
		pow(enemyCoords[1] - myCoords[1], 2.0) +
		pow(enemyCoords[2] - myCoords[2], 2.0));
}

bool hack::worldToScreen(Vector &vOrigin, Vector &vScreen) {
	const matrix3x4& worldToScreen = *m_vMatrix;

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0;

	if (w > .001)
	{
		int wScreen, hScreen;
		core->g_pEngine->GetScreenSize(wScreen, hScreen);

		float fl1DBw = 1 / w;

		vScreen.x = (int)((wScreen / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * wScreen + 0.5));
		vScreen.y = (int)((hScreen / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * hScreen + 0.5));
		return 1;
	}

	return 0;
}

void hack::getMatrix()
{
	DWORD_PTR dwFunc1 = (DWORD_PTR)VMThook2->dwGetMethodAddress(74) + 0x6;
	DWORD_PTR dwRenderHolder = (DWORD_PTR)((*(DWORD*)(dwFunc1 + 0x3)) + (dwFunc1 + 0x7)); // cut 4 bytes

	CViewRender* pRender;
	do {
		Sleep(50);
		pRender = *(CViewRender**)dwRenderHolder;
	} while (!pRender);

	m_vMatrix = pRender->GetWorldToScreenMatrix();
}

void hack::getHead(CBaseEntity* player, Vector* posToWrite)
{
	static matrix3x4 boneList[128];
	static int boneId = 12;

	if (player->SetupBones(boneList, 128, 256, PlatFloatTime()))
	{
		if (boneList[11][2][3] > boneList[12][2][3])
		{
			boneId = 11;
		}

		posToWrite = new Vector(
			boneList[boneId][0][3],
			boneList[boneId][1][3],
			boneList[boneId][2][3]
		);
	}
}

void hack::getBonePos(CBaseEntity* player, int boneId, Vector* posToWrite)
{
	static matrix3x4 boneList[128];

	if (player->SetupBones(boneList, 128, 256, PlatFloatTime()))
	{
		posToWrite = new Vector(
			boneList[boneId][0][3],
			boneList[boneId][1][3],
			boneList[boneId][2][3]
		);
	}
}
