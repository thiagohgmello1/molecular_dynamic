#include "../headers/grain.h"
#include "../headers/random.h"


Grain::Grain(int id, float radius, float mass)
{
    this->set_id(id);
    this->set_mass(mass);
    this->set_radius(radius);
}


int Grain::get_id() const
{
    return this->id;
}


void Grain::set_id(int id)
{
    this->id = id;
}


float Grain::get_mass() const
{
    return this->mass;
}


void Grain::set_mass(float mass)
{
    this->mass = mass;
}


float Grain::get_radius() const
{
    return this->radius;
}


void Grain::set_radius(float radius)
{
    this->radius = radius;
}


std::array<float, 3> Grain::get_s() const
{
    return this->s;
}


void Grain::set_s(std::array<float, 3> s)
{
    this->s = s;
}


std::array<float, 3> Grain::get_v() const
{
    return this->v;
}


void Grain::set_v(std::array<float, 3> v)
{
    this->v = v;
}


std::array<float, 3> Grain::get_a() const
{
    return this->a;
}


void Grain::set_a(std::array<float, 3> a)
{
    this->a = a;
}


std::vector<int> Grain::get_neighbors() const
{
    return this->neighbors;
}


void Grain::set_neighbors(std::vector<int> neighbors)
{
    this->neighbors = neighbors;
}