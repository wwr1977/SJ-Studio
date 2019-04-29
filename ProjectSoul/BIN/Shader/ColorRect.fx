
// 상수버퍼라는 것부터 만들어야 한다.
// 그걸 위한 통로도 아직 안만들었지만 
// 우리가 만든 행렬은 버텍스 쉐이더에 들어가서
// 버텍스에 곱해지게 된다.
// b0의 b는 상수버퍼라는 의미이고
// 0은 0번째 슬롯에 넣어주겠다는 의미가 된다.
// 상수버퍼 레지스터 슬롯 개수라고 검색해보면 나옵니다.

cbuffer TransformBuffer : register(b0) 
{
	matrix g_WVP;
}

struct VTXCOL_INPUT 
{
	float4	vPos : POSITION;
	float4  vColor : COLOR;
    float2 vTex : TEXCOORD;
};

struct VTXCOL_OUTPUT 
{
	float4	vPos : SV_POSITION;
	float4  vColor : COLOR;
    float2 vTex : TEXCOORD;
};

struct PS_OUTPUT 
{
	float4 vColor : SV_Target;
};


VTXCOL_OUTPUT VS_ColorRect(VTXCOL_INPUT  _Input) 
{
	VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT)0.f;

	// mul은 쉐이더에서 사용하는 함수로
	// 어떤 행렬이나 혹은 벡터간의 곱샘을 처리해준다.
	// OutData.vPos = mul(float4(float3, 1.f), m_gWVP);
	OutData.vPos = mul(_Input.vPos, g_WVP);
	OutData.vColor = _Input.vColor;
    OutData.vTex = _Input.vTex;

    
    return OutData;
}



bool CheckTri1(float2 _Point)
{
    float pi = 3.141592653f;

    float2 Tri1[3] =
    {
        float2(0.4f * cos(-pi / 6.f) + 0.5f, -(0.4f * sin(-pi / 6.f) - 0.5f))
      , float2(0.4f * cos(pi / 2.f) + 0.5f, -(0.4f * sin(pi / 2.f) - 0.5f))
      , float2(0.4f * cos(pi + (pi / 6.f)) + 0.5f, -(0.4f * sin(pi + (pi / 6.f)) - 0.5f))
    };

    float2 STri1[3] =
    {
        float2(0.37f * cos(-pi / 6.f) + 0.5f, -(0.37f * sin(-pi / 6.f) - 0.5f))
      , float2(0.37f * cos(pi / 2.f) + 0.5f, -(0.37f * sin(pi / 2.f) - 0.5f))
      , float2(0.37f * cos(pi + (pi / 6.f)) + 0.5f, -(0.37f * sin(pi + (pi / 6.f)) - 0.5f))
    };

    // Tri 0 : A  , Tri 1 : B , Tri : C
    float2 VecAB = Tri1[1] - Tri1[0];
    float2 VecBC = Tri1[2] - Tri1[1];
    float2 VecCA = Tri1[0] - Tri1[2];
    
    // 작은 삼각형
    float2 SVecAB = STri1[1] - STri1[0];
    float2 SVecBC = STri1[2] - STri1[1];
    float2 SVecCA = STri1[0] - STri1[2];

    float2 AP = Tri1[0] - _Point;
    float2 BP = Tri1[1] - _Point;
    float2 CP = Tri1[2] - _Point;

    float2 SAP = STri1[0] - _Point;
    float2 SBP = STri1[1] - _Point;
    float2 SCP = STri1[2] - _Point;

    float A = (VecAB.x * AP.y) - (VecAB.y * AP.x);
    float B = (VecBC.x * BP.y) - (VecBC.y * BP.x);
    float C = (VecCA.x * CP.y) - (VecCA.y * CP.x);

    float SA = (SVecAB.x * SAP.y) - (SVecAB.y * SAP.x);
    float SB = (SVecBC.x * SBP.y) - (SVecBC.y * SBP.x);
    float SC = (SVecCA.x * SCP.y) - (SVecCA.y * SCP.x);

    if (A > 0.f && B > 0.f && C > 0.f)
    {
        if (SA > 0.f&& SB > 0.f && SC > 0.f)
            return false;
        if (SA < 0.f && SB < 0.f && SC < 0.f)
            return false;
        return true;
    }

    if (A < 0.f && B < 0.f && C < 0.f)
    {
        if (SA > 0.f && SB > 0.f && SC > 0.f)
            return false;
        if (SA < 0.f && SB < 0.f && SC < 0.f)
            return false;
        return true;

        return true;
    }

    return false;
}

