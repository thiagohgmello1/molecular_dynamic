#include <iostream>
#include <array>
#include <vector>
#include "random.h"
#include "grain.h"

using namespace std;


int main(){
    Grain g = Grain(0.001, 1);
    array<float, 2> s = {0,1};
    array<float, 2> v = {1,2};
    array<float, 2> a = {2,3};
    vector<int> id = {1, 2, 3};

    g.set_s(s);
    g.set_v(v);
    g.set_a(a);
    g.set_neighbors(id);

    cout << "Posicao: " << g.get_s()[0] << endl;
    cout << "Velocidade: " << g.get_v()[0] << endl;
    cout << "Aceleracao: " << g.get_a()[0] << endl;
    cout << "Raio: " << g.get_radius() << endl;
    cout << "ID: " << g.get_id() << endl;
    cout << "Vizinhos 1:" << endl;
    for (auto i: g.get_neighbors())
    {
        cout << i << endl;
    }
    id = {4, 5, 6, 8, 9, 3};
    g.set_neighbors(id);
    cout << "Vizinhos 2:" << endl;
    for (auto i: g.get_neighbors())
    {
        cout << i << endl;
    }

}