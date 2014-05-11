// ----------------------------------------------------------------------------
// cubemap_func.fxh
// ----------------------------------------------------------------------------
// Description : Cubemap BRDF approximation. 
// 
// Reference:
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf page 4 ~ page6.
// http://www.gamedev.net/topic/655431-ibl-problem-with-consistency-using-ggx-anisotropy/
#ifndef CUBEMAP_FUNC_FXH
#define CUBEMAP_FUNC_FXH

#ifndef PI
#define PI 3.141592653
#endif 

float radicalInverse_VdC(uint bits)
{   // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i)/float(N), radicalInverse_VdC(i));
}

//float2 Hammersley(uint index, uint numSamples, uint2 randomSeed)
//{
//    float e1 = frac((float)index / numSamples + float(randomSeed.x & 0xffff) / (1 << 16));
//    float e2 = float(reversebits(index) ^ randomSeed.y) * 2.3283064365386963e-10;
//
//    return float2(e1, e2);
//}

float3 ImportanceSampleBlinn(float2 e, float roughness, float3 normal)
{
    float m = roughness * roughness;
    float n = 2 / (m*m) - 2;

    float phi = 2 * PI * e.x;

    float cosTheta = pow(max(e.y, 0.0001), 1/(n+1));
    float sinTheta = sqrt(1 - cosTheta*cosTheta);

    float3 h;
    h.x = sinTheta * cos(phi);
    h.y = cosTheta * cos(phi);
    h.z = cosTheta;

    float3 upVector = abs(normal.z) < 0.999 ? float3(0,0,1) : float3(1,0,0);
    float3 tangentX = normalize(cross(upVector, normal));
    float3 tangentY = cross(normal, tangentX);

    return tangentX * h.x + tangentY * h.y + normal * h.z;
}

float3 ImportanceSampleGGX(float2 e, float roughness, float3 normal)
{
    float m = roughness * roughness;

    float phi = 2 * PI * e.x;
    float cosTheta = sqrt((1-e.y) / (1 + (m*m - 1) * e.y));
    float sinTheta = sqrt(1 - cosTheta*cosTheta);

    float3 h;
    h.x = sinTheta * cos(phi);
    h.y = sinTheta * sin(phi);
    h.z = cosTheta;

    float3 upVector = abs(normal.z) < 0.999 ? float3(0,0,1) : float3(1,0,0);
    float3 tangentX = normalize(cross(upVector, normal));
    float3 tangentY = cross(normal, tangentX);

    return tangentX * h.x + tangentY * h.y + normal * h.z;
}

float3 PrefilterEnvMap(float roughness, float3 R){    float3 N = R;    float3 V = R;}

#endif // CUBEMAP_FUNC_FXH 