bool CheckTri2(float2 _Point)
{
    float pi = 3.141592653f;

    float2 Tri1[3] =
    {
        float2(0.4f * cos(pi / 6.f) + 0.5f, -(0.4f * sin(pi / 6.f) - 0.5f))
      , float2(0.4f * cos(5.f * pi / 6.f) + 0.5f, -(0.4f * sin(5.f * pi / 6.f) - 0.5f))
      , float2(0.4f * cos(3.f * pi / 2.f) + 0.5f, -(0.4f * sin(3.f * pi / 2.f) - 0.5f))
    };

    float2 STri1[3] =
    {
        float2(0.37f * cos(pi / 6.f) + 0.5f, -(0.37f * sin(pi / 6.f) - 0.5f))
      , float2(0.37f * cos(5.f * pi / 6.f) + 0.5f, -(0.37f * sin(5.f * pi / 6.f) - 0.5f))
      , float2(0.37f * cos(3.f * pi / 2.f) + 0.5f, -(0.37f * sin(3.f * pi / 2.f) - 0.5f))
    };

    // Tri 0 : A  , Tri 1 : B , Tri : C
    float2 VecAB = Tri1[1] - Tri1[0];
    float2 VecBC = Tri1[2] - Tri1[1];
    float2 VecCA = Tri1[0] - Tri1[2];
    
    // 작은 삼각형
    float2 SVecAB = STri1[1] - STri1[0];
    float2 SVecBC = STri1[2] - STri1[1];
    float2 SVecCA = STri1[0] - STri1[2];

    float2 AP = Tri1[0] - _Point;
    float2 BP = Tri1[1] - _Point;
    float2 CP = Tri1[2] - _Point;

    float2 SAP = STri1[0] - _Point;
    float2 SBP = STri1[1] - _Point;
    float2 SCP = STri1[2] - _Point;

    float A = (VecAB.x * AP.y) - (VecAB.y * AP.x);
    float B = (VecBC.x * BP.y) - (VecBC.y * BP.x);
    float C = (VecCA.x * CP.y) - (VecCA.y * CP.x);

    float SA = (SVecAB.x * SAP.y) - (SVecAB.y * SAP.x);
    float SB = (SVecBC.x * SBP.y) - (SVecBC.y * SBP.x);
    float SC = (SVecCA.x * SCP.y) - (SVecCA.y * SCP.x);

    if (A > 0.f && B > 0.f && C > 0.f)
    {
        if (SA > 0.f && SB > 0.f && SC > 0.f)
            return false;
        if (SA < 0.f && SB < 0.f && SC < 0.f)
            return false;
        return true;
    }

    if (A < 0.f && B < 0.f && C < 0.f)
    {
        if (SA > 0.f && SB > 0.f && SC > 0.f)
            return false;
        if (SA < 0.f && SB < 0.f && SC < 0.f)
            return false;
   
        return true;
    }

    return false;
}

bool Granzort(const float2 _Point)
{
    if (CheckTri1(_Point) || CheckTri2(_Point))
        return true;

    return false;
}

