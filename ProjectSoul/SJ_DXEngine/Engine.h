#pragma once

class Engine 
{
public:
	template<typename OBJ>
	static OBJ* CreateTypeObject()
	{
		OBJ* NewObj = new OBJ();
		NewObj->InitType();
		return NewObj;
	}


	template<typename RES>
	static RES* CreateResourceObject()
	{
		RES* NewRes = new RES();
		NewRes->TypeInit();
		return NewRes;
	}

	template<typename RES,typename RESDATA>
	static RES* CreateResourceObject(const RESDATA& _Data)
	{
		RES* NewRes = new RES(_Data);
		NewRes->TypeInit();
		return NewRes;
	}

	template<typename RES>
	static RES* CreateGameObject()
	{
		RES* NewRes = new RES();
		NewRes->TypeInit();
		return NewRes;
	}

	template<typename RES,typename PARA>
	static RES* CreateGameObject(PARA* _P)
	{
		RES* NewRes = new RES(_P);
		NewRes->TypeInit();
		return NewRes;
	}
};

