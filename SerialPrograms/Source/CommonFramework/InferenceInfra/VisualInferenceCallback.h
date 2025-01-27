/*  Visual Inference Callback
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_CommonFramework_VisualInferenceCallback_H
#define PokemonAutomation_CommonFramework_VisualInferenceCallback_H

#include <memory>
#include <string>
#include "Common/Compiler.h"
#include "Common/Cpp/Time.h"
#include "InferenceCallback.h"

namespace PokemonAutomation{

class ImageViewRGB32;
class ImageRGB32;
class VideoOverlaySet;

//  Base class for a visual inference object to be called perioridically by
//  inference routines in InferenceRoutines.h.
class VisualInferenceCallback : public InferenceCallback{
public:
    VisualInferenceCallback(std::string label)
        : InferenceCallback(InferenceType::VISUAL, std::move(label))
    {}

    //  The inference routines call this function to create video overlays to visualize
    //  regions of interest of the inference callback.
    virtual void make_overlays(VideoOverlaySet& items) const = 0;

    //  Return true if the inference session should stop.
    //  You must override at least one of the overloaded `process_frame()`.
    virtual bool process_frame(const std::shared_ptr<const ImageRGB32>& frame, WallClock timestamp);
    //  Return true if the inference session should stop.
    //  You must override at least one of the overloaded `process_frame()`.
    virtual bool process_frame(const ImageViewRGB32& frame, WallClock timestamp);

};




}
#endif
