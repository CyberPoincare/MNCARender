#include "structures.fxh"

cbuffer Constants
{
    GlobalConstants g_Constants;
};

StructuredBuffer<ParticleAttribs> g_Particles;

struct VSInput
{
    uint VertID : SV_VertexID;
    uint InstID : SV_InstanceID;
};

struct PSInput 
{ 
    float4 Pos  : SV_POSITION; 
    float2 uv   : TEX_COORD;
    float4 Life : LIFE;
    float  Scale : SCALE;
};

void main(in  VSInput VSIn,
          out PSInput PSIn)
{
    float4 pos_uv[4];
    pos_uv[0] = float4(-1.0,+1.0, 0.0,0.0);
    pos_uv[1] = float4(-1.0,-1.0, 0.0,1.0);
    pos_uv[2] = float4(+1.0,+1.0, 1.0,0.0);
    pos_uv[3] = float4(+1.0,-1.0, 1.0,1.0);

    float2 f2Off  = (g_Constants.f2WindowSize - g_Constants.i2WorldSize) * 0.5 + g_Constants.f2ViewShift; //view shift
    float  fScale = g_Constants.fViewScale; //view scale
    PSIn.Scale = fScale;

    float2 fSize = 1.0 / g_Constants.f2WindowSize;
    fSize *= fScale; 

    ParticleAttribs Attribs = g_Particles[VSIn.InstID];

    PSIn.Life = float4(Attribs.iLife, Attribs.f4Temp.x, Attribs.f4Temp.y, Attribs.f4Temp.z);

    float2 cpos = Attribs.i2Pos + f2Off;
    float2 pos  = pos_uv[VSIn.VertID].xy * g_Constants.f2Scale.xy;
    pos        = pos * fSize / g_Constants.f2Scale.xy + ((cpos / g_Constants.f2WindowSize) - 0.5) * 2.0 * fScale;
    PSIn.Pos = float4(pos, 0.0, 1.0);
    PSIn.uv = pos_uv[VSIn.VertID].zw;
}
