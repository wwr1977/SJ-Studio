#pragma once

class CSJRandom
{
	static random_device RD;
	static mt19937 Mt;

public:
	static float RandomFloat() 
	{
		return (float)Mt();
	}
	static float RandomFloat(float _Min, float _Max) 
	{
		if (_Min > _Max)
		{
			float temp = _Min; _Min = _Max; _Max = temp;
		}
		uniform_real_distribution<float> FUID = uniform_real_distribution<float>(_Min,_Max);
		return FUID(Mt);
	}
	static float RandomFloat(float _Max) 
	{
		if (_Max < 0) 
		{
			_Max = CSJMath::ABS(_Max);
		}
		uniform_real_distribution<float> FUID = uniform_real_distribution<float>(0.0f,_Max);
		return FUID(Mt);
	}
	static int RandomInt() 
	{
		return Mt();
	}
	static int RandomInt(int _Min, int _Max) 
	{
		if (_Min > _Max) 
		{
			int temp = _Min; _Min = _Max; _Max = temp;
		}
		uniform_int_distribution<int> UID = uniform_int_distribution<int>(_Min, _Max);
		return UID(Mt);
	}
	static int RandomInt(int _Max) 
	{
		if (0 > _Max)
		{
			_Max = CSJMath::ABS(_Max);
		}

		uniform_int_distribution<int> UID = uniform_int_distribution<int>(0, _Max);
		return UID(Mt);
	}

	template<typename T>
	static void ShuffleVector(T& Temp)
	{
		if (Temp.size() <= 0)
			return;
		
		std::shuffle(Temp.begin(), Temp.end(), Mt);
	}
	
	

private:
	CSJRandom();
	~CSJRandom();
};

