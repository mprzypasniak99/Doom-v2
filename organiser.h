/*
Header for class that will keep all rendering and collision objects and handle collisions for specific objects, updating and drawing them.
version 0.8
*/
#pragma once
#ifndef ORGANISER_H
#define ORGANISER_H

#include <Foe.h>
#include "camera.hpp"
#include "coldet.h"
#include "shaderprogram.h"

class organiser
{
private:
	struct
	{
		int player_health = 8;
	} player_attributes;
	ColDet detector;
	Hitbox player;
	std::vector<Foe> foes; //vector with (obviously) foes objects
	Hitbox foes_hitbox; //while we have one type of foes we don't need a hitbox for every foe, updating position is enough
	std::vector<Projectile> player_bullets; //another pretty obvious vector
	std::vector<Projectile> foe_bullets; //and one more
	Hitbox bullet; //same thing like with foes, but here we have same type of bullets for foes and player, so one hitbox is just enough
	std::vector<Hitbox> surrounding; //surrounding won't be moving, so we don't have to keep them as objects, but they can vary, so every one have to be kept in vector
	//go to collisionHandling if you want to know why those functions are private
	void player_foes(glm::vec3 player, const char* output_text, GLFWwindow* window); //under construction 
	void player_enemy_bullets(glm::vec3 player, const char* output_text, GLFWwindow* window); //collisons between player and enemy bullets
	void player_surrounding(Camera* cam); //done, but NOT tested, do it Majkel
	//void foes_foes(); under construction
	void foes_player_bullets(); //collison handling between foes and player bullets
	void foes_foe_bullets(); //foes may hit each other so we have to handle this collision (only if don't want foe bullets fly through foes)
	void foes_surrounding(); //under construction
	void bullets_surrounding(); //destroying bullets after hitting surrounding (but creating penetrable surrounding is pretty simple i think)
	void hit_the_player(); //we don't want player to be invincible
	bool is_player_alive();
	void death(const char* output_text, GLFWwindow* window); //when you have contact with foe, you die
	void shoot(Foe * foe, glm::vec4 playerPos); //experimental try to debug program
public:
	organiser(Hitbox player, Hitbox foe, Hitbox bullet);
	void addFoe(Foe foe);
	void addPlayerBullet(Projectile bullet);
	void addFoeBullet(Projectile bullet);
	void addSurroundingElement(Hitbox element);
	void positionUpdate(glm::mat4 V); //Updating all elements positions
	void collisionsHandling(glm::vec3 player, GLFWwindow* window, Camera* cam); //One Function to rule them all, One Function to find them, One Function to bring them all, and in the darkness bind them,
	void drawAllElements(ShaderProgram* sp, glm::vec4 ppos); //if you like drawing, it's function for you
	void addRouteForFoe(int fn, glm::vec4 point); //go to Foe -- addRoutePoint, fn is just foe number in the vector
};
#endif
