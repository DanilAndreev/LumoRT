#include "fractalTypes.hlsli"

ConstantBuffer<FractalSettings> Constants : register(b0, space0);
RWTexture2D<float> OutImage : register(u2, space0);

// complex addition, and derivatives
float4 dcAdd( float4 a, float4 b )
{
    return a + b;
}

// complex multiplication, and derivatives
float4 dcMul( float4 a, float4 b )
{
    return float4( a.x*b.x - a.y*b.y,
                 a.x*b.y + a.y*b.x,
                 a.x*b.z + a.z*b.x - a.y*b.w - a.w*b.y,
                 a.x*b.w + a.w*b.x + a.z*b.y + a.y*b.z );
}

// complex squaring, and derivatives
float4 dcSqr( float4 a )
{
    return float4( a.x*a.x - a.y*a.y,
                 2.0*a.x*a.y,
                 2.0*(a.x*a.z - a.y*a.w),
                 2.0*(a.x*a.w + a.y*a.z) );
}

//--------------------------------------------------

float4 mainImage(float2 fragCoord, float3 iResolution, float iTime)
{
    float2 p = -1.0 + 2.0 * fragCoord.xy / iResolution.xy;
    p.x *= iResolution.x/iResolution.y;

    // animation
    float tz = 0.5 - 0.5*cos(0.225*iTime);
    float zo = pow( 0.5, 13.0*tz );

    float4 c = float4( float2(-0.05,.6805) + p*zo, 1.0, 0.0 );

    // compute orbit
    float m2 = 0.0;
    float co = 0.0;
    float4 z = float4( 0.0, 0.0, 0.0, 0.0 );
    for( int i=0; i<256; i++ )
    {
        if( m2>1024.0 ) break;

        // Z -> Z² + c
        z = dcAdd( dcSqr(z), c );

        m2 = dot( z.xy, z.xy );
        co += 1.0;
    }

    // distance
    // d(c) = |Z|·log|Z|/|Z'|
    float d = 0.0;
    if( co<256.0 ) d = sqrt( dot(z.xy,z.xy)/dot(z.zw,z.zw) )*log(dot(z.xy,z.xy));


    // do some soft coloring based on distance
    d = clamp( 4.0*d/zo, 0.0, 1.0 );
    d = pow( d, 0.25 );
    float3 col = float3( d, d, d );

    // output color
    return float4( col, 1.0 );
}

[numthreads(32, 32, 1)]
void main(uint3 DTid: SV_DispatchThreadID) {
    float2 uv = float2(DTid.xy) / float2(Constants.backbufferWidth, Constants.backbufferHeight);
    float3 iResolution = float3(Constants.backbufferWidth, Constants.backbufferHeight, 1.0f);
    float iTime = Constants.time;
    float mandelbrot_color = mainImage(float2(DTid.xy), iResolution, iTime).x;

    OutImage[DTid.xy] = mandelbrot_color;
}
