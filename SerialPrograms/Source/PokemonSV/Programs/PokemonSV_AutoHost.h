/*  Auto Host
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_AutoHost_H
#define PokemonAutomation_PokemonSV_AutoHost_H

#include "Common/Cpp/Options/BooleanCheckBoxOption.h"
#include "Common/Cpp/Options/SimpleIntegerOption.h"
#include "Common/Cpp/Options/TextEditOption.h"
#include "Common/Cpp/Options/EnumDropdownOption.h"
#include "CommonFramework/Notifications/EventNotificationsTable.h"
#include "NintendoSwitch/NintendoSwitch_SingleSwitchProgram.h"
#include "PokemonSV/Options/PokemonSV_PlayerList.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{



class AutoHost_Descriptor : public SingleSwitchProgramDescriptor{
public:
    AutoHost_Descriptor();

    struct Stats;
    virtual std::unique_ptr<StatsTracker> make_stats() const override;

};




class AutoHost : public SingleSwitchProgramInstance{
public:
    AutoHost();
    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:
    WallClock wait_for_lobby_open(SingleSwitchProgramEnvironment& env, BotBaseContext& context);
    void update_stats_on_raid_start(SingleSwitchProgramEnvironment& env, uint8_t player_count);
    bool start_raid(
        SingleSwitchProgramEnvironment& env, BotBaseContext& context,
        WallClock start_time,
        uint8_t player_count
    );
    bool run_lobby(SingleSwitchProgramEnvironment& env, BotBaseContext& context);

private:
//    OCR::LanguageOCR LANGUAGE;

    enum class Mode{
        LOCAL,
        ONLINE_CODED,
        ONLINE_EVERYONE,
    };

    EnumDropdownOption<Mode> MODE;

    SimpleIntegerOption<uint16_t> LOBBY_WAIT_DELAY;
    IntegerEnumDropdownOption START_RAID_PLAYERS;
    BooleanCheckBoxOption ROLLOVER_PREVENTION;

    TextEditOption DESCRIPTION;

    SimpleIntegerOption<uint8_t> CONSECUTIVE_FAILURE_PAUSE;
    SimpleIntegerOption<uint16_t> FAILURE_PAUSE_MINUTES;
    BooleanCheckBoxOption TRY_TO_TERASTILIZE;

    RaidPlayerBanList BAN_LIST;
    BooleanCheckBoxOption IGNORE_WHITELIST;

    EventNotificationOption NOTIFICATION_RAID_POST;
    EventNotificationOption NOTIFICATION_RAID_START;
    EventNotificationsOption NOTIFICATIONS0;
};




}
}
}
#endif
