#include "stdafx.h"
#include "SJRandom.h"


random_device CSJRandom::RD;
mt19937 CSJRandom::Mt = mt19937(RD());

CSJRandom::CSJRandom()
{
}


CSJRandom::~CSJRandom()
{
}
