#pragma once

#include "SampleBase.hpp"
#include "ResourceMapping.h"
#include "BasicMath.hpp"

#ifndef M_PI
#    define M_PI 3.14159265358979323846 // pi
#endif

namespace Diligent
{

class CyberSpace final : public SampleBase
{
public:
    virtual void ModifyEngineInitInfo(const ModifyEngineInitInfoAttribs& Attribs) override final;

    virtual void Initialize(const SampleInitInfo& InitInfo) override final;

    virtual void Render() override final;
    virtual void Update(double CurrTime, double ElapsedTime) override final;

    virtual const Char* GetSampleName() const override final { return "XPeng's CyberParticle"; }

private:
    void CreateRenderParticlePSO();
    void CreateUpdateParticlePSO();
    void CreateParticleBuffers();
    void CreateConsantBuffer();
    void UpdateUI();

    int m_NumParticles    = 10000;
    int m_iWorldSize = 512;
    int m_ThreadGroupSize = 256;

    RefCntAutoPtr<IPipelineState>         m_pUpdateParticlesPSO;
    RefCntAutoPtr<IShaderResourceBinding> m_pUpdateParticlesSRB;
    RefCntAutoPtr<IPipelineState>         m_pMoveParticlesPSO;
    RefCntAutoPtr<IShaderResourceBinding> m_pMoveParticlesSRB;
    RefCntAutoPtr<IPipelineState>         m_pRenderParticlePSO;
    RefCntAutoPtr<IShaderResourceBinding> m_pRenderParticleSRB;

    RefCntAutoPtr<IBuffer>                m_Constants;
    RefCntAutoPtr<IBuffer>                m_pParticleAttribsBuffer;
    RefCntAutoPtr<IBuffer>                m_pNeighbourAttribsBuffer;
    RefCntAutoPtr<IResourceMapping>       m_pResMapping;

    int4 m_i4NID = int4(1, 2, 3, 4);

    float m_fTimeDelta       = 0;
    float m_fSimulationSpeed = 1;
    bool  m_bPaused = false;

    MouseState m_LastMouseState;
    float2 m_f2ViewShift = float2(0,0);
    float m_fViewScale = 1.0f;
};

} // namespace Diligent
