#include "header.h"

Player::Player()
{
}

Player::Player(int index)
{
	player = core->g_pEntList->GetClientEntity(index);
	core->g_pEngine->GetPlayerInfo(index, &pInfo);

	enemy = isEnemy();
	getEyes();
	setType();
	distanceFromMe = myHack->get3dDist(myPlayer.position, position);
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

bool Player::isEnemy() {
	if (team == myPlayer.team) {
		return false;
	}
	return true;
}

void Player::setType() {
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

Player::~Player()
{
}
