#include "../headers/grain.h"
#include "../headers/random.h"


Grain::Grain(float radius, int id)
{
    this->radius = radius;
    this->id = id;
}

std::array<float, 2> Grain::get_s() const
{
    return this->s;
}


std::array<float, 2> Grain::get_v() const
{
    return this->v;
}


std::array<float, 2> Grain::get_a() const
{
    return this->a;
}


float Grain::get_radius() const
{
    return this->radius;
}


int Grain::get_id() const
{
    return this->id;
}


std::vector<int> Grain::get_neighbors() const
{
    return this->neighbors;
}


void Grain::set_s(std::array<float, 2> s)
{
    this->s = s;
}


void Grain::set_v(std::array<float, 2> v)
{
    this->v = v;
}


void Grain::set_a(std::array<float, 2> a)
{
    this->a = a;
}


void Grain::set_neighbors(std::vector<int> neighbors)
{
    this->neighbors = neighbors;
}