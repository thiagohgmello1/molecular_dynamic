#include <vector>
#include "grain.h"
#include "random.h"


class Grains
{
private:
    int grains_num = 0;
    float friction = 0;
    float min_radius = 0;
    float max_radius = 0;
    float sample_time = 0;
    std::vector<Grain> grains;

public:
    Grains();

    void set_grains_num();
    void set_friction();
    void set_min_radius();
    void set_max_radius();
    void set_sample_time(float sample_time);
    void set_initial_grains();

    int get_grains_num() const;
    float get_friction() const;
    float get_min_radius() const;
    float get_max_radius() const;
    float get_sample_time() const;
    std::vector<Grain> get_grains() const;

    auto define_radius_dist();
    void specific_grain(float radius, std::array<float, 3> s, std::array<float, 3> v, std::array<float, 3> a);
};