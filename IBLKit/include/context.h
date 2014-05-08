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

        UINT                            m_jobIndex;
        UINT                            m_jobSize;

        ID3D11Texture2D*                m_inCubemap;
        ID3D11UnorderedAccessView*      m_outCubemap;

        Context()
            : m_mode        (kMode_None)
            , m_state       (kState_Ready)
            , m_d3dDevice   (nullptr)
            , m_d3dImContext(nullptr)
            , m_jobIndex    (0)
            , m_jobSize     (0)
        {
        }

        Context(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dImContext)
            : m_mode        (kMode_None)
            , m_state       (kState_Ready)
            , m_d3dDevice   (d3dDevice)
            , m_d3dImContext(d3dImContext)
            , m_jobIndex    (0)
            , m_jobSize     (0)
        {
        }

        ~Context()
        {
            
        }

        bool                Processing() const
        {
            return (m_mode==kState_Ready) ? true : false;
        }

    }; // struct Context 


} // namespace iblkit 

