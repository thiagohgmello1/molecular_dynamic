#include "random.h"

#define NORMALIZE (int)1073741824
#define A (int)883314861
#define B (int)453816693


Random::Random(int seed)
{
    this->seed = seed;
}


int Random::int_rand()
{
    return 0;
}


float Random::rand()
{
    double norm = 0.5/(NORMALIZE);
    double random_num = 0;

    this->seed = this->seed * A + B;

    if (this->seed < 0)
    {
        this->seed = (this->seed + NORMALIZE) + NORMALIZE;
    }

    random_num = this->seed * norm;

    return random_num;
}


float Random::gauss_rand()
{
    float sum = 0;
    float aux = 0;

      for (int i = 1; i < 1000; i++)
      {
          sum = sum + Random::rand();
      }

      aux = (sum - 500.0) / 83.25;
      aux = (aux + 0.4084) / 0.8585;

      return aux;
}


float Random::uni_rand()
{
    return Random::rand();
}


float Random::bi_rand()
{
    float aux = Random::rand();

    if (aux > (4.0 / 7.0))
    {
        return 0.0;
    }
    else
    {
        return 1.0;
    }
}


float Random::mono_rand()
{
    return 1.0;
}
