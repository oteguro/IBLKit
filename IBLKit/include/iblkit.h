// ----------------------------------------------------------------------------
// iblkit.h
// ----------------------------------------------------------------------------
#pragma once
#include "context.h"

namespace iblkit
{
    /*!
     * . 
     */
    bool                                FilteringCubemap(Context*                       context,
                                                         ID3D11Texture2D*               inCubemap,
                                                         ID3D11Texture2D**              outCubemap);

    /*!
     * . 
     */
    bool                                ProcessFiltering(Context*                       context);

} // namespace iblkit 
