// Ÿ��ź �ҿ� ���� ��ƼŬ ���̴�
#define YSORTING    0
#define FRONT       1
#define BACK        2

// �ӵ� �������� ��ȣ ��� ���� & ����(�ʱ� ���)
#define ACEEL_ABSOLUTE 0
// �ʱ� �ӵ��� �������� �ӵ� �����͸� �ؼ�
#define ACEEL_RELATIVE 1



cbuffer PerFrameGlobalValue : register(b0)
{
    float4x4 gViewProj;
    // �����Ӹ��� ���ŵǴ� �۷ι� ����
    // x : DeltaTime(dt)
    float4   gGlovalFrameData;


};



// ��ƼŬ ���� ����
typedef struct VTX_INPUT 
{
	float3 vInitPosW : POSITION;
    float2 vInitSizeW : SIZE;
    float2 vInitVelocity : VELOCITY;
    float  vRotZ : ROTATEZ;
    float  vLifeTime : LIFETIME;
    float  vAccTime : ACCTIME;
    float4 vSpriteUV: SPRITEUV;
    float4 vSizeData : SIZEDATA;
    float4 vVelocityData : VELDATA;
    float4 vAlphaData : ALPHADATA;
    uint vVelocityOption : VELOPTION;
    uint vDepthOption : DEPTHOPTION;
}Particle;



struct GS_OUT
{
    float4 vPosH : SV_Position;
    float2 vTexCoord : TEXCOORD;
    float Alpha : ALPHA;

};

// ��Ʈ�� ����� �̿��� ��� ��Ʈ����� ���� ���̴��� ���� �ۼ������ϴ�
// ��Ʈ�� ������� ���ϼ��̴��� ��� �ȼ����̴��� ���� ���۸� ��Ȱ�� ���·� ����� ����Ѵ�.
Particle StreamOutVS(Particle _Input)
{
    return _Input;
}

[maxvertexcount(2)]
void StreamOutGS(point Particle _gInput[1], inout PointStream<Particle> _StreamOut)
{
    Particle OutData = _gInput[0];
    float DeltaTime = gGlovalFrameData.x;

    OutData.vAccTime += DeltaTime;


    // ������ ���(��ġ,ȸ��)���� ����
    if (OutData.vAccTime >= OutData.vLifeTime)
    {
        //_StreamOut.RestartStrip();
        return;
    }
        
    _StreamOut.Append(OutData);
    //_StreamOut.RestartStrip();
}


// �׸��� ���� ���̴�
Particle VS_Main(Particle _Input)
{
    return _Input;
}

float3 AbsoluteAcceleration(Particle _VtxInput)
{
    float3 Pos = _VtxInput.vInitPosW;

    // ���� ���� �ð�  
    // V(end) = V(Zero) + aT => T = (V - V0) / a;
    float2 VelEndTime = _VtxInput.vVelocityData.zw - _VtxInput.vInitVelocity.xy;
   
    // x���� �������� ������ �ִ°�� ������ �����Ѵ�
    if (_VtxInput.vVelocityData.x != 0.f)
    {
        VelEndTime.x /= _VtxInput.vVelocityData.x;
        float InitSpeed = _VtxInput.vInitVelocity.x;
        float Accel = _VtxInput.vVelocityData.x;
        float EndSpeed = _VtxInput.vVelocityData.z;
        float dt = VelEndTime.x;

        // ���� �ð��� ���� ���
        if (VelEndTime.x <= _VtxInput.vAccTime)
        {
            dt = VelEndTime.x;
            Pos.x += InitSpeed * dt + 0.5f * Accel * dt * dt + EndSpeed * (_VtxInput.vAccTime - dt);
        }
        else // ������ �ȳ��� ���
        {
            dt = _VtxInput.vAccTime;
            Pos.x += InitSpeed * dt + 0.5f * Accel * dt * dt;
        }

    }
    else
        Pos.x += _VtxInput.vInitVelocity.x * _VtxInput.vAccTime;


    // y���� �������� ������ �ִ°�� ������ �����Ѵ�
    if (_VtxInput.vVelocityData.y != 0.f)
    {
        VelEndTime.y /= _VtxInput.vVelocityData.y;
        float InitSpeed = _VtxInput.vInitVelocity.y;
        float Accel = _VtxInput.vVelocityData.y;
        float EndSpeed = _VtxInput.vVelocityData.w;
        float dt = VelEndTime.y;

        // ���� �ð��� ���� ���
        if (VelEndTime.y <= _VtxInput.vAccTime)
        {
            dt = VelEndTime.y;
            Pos.y += InitSpeed * dt + 0.5f * Accel * dt * dt + EndSpeed * (_VtxInput.vAccTime - dt);
        }
        else // ������ �ȳ��� ���
        {
            dt = _VtxInput.vAccTime;
            Pos.y += InitSpeed * dt + 0.5f * Accel * dt * dt;
        }

    }
    else
        Pos.y += _VtxInput.vInitVelocity.y * _VtxInput.vAccTime;

    return Pos;
}

