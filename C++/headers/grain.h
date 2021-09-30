#include <vector>
#include <array>


class Grain
{
private:
    int id;
    float mass;
    float radius;
    std::array<float, 3> s = {0, 0, 0};
    std::array<float, 3> v = {0, 0, 0};
    std::array<float, 3> a = {0, 0, 0};
    std::vector<int> neighbors;

public:
    Grain(int id, float radius, float mass);
    
    void set_id(int id);
    void set_mass(float mass);
    void set_radius(float radius);
    void set_s(std::array<float, 3> s);
    void set_v(std::array<float, 3> v);
    void set_a(std::array<float, 3> a);
    void set_neighbors(std::vector<int> neighbors);

    int get_id() const;
    float get_mass() const;
    float get_radius() const;
    std::array<float, 3> get_s() const;
    std::array<float, 3> get_v() const;
    std::array<float, 3> get_a() const;
    std::vector<int> get_neighbors() const;
};