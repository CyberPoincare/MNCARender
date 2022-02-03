#include "structures.fxh"

cbuffer Constants
{
    GlobalConstants g_Constants;
};

RWStructuredBuffer<ParticleAttribs> g_Particles;
StructuredBuffer<NeighbourAttribs>  g_NeighbourShapes;

#ifndef THREAD_GROUP_SIZE
#    define THREAD_GROUP_SIZE 64
#endif

int getNeighbour(int2 pos, int2 offset)
{
    int x = int(fmod(g_Constants.i2WorldSize.x + pos.x + offset.x + 0.5, g_Constants.i2WorldSize.x));
    int y = int(fmod(g_Constants.i2WorldSize.y + pos.y + offset.y + 0.5, g_Constants.i2WorldSize.y));

    int iParticleIdx = x + y * g_Constants.i2WorldSize.x;
    return g_Particles[iParticleIdx].iLife;
}
//-----------------------------------------------------------------------------
float seek_neighbour(int2 pos, int nbid)
{
    float s = 0.0;
    uint v = 0;
    uint c = 0;
    int h = int((NEIGHBOUR_SIZE - 1) * 0.5 + 0.5);
    for (int i = 0; i < NEIGHBOUR_SIZE; i++)
    {
        for (int j = 0; j < NEIGHBOUR_SIZE; j++)
        {
            v = g_NeighbourShapes[nbid].NB[j + i * NEIGHBOUR_SIZE];
            c += v;
            s += getNeighbour(pos, int2(j - h, i - h)) * v;
        }
    }

    if (c > 0)
        return s / float(c); //div by number of neighbours
    else
        return 0.0;
}

