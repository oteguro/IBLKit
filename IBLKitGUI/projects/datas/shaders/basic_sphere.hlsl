// ----------------------------------------------------------------------------
// basic_sphere.hlsl 
// ----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer VSBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer PSBuffer : register( b1 )
{
    float4 Eye;
}

cbuffer PSBuffer : register( b2 )
{
    matrix InvView;
}

TextureCube  txIBL : register( t0 );
SamplerState smIBL : register( s0 );

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos   : SV_POSITION;
    float4 WPos  : TEXCOORD0;
    float2 Tc    : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.Pos   = mul( Pos, World );
    output.WPos  = float4(output.Pos.xyz, 1);
    output.Pos   = mul( output.Pos, View );
    output.Pos   = mul( output.Pos, Projection );

    output.Tc    = Tex.xy;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    float3 pt = input.WPos.xyz;
    float2 v2 = input.Tc.xy;

    float  ft = length(v2.xy);

    float alpha = 1.0f;
    if(ft>0.975f)
    {
        alpha = 0.0f;
        discard;
    }

    alpha = saturate((1 - smoothstep(0.975, 1.0f, ft)) * 0.5f + 0.5f);

    float3 ndir;

    ndir.x = v2.x;
    ndir.y = v2.y;
    ndir.z = -sqrt(1 - (ndir.x * ndir.x + ndir.y * ndir.y));

    float3 N      = mul(normalize(ndir.xyz), (float3x3)InvView);

    float3 E      = normalize(-Eye.xyz);

    float3 R      = reflect(E, N);

    float3 rrc    = txIBL.Sample(smIBL, R.xyz);

//    float3 ncolor = N.xyz * 0.5 + 0.5;
//    return float4(ncolor.xyz, alpha);
    return float4(pow(rrc.xyz, 0.4545f), alpha);
}
