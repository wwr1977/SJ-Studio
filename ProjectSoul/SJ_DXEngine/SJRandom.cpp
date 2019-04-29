
#include "SJRandom.h"


std::random_device CSJRandom::RD;
std::mt19937 CSJRandom::Mt = std::mt19937(RD());

CSJRandom::CSJRandom()
{
}


CSJRandom::~CSJRandom()
{
}
