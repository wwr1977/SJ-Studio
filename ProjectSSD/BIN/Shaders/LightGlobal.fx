
#define DIRECTION_LIGHT     0
#define POINT_LIGHT         1
#define SPOT_LIGHT          2

struct LIGHTCOLOR
{
    float4      Diffuse;
    float4      Specular;
    float4      Ambient;
};


struct LIGHTDATA
{
    float4      LightColor;
    float4      LightPos;
    float4      LightDir;
    float4      LightDirInv;
    float4      LightViewPos;
    float4      LightViewDir;
    float4      LightViewDirInv;
    float4      LightAtenuation;
    float       LightRange;
    float       LightPower;
    int         LightType;
    float       SpecularPower;
    float       SpotPower;
    float       AmbientFactor;
    float2      Temp;
    float4x4    ViewMat;
    float4x4    ProjMat;
    float4x4    VP;
};

LIGHTCOLOR GetDirectionLight(inout float3 _ViewPos, inout float3 _ViewNormal, LIGHTDATA _Data)
{
    LIGHTCOLOR ReturnColor = (LIGHTCOLOR) 0.f;

    float3 Normal = _ViewNormal;
    float4 LightDir = (float4) 0.f;
    LightDir = normalize(_Data.LightViewDirInv);
  
    float3 CamDir = normalize(-_ViewPos);
    float3 RefDir = normalize(reflect((float3) -LightDir, Normal));
    //float3 RefDir = normalize((float3) -LightDir + (2 * Normal * dot((float3) LightDir, Normal)));

    float DiffFactor = saturate(dot(Normal, (float3) LightDir));

    if (DiffFactor > 0.f)
    {
        ReturnColor.Diffuse = DiffFactor * _Data.LightColor * _Data.LightPower;

        float SpecValue = pow(saturate(dot(RefDir, CamDir)), _Data.SpecularPower);
        ReturnColor.Specular = (float4) SpecValue * _Data.LightColor * _Data.LightPower;
    }

    ReturnColor.Ambient = (float4)_Data.AmbientFactor;

    return ReturnColor;
}

LIGHTCOLOR GetPointLight(inout float3 _ViewPixPos, inout float3 _ViewNormal, LIGHTDATA _Data)
{
    //LIGHTCOLOR ReturnColor = (LIGHTCOLOR) 0.f;

    //float4 LightDir = (float4) 0.f;
    //LightDir.xyz = _Data.LightViewPos.xyz - _ViewPixPos;
    //float Distance = length(LightDir.xyz);
    
    //if (Distance > _Data.LightRange)
    //    return ReturnColor;

    ////float Atenuation = _Data.LightAtenuation.x / dot(_Data.LightAtenuation.yzw, float3(1.f, Distance, Distance * Distance));
    //float Atenuation = _Data.LightAtenuation.x / dot(float3(0.f, 0.f, 1.f), float3(1.f, Distance, Distance * Distance));
    //float3 Normal = _ViewNormal;
    //LightDir = normalize(LightDir);
  
    //float3 CamDir = normalize(-_ViewPixPos);
    //float3 RefDir = normalize(reflect((float3) -LightDir, Normal));

    //float DiffFactor = saturate(dot(Normal, (float3) LightDir));

    //if (DiffFactor > 0.f)
    //{
    //    ReturnColor.Diffuse = DiffFactor * _Data.LightColor * _Data.LightPower * Atenuation;
    
    //    float SpecValue = pow(saturate(dot(RefDir, CamDir)), _Data.SpecularPower);
    //    ReturnColor.Specular = (float4) (SpecValue * _Data.LightColor * _Data.LightPower * Atenuation);
    //}

    //ReturnColor.Ambient = (float4) _Data.AmbientFactor;

    //return ReturnColor;

    LIGHTCOLOR ReturnColor = (LIGHTCOLOR) 0.f;

    float3 LightVec = (float3) 0.f;
    LightVec = _Data.LightViewPos.xyz - _ViewPixPos;
    float Distance = length(LightVec);
    LightVec = normalize(LightVec);

    if (Distance > _Data.LightRange)
        return ReturnColor;

    float3 Normal = normalize(_ViewNormal);
    float3 CamDir = normalize(-_ViewPixPos);

    float DiffuseFactor = dot(LightVec, Normal);
    if (DiffuseFactor > 0.f)
    {
        float3 Re = reflect(-LightVec, Normal);
        float SpecFactor = pow(max(dot(Re, CamDir), 0.f), _Data.SpecularPower);

        ReturnColor.Diffuse = DiffuseFactor * _Data.LightColor * _Data.LightPower;
        ReturnColor.Specular = SpecFactor * _Data.LightColor * _Data.LightPower;
    }

    float Att = _Data.LightAtenuation.x / dot(_Data.LightAtenuation.yzw, float3(1.f, Distance, Distance * Distance));
    
    ReturnColor.Diffuse *= Att;
    ReturnColor.Specular *= Att;
    ReturnColor.Ambient = (float4) _Data.AmbientFactor;

    return ReturnColor;
}


