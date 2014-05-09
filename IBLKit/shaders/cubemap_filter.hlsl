// ----------------------------------------------------------------------------
// cubemap_filter.hlsl 
// ----------------------------------------------------------------------------
#define TILE (8)

RWTexture2DArray<float4> outTexture : register( u0 );

[numthreads(TILE,TILE,1)]
void CS(uint3 groupID           : SV_GroupID,
        uint3 groupThreadID     : SV_GroupThreadID)
{
    uint outWidth;
    uint outHeight;
    uint outElement;

    outTexture.GetDimensions(outWidth, outHeight, outElement);

    uint2 pos = groupID.xy * uint2(TILE, TILE) + groupThreadID.xy;
    if(all(pos.xy < uint2(outWidth, outHeight)))
    {
        outTexture[uint3(pos.xy, 0)] = (outWidth >= 256) ? float4(1,0,0,1) : (outWidth >= 128) ? float4(0,1,0,1) : float4(0,0,1,1);
    }

}
