#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../headers/grains.h"
#include "../headers/random.h"


using namespace std;


Grains::Grains()
{
    this->set_grains_num();
    this->set_friction();
    this->set_random_generator();
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
    cout << endl;

    if((friction >= 0) && (friction <= 1))
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


void set_initial_grains()
{
    int distribution;
    int seed;

    cout << "Define radius distribution:" << endl;
    cout << "0 - gaussian" << endl << "1 - uniform"<< endl;
    cout << "2 - bidisperse" << endl << "3 - monodisperse" << endl;
    cin >> distribution;
    cout << endl;

    cout << "Insert desired seed: ";
    cin >> seed;
    cout << endl;

    switch (distribution)
    {
    case 0:
        Random rand_func = Random(seed).gauss_rand();
        break;
    
    default:
        break;
    }

    
}




