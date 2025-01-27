/*  PokemonSV Tests
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *  
 *  
 */


#ifndef PokemonAutomation_Tests_PokemonSV_Tests_H
#define PokemonAutomation_Tests_PokemonSV_Tests_H

#include <vector>
#include <string>

namespace PokemonAutomation{

class ImageViewRGB32;

int test_pokemonSV_MapDetector(const ImageViewRGB32& image, const std::vector<std::string>& words);

int test_pokemonSV_PicnicDetector(const ImageViewRGB32& image, bool target);

int test_pokemonSV_TeraCardFinder(const ImageViewRGB32& image, bool target);

int test_pokemonSV_TerastallizingDetector(const ImageViewRGB32& image, bool target);

int test_pokemonSV_TeraSilhouetteReader(const ImageViewRGB32& image, const std::vector<std::string>& keywords);

int test_pokemonSV_TeraTypeReader(const ImageViewRGB32& image, const std::vector<std::string>& keywords);

int test_pokemonSV_SandwichRecipeDetector(const ImageViewRGB32& image, const std::vector<std::string>& words);

int test_pokemonSV_SandwichHandDetector(const ImageViewRGB32& image, const std::vector<std::string>& words);

}

#endif
