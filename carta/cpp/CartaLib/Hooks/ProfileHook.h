/**
 * Hook for generating profile data.
 *
 **/

#pragma once

#include "CartaLib/CartaLib.h"
#include "CartaLib/IPlugin.h"
#include "CartaLib/RegionInfo.h"
#include "CartaLib/ProfileInfo.h"

namespace Carta
{
namespace Lib
{
namespace Image {
class ImageInterface;
}
namespace Hooks
{
class ProfileHook : public BaseHook
{
    CARTA_HOOK_BOILER1( ProfileHook );

public:
   //The intensity counts
   typedef std::vector<double> ResultType;

    /**
     * @brief Params
     */
     struct Params {

            Params( std::shared_ptr<Image::ImageInterface> dataSource,
                    Carta::Lib::RegionInfo regionInfo,
                    Carta::Lib::ProfileInfo profileInfo ){
                m_dataSource = dataSource;
                m_regionInfo = regionInfo;
                m_profileInfo = profileInfo;
            }

            std::shared_ptr<Image::ImageInterface> m_dataSource;
            Carta::Lib::RegionInfo m_regionInfo;
            Carta::Lib::ProfileInfo m_profileInfo;
        };

    /**
     * @brief PreRender
     * @param pptr
     *
     * @todo make hook constructors protected, so that only hook helper can create them
     */
    ProfileHook( Params * pptr ) : BaseHook( staticId ), paramsPtr( pptr )
    {
        CARTA_ASSERT( is < Me > () );
    }

    ResultType result;
    Params * paramsPtr;
};
}
}
}