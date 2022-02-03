#ifndef __STRUCTURES_HLSL__
#define __STRUCTURES_HLSL__

#ifndef NEIGHBOUR_SIZE
#    define NEIGHBOUR_SIZE 17
#endif

#ifndef NEIGHBOUR_NUM
#    define NEIGHBOUR_NUM  289 //NEIGHBOUR_SIZE*NEIGHBOUR_SIZE
#endif

#ifndef NEIGHBOUR_HOOD_DEFINITION_NUM
#    define NEIGHBOUR_HOOD_DEFINITION_NUM 5
#endif


struct ParticleAttribs
{
    int2 i2Pos;
    int iLife;
    int iNewLife;

    float4 f4Temp;

    int4 i4NID;             //4 neighbour hood index

    float4 f3threshold_0[NEIGHBOUR_HOOD_DEFINITION_NUM]; //[nb_id][pass]
    float4 f3threshold_1[NEIGHBOUR_HOOD_DEFINITION_NUM]; //[nb_id][pass]
    float4 f3threshold_2[NEIGHBOUR_HOOD_DEFINITION_NUM]; //[nb_id][pass]
    float4 f3threshold_3[NEIGHBOUR_HOOD_DEFINITION_NUM]; //[nb_id][pass]
    float4 f3threshold_4[NEIGHBOUR_HOOD_DEFINITION_NUM]; //[nb_id][pass]
};

struct NeighbourAttribs
{
    uint NB[NEIGHBOUR_NUM];
};

struct GlobalConstants
{
    uint   uiNumParticles;
    float  fDeltaTime;
    float2 f2Scale;

    int2   i2WorldSize;
    float2 f2WindowSize;

    float2 f2ViewShift;
    float  fViewScale;
    int    iNeighbourSize;
};

#endif //__STRUCTURES_HLSL__