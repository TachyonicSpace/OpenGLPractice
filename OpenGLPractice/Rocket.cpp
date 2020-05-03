#include "Rocket.h"
#include <iostream>



Rocket::Rocket():Pos(startingPos), vel(startingVel), acc(startingAcc)//, r(Rand(0, 1)), g(Rand(0, 1)), b(Rand(0, 1))
{
}

Rocket::Rocket(float width, float height) :Pos(startingPos), vel(startingVel), acc(startingAcc), width(width), height(height), r(Rand(0, 1)),
	g(Rand(0, 1)), b(Rand(0, 1))
{
}

Rocket::Rocket(DNA _dna) :Pos(startingPos), vel(startingVel), acc(startingAcc), dna(_dna)/*, r(Rand(0, 1)),
	g(Rand(0, 1)), b(Rand(0, 1))*/
{
}

Rocket::~Rocket()
{
	//remove(this->target);
}





auto Rocket::applyForce(vec2 force)
{
	acc += force;
}

bool Rocket::update(vec4 *barrier)
{	
	if (Pos.x < 0 || Pos.x > Renderer::width || Pos.y < 0 || Pos.y > Renderer::height) {
		return false;
	}

	if (barrier && Pos.x > barrier->x - barrier->z/2.0 && Pos.x < barrier->x + barrier->z/2 &&
		Pos.y > barrier->y && Pos.y < barrier->y + barrier->w) {
		return false;
	}

	if (distance(Pos, vec2(target.x, target.y)) > 10) {
		try {
			setMag(dna.genes[count], .1);
			this->applyForce(dna.genes[count]);


			vel += acc;

			Pos += vel;
			angle = atan2(vel.y, vel.x);
			acc *= 0;
			return true;
		}
		catch (...) {
			return true;
		}
	} return true;
}

float Rocket::heading()
{
	return atan2(vel.x, vel.y);
}

int Rocket::count;
float Rocket::mutationRate;
float Rocket::mutationpow;

float* Rocket::rect(float width, float height) {
	float rocketRect[] = {
			-width / 2.0, -height / 2.0, 0, 0,
			-width / 2.0,  height / 2.0, 1, 0,
			 width / 2.0,  height / 2.0, 1, 1,
			 width / 2.0, -height / 2.0, 0, 1
	};
	float* heapRocketRect = new float[sizeof(rocketRect)]();
	for (int i = 0; i < sizeof(rocketRect); i++) {
		heapRocketRect[i] = rocketRect[i];
	}
	return heapRocketRect;
}

vec3 Rocket::pos(){
	return vec3(Pos.x, Pos.y, 0);
}

float Rocket::fitness(vec2* position)
{
	if (position == nullptr)
		position = &Pos;

	if (distance(*position, vec2(target.x, target.y)) > 10) {
		auto d = distance(*position, vec2(target.x, target.y));
		d = Renderer::width - d;

		d = pow(d / 200.f, 3) + 1;
		this->Fitness = d;
		target.z = count;
	}if ((*position).x < 0 || (*position).x > Renderer::width || (*position).y < 0 || (*position).y > Renderer::height) {
		Fitness += -10;
	}
	else if(distance(*position, vec2(target.x, target.y)) <= 10)
		Fitness = 2000 * (DNA::lifespan-target.z);
	return this->Fitness;
}

float Rocket::fitness(vec4* barrier)
{
	if (barrier && Pos.x > barrier->x - barrier->z / 2.0 && Pos.x < barrier->x + barrier->z / 2 &&
		Pos.y > barrier->y && Pos.y < barrier->y + barrier->w) {
		return 1;
	}
}

Rocket Rocket::crossover(const Rocket& b)
{
	auto child = this->dna;
	int middle = Rand(DNA::lifespan, 0);
	for (int i = middle; i < DNA::lifespan; i++) {
		child.genes[i] = b.dna.genes[i];
		float mutationChance = Rand(1, -1);
		if (abs(mutationChance) < mutationRate)
			child.genes[i] += vec2((pow(mutationpow, mutationChance) - 1));
	}
	Rocket childRocket(child);
	childRocket.r = (Rand(0, 1) > .5) ? this->r : b.r;
	childRocket.g = (Rand(0, 1) > .5) ? this->g : b.g;
	childRocket.b = (Rand(0, 1) > .5) ? this->b : b.b;

	float mutationChance = Rand(1, -1);
	if (abs(mutationChance) < mutationRate) {
		childRocket.r += (pow(mutationpow, mutationChance) - 1);
		childRocket.r = clamp(childRocket.r, 0.f, 1.f);
	}

	mutationChance = Rand(1, -1);
	if (abs(mutationChance) < mutationRate) {
		childRocket.g += (pow(mutationpow, mutationChance) - 1);
		childRocket.g = clamp(childRocket.g, 0.f, 1.f);
	}

	mutationChance = Rand(1, -1);
	if (abs(mutationChance) < mutationRate) {
		childRocket.b += (pow(mutationpow, mutationChance) - 1);
		childRocket.b = clamp(childRocket.b, 0.f, 1.f);
	}

	/*mutationChance = Rand(1, -1);
	if (abs(mutationChance) < mutationRate) {
		childRocket.mutationRate += (pow(mutationpow, mutationChance) - 1);
		childRocket.mutationRate = clamp(childRocket.mutationRate, 0.0001f, .9999f);
	}*/

	return childRocket;
}

Rocket& Rocket::operator=(const Rocket& R)
{
	if (&R == this) {
	}
	this->dna = R.dna;
	this->Pos = R.startingPos;
	this->vel = R.startingVel;
	this->acc = R.startingAcc;
	this->r = R.r;
	this->g = R.g;
	this->b = R.b;
	this->mutationRate = R.mutationRate;
	this->width = R.width;
	this->height = R.height;

	return *this;
}

float* Rocket::rect()
{

	float rocketRect[] = {
			-width/2.0, -height / 2.0, 0, 0,
			-width/2.0,  height / 2.0, 1, 0,
			 width/2.0,  height / 2.0, 1, 1,
			 width/2.0, -height / 2.0, 0, 1
	}; 
	float* heapRocketRect = new float[sizeof(rocketRect)]();
	for (int i = 0; i < sizeof(rocketRect); i++) {
		heapRocketRect[i] = rocketRect[i];
	}
	return heapRocketRect;
}
