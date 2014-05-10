// ----------------------------------------------------------------------------
// cubemap_filter.hlsl 
// ----------------------------------------------------------------------------
#define TILE (8)

RWTexture2DArray<float4> txOUT      : register( u0 );

TextureCube              txIBL      : register( t0 );
SamplerState             smIBL      : register( s0 );


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
        txOUT[uint3(pos.xy, 0)] = (outWidth >= 256) ? float4(1,0,0,1) :
                                  (outWidth >= 128) ? float4(0,1,0,1) :
                                  (outWidth >=  64) ? float4(0,0,1,1) : float4(0,0.25,0.25,1);
    }

}
