#ifndef FUNCS
#define FUNCS
class time
{
    public:
        
};

class noise
{
    public:
    unsigned int RNG(unsigned int high)
    {
        unsigned int seed = getTimeMicrosec() * numOfRNGCalls;
        numOfRNGCalls += 1;
        for (int i = 0; i < 3; i++)
        {
          if (!(seed % 2))
          {
            seed /= 2;
          }
          else
          {
            seed *= 3;
            seed += 1;
          }
        }
        return (seed % high);
    }
};

#endif