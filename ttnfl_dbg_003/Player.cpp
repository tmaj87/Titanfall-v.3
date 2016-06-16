#include "header.h"

Player::Player()
{
}

Player::Player(int index)
{
	player = core->g_pEntList->GetClientEntity(index);
	core->g_pEngine->GetPlayerInfo(index, &pInfo);

	enemy = true;
	if (team == myPlyr.team) {
		enemy = false;
	}

	getEyes();

	type = PLAYER;
	if (strlen(pInfo.name) < 6)
	{
		type = MINION;
	}
	if (health > 700)
	{
		type = TITAN;
	}
}

bool Player::check() {
	return !(
		player == NULL
		|| lifeState != 0
		|| team == 0
		|| inventory == 0
		);
}

void Player::getEyes() {
	eyesPositon = position;
	eyesPositon.z + viewOffset;
}

Player::~Player()
{
}
