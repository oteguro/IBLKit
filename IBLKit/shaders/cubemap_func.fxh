// ----------------------------------------------------------------------------
// cubemap_func.fxh
// ----------------------------------------------------------------------------
#ifndef CUBEMAP_FUNC_FXH
#define CUBEMAP_FUNC_FXH


float2 Hammersley(uint index, uint numSamples, uint2 randomSeed)
{
    float e1 = frac((float)index / numSamples + float(randomSeed.x & 0xffff) / (1 << 16));
    float e2 = float(reversebits(index) ^ randomSeed.y) * 2.3283064365386963e-10;

    return float2(e1, e2);
}



#endif // CUBEMAP_FUNC_FXH 
