// ----------------------------------------------------------------------------
// basic_background.hlsl 
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

TextureCube  txIBL : register( t0 );
SamplerState smIBL : register( s0 );

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos   : SV_POSITION;
    float3 WPos  : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos   = mul( Pos, World );
    output.WPos  = output.Pos.xyz;

    output.Pos   = mul( output.Pos, View );
    output.Pos   = mul( output.Pos, Projection );

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    float3 vv = input.WPos.xyz - Eye.xyz;

//    vv = normalize(vv.xyz);
//    float3 vvc = vv.xyz * 0.5f + 0.5f;
    float3 vvc = txIBL.Sample(smIBL, vv.xyz);

    return float4(pow(vvc.xyz, 0.4545f).xyz,1);
}
