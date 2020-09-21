/*
version 0.8
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
			if (detector.detector(bullet, foes_hitbox, nullptr))
			{
				bit = player_bullets.erase(bit);
				(*fit).kill_this_bastard();
				if (!(*fit).is_bastard_alive())
				{
					fit = foes.erase(fit); //this is pretty tricky, because you delete elements from vector you are searching, so to keep everything in good
					//order we need to use vector iterators
					break;
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
			if (detector.detector(bullet, foes_hitbox, nullptr))
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
			if (detector.detector(bullet, surrounding[i], nullptr))
			{
				bit = foe_bullets.erase(bit);
			}
			else
			{
				bit++;
			}
		}
		auto bpit = player_bullets.begin();
		while (bpit < player_bullets.end())
		{
			bullet.hitbox.sphere.Middle.X = (*bpit).getPosX();
			bullet.hitbox.sphere.Middle.Y = (*bpit).getPosY();
			bullet.hitbox.sphere.Middle.Z = (*bpit).getPosZ();
			if (detector.detector(bullet, foes_hitbox, nullptr))
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

void organiser::collisionsHandling(glm::vec3 player, GLFWwindow* window, Camera* cam)
{
	foes_player_bullets();
	foes_foe_bullets();
	bullets_surrounding();
	player_foes(player, "You have been touched by foe... It means you die... Instantly.", window);
	player_enemy_bullets(player, "You've tried your best, but... it was not enough.", window);
	player_surrounding(cam);
	//void foes_foes();
	foes_surrounding();
}

void organiser::drawAllElements(ShaderProgram* sp, glm::vec4 ppos)
{
	for (int i = 0; i < foes.size(); i++)
	{
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(foes[i].getPos()));
		foes[i].draw();
		shoot(&foes[i], ppos);
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
void organiser::shoot(Foe * foe, glm::vec4 playerPos)
{
	glm::vec4 dist = playerPos - foe->getPos()[3]; // computing position in eye space
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsedTime = now - foe->lS();

	if (glm::length(dist) < 20 && elapsedTime.count() >= 2.f)
	{
		foe->setlS(now);

		glm::vec4 dir = glm::normalize(dist);

		glm::mat4 pos = foe->getPos();

		float angleY = glm::orientedAngle(glm::normalize(glm::vec2(pos[2].x, pos[2].z)), glm::normalize(glm::vec2(dir.x, dir.z)));

		// angles by which projectile will be rotated to start facing the camera

		pos = glm::rotate(pos, -angleY, glm::vec3(0.f, 1.f, 0.f));

		float angleX = glm::orientedAngle(glm::normalize(glm::vec2(pos[2].y, pos[2].z)), glm::normalize(glm::vec2(dir.y, dir.z)));

		pos = glm::rotate(pos, -angleX, glm::vec3(1.f, 0.f, 0.f));

		pos = glm::translate(pos, 10.f * glm::vec3(glm::normalize(glm::transpose(pos) * dir)));

		addFoeBullet(Projectile(foe->returnBullet(), pos, dir, 2.f, 5.f));
	}
}

void organiser::hit_the_player()
{
	player_attributes.player_health--;
}
bool organiser::is_player_alive()
{
	if (player_attributes.player_health) return true;
	else return false;
}
void organiser::death(const char* output_text, GLFWwindow* window)
{
	std::cout << output_text;
	glfwSetWindowShouldClose(window, 1);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void organiser::player_enemy_bullets(glm::vec3 player, const char* output_text, GLFWwindow* window)
{
	this->player.hitbox.sphere.Middle.X = player[0];
	this->player.hitbox.sphere.Middle.Y = player[1];
	this->player.hitbox.sphere.Middle.Z = player[2];
	auto bit = foe_bullets.begin();
	while (bit < foe_bullets.end())
	{
		bullet.hitbox.sphere.Middle.X = (*bit).getPosX();
		bullet.hitbox.sphere.Middle.Y = (*bit).getPosY();
		bullet.hitbox.sphere.Middle.Z = (*bit).getPosZ();
		if (detector.detector(bullet, this->player, nullptr))
		{
			bit = foe_bullets.erase(bit);
			hit_the_player();
			if (!is_player_alive())
			{
				death(output_text, window);
			}
		}
		else
		{
			bit++;
		}
	}
}

void organiser::player_foes(glm::vec3 player, const char* output_text, GLFWwindow* window)
{
	this->player.hitbox.sphere.Middle.X = player[0];
	this->player.hitbox.sphere.Middle.Y = player[1];
	this->player.hitbox.sphere.Middle.Z = player[2];
	auto fit = foes.begin();
	while (fit < foes.end())
	{
		foes_hitbox.hitbox.sphere.Middle.X = (*fit).getPosX();
		foes_hitbox.hitbox.sphere.Middle.Y = (*fit).getPosY();
		foes_hitbox.hitbox.sphere.Middle.Z = (*fit).getPosZ();
		if (detector.detector(foes_hitbox, this->player, nullptr))
		{
			hit_the_player();
			if (!is_player_alive())
			{
				death(output_text, window);
			}
		}
		else
		{
			fit++;
		}
	}
}

void organiser::player_surrounding(Camera* cam)
{
	glm::vec3 tmpos = cam->getPos();
	this->player.hitbox.sphere.Middle.X = tmpos.x;
	this->player.hitbox.sphere.Middle.Y = tmpos.y;
	this->player.hitbox.sphere.Middle.Z = tmpos.z;
	for (int i = 0; i < surrounding.size(); i++)
	{
		float shift[4];
		if (detector.detector(this->player, surrounding[i], shift))
		{
			float speed = cam->getCamSpeed();
			shift[0] /= shift[3];
			shift[1] /= shift[3];
			shift[2] /= shift[3];
			if (detector.distance(Point(shift[0], shift[1], shift[2]), Point()) > speed)
			{
				float l = detector.distance(Point(shift[0], shift[1], shift[2]), Point());
				shift[0] /= l;
				shift[1] /= l;
				shift[2] /= l;
				shift[0] *= speed * 1.1f;
				shift[1] *= speed * 1.1f;
				shift[2] *= speed * 1.1f;
			}
			cam->UpdatePos(shift);
			this->player.hitbox.sphere.Middle.X += shift[0];
			this->player.hitbox.sphere.Middle.Y += shift[1];
			this->player.hitbox.sphere.Middle.Z += shift[2];
		}
	}
}

void organiser::foes_surrounding()
{
	for (int i = 0; i < foes.size(); i++)
	{
		foes_hitbox.hitbox.sphere.Middle.X = foes[i].getPosX();
		foes_hitbox.hitbox.sphere.Middle.Y = foes[i].getPosY();
		foes_hitbox.hitbox.sphere.Middle.Z = foes[i].getPosZ();
		for (int j = 0; j < surrounding.size(); j++)
		{
			float shift[4];
			if (detector.detector(foes_hitbox, surrounding[i], shift))
			{
				float speed = foes[i].getSpeed();
				shift[0] /= shift[3];
				shift[1] /= shift[3];
				shift[2] /= shift[3];
				if (detector.distance(Point(shift[0], shift[1], shift[2]), Point()) > speed)
				{
					float l = detector.distance(Point(shift[0], shift[1], shift[2]), Point());
					shift[0] /= l;
					shift[1] /= l;
					shift[2] /= l;
					shift[0] *= speed * 1.1f;
					shift[1] *= speed * 1.1f;
					shift[2] *= speed * 1.1f;
				}
				foes[i].updatePos(shift);
				foes_hitbox.hitbox.sphere.Middle.X = foes[i].getPosX();
				foes_hitbox.hitbox.sphere.Middle.Y = foes[i].getPosY();
				foes_hitbox.hitbox.sphere.Middle.Z = foes[i].getPosZ();
			}
		}
	}
}