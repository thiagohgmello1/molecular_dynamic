#include <vector>
#include <array>


class Grain
{
private:
    float radius;
    std::array<float, 2> s = {0, 0};
    std::array<float, 2> v = {0, 0};
    std::array<float, 2> a = {0, 0};
    int id;
    std::vector<int> neighbors;

public:
    Grain(float radius, int id);
    void set_s(std::array<float, 2> s);
    void set_v(std::array<float, 2> v);
    void set_a(std::array<float, 2> a);
    void set_neighbors(std::vector<int> neighbors);

    float get_radius() const;
    std::array<float, 2> get_s() const;
    std::array<float, 2> get_v() const;
    std::array<float, 2> get_a() const;
    int get_id() const;
    std::vector<int> get_neighbors() const;
};