/*  Egg Hatch Filter
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_Pokemon_EggHatchFilter_H
#define PokemonAutomation_Pokemon_EggHatchFilter_H

#include "Common/Cpp/Options/EnumDropdownOption.h"
#include "Common/Cpp/Options/EditableTableOption.h"
#include "Pokemon/Pokemon_IVChecker.h"
#include "Pokemon/Options/Pokemon_IVCheckerOption.h"
#include "Pokemon/Inference/Pokemon_IVCheckerReader.h"

namespace PokemonAutomation{
namespace Pokemon{


enum class EggHatchAction{
    StopProgram,
    Keep,
    Release,
};

enum class EggHatchShinyFilter{
    Anything,
    NotShiny,
    Shiny,
};

enum class EggHatchGenderFilter{
    Any,
    Male,
    Female,
    Genderless
};

std::string gender_to_string(EggHatchGenderFilter gender);




class EggHatchFilterRow : public EditableTableRow{
public:
    EggHatchFilterRow();
    EggHatchFilterRow(EggHatchShinyFilter p_shiny);
    virtual std::unique_ptr<EditableTableRow> clone() const override;

public:
    EnumDropdownCell<EggHatchAction> action;
    EnumDropdownCell<EggHatchShinyFilter> shiny;
    EnumDropdownCell<EggHatchGenderFilter> gender;
    IVCheckerFilterCell iv_hp;
    IVCheckerFilterCell iv_atk;
    IVCheckerFilterCell iv_def;
    IVCheckerFilterCell iv_spatk;
    IVCheckerFilterCell iv_spdef;
    IVCheckerFilterCell iv_speed;
};

class EggHatchFilterTable : public EditableTableOption_t<EggHatchFilterRow>{
public:
    EggHatchFilterTable();
    virtual std::vector<std::string> make_header() const override;
    static std::vector<std::unique_ptr<EditableTableRow>> make_defaults();

    EggHatchAction get_action(bool shiny, const IVCheckerReader::Results& IVs, EggHatchGenderFilter gender) const;
};










}
}
#endif
