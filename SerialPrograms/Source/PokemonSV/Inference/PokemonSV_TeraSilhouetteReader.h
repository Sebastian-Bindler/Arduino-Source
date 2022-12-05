/*  Tera Silhouette Reader
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_TeraSilhouetteReader_H
#define PokemonAutomation_PokemonSV_TeraSilhouetteReader_H

#include "Common/Cpp/Color.h"
#include "CommonFramework/ImageMatch/ImageMatchResult.h"
#include "CommonFramework/ImageMatch/SilhouetteDictionaryMatcher.h"
#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/VideoPipeline/VideoOverlayScopes.h"

namespace PokemonAutomation{
    struct ProgramInfo;
namespace NintendoSwitch{
namespace PokemonSV{



class TeraSilhouetteReader{
public:
    TeraSilhouetteReader(Color color = COLOR_GREEN);

    void make_overlays(VideoOverlaySet& items) const;
    ImageMatch::ImageMatchResult read(const ImageViewRGB32& screen) const;

private:
    const ImageMatch::SilhouetteDictionaryMatcher& m_matcher;
    Color m_color;
    ImageFloatBox m_box;
};



}
}
}
#endif