bool Fenix(float2 _Point)
{
    float Root3 = sqrt(3.f);
    float InvRoot3 = rsqrt(3.f);
   

    float2 Temp1[4], Temp2[4];
    float2 Arr1[4] =
    {
        float2(0.5f, 0.5f)
      , float2(0.4f * InvRoot3 + 0.5f, 0.5f)
      , float2(0.4f * cos(radians(30.f)) + 0.5f, -(0.4f * sin(radians(30.f)) - 0.5f))
      , float2(0.4f * (InvRoot3 * 0.5f) + 0.5f, -(0.4f * sin(radians(30.f)) - 0.5f))
    };

    float2 RectMid = (Arr1[0] + Arr1[1] + Arr1[2] + Arr1[3]) / 4.f;

    float2 Arr2[4];
    
    Arr2[0] = Arr1[0] * 0.85f + RectMid * 0.15f;
    Arr2[1] = Arr1[1] * 0.85f + RectMid * 0.15f;
    Arr2[2] = Arr1[2] * 0.85f + RectMid * 0.15f;
    Arr2[3] = Arr1[3] * 0.85f + RectMid * 0.15f;
    

    float2 RectVec[4]; // 큰도형 외곽 벡터
    float2 SRectVec[4]; // 작은 도형 외곽벡터
    float2 CheckVec[4]; // 도형의 정점에서 임의의 점으로 향하는 벡터
    float ZVec[4] = { 0.f, 0.f, 0.f, 0.f };
    float SZVec[4] = { 0.f, 0.f, 0.f, 0.f };

    for (int i = 0; i < 4; ++i)
    {
        RectVec[i] = Arr1[(i + 1) % 4] - Arr1[i];
        CheckVec[i] = _Point - Arr1[i];
        ZVec[i] = (RectVec[i].x * CheckVec[i].y) - (RectVec[i].y * CheckVec[i].x);
    }

    for (int j = 0; j < 4; ++j)
    {
        SRectVec[j] = Arr2[(j + 1) % 4] - Arr2[j];
        CheckVec[j] = _Point - Arr2[j];
        SZVec[j] = SRectVec[j].x * CheckVec[j].y - SRectVec[j].y * CheckVec[j].x;
    }
        
    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f && ZVec[3] > 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f && ZVec[3] < 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

    // y = 0.5 축 대칭   y' = 2 * 0.5f - y
    for (int k = 0; k < 4; ++k)
    {
        Temp1[k] = Arr1[k];
        Temp1[k].y = 0.99f - Temp1[k].y;
        Temp2[k] = Arr2[k];
        Temp2[k].y = 0.99f - Temp2[k].y;

    }

    for (int i = 0; i < 4; ++i)
    {
        RectVec[i] = Temp1[(i + 1) % 4] - Temp1[i];
        CheckVec[i] = _Point - Temp1[i];
        ZVec[i] = (RectVec[i].x * CheckVec[i].y) - (RectVec[i].y * CheckVec[i].x);
    }

    for (int j = 0; j < 4; ++j)
    {
        SRectVec[j] = Temp2[(j + 1) % 4] - Temp2[j];
        CheckVec[j] = _Point - Temp2[j];
        SZVec[j] = SRectVec[j].x * CheckVec[j].y - SRectVec[j].y * CheckVec[j].x;
    }
        
    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f && ZVec[3] > 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f && ZVec[3] < 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

     // x = 0.5 축 대칭   x' = 2 * 0.5f - x
    for (int k = 0; k < 4; ++k)
    {
        Temp1[k] = Arr1[k];
        Temp1[k].x = 1.01f - Temp1[k].x;
        Temp2[k] = Arr2[k];
        Temp2[k].x = 1.01f - Temp2[k].x;

    }

    for (int i = 0; i < 4; ++i)
    {
        RectVec[i] = Temp1[(i + 1) % 4] - Temp1[i];
        CheckVec[i] = _Point - Temp1[i];
        ZVec[i] = (RectVec[i].x * CheckVec[i].y) - (RectVec[i].y * CheckVec[i].x);
    }

    for (int j = 0; j < 4; ++j)
    {
        SRectVec[j] = Temp2[(j + 1) % 4] - Temp2[j];
        CheckVec[j] = _Point - Temp2[j];
        SZVec[j] = SRectVec[j].x * CheckVec[j].y - SRectVec[j].y * CheckVec[j].x;
    }
        
    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f && ZVec[3] > 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f && ZVec[3] < 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

     // 점 (0.5,0.5 ) x' = 2 * 0.5f - x  , y' = 2*0.5f - y
    for (int k = 0; k < 4; ++k)
    {
        Temp1[k] = Arr1[k];
        Temp1[k].x = 1.01f - Temp1[k].x;
        Temp1[k].y = 0.99f - Temp1[k].y;
        Temp2[k] = Arr2[k];
        Temp2[k].x = 1.01f - Temp2[k].x;
        Temp2[k].y = 0.99f - Temp2[k].y;

    }

    for (int i = 0; i < 4; ++i)
    {
        RectVec[i] = Temp1[(i + 1) % 4] - Temp1[i];
        CheckVec[i] = _Point - Temp1[i];
        ZVec[i] = (RectVec[i].x * CheckVec[i].y) - (RectVec[i].y * CheckVec[i].x);
    }

    for (int j = 0; j < 4; ++j)
    {
        SRectVec[j] = Temp2[(j + 1) % 4] - Temp2[j];
        CheckVec[j] = _Point - Temp2[j];
        SZVec[j] = SRectVec[j].x * CheckVec[j].y - SRectVec[j].y * CheckVec[j].x;
    }
        
    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f && ZVec[3] > 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f && ZVec[3] < 0.f)
    {
        if (SZVec[0] > 0.f && SZVec[1] > 0.f && SZVec[2] > 0.f && SZVec[3] > 0.f)
            return false;

        if (SZVec[0] < 0.f && SZVec[1] < 0.f && SZVec[2] < 0.f && SZVec[3] < 0.f)
            return false;

        return true;
    }
        return false;
}

