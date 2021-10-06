#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../headers/grains.h"


using namespace std;


Grains::Grains()
{
    this->set_grains_num();
    this->set_min_radius();
    this->set_max_radius();
    this->set_friction();
    this->set_initial_grains();
    // TODO Falta adicionar algumas chamadas de métodos como cálculo do periodo, instanciar os graos
}


int Grains::get_grains_num() const
{
    return this->grains_num;
}


void Grains::set_grains_num()
{
    int grains_num = 0;

    cout << "Insert number of desired grains: ";
    cin >> grains_num;
    cin.ignore();
    cout << endl;

    if(grains_num > 0)
    {
        this->grains_num = grains_num;
    }
    else
    {
        cout << "Invalid grains number." << endl;
        exit(1);
    }
}


float Grains::get_friction() const
{
    return this->friction;
}


void Grains::set_friction()
{
    int friction = 0;

    cout << "Insert desired friction: ";
    cin >> friction;
    cin.ignore();
    cout << endl;

    if(0 <= friction <= 1)
    {
        this->friction = friction;
    }
    else
    {
        cout << "Invalid friction coefficient." << endl;
        exit(1);
    }
}


float Grains::get_min_radius() const
{
    return this->min_radius;
}


void Grains::set_min_radius()
{
    float min_radius = 0;

    cout << "Insert minimum radius: ";
    cin >> min_radius;
    cin.ignore();
    cout << endl;
    
    if(min_radius > 0)
    {
        this->min_radius = min_radius;
    }
    else
    {
        cout << "Invalid minimum radius." << endl;
        exit(1);
    }
}


float Grains::get_max_radius() const
{
    return this->max_radius;
}


void Grains::set_max_radius()
{
    float max_radius = 0;

    cout << "Insert maximum radius: ";
    cin >> max_radius;
    cin.ignore();
    cout << endl;
    
    if((max_radius > 0) && (max_radius > this->min_radius))
    {
        this->max_radius = max_radius;
    }
    else
    {
        cout << "Invalid maximum radius." << endl;
        exit(1);
    }
}


float Grains::get_sample_time() const
{
    return this->sample_time;
}


void Grains::set_sample_time(float sample_time)
{
    //TODO Consider insert specific sample time or calculated sample time
    // (based on 1/2 of critical damping or 1/10 max grains speed)
}


auto Grains::define_radius_distribution()
{
    int distribution;
    float (Random::*ptr_random)() = NULL;

    cout << "Define radius distribution:" << endl;
    cout << "0 - gaussian" << endl << "1 - uniform"<< endl;
    cout << "2 - bidisperse" << endl << "3 - monodisperse" << endl;
    cin >> distribution;
    cin.ignore();
    cout << endl;

    switch (distribution)
    {
        case 0:
            ptr_random = &Random::gauss_rand;
            break;
        case 1:
            ptr_random = &Random::uni_rand;
            break;
        case 2:
            ptr_random = &Random::bi_rand;
            break;
        case 3:
            ptr_random = &Random::mono_rand;
            break;
        default:
            cout << "Invalid choice." << endl;
            exit(1);
    }
    return ptr_random;
}


void Grains::set_initial_grains()
{
    //TODO Create logic process to define mass
    int seed;
    float radius = 0;
    float mass = 0;
    float dr = this->max_radius - this->min_radius;
    float min_radius = 1E10;
    float max_radius = 0;
    
    float (Random::*ptr_random)() = Grains::define_radius_distribution();

    cout << "Insert desired seed: ";
    cin >> seed;
    cout << endl;

    Random rand_gen = Random(seed);

    for(int id=0; id <= (this->grains_num - 1); id++)
    {
        radius = this->min_radius + dr * (rand_gen.*ptr_random)();
        mass = radius;

        min_radius = (radius < min_radius) ? radius : min_radius;
        max_radius = (radius > max_radius) ? radius : max_radius;

        grains.push_back(Grain(id, radius, mass));
    }
    this->min_radius = min_radius;
    this->max_radius = max_radius;
    cout << "Minimum radius: " << this->min_radius << endl;
    cout << "Maximum radius: " << this->max_radius << endl;
}


std::vector<Grain> Grains:: get_grains() const
{
    for(Grain grain:this->grains)
    {
        cout << "ID: " << grain.get_id() << endl;
        cout << "Radius: " << grain.get_radius() << endl;
        cout << "-------" << endl;
    }
    return this->grains;
}


void Grains::add_specific_grain(float radius, std::array<float, 3> s, std::array<float, 3> v, std::array<float, 3> a)
{
    float mass = radius;
    int id = this->grains.size() + 1;

    Grain grain = Grain(id, radius, mass);
    grain.set_s(s);
    grain.set_v(v);
    grain.set_a(a);

    this->grains.push_back(grain);
}


vector<array<float, 3>> Grains::get_positions() const
{
    vector<array<float, 3>> positions;

    for(Grain grain:this->grains)
    {
        positions.push_back(grain.get_s());
    }
    return positions;
}


void Grains::set_positions()
{

}


vector<array<float, 3>> Grains::get_speeds() const
{
    vector<array<float, 3>> speeds;

    for(Grain grain:this->grains)
    {
        speeds.push_back(grain.get_v());
    }
    return speeds;
}


void Grains::set_speeds()
{

}


vector<array<float, 3>> Grains::get_accelerations() const
{
    vector<array<float, 3>> accelerations;

    for(Grain grain:this->grains)
    {
        accelerations.push_back(grain.get_a());
    }
    return accelerations;
}


void Grains::set_accelerations()
{

}