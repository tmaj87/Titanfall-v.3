#include "header.h"

Player::Player(int index)
{
	player = core->g_pEntList->GetClientEntity(index);

	enemy = true;
	if (team == myPlyr.team) {
		enemy = false;
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

Player::~Player()
{
}
