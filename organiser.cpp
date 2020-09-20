/*
version 0.4
*/
#include <organiser.h>

organiser::organiser(Hitbox player, Hitbox foe, Hitbox bullet)
{
	this->player = player;
	foes_hitbox = foe;
	this->bullet = bullet;
}

void organiser::addFoe(Foe foe)
{
	foes.push_back(foe);
}

void organiser::addPlayerBullet(Projectile bullet)
{
	player_bullets.push_back(bullet);
}

void organiser::addFoeBullet(Projectile bullet)
{
	foe_bullets.push_back(bullet);
}

void organiser::addSurroundingElement(Hitbox element)
{
	surrounding.push_back(element);
}

void organiser::foes_player_bullets()
{
	auto fit = foes.begin();
	while (fit < foes.end())
	{
		foes_hitbox.hitbox.sphere.Middle.X = (*fit).getPosX();
		foes_hitbox.hitbox.sphere.Middle.Y = (*fit).getPosY();
		foes_hitbox.hitbox.sphere.Middle.Y = (*fit).getPosZ();
		auto bit = player_bullets.begin();
		while (bit < player_bullets.end())
		{
			bullet.hitbox.sphere.Middle.X = (*bit).getPosX();
			bullet.hitbox.sphere.Middle.Y = (*bit).getPosY();
			bullet.hitbox.sphere.Middle.Z = (*bit).getPosZ();
			if (detector.detector(bullet, foes_hitbox))
			{
				(*fit).kill_this_bastard();
				if (!(*fit).is_bastard_alive())
				{
					fit = foes.erase(fit); //this is pretty tricky, because you delete elements from vector you are searching, so to keep everything in good
					//order we need to use vector iterators
					continue;
				}
				bit = player_bullets.erase(bit);
			}
			else
			{
				bit++;
			}
		}
		fit++;
	}
}

void organiser::foes_foe_bullets()
{
	auto fit = foes.begin();
	while (fit < foes.end())
	{
		foes_hitbox.hitbox.sphere.Middle.X = (*fit).getPosX();
		foes_hitbox.hitbox.sphere.Middle.Y = (*fit).getPosY();
		foes_hitbox.hitbox.sphere.Middle.Y = (*fit).getPosZ();
		auto bit = foe_bullets.begin();
		while (bit < foe_bullets.end())
		{
			bullet.hitbox.sphere.Middle.X = (*bit).getPosX();
			bullet.hitbox.sphere.Middle.Y = (*bit).getPosY();
			bullet.hitbox.sphere.Middle.Z = (*bit).getPosZ();
			if (detector.detector(bullet, foes_hitbox))
			{
				bit = foe_bullets.erase(bit);
			}
			else
			{
				bit++;
			}
		}
		fit++;
	}
}

void organiser::bullets_surrounding()
{
	for (int i = 0; i < surrounding.size(); i++)
	{
		auto bit = foe_bullets.begin();
		while (bit < foe_bullets.end())
		{
			bullet.hitbox.sphere.Middle.X = (*bit).getPosX();
			bullet.hitbox.sphere.Middle.Y = (*bit).getPosY();
			bullet.hitbox.sphere.Middle.Z = (*bit).getPosZ();
			if (detector.detector(bullet, surrounding[i]))
			{
				bit = foe_bullets.erase(bit);
			}
			else
			{
				bit++;
			}
		}
		auto bpit = player_bullets.begin();
		while (bit < player_bullets.end())
		{
			bullet.hitbox.sphere.Middle.X = (*bpit).getPosX();
			bullet.hitbox.sphere.Middle.Y = (*bpit).getPosY();
			bullet.hitbox.sphere.Middle.Z = (*bpit).getPosZ();
			if (detector.detector(bullet, foes_hitbox))
			{
				bpit = player_bullets.erase(bpit);
			}
			else
			{
				bpit++;
			}
		}
	}
}

void organiser::positionUpdate(glm::mat4 V)
//just updating positions
{
	for (int i = 0; i < foes.size(); i++)
	{
		foes[i].updatePos(V);
	}
	for (int i = 0; i < player_bullets.size(); i++)
	{
		player_bullets[i].updatePos(V);
	}
	for (int i = 0; i < foe_bullets.size(); i++)
	{
		foe_bullets[i].updatePos(V);
	}
}

void organiser::collisionsHandling()
{
	//void player_foes(); 
	//void player_enemy_bullets();
	//void player_surrounding();
	//void foes_foes();
	void foes_player_bullets();
	void foes_foe_bullets();
	//void foes_surrounding();
	void bullets_surrounding();
}

void organiser::drawAllElements()
{
	for (int i = 0; i < foes.size(); i++)
	{
		foes[i].draw();
	}
	for (int i = 0; i < player_bullets.size(); i++)
	{
		player_bullets[i].draw();
	}
	for (int i = 0; i < foe_bullets.size(); i++)
	{
		foe_bullets[i].draw();
	}
}