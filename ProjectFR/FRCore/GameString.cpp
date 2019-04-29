#include "stdafx.h"
#include "GameString.h"


const int CGameString::StringToInteger(const tstring& _String)
{
	tstring IntString = _String;
	int Sign = 1;
	int Value = 0;
	char Zero = '0';
	char Nine = '9';

	if (IntString[0] == '-')
	{
		Sign = -1;
		IntString.replace(0, 1, _T(""));
	}

	size_t Pow = IntString.find_first_of(_T("."));

	// -,.ó���� ���ڰ� �ƴ� ���ڰ� ������ ���ü� ���� ���� �����ϵ��� ����
	for (size_t i = 0; i < IntString.size(); i++)
	{
		if (Zero > IntString[i] || Nine < IntString[i]) 
		{		
			IntString.replace(i, 1, _T(""));
		}
	}

	int Base = (int)pow(10.0f, (Pow - 1));

	for (size_t i = 0; i < IntString.size(),Base >0; i++,Base /= 10)
	{
		int value = (int)IntString[i];
		value -= '0';
		Value += value * Base;
	}

	return Value*Sign;

}
const float CGameString::StringToFloat(const tstring& _String)
{
	tstring FloatString = _String;

	float Sign = 1.0f;
	float Value = 0.0f;

	if (FloatString == _T("") || FloatString == _T("-")) 
	{
		return 0.0f;
	}

	if (FloatString[0] == '-')
	{
		Sign = -1.0f;
		FloatString.replace(0, 1, _T(""));
	}

	size_t LastIndex = FloatString.size() - 1;
	if (FloatString[LastIndex] == '.')
	{
		FloatString.replace(LastIndex, 1, _T(""));
	}

	size_t Pow = (int)FloatString.find('.');

	if (-1 == Pow)
	{
		Pow = FloatString.size();
	}
	else
	{
		FloatString.replace(Pow, 1, _T(""));
	}

	float Base = powf(10.0f, (float)(Pow - 1));
	
	if (FloatString == _T("")) 
	{
		return 0.0f;
	}
	for (int i = 0; i < (int)FloatString.size() && Base >=0.0000001f; ++i, Base /= 10.0f)
	{
		int value = (int)FloatString[i];
		value -= (int)'0';
		Value += value * Base;
	}
	
	return Value * Sign;
}
//void CGameString::ChangeFloatString(CString& _String)
//{
//	char Zero = '0';
//	char Nine = '9';
//
//
//	for (int i = 0; i < _String.GetLength(); ++i)
//	{
//		// ��ȣ�� �׻� ���ڿ��� ���� �κп��� ���� ����
//		if (_String[i] == '-' && 0 == i)
//		{
//			continue;
//		}
//		if (_String[i] == '.')
//		{
//			// ù��° �ڸ��� .�� ����� . ����
//			if (i == 0)
//			{
//				_String.Delete(i, 1);
//				continue;
//			}
//			// .�տ� -�� �����Ұ�� . ����
//			if (_String[i - 1] == '-')
//			{
//				_String.Delete(i, 1);
//				continue;
//			}
//			// �ڱ� �տ� .�� �̹� �����Ѵٸ� .����
//			for (int j = 0; j < i; j++)
//			{
//				if (_String[j] == '.')
//				{
//					_String.Delete(i, 1);
//					break;
//				}
//			}
//			continue;
//		}
//		if (_String[i] < Zero || _String[i] > Nine)
//		{
//			_String.Delete(i, 1);
//		}
//	}
//
//	return;
//}
void CGameString::ChangeFloatString(tstring& _String)
{
	char Zero = '0';
	char Nine = '9';


	for (int i = 0; i < (int)_String.size(); ++i)
	{
		// ��ȣ�� �׻� ���ڿ��� ���� �κп��� ���� ����
		if (_String[i] == '-' && 0 == i)
		{
			continue;
		}
		if (_String[i] == '.')
		{
			// ù��° �ڸ��� .�� ����� . ����
			if (i == 0)
			{
				_String.replace(i, 1,_T(""));
				continue;
			}
			// .�տ� -�� �����Ұ�� . ����
			if (_String[i - 1] == '-')
			{
				_String.replace(i, 1,_T(""));
				continue;
			}
			// �ڱ� �տ� .�� �̹� �����Ѵٸ� .����
			for (int j = 0; j < i; j++)
			{
				if (_String[j] == '.')
				{
					_String.replace(i, 1,_T(""));
					break;
				}
			}
			continue;
		}
		if (_String[i] < Zero || _String[i] > Nine)
		{
			_String.replace(i, 1,_T(""));
		}
	}

	return;
}