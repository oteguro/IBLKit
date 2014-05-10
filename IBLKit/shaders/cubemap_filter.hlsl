// ----------------------------------------------------------------------------
// cubemap_filter.hlsl 
// ----------------------------------------------------------------------------
#include "cubemap_func.fxh"
#define TILE (8)

RWTexture2DArray<float4> txOUT      : register( u0 );

TextureCube              txIBL      : register( t0 );
SamplerState             smIBL      : register( s0 );

cbuffer CSBuffer : register( b0 )
{
    float4      zDirection;
    float4      xDirection;
    float4      yDirection;
    float4      texelOffset;
    float4      roughness;
    uint4       uavSize;
}

[numthreads(TILE,TILE,1)]
void CS(uint3 groupID           : SV_GroupID,
        uint3 groupThreadID     : SV_GroupThreadID)
{
    uint outWidth;
    uint outHeight;
    uint outElement;

    txOUT.GetDimensions(outWidth, outHeight, outElement);

    uint2 pos = groupID.xy * uint2(TILE, TILE) + groupThreadID.xy;
    [branch] if(all(pos.xy < uint2(outWidth, outHeight)))
    {
        int2 ipos = (int2)(pos.xy);
        int2 ihsz = (int2)(uavSize.zw);

        float2 offsetCoord = ((float2)(ipos.xy - ihsz.xy) / (float2)(ihsz.xy)) + texelOffset.xy;

        // compute 'base normal'. 
        float3 normalDirection  = zDirection.xyz + (xDirection.xyz * offsetCoord.xxx) + (yDirection.xyz * offsetCoord.yyy);
               normalDirection  = normalize(normalDirection.xyz);

        // 

//        float4 oColor = (outWidth >= 256) ? float4(1,0,0,1) :
//                        (outWidth >= 128) ? float4(0,1,0,1) :
//                        (outWidth >=  64) ? float4(0,0,1,1) : float4(0,0.25,0.25,1);

        float  oLod   = 0.0;
        float4 oColor = txIBL.SampleLevel(smIBL, normalDirection.xyz, oLod);

        txOUT[int3(pos.xy, 0)] = oColor.xyzw;
    }

}
