// ----------------------------------------------------------------------------
// context.h
// ----------------------------------------------------------------------------
#pragma once
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>

namespace iblkit
{
    /*!
     *  
     */
    enum ProcessingMode
    {
        kMode_None = 0,
        kMode_FilterCubemap,
        kMode_SHCubemap,
    };

    /*!
     *  
     */
    enum ProcessingState
    {
        kState_Ready = 0,
        kState_Processing,
        kState_Finished,
    };

    /*!
     *  
     */
    struct Context
    {
        ProcessingMode                  m_mode;
        ProcessingState                 m_state;

        ID3D11Device*                   m_d3dDevice;
        ID3D11DeviceContext*            m_d3dImContext;
        ID3D11ComputeShader*            m_filterCS;

        UINT                            m_jobIndex;
        UINT                            m_jobSize;
        UINT                            m_uavCount;

        ID3D11Texture2D*                m_inCubemap;
        ID3D11ShaderResourceView*       m_inCubemapSRV;
        ID3D11SamplerState*             m_inCubemapSampler;
        ID3D11UnorderedAccessView**     m_outCubemap;
        ID3D11Buffer*                   m_parameter;

        D3D11_TEXTURE2D_DESC            m_desc;

        Context()
            : m_mode            (kMode_None)
            , m_state           (kState_Ready)
            , m_d3dDevice       (nullptr)
            , m_d3dImContext    (nullptr)
            , m_filterCS        (nullptr)
            , m_jobIndex        (0)
            , m_jobSize         (0)
            , m_uavCount        (0)
            , m_inCubemap       (nullptr)
            , m_inCubemapSRV    (nullptr)
            , m_inCubemapSampler(nullptr)
            , m_outCubemap      (nullptr)
            , m_parameter       (nullptr)
        {
        }

        Context(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dImContext)
            : m_mode            (kMode_None)
            , m_state           (kState_Ready)
            , m_d3dDevice       (d3dDevice)
            , m_d3dImContext    (d3dImContext)
            , m_filterCS        (nullptr)
            , m_jobIndex        (0)
            , m_jobSize         (0)
            , m_uavCount        (0)
            , m_inCubemap       (nullptr)
            , m_inCubemapSRV    (nullptr)
            , m_inCubemapSampler(nullptr)
            , m_outCubemap      (nullptr)
            , m_parameter       (nullptr)
        {
        }

        ~Context()
        {
        }

        void                Release()
        {
            if (m_uavCount && m_outCubemap)
            {
                for (UINT i = 0; i < m_uavCount; ++i)
                {
                    if (m_outCubemap[i] != nullptr)
                    {
                        m_outCubemap[i]->Release();
                    }
                }
                m_uavCount = 0;

                delete[] m_outCubemap;
                m_outCubemap = nullptr;
            }
            if(m_parameter)
            {
                m_parameter->Release();
                m_parameter = nullptr;
            }
            if(m_inCubemapSampler)
            {
                m_inCubemapSampler->Release();
                m_inCubemapSampler = nullptr;
            }
            if(m_inCubemapSRV)
            {
                m_inCubemapSRV->Release();
                m_inCubemapSRV = nullptr;
            }
            if(m_filterCS)
            {
                m_filterCS->Release();
                m_filterCS = nullptr;
            }
        }

        bool                Processing() const
        {
            return (m_state==kState_Processing) ? true : false;
        }

    }; // struct Context 


} // namespace iblkit 