[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void main(uint3 Gid  : SV_GroupID,
          uint3 GTid : SV_GroupThreadID)
{
    uint uiGlobalThreadIdx = Gid.x * uint(THREAD_GROUP_SIZE) + GTid.x;
    if (uiGlobalThreadIdx >= g_Constants.uiNumParticles)
        return;

    int iParticleIdx = int(uiGlobalThreadIdx);

    ParticleAttribs Particle = g_Particles[iParticleIdx];

    int iNewLife = Particle.iLife;  //default

    float sum0 = seek_neighbour(Particle.i2Pos, Particle.i4NID.x);
    float sum1 = seek_neighbour(Particle.i2Pos, Particle.i4NID.y);
    float sum2 = seek_neighbour(Particle.i2Pos, Particle.i4NID.z);
    float sum3 = seek_neighbour(Particle.i2Pos, Particle.i4NID.w);
    g_Particles[iParticleIdx].f4Temp = float4(sum0, sum1, sum2, sum3);

    if (sum0 >= Particle.f3threshold_0[Particle.i4NID.x].x && sum0 <= Particle.f3threshold_0[Particle.i4NID.x].y)
        iNewLife = int(Particle.f3threshold_0[Particle.i4NID.x].z);
    if (sum1 >= Particle.f3threshold_0[Particle.i4NID.y].x && sum1 <= Particle.f3threshold_0[Particle.i4NID.y].y)
        iNewLife = int(Particle.f3threshold_0[Particle.i4NID.y].z);
    if (sum2 >= Particle.f3threshold_0[Particle.i4NID.z].x && sum2 <= Particle.f3threshold_0[Particle.i4NID.z].y)
        iNewLife = int(Particle.f3threshold_0[Particle.i4NID.z].z);
    if (sum3 >= Particle.f3threshold_0[Particle.i4NID.w].x && sum3 <= Particle.f3threshold_0[Particle.i4NID.w].y)
        iNewLife = int(Particle.f3threshold_0[Particle.i4NID.w].z);

    if (sum0 >= Particle.f3threshold_1[Particle.i4NID.x].x && sum0 <= Particle.f3threshold_1[Particle.i4NID.x].y)
        iNewLife = int(Particle.f3threshold_1[Particle.i4NID.x].z);
    if (sum1 >= Particle.f3threshold_1[Particle.i4NID.y].x && sum1 <= Particle.f3threshold_1[Particle.i4NID.y].y)
        iNewLife = int(Particle.f3threshold_1[Particle.i4NID.y].z);
    if (sum2 >= Particle.f3threshold_1[Particle.i4NID.z].x && sum2 <= Particle.f3threshold_1[Particle.i4NID.z].y)
        iNewLife = int(Particle.f3threshold_1[Particle.i4NID.z].z);
    if (sum3 >= Particle.f3threshold_1[Particle.i4NID.w].x && sum3 <= Particle.f3threshold_1[Particle.i4NID.w].y)
        iNewLife = int(Particle.f3threshold_1[Particle.i4NID.w].z);

    if (sum0 >= Particle.f3threshold_2[Particle.i4NID.x].x && sum0 <= Particle.f3threshold_2[Particle.i4NID.x].y)
        iNewLife = int(Particle.f3threshold_2[Particle.i4NID.x].z);
    if (sum1 >= Particle.f3threshold_2[Particle.i4NID.y].x && sum1 <= Particle.f3threshold_2[Particle.i4NID.y].y)
        iNewLife = int(Particle.f3threshold_2[Particle.i4NID.y].z);
    if (sum2 >= Particle.f3threshold_2[Particle.i4NID.z].x && sum2 <= Particle.f3threshold_2[Particle.i4NID.z].y)
        iNewLife = int(Particle.f3threshold_2[Particle.i4NID.z].z);
    if (sum3 >= Particle.f3threshold_2[Particle.i4NID.w].x && sum3 <= Particle.f3threshold_2[Particle.i4NID.w].y)
        iNewLife = int(Particle.f3threshold_2[Particle.i4NID.w].z);

    if (sum0 >= Particle.f3threshold_3[Particle.i4NID.x].x && sum0 <= Particle.f3threshold_3[Particle.i4NID.x].y)
        iNewLife = int(Particle.f3threshold_3[Particle.i4NID.x].z);
    if (sum1 >= Particle.f3threshold_3[Particle.i4NID.y].x && sum1 <= Particle.f3threshold_3[Particle.i4NID.y].y)
        iNewLife = int(Particle.f3threshold_3[Particle.i4NID.y].z);
    if (sum2 >= Particle.f3threshold_3[Particle.i4NID.z].x && sum2 <= Particle.f3threshold_3[Particle.i4NID.z].y)
        iNewLife = int(Particle.f3threshold_3[Particle.i4NID.z].z);
    if (sum3 >= Particle.f3threshold_3[Particle.i4NID.w].x && sum3 <= Particle.f3threshold_3[Particle.i4NID.w].y)
        iNewLife = int(Particle.f3threshold_3[Particle.i4NID.w].z);

    if (sum0 >= Particle.f3threshold_4[Particle.i4NID.x].x && sum0 <= Particle.f3threshold_4[Particle.i4NID.x].y)
        iNewLife = int(Particle.f3threshold_4[Particle.i4NID.x].z);
    if (sum1 >= Particle.f3threshold_4[Particle.i4NID.y].x && sum1 <= Particle.f3threshold_4[Particle.i4NID.y].y)
        iNewLife = int(Particle.f3threshold_4[Particle.i4NID.y].z);
    if (sum2 >= Particle.f3threshold_4[Particle.i4NID.z].x && sum2 <= Particle.f3threshold_4[Particle.i4NID.z].y)
        iNewLife = int(Particle.f3threshold_4[Particle.i4NID.z].z);
    if (sum3 >= Particle.f3threshold_4[Particle.i4NID.w].x && sum3 <= Particle.f3threshold_4[Particle.i4NID.w].y)
        iNewLife = int(Particle.f3threshold_4[Particle.i4NID.w].z);

    g_Particles[iParticleIdx].iNewLife = iNewLife;
}