LIGHTCOLOR GetSpotLight(inout float3 _ViewPixPos, inout float3 _ViewNormal, LIGHTDATA _Data)
{
    //LIGHTCOLOR ReturnColor = (LIGHTCOLOR) 0.f;

    //float4 LightDir = (float4) 0.f;
    //LightDir.xyz = _Data.LightViewPos.xyz - _ViewPos;
    //float Distance = length(LightDir.xyz);
    
    //if (Distance > _Data.LightRange)
    //    return ReturnColor;

    //float Atenuation = _Data.LightAtenuation.x / dot(_Data.LightAtenuation.yzw, float3(1.f, Distance, Distance * Distance));
    //float3 Normal = _ViewNormal;
    //LightDir = normalize(LightDir);
  
    //float3 CamDir = normalize(-_ViewPos);
    //float3 RefDir = normalize(reflect((float3) -LightDir, Normal));

    //float DiffFactor = saturate(dot(Normal, (float3) LightDir));
    //float SpotFactor = pow(saturate(dot(_Data.LightDir, -LightDir)), _Data.SpotPower);

    //if (DiffFactor > 0.f)
    //{
    //    ReturnColor.Diffuse = DiffFactor * _Data.LightColor * _Data.LightPower * Atenuation * SpotFactor;
    
    //    float SpecValue = pow(saturate(dot(RefDir, CamDir)), _Data.SpecularPower);
    //    ReturnColor.Specular = (float4) (SpecValue * _Data.LightColor * _Data.LightPower * Atenuation) * SpotFactor;
    //}

    //ReturnColor.Ambient = (float4) _Data.AmbientFactor * SpotFactor;
    //return ReturnColor;

    LIGHTCOLOR ReturnColor = (LIGHTCOLOR) 0.f;

    float3 LightVec = (float3) 0.f;
    LightVec = _Data.LightViewPos.xyz - _ViewPixPos;
    float Distance = length(LightVec);
    LightVec = normalize(LightVec);

    if (Distance > _Data.LightRange)
        return ReturnColor;

    float3 Normal = normalize(_ViewNormal);
    float3 CamDir = normalize(-_ViewPixPos);

    float DiffuseFactor = max(0.f, dot(LightVec, Normal));
    float3 Re = reflect(-LightVec, Normal);
    float SpecFactor = pow(max(dot(Re, CamDir), 0.f), _Data.SpecularPower);

    ReturnColor.Diffuse = DiffuseFactor * _Data.LightColor * _Data.LightPower;
    ReturnColor.Specular = SpecFactor * _Data.LightColor * _Data.LightPower;

    float Att = _Data.LightAtenuation.x / dot(_Data.LightAtenuation.yzw, float3(1.f, Distance, Distance * Distance));
    float Spot = pow(max(dot(-LightVec, _Data.LightViewDir.xyz), 0), _Data.SpotPower);
    

    ReturnColor.Diffuse *= (Spot * Att);
    ReturnColor.Specular *= (Spot * Att);
    ReturnColor.Ambient = (float4) _Data.AmbientFactor;

    return ReturnColor;
}

//ÇÈ¼¿ ½¦ÀÌ´õ¿¡¼­ ºûÀ» °è»êÇÏ´Â ÇÔ¼ö
LIGHTCOLOR GetLight(inout float3 _ViewPos, inout float3 _ViewNormal, LIGHTDATA _Data)
{
     switch (_Data.LightType)
     {
        case DIRECTION_LIGHT:
            return GetDirectionLight(_ViewPos, _ViewNormal, _Data);
        case POINT_LIGHT:
            return GetPointLight(_ViewPos, _ViewNormal, _Data);
        case SPOT_LIGHT:
            return GetSpotLight(_ViewPos, _ViewNormal, _Data);
        default:
            break;
    }

    LIGHTCOLOR ReturnColor = (LIGHTCOLOR) 0.f;

    return ReturnColor;
}

float3 GetBumpNormal(float4 _BumpNormal, float3 _Tangent, float3 _Binormal, float3 _Normal)
{
    float3 BumpNormal = _BumpNormal.xyz;
   
     if (_BumpNormal.a  == 0.f)
    {
        return _Normal;
    }

    if (_Tangent.x == 0.f)
    {
        if (_Tangent.y == 0.f)
        {
            if (_Tangent.z == 0.f)
            {
                return _Normal;
            }
        }
    }

    if (_Binormal.x == 0.f)
    {
        if (_Binormal.y == 0.f)
        {
            if (_Binormal.z == 0.f)
            {
                return _Normal;
            }
        }
    }

    BumpNormal = normalize((_BumpNormal * 2.f) - 1.f);

    float3 T, B, N;
    T = normalize(_Tangent);
    B = normalize(_Binormal);
    N = normalize(_Normal);

    float3x3 tbnMat = float3x3(T, B, N);

    BumpNormal = mul(BumpNormal, tbnMat);
    return BumpNormal;
}
