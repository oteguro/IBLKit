// ----------------------------------------------------------------------------
// sh3.h
// ----------------------------------------------------------------------------
#pragma once
#include <DirectXMath.h>

namespace iblkit
{
    /*!
     * 
     */
    class SH3
    {
    private:
        static const int kMaxBandCount = 1 + 3 + 5;
        static float                    SHBandFactor[kMaxBandCount];

        float                           m_SHr[kMaxBandCount];
        float                           m_SHg[kMaxBandCount];
        float                           m_SHb[kMaxBandCount];

    public:
         SH3();
        ~SH3();



    }; // class SH3 


} // namespace iblkit 