bool Poseidon(float2 _Point)
{

    float2 Tri1[3] =
    {
        float2(0.4f * cos(radians(30.f)) + 0.5f, -(0.4f * sin(radians(30.f)) - 0.5f)),
        float2(0.4f * -cos(radians(30.f)) / 3.f + 0.5f, -(0.4f * sin(radians(30.f)) - 0.5f)),
        float2(0.4f * (cos(radians(30.f)) / 3.f) + 0.5f, -(0.4f * -sin(radians(30.f)) - 0.5f))
    };

    float2 TriMid = (Tri1[0] + Tri1[1] + Tri1[2]) / 3.f;
    
    float2 Tri2[3];

    for (int i = 0; i < 3; ++i)
    {
        Tri2[i] = Tri1[i] * 0.85f + TriMid * 0.15f;
    }

    float2 Temp1[3];
    float2 Temp2[3];
    
    float2 TriVec[3];
    float2 CheckVec[3];
    float ZVec[3];

    float2 STriVec[3];
    float2 SCheckVec[3];
    float SZVec[3];

    for (int i = 0; i < 3; ++i)
    {
        // 큰 삼각형 내부 계산
        TriVec[i] = Tri1[(i + 1) % 3] - Tri1[i];
        CheckVec[i] = Tri1[i] - _Point;
        ZVec[i] = TriVec[i].x * CheckVec[i].y - TriVec[i].y * CheckVec[i].x;

        // 작은 삼각형 내부 계산
        STriVec[i] = Tri2[(i + 1) % 3] - Tri2[i];
        SCheckVec[i] = Tri2[i] - _Point;
        SZVec[i] = STriVec[i].x * SCheckVec[i].y - STriVec[i].y * SCheckVec[i].x;
    }

    

    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;

    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;
    }

    // y = 0.5f 축 대칭 이동 y' = 2*0.5f - y
    for (int i = 0; i < 3; ++i)
    {
        Temp1[i] = Tri1[i];
        Temp1[i].y = 1.f - Temp1[i].y;

        Temp2[i] = Tri2[i];
        Temp2[i].y = 1.f - Temp2[i].y;
    }


    for (int i = 0; i < 3; ++i)
    {
        // 큰 삼각형 내부 계산
        TriVec[i] = Temp1[(i + 1) % 3] - Temp1[i];
        CheckVec[i] = Temp1[i] - _Point;
        ZVec[i] = TriVec[i].x * CheckVec[i].y - TriVec[i].y * CheckVec[i].x;

        // 작은 삼각형 내부 계산
        STriVec[i] = Temp2[(i + 1) % 3] - Temp2[i];
        SCheckVec[i] = Temp2[i] - _Point;
        SZVec[i] = STriVec[i].x * SCheckVec[i].y - STriVec[i].y * SCheckVec[i].x;
    }

    

    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;

    }

      // x = 0.5f 축 대칭 이동 x' = 2*0.5f - x
    for (int i = 0; i < 3; ++i)
    {
        Temp1[i] = Tri1[i];
        Temp1[i].x = 1.02f - Temp1[i].x;

        Temp2[i] = Tri2[i];
        Temp2[i].x = 1.02f - Temp2[i].x;
    }


    for (int i = 0; i < 3; ++i)
    {
        // 큰 삼각형 내부 계산
        TriVec[i] = Temp1[(i + 1) % 3] - Temp1[i];
        CheckVec[i] = Temp1[i] - _Point;
        ZVec[i] = TriVec[i].x * CheckVec[i].y - TriVec[i].y * CheckVec[i].x;

        // 작은 삼각형 내부 계산
        STriVec[i] = Temp2[(i + 1) % 3] - Temp2[i];
        SCheckVec[i] = Temp2[i] - _Point;
        SZVec[i] = STriVec[i].x * SCheckVec[i].y - STriVec[i].y * SCheckVec[i].x;
    }

    

    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;

    }

    // (0.5f,0.5f)점 대칭이동      x' = 2*0.5f - x ,y' = 2*0.5f - y  
    for (int i = 0; i < 3; ++i)
    {
        Temp1[i] = Tri1[i];
        Temp1[i].x = 1.02f - Temp1[i].x;
        Temp1[i].y = 1.f - Temp1[i].y;

        Temp2[i] = Tri2[i];
        Temp2[i].x = 1.02f - Temp2[i].x;
        Temp2[i].y = 1.f - Temp2[i].y;
    }


    for (int i = 0; i < 3; ++i)
    {
        // 큰 삼각형 내부 계산
        TriVec[i] = Temp1[(i + 1) % 3] - Temp1[i];
        CheckVec[i] = Temp1[i] - _Point;
        ZVec[i] = TriVec[i].x * CheckVec[i].y - TriVec[i].y * CheckVec[i].x;

        // 작은 삼각형 내부 계산
        STriVec[i] = Temp2[(i + 1) % 3] - Temp2[i];
        SCheckVec[i] = Temp2[i] - _Point;
        SZVec[i] = STriVec[i].x * SCheckVec[i].y - STriVec[i].y * SCheckVec[i].x;
    }

    

    if (ZVec[0] > 0.f && ZVec[1] > 0.f && ZVec[2] > 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;
    }

    if (ZVec[0] < 0.f && ZVec[1] < 0.f && ZVec[2] < 0.f)
    {
        bool A = SZVec[0] > 0.f;
        bool B = SZVec[1] > 0.f;
        bool C = SZVec[2] > 0.f;

        if (A != B || B != C || A != C)
            return true;

        A = SZVec[0] < 0.f;
        B = SZVec[1] < 0.f;
        C = SZVec[2] < 0.f;
       
        if (A != B || B != C || A != C)
            return true;

    }

        return false;
}

PS_OUTPUT PS_ColorRect(VTXCOL_OUTPUT _Input)
{
    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;


    float2 M = float2(0.5f, 0.5f);
    float2 SR = float2(0.4f, 0.4f);

    //float2 Tri1[3] = { float2(0.4f * cos(radians(-30.f)), 0.4f * sin(radians(-30.f)))
    //            , float2(0.4f * cos(radians(90.f)), 0.4f * sin(radians(90.f)))
    //            , float2(0.4f * cos(radians(210.f)), 0.4f * sin(radians(210.f)))
    //};

    if (0.5f > length(M - _Input.vTex) && (0.48f) < length(M - _Input.vTex))
        OutData.vColor = float4(1.f, 1.f, 1.f, 1.f);
    else if (0.42f > length(M - _Input.vTex) && 0.4f < length(M - _Input.vTex))
        OutData.vColor = float4(1.f, 1.f, 1.f, 1.f);
    else if (true == Poseidon(_Input.vTex))
        OutData.vColor = float4(1.f, 1.f, 1.f, 1.f);
    else
        clip(-1);
    //if (0.4f > length(_Input.vTex - Mid) )
    //    clip(-1);
   
    return OutData;
}