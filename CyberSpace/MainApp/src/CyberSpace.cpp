/*
 *  Copyright 2019-2021 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#include <random>

#include "CyberSpace.hpp"
#include "BasicMath.hpp"
#include "MapHelper.hpp"
#include "imgui.h"
#include "ShaderMacroHelper.hpp"
#include "NeighbourShapes.h"

namespace Diligent
{

SampleBase* CreateSample()
{
    return new CyberSpace();
}
//-----------------------------------------------------------------------------
void CyberSpace::CreateRenderParticlePSO()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    PSOCreateInfo.PSODesc.Name = "Render particles PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]                = m_pSwapChain->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat                    = m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    // Disable back face culling
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;
    // Disable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
    // clang-format on

    auto& BlendDesc = PSOCreateInfo.GraphicsPipeline.BlendDesc;

    BlendDesc.RenderTargets[0].BlendEnable = True;
    BlendDesc.RenderTargets[0].SrcBlend    = BLEND_FACTOR_SRC_ALPHA;
    BlendDesc.RenderTargets[0].DestBlend   = BLEND_FACTOR_INV_SRC_ALPHA;

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // Create a shader source stream factory to load shaders from files.
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
    // Create particle vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Particle VS";
        ShaderCI.FilePath        = "particle.vsh";
        m_pDevice->CreateShader(ShaderCI, &pVS);
    }

    // Create particle pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Particle PS";
        ShaderCI.FilePath        = "particle.psh";
        m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    // clang-format off
    // Shader variables should typically be mutable, which means they are expected
    // to change on a per-instance basis
    ShaderResourceVariableDesc Vars[] = 
    {
        {SHADER_TYPE_VERTEX, "g_Particles", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
    };
    // clang-format on
    PSOCreateInfo.PSODesc.ResourceLayout.Variables    = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pRenderParticlePSO);
    m_pRenderParticlePSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_Constants);
    //m_pRenderParticlePSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "Constants")->Set(m_Constants);
}
//-----------------------------------------------------------------------------
void CyberSpace::CreateUpdateParticlePSO()
{
    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // Create a shader source stream factory to load shaders from files.
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;

    ShaderMacroHelper Macros;
    Macros.AddShaderMacro("THREAD_GROUP_SIZE", m_ThreadGroupSize);
    Macros.AddShaderMacro("NEIGHBOUR_SIZE", NEIGHBOUR_SIZE);
    Macros.AddShaderMacro("NEIGHBOUR_NUM", NEIGHBOUR_NUM);
    Macros.AddShaderMacro("NEIGHBOUR_HOOD_DEFINITION_NUM", NEIGHBOUR_HOOD_DEFINITION_NUM);    
    Macros.Finalize();

    RefCntAutoPtr<IShader> pUpdateParticlesCS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_COMPUTE;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Update particles CS";
        ShaderCI.FilePath        = "update.hlsl";
        ShaderCI.Macros          = Macros;
        m_pDevice->CreateShader(ShaderCI, &pUpdateParticlesCS);
    }

    RefCntAutoPtr<IShader> pMoveParticlesCS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_COMPUTE;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Move particles CS";
        ShaderCI.FilePath        = "move.hlsl";
        ShaderCI.Macros          = Macros;
        m_pDevice->CreateShader(ShaderCI, &pMoveParticlesCS);
    }

    ComputePipelineStateCreateInfo PSOCreateInfo;
    PipelineStateDesc&             PSODesc = PSOCreateInfo.PSODesc;

    // This is a compute pipeline
    PSODesc.PipelineType = PIPELINE_TYPE_COMPUTE;

    PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;
    // clang-format off
    ShaderResourceVariableDesc Vars[] = 
    {
        {SHADER_TYPE_COMPUTE, "Constants", SHADER_RESOURCE_VARIABLE_TYPE_STATIC}
    };
    // clang-format on
    PSODesc.ResourceLayout.Variables    = Vars;
    PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    PSODesc.Name      = "Update particles PSO";
    PSOCreateInfo.pCS = pUpdateParticlesCS;
    m_pDevice->CreateComputePipelineState(PSOCreateInfo, &m_pUpdateParticlesPSO);
    m_pUpdateParticlesPSO->GetStaticVariableByName(SHADER_TYPE_COMPUTE, "Constants")->Set(m_Constants);

    PSODesc.Name      = "Move particles PSO";
    PSOCreateInfo.pCS = pMoveParticlesCS;
    m_pDevice->CreateComputePipelineState(PSOCreateInfo, &m_pMoveParticlesPSO);
    m_pMoveParticlesPSO->GetStaticVariableByName(SHADER_TYPE_COMPUTE, "Constants")->Set(m_Constants);
}
//-----------------------------------------------------------------------------
void CyberSpace::CreateParticleBuffers()
{
    //Particle buffer
    m_pParticleAttribsBuffer.Release();

    m_NumParticles = m_iWorldSize * m_iWorldSize;

    BufferDesc BuffDesc;
    BuffDesc.Name              = "Particle attribs buffer";
    BuffDesc.Usage             = USAGE_DEFAULT;
    BuffDesc.BindFlags         = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
    BuffDesc.Mode              = BUFFER_MODE_STRUCTURED;
    BuffDesc.ElementByteStride = sizeof(ParticleAttribs);
    BuffDesc.uiSizeInBytes     = sizeof(ParticleAttribs) * m_NumParticles;

    std::vector<ParticleAttribs> ParticleData(m_NumParticles);

    std::mt19937 gen; // Standard mersenne_twister_engine. Use default seed
                      // to generate consistent distribution.

    std::uniform_real_distribution<float> pos_distr(0.0f, 1.f);
    std::uniform_real_distribution<float> phi_distr(0.f, 2.0*static_cast<float>(M_PI));
    float hSize = m_iWorldSize/2.0f;
    for (int i = 0; i < m_iWorldSize; i++)
    {
        for (int j = 0; j < m_iWorldSize; j++)
        {
            int idx = j + i * m_iWorldSize;
            assert(idx < m_NumParticles);
            auto& p   = ParticleData[idx]; 
            p.i2Pos.x = j;
            p.i2Pos.y = i;
            float r = sqrtf((i - hSize) * (i - hSize) + (j - hSize) * (j - hSize));
            float k = 1.0f - r/hSize;
            if (k < 0) k = 0.0f;
            p.iLife = (pos_distr(gen) > k ? 1 : 0);
            p.iNewLife = 0;

            p.f4Temp = float4(0,0,0,0);

            p.i4NID = m_i4NID;

            for(int jj = 0; jj < NEIGHBOUR_HOOD_DEFINITION_NUM; jj++) {
                p.f3threshold_0[jj] = g_nbs_factors[jj][0];
                p.f3threshold_1[jj] = g_nbs_factors[jj][1];
                p.f3threshold_2[jj] = g_nbs_factors[jj][2];
                p.f3threshold_3[jj] = g_nbs_factors[jj][3];
                p.f3threshold_4[jj] = g_nbs_factors[jj][4];
            }
        }
    }

    BufferData VBData;
    VBData.pData    = ParticleData.data();
    VBData.DataSize = sizeof(ParticleAttribs) * static_cast<Uint32>(ParticleData.size());
    m_pDevice->CreateBuffer(BuffDesc, &VBData, &m_pParticleAttribsBuffer);
    IBufferView* pParticleAttribsBufferSRV = m_pParticleAttribsBuffer->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE);
    IBufferView* pParticleAttribsBufferUAV = m_pParticleAttribsBuffer->GetDefaultView(BUFFER_VIEW_UNORDERED_ACCESS);

    //neighbour shape buffer
    m_pNeighbourAttribsBuffer.Release();

    BuffDesc.Name              = "Neighbour attribs buffer";
    BuffDesc.Usage             = USAGE_DEFAULT;
    BuffDesc.BindFlags         = BIND_SHADER_RESOURCE;
    BuffDesc.Mode              = BUFFER_MODE_STRUCTURED;
    BuffDesc.ElementByteStride = sizeof(NeighbourAttribs);
    BuffDesc.uiSizeInBytes     = sizeof(NeighbourAttribs) * NEIGHBOUR_HOOD_DEFINITION_NUM; //NEIGHBOUR_HOOD_NUM neighbour hoods

    std::vector<NeighbourAttribs> NeighbourData(NEIGHBOUR_HOOD_DEFINITION_NUM);
    for (int k = 0; k < NEIGHBOUR_HOOD_DEFINITION_NUM; k++)
    {
        auto& nb = NeighbourData[k];
        memcpy((char*)(&nb.NB[0]), (char*)(&g_nbs[k][0]), NEIGHBOUR_NUM * sizeof(int));
    }

    BufferData VBNBData;
    VBNBData.pData    = NeighbourData.data();
    VBNBData.DataSize = sizeof(NeighbourAttribs) * static_cast<Uint32>(NeighbourData.size());
    m_pDevice->CreateBuffer(BuffDesc, &VBNBData, &m_pNeighbourAttribsBuffer);
    IBufferView* pNeighbourAttribsBufferSRV = m_pNeighbourAttribsBuffer->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE);

    //set buffer sources
    m_pRenderParticleSRB.Release();
    m_pRenderParticlePSO->CreateShaderResourceBinding(&m_pRenderParticleSRB, true);
    m_pRenderParticleSRB->GetVariableByName(SHADER_TYPE_VERTEX, "g_Particles")->Set(pParticleAttribsBufferSRV);

    m_pMoveParticlesSRB.Release();
    m_pMoveParticlesPSO->CreateShaderResourceBinding(&m_pMoveParticlesSRB, true);
    m_pMoveParticlesSRB->GetVariableByName(SHADER_TYPE_COMPUTE, "g_Particles")->Set(pParticleAttribsBufferUAV);

    m_pUpdateParticlesSRB.Release();
    m_pUpdateParticlesPSO->CreateShaderResourceBinding(&m_pUpdateParticlesSRB, true);
    m_pUpdateParticlesSRB->GetVariableByName(SHADER_TYPE_COMPUTE, "g_Particles")->Set(pParticleAttribsBufferUAV);
    m_pUpdateParticlesSRB->GetVariableByName(SHADER_TYPE_COMPUTE, "g_NeighbourShapes")->Set(pNeighbourAttribsBufferSRV);
}
//-----------------------------------------------------------------------------
void CyberSpace::CreateConsantBuffer()
{
    BufferDesc BuffDesc;
    BuffDesc.Name           = "Constants buffer";
    BuffDesc.Usage          = USAGE_DYNAMIC;
    BuffDesc.BindFlags      = BIND_UNIFORM_BUFFER;
    BuffDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    BuffDesc.uiSizeInBytes  = sizeof(float4) * 3+16;
    m_pDevice->CreateBuffer(BuffDesc, nullptr, &m_Constants);
}
//-----------------------------------------------------------------------------
static std::mt19937 s_gen;
void CyberSpace::UpdateUI()
{
    std::uniform_real_distribution<float> distr1(0.0f, 1.0f);
    g_nbs_factors[0][0].w = distr1(s_gen); //dummy

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        bool recreateBuff = false;
        if (ImGui::InputInt("World Size", &m_iWorldSize, 32, 600, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_NumParticles = m_iWorldSize*m_iWorldSize;
            recreateBuff   = true;
        }
        ImGui::Text("Particle Num: %d", m_NumParticles);
        if (ImGui::Button("Random All & Respawn"))
        {
            for (int k = 0; k < NEIGHBOUR_HOOD_DEFINITION_NUM; k++)
            {
                for(int i = 0; i < 5; i++) 
                {
                    g_nbs_factors[k][i].x = distr1(s_gen);
                    g_nbs_factors[k][i].y = distr1(s_gen);
                    g_nbs_factors[k][i].z = distr1(s_gen) > 0.5 ? 1.1f : 0.0f; //1.1f to int = 1
                }
            }

            std::uniform_int_distribution<int> distrInt(0, NEIGHBOUR_HOOD_DEFINITION_NUM-1);
            m_i4NID.x = distrInt(s_gen);
            m_i4NID.y = distrInt(s_gen);
            m_i4NID.z = distrInt(s_gen);
            m_i4NID.w = distrInt(s_gen);

            recreateBuff = true;
        }
        
        if(ImGui::Button("Random Neighbour Def & Respawn")) {
            for(int k = 0; k < NEIGHBOUR_HOOD_DEFINITION_NUM; k++) {
                for(int i = 0; i < 5; i++) {
                    g_nbs_factors[k][i].x = distr1(s_gen);
                    g_nbs_factors[k][i].y = distr1(s_gen);
                    g_nbs_factors[k][i].z = distr1(s_gen) > 0.5 ? 1.1f : 0.0f; //1.1f to int = 1
                }
            }

            recreateBuff = true;
        }

        if(ImGui::Button("Random Neighbour Order & Respawn")) {

            std::uniform_int_distribution<int> distrInt(0, NEIGHBOUR_HOOD_DEFINITION_NUM - 1);
            m_i4NID.x = distrInt(s_gen);
            m_i4NID.y = distrInt(s_gen);
            m_i4NID.z = distrInt(s_gen);
            m_i4NID.w = distrInt(s_gen);

            recreateBuff = true;
        }

        if(ImGui::Button("Respawn")) {

            recreateBuff = true;
        }

        ImGui::Text("Neighbour Order:");
        if(ImGui::InputInt("NB#0", &m_i4NID.x, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
            m_i4NID.x = clamp(m_i4NID.x, 0, NEIGHBOUR_HOOD_DEFINITION_NUM);
            recreateBuff = true;
        }
        if(ImGui::InputInt("NB#1", &m_i4NID.y, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
            m_i4NID.y = clamp(m_i4NID.y, 0, NEIGHBOUR_HOOD_DEFINITION_NUM);
            recreateBuff = true;
        }
        if(ImGui::InputInt("NB#2", &m_i4NID.z, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
            m_i4NID.z = clamp(m_i4NID.z, 0, NEIGHBOUR_HOOD_DEFINITION_NUM);
            recreateBuff = true;
        }
        if(ImGui::InputInt("NB#3", &m_i4NID.w, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
            m_i4NID.w = clamp(m_i4NID.w, 0, NEIGHBOUR_HOOD_DEFINITION_NUM);
            recreateBuff = true;
        }

        
//        char szName[128];
//         for (int jj = 0; jj < NEIGHBOUR_HOOD_DEFINITION_NUM; jj++)
//         {
//             for(int kk = 0; kk < 5; kk++) {
//                 sprintf_s(szName, "Factor Pass %d NB %d(x)", jj, kk);
//                 ImGui::SliderFloat(szName, &g_nbs_factors[jj][kk].x, 0.0f, 1.0f);
//                 sprintf_s(szName, "Factor Pass %d NB %d(y)", jj, kk);
//                 ImGui::SliderFloat(szName, &g_nbs_factors[jj][kk].y, 0.0f, 1.0f);
//                 sprintf_s(szName, "Factor Pass %d NB %d(z)", jj, kk);
//                 ImGui::SliderFloat(szName, &g_nbs_factors[jj][kk].z, -1.0f, 1.0f);
//             }
//         }

        if (recreateBuff)
            CreateParticleBuffers();

        ImGui::SliderFloat("Simulation Speed", &m_fSimulationSpeed, 0.5f, 200.f);

        ImGui::Text("TimeDelta: %f", m_fTimeDelta);
    }
    ImGui::End();
}
//-----------------------------------------------------------------------------
void CyberSpace::ModifyEngineInitInfo(const ModifyEngineInitInfoAttribs& Attribs)
{
    SampleBase::ModifyEngineInitInfo(Attribs);

    Attribs.EngineCI.Features.ComputeShaders = DEVICE_FEATURE_STATE_ENABLED;
}
//-----------------------------------------------------------------------------
void CyberSpace::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);

    CreateConsantBuffer();
    CreateRenderParticlePSO();
    CreateUpdateParticlePSO();
    CreateParticleBuffers();
}
//-----------------------------------------------------------------------------
void CyberSpace::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    // Clear the back buffer
    const float ClearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};//{0.350f, 0.350f, 0.350f, 1.0f};
    // Let the engine perform required state transitions
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        // Map the buffer and write current world-view-projection matrix
        MapHelper<GlobalConstants> ConstData(m_pImmediateContext, m_Constants, MAP_WRITE, MAP_FLAG_DISCARD);
        ConstData->uiNumParticles = static_cast<Uint32>(m_NumParticles);
        ConstData->fDeltaTime     = m_fTimeDelta;

        float  AspectRatio = static_cast<float>(m_pSwapChain->GetDesc().Width) / static_cast<float>(m_pSwapChain->GetDesc().Height);
        float2 f2Scale     = float2(std::sqrt(1.f / AspectRatio), std::sqrt(AspectRatio));
        ConstData->f2Scale = f2Scale;

        ConstData->i2WorldSize.x = m_iWorldSize;
        ConstData->i2WorldSize.y = m_iWorldSize;

        ConstData->f2WindowSize.x = static_cast<float>(m_pSwapChain->GetDesc().Width);
        ConstData->f2WindowSize.y = static_cast<float>(m_pSwapChain->GetDesc().Height);

        ConstData->f2ViewShift = m_f2ViewShift;
        ConstData->fViewScale  = m_fViewScale;

        ConstData->iNeighbourSize = NEIGHBOUR_SIZE;
    }

    if (!m_bPaused && m_fTimeDelta > (1.0f / m_fSimulationSpeed))
    {
        DispatchComputeAttribs DispatAttribs;
        DispatAttribs.ThreadGroupCountX = (m_NumParticles + m_ThreadGroupSize - 1) / m_ThreadGroupSize;

        m_pImmediateContext->SetPipelineState(m_pUpdateParticlesPSO);
        m_pImmediateContext->CommitShaderResources(m_pUpdateParticlesSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->DispatchCompute(DispatAttribs);

        m_pImmediateContext->SetPipelineState(m_pMoveParticlesPSO);
        m_pImmediateContext->CommitShaderResources(m_pMoveParticlesSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->DispatchCompute(DispatAttribs);

        m_fTimeDelta = 0;
    }

    m_pImmediateContext->SetPipelineState(m_pRenderParticlePSO);
    m_pImmediateContext->CommitShaderResources(m_pRenderParticleSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    DrawAttribs drawAttrs;
    drawAttrs.NumVertices  = 4;
    drawAttrs.NumInstances = static_cast<Uint32>(m_NumParticles);
    m_pImmediateContext->Draw(drawAttrs);
}
//-----------------------------------------------------------------------------
void CyberSpace::Update(double CurrTime, double ElapsedTime)
{
    SampleBase::Update(CurrTime, ElapsedTime);
    UpdateUI();

    m_fTimeDelta += static_cast<float>(ElapsedTime);

    if(m_InputController.WasKeyDown(InputKeys::Space)) {
        m_bPaused = !m_bPaused;
        m_InputController.ClearState();
    }

    const auto& mouseState = m_InputController.GetMouseState();

    //scale
    m_fViewScale += mouseState.WheelDelta;
    if (m_fViewScale < 0) m_fViewScale = 0;

    //shift
    if ((m_LastMouseState.ButtonFlags & MouseState::BUTTON_FLAG_LEFT) != 0)
    {
        float MouseDeltaX = 0;
        float MouseDeltaY = 0;
        if (m_LastMouseState.PosX >= 0 && m_LastMouseState.PosY >= 0 &&
            m_LastMouseState.ButtonFlags != MouseState::BUTTON_FLAG_NONE)
        {
            MouseDeltaX = mouseState.PosX - m_LastMouseState.PosX;
            MouseDeltaY = mouseState.PosY - m_LastMouseState.PosY;
        }
        m_f2ViewShift += float2(MouseDeltaX / m_fViewScale, -MouseDeltaY / m_fViewScale);
    }

    m_LastMouseState = mouseState;
}

} // namespace Diligent
