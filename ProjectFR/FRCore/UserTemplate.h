#pragma once
template<typename Value, typename Key, typename Map>
SPTR<Value> FINDMAP_SPTR(Key _Key, Map _Map)
{
	Map::iterator FindIter = _Map.find(_Key);

	if (_Map.end() == FindIter) 
	{
		return nullptr;
	}

	return FindIter->second;
}
template<typename Value, typename Key, typename Map>
Value* FINDMAP_PTR(Key _Key, Map _Map)
{
	Map::iterator FindIter = _Map.find(_Key);

	if (FindIter == _Map.end())
	{
		return nullptr;
	}

	return FindIter->second;
}
template<typename Value, typename Key, typename Map>
Value FINDMAP_VALUE(Key _Key, Map _Map)
{
	Map::iterator FindIter = _Map.find(_Key);

	if (FindIter == _Map.end())
	{
		return Value();
	}

	return FindIter->second;
}

template<typename Value, typename Key, typename Map>
BOOL EXISTMAP_SPTR(Key _Key, Map _Map)
{
	SPTR<Value> Find = FINDMAP_SPTR<Value>(_Key, _Map);

	if (nullptr == Find)
	{
		return FALSE;
	}
	return TRUE;
}

template<typename T>
void SAFE_DELETE_MAP(T _Map)
{
	T::iterator Start = _Map.begin();
	T::iterator End = _Map.end();

	for (; Start != End; ++Start)
	{
		SAFE_DELETE(Start->second)
	}

	_Map.clear();
}

template<typename T>
void ALLCLEAR_UMAP(T _UMap)
{
	T::iterator Start = _UMap.begin();
	T::iterator End = _UMap.end();

	for (; Start != End; ++Start)
		Start->second.clear();
	

	_UMap.clear();
}

template<typename T>
void SAFE_DELETE_VECTOR(T _Vec)
{
	for (size_t i = 0; i < _Vec.size(); i++)
	{
		SAFE_DELETE(_Vec[i])
	}

	_Vec.clear();
}

template<typename T>
void SAFE_DELETE_STACK(T _STACK)
{
	for (size_t i = 0; i < _STACK.size(); i++)
	{
		_STACK.pop();
	}

}
template<typename T>
void SAFE_DELETE_QUEUE(T _QUEUE)
{
	for (size_t i = 0; i < _QUEUE.size(); i++)
	{
		_QUEUE.pop();
	}

}
