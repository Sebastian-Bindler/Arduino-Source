/*  End Battle Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonBDSP_EndBattleDetector_H
#define PokemonAutomation_PokemonBDSP_EndBattleDetector_H

#include "Common/Cpp/Color.h"
#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/Inference/VisualDetector.h"
#include "CommonFramework/InferenceInfra/VisualInferenceCallback.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonBDSP{



class EndBattleWatcher : public VisualInferenceCallback{
public:
    EndBattleWatcher(
        const ImageFloatBox& box = {0.1, 0.1, 0.8, 0.8},
        Color color = COLOR_RED
    );

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool process_frame(const ImageViewRGB32& frame, WallClock timestamp) override;

private:
    bool battle_is_over(const ImageViewRGB32& frame);

private:
    Color m_color;
    ImageFloatBox m_box;
    bool m_has_been_black = false;
};



}
}
}
#endif
