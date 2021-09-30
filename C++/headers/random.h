class Random
{
private:
    int seed;

public:
    Random(int s = 123);

    int int_rand();
    float rand();
    float gauss_rand();
    float uni_rand();
    float bi_rand();
    float mono_rand();
};