float3 RelativeAcceleration(Particle _VtxInput)
{
    float3 Pos = _VtxInput.vInitPosW;

    float2 InitSpeed = _VtxInput.vInitVelocity.xy;
    float2 InitSign = float2((float) sign(InitSpeed.x), (float) sign(InitSpeed.y));
    float2 Accel = _VtxInput.vVelocityData.xy * InitSign;
    float2 EndSpeed = _VtxInput.vVelocityData.zw * InitSign;
    float2 VelEndTime = EndSpeed - InitSpeed;

    if (0.f != Accel.x)
    {
        VelEndTime.x /= Accel.x;

        float dt = VelEndTime.x;

         // ���� �ð��� ���� ���
        if (VelEndTime.x <= _VtxInput.vAccTime)
        {
            dt = VelEndTime.x;
            Pos.x += InitSpeed.x * dt + 0.5f * Accel.x * dt * dt + EndSpeed.x * (_VtxInput.vAccTime - dt);
        }
        else // ������ �ȳ��� ���
        {
            dt = _VtxInput.vAccTime;
            Pos.x += InitSpeed.x * dt + 0.5f * Accel.x * dt * dt;
        }
    }
    else
        Pos.x += _VtxInput.vInitVelocity.x * _VtxInput.vAccTime;


    if (0.f != Accel.y)
    {
        VelEndTime.y /= Accel.y;
        float dt = VelEndTime.y;

         // ���� �ð��� ���� ���
        if (VelEndTime.y <= _VtxInput.vAccTime)
        {
            dt = VelEndTime.y;
            Pos.y += InitSpeed.y * dt + 0.5f * Accel.y * dt * dt + EndSpeed.y * (_VtxInput.vAccTime - dt);
        }
        else // ������ �ȳ��� ���
        {
            dt = _VtxInput.vAccTime;
            Pos.y += InitSpeed.y * dt + 0.5f * Accel.y * dt * dt;
        }
    }
    else
        Pos.y += _VtxInput.vInitVelocity.y * _VtxInput.vAccTime;
    return Pos;
}

[maxvertexcount(4)]
void GS_Main(point Particle _gInput[1], inout TriangleStream<GS_OUT> _gOutStream)
{

    float2 BaseTexCoord[4] =
    {
        float2(0.f, 0.f)
      , float2(1.f, 0.f)
      , float2(0.f, 1.f)
      , float2(1.f, 1.f)
    };

    // 0        1
    // 3        2
    float3 Right = float3(1.f, 0.f, 0.f);
    float3 Up    = float3(0.f, 1.f, 0.f);

    float Sin = sin(_gInput[0].vAccTime * _gInput[0].vRotZ);
    float Cos = cos(_gInput[0].vAccTime * _gInput[0].vRotZ);
    float3x3 Rotate = float3x3(  Cos, Sin, 0.f
                               ,-Sin, Cos, 0.f
                               , 0.f, 0.f, 1.f);

    Right = mul(Right, Rotate);
    Up = mul(Up, Rotate);

    

    float2 HS = _gInput[0].vInitSizeW * .5f;
   
    // ũ�� ��ȭ�� �����Ͱ� ��ȿ�� ������ ũ�⸦ ��ȭ ��Ų��.
    if (_gInput[0].vSizeData.x != _gInput[0].vSizeData.y && _gInput[0].vSizeData.z <= _gInput[0].vAccTime)
    {
        float T = clamp((_gInput[0].vAccTime - _gInput[0].vSizeData.z) / _gInput[0].vSizeData.w, 0.f, 1.f);
        HS *= _gInput[0].vSizeData.x * (1.f - T) + _gInput[0].vSizeData.y * T;
    }


    float3 Pos = (float3)0;
    // ������ ��� ����
    switch (_gInput[0].vVelocityOption)
    {
        case ACEEL_ABSOLUTE:
            Pos = AbsoluteAcceleration(_gInput[0]);
            break;
        case ACEEL_RELATIVE:
            Pos = RelativeAcceleration(_gInput[0]);
            break;
    }


    switch (_gInput[0].vDepthOption)
    {
        case YSORTING:
            {
                if (Pos.y >= 0.f)
                    Pos.z = log2(Pos.y + 1.f) * 0.01f;
                else
                    Pos.z = -log2(-(Pos.y - 1.f)) * 0.01f;
            }
        break;
        case FRONT:
            Pos.z = -60.f;
            break;
        case BACK:
            Pos.z = 5.f;
            break;
    }


    float4 Vtx[4] =
    {
        float4(Pos - Right * HS.x + Up * HS.y, 1.f),
        float4(Pos + Right * HS.x + Up * HS.y, 1.f),
        float4(Pos - Right * HS.x - Up * HS.y, 1.f),
        float4(Pos + Right * HS.x - Up * HS.y, 1.f)
    };

    GS_OUT OutData;

    // ������ ������ ����
    if (_gInput[0].vAlphaData.x != _gInput[0].vAlphaData.y && _gInput[0].vAlphaData.w != 0.f )
    {
        float T = clamp((_gInput[0].vAccTime - _gInput[0].vAlphaData.z) / _gInput[0].vAlphaData.w, 0.f, 1.f);
        OutData.Alpha = _gInput[0].vAlphaData.x * (1.f - T) + _gInput[0].vAlphaData.y * T;
    }
    else
        OutData.Alpha = _gInput[0].vAlphaData.x;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        OutData.vPosH = mul(Vtx[i], gViewProj);
        OutData.vTexCoord = _gInput[0].vSpriteUV.xy + (BaseTexCoord[i] * _gInput[0].vSpriteUV.zw);
        _gOutStream.Append(OutData);
    }

    _gOutStream.RestartStrip();

}


struct PIX_OUTPUT
{
	float4 vTexColor : SV_Target;
};

texture2D Image : register(t0);
SamplerState Sampler : register(s0);


PIX_OUTPUT PS_Main(GS_OUT _pInput)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT)0.f;
    
    //OutData.vTexColor = float4(1.f, 1.f, 1.f, 1.f);
    OutData.vTexColor = Image.Sample(Sampler, _pInput.vTexCoord) * float4(1.f, 1.f, 1.f, _pInput.Alpha);
   
    if (OutData.vTexColor.a == 0.f)
        clip(-1);
 

   return OutData;
}