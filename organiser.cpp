/*
version 0.45b
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
		foes_hitbox.hitbox.sphere.Middle.Z = (*fit).getPosZ();
		auto bit = player_bullets.begin();
		while (bit < player_bullets.end())
		{
			bullet.hitbox.sphere.Middle.X = (*bit).getPosX();
			bullet.hitbox.sphere.Middle.Y = (*bit).getPosY();
			bullet.hitbox.sphere.Middle.Z = (*bit).getPosZ();
			if (detector.detector(bullet, foes_hitbox))
			{
				bit = player_bullets.erase(bit);
				(*fit).kill_this_bastard();
				if (!(*fit).is_bastard_alive())
				{
					fit = foes.erase(fit); //this is pretty tricky, because you delete elements from vector you are searching, so to keep everything in good
					//order we need to use vector iterators
					continue;
				}
			}
			else
			{
				bit++;
			}
		}
		if (fit != foes.end())
		{
			fit++;
		}
		else break;
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
		if (fit != foes.end())
		{
			fit++;
		};
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
	auto bpit = player_bullets.begin();
	while (bpit < player_bullets.end())
	{
		if (!(*bpit).updatePos(V))
		{
			bpit = player_bullets.erase(bpit);
		}
		else bpit++;
	}
	auto bit = foe_bullets.begin();
	while (bit < foe_bullets.end())
	{
		if (!(*bit).updatePos(V))
		{
			bit = foe_bullets.erase(bit);
		}
		else bit++;
	}
}

void organiser::collisionsHandling()
{
	foes_player_bullets();
	foes_foe_bullets();
	bullets_surrounding();
	//void player_foes(); 
	//void player_enemy_bullets();
	//void player_surrounding();
	//void foes_foes();
	//void foes_surrounding();
}

void organiser::drawAllElements(ShaderProgram* sp, glm::vec4 ppos)
{
	for (int i = 0; i < foes.size(); i++)
	{
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(foes[i].getPos()));
		foes[i].draw();
		//shoot(foes[i], ppos);
	}
	for (int i = 0; i < player_bullets.size(); i++)
	{
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(player_bullets[i].getPos()));
		player_bullets[i].draw();
	}
	for (int i = 0; i < foe_bullets.size(); i++)
	{
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(foe_bullets[i].getPos()));
		foe_bullets[i].draw();
	}
}
void organiser::addRouteForFoe(int fn, glm::vec4 point)
{
	foes[fn].addRoutePoint(point);
}
//debugging
void organiser::shoot(Foe foe, glm::vec4 playerPos)
{
	glm::vec4 dist = playerPos - foe.getPos()[3]; // computing position in eye space
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsedTime = now - foe.lS();

	if (glm::length(dist) < 20 && elapsedTime.count() >= 2.f)
	{
		foe.setlS(now);

		glm::vec4 dir = glm::normalize(dist);

		glm::mat4 pos = foe.getPos();

		float angleY = glm::orientedAngle(glm::normalize(glm::vec2(pos[2].x, pos[2].z)), glm::normalize(glm::vec2(dir.x, dir.z)));

		// angles by which projectile will be rotated to start facing the camera

		pos = glm::rotate(pos, -angleY, glm::vec3(0.f, 1.f, 0.f));

		float angleX = glm::orientedAngle(glm::normalize(glm::vec2(pos[2].y, pos[2].z)), glm::normalize(glm::vec2(dir.y, dir.z)));

		pos = glm::rotate(pos, -angleX, glm::vec3(1.f, 0.f, 0.f));

		pos = glm::translate(pos, 10.f * glm::vec3(glm::normalize(glm::transpose(pos) * dir)));

		addFoeBullet(Projectile(foe.returnBullet(), pos, dir, 2.f, 5.f));
	}
}