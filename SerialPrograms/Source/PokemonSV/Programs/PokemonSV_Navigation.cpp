/*  Navigation
 exit_tera_without_catching/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/Exceptions.h"
#include "CommonFramework/InferenceInfra/InferenceRoutines.h"
//#include "CommonFramework/Inference/BlackScreenDetector.h"
#include "NintendoSwitch/NintendoSwitch_Settings.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_PushButtons.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_ScalarButtons.h"
#include "NintendoSwitch/Inference/NintendoSwitch_DateReader.h"
#include "NintendoSwitch/Programs/NintendoSwitch_GameEntry.h"
#include "PokemonSwSh/Commands/PokemonSwSh_Commands_DateSpam.h"
#include "PokemonSV/PokemonSV_Settings.h"
#include "PokemonSV/Inference/PokemonSV_DialogDetector.h"
#include "PokemonSV/Inference/PokemonSV_MainMenuDetector.h"
#include "PokemonSV/Inference/PokemonSV_OverworldDetector.h"
#include "PokemonSV/Inference/PokemonSV_MapDetector.h"
#include "PokemonSV/Inference/PokemonSV_PicnicDetector.h"
#include "PokemonSV_Navigation.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

namespace {

void save_game_from_menu_impl(ConsoleHandle& console, BotBaseContext& context){
    console.log("Saving game from menu...");
    WallClock start = current_time();
    bool saved = false;
    while (true){
        context.wait_for_all_requests();
        if (current_time() - start > std::chrono::minutes(5)){
            throw OperationFailedException(console, "save_game_from_menu(): Failed to save game after 5 minutes.");
        }

        MainMenuWatcher menu(COLOR_RED);
        GradientArrowWatcher confirmation(COLOR_YELLOW, GradientArrowType::RIGHT, {0.72, 0.55, 0.05, 0.08});
        AdvanceDialogWatcher finished(COLOR_GREEN);

        int ret = wait_until(
            console, context,
            std::chrono::seconds(60),
            {menu, confirmation, finished}
        );
        context.wait_for(std::chrono::milliseconds(100));
        switch (ret){
        case 0:
            if (saved){
                console.log("Detected main menu. Finished!");
                return;
            }else{
                console.log("Detected main menu. Saving game...");
                pbf_press_button(context, BUTTON_R, 20, 105);
                continue;
            }
        case 1:
            console.log("Detected save confirmation prompt.");
            pbf_press_button(context, BUTTON_A, 20, 105);
            continue;
        case 2:
            console.log("Detected save finished dialog.");
            pbf_press_button(context, BUTTON_B, 20, 105);
            saved = true;
            continue;
        default:
            throw OperationFailedException(console.logger(), "save_game_from_menu(): No recognized state after 60 seconds.");
        }
    }
}

} // anonymous namespace

void save_game_from_menu(ConsoleHandle& console, BotBaseContext& context){
    context.wait_for_all_requests();
    console.overlay().add_log("Save game", COLOR_YELLOW);
    save_game_from_menu_impl(console, context);
}

void save_game_from_overworld(ConsoleHandle& console, BotBaseContext& context){
    context.wait_for_all_requests();
    console.overlay().add_log("Save game", COLOR_YELLOW);
    console.log("Saving game from overworld...");
    WallClock start = current_time();
    bool saved = false;
    while (true){
        if (current_time() - start > std::chrono::minutes(5)){
            throw OperationFailedException(console, "save_game_from_overworld(): Failed to save game after 5 minutes.");
        }

        int ret;
        {
            OverworldWatcher overworld(COLOR_CYAN);
            MainMenuWatcher main_menu(COLOR_RED);
            context.wait_for_all_requests();
            ret = wait_until(
                console, context,
                std::chrono::seconds(60),
                {overworld, main_menu}
            );
        }
        context.wait_for(std::chrono::milliseconds(100));
        switch (ret){
        case 0:
            console.log("Detected overworld.");
            if (saved){
                return;
            }else{
                pbf_press_button(context, BUTTON_X, 20, 105);
                continue;
            }
        case 1:
            console.log("Detected main menu.");
            if (!saved){
                save_game_from_menu_impl(console, context);
                saved = true;
            }
            pbf_press_button(context, BUTTON_B, 20, 105);
            continue;
        default:
            throw OperationFailedException(console.logger(), "save_game_from_overworld(): No recognized state after 60 seconds.");
        }
    }
}



void connect_to_internet_from_overworld(ConsoleHandle& console, BotBaseContext& context){
    WallClock start = current_time();
    bool connected = false;
    while (true){
        if (current_time() - start > std::chrono::minutes(5)){
            throw OperationFailedException(console, "connect_to_internet_from_overworld(): Failed to connect to internet after 5 minutes.");
        }

        OverworldWatcher overworld(COLOR_RED);
        MainMenuWatcher main_menu(COLOR_YELLOW);
        AdvanceDialogWatcher dialog(COLOR_GREEN);
        PromptDialogWatcher prompt(COLOR_CYAN);
        context.wait_for_all_requests();
        int ret = wait_until(
            console, context,
            std::chrono::seconds(60),
            {overworld, main_menu, dialog, prompt}
        );
        context.wait_for(std::chrono::milliseconds(100));
        switch (ret){
        case 0:
            console.log("Detected overworld.");
            if (connected){
                return;
            }else{
                pbf_press_button(context, BUTTON_X, 20, 105);
                continue;
            }
        case 1:
            console.log("Detected main menu.");
            if (connected){
                pbf_press_button(context, BUTTON_B, 20, 105);
            }else{
                pbf_press_button(context, BUTTON_L, 20, 105);
            }
            continue;
        case 2:
            console.log("Detected dialog.");
            connected = true;
            pbf_press_button(context, BUTTON_B, 20, 105);
            continue;
        case 3:
            console.log("Already connected to internet.");
            connected = true;
            pbf_press_button(context, BUTTON_B, 20, 105);
            continue;
        default:
            throw OperationFailedException(console.logger(), "connect_to_internet_from_overworld(): No recognized state after 60 seconds.");
        }
    }
}

void set_time_to_12am_from_home(ConsoleHandle& console, BotBaseContext& context){
    DateReader reader;
    VideoOverlaySet overlays(console.overlay());
    reader.make_overlays(overlays);

//    pbf_press_button(context, BUTTON_HOME, 10, GameSettings::instance().GAME_TO_HOME_DELAY);
    home_to_date_time(context, true, false);
    pbf_press_button(context, BUTTON_A, 20, 30);
    reader.set_hours(console, context, 0);
    pbf_press_button(context, BUTTON_A, 20, 30);
    pbf_press_button(context, BUTTON_HOME, 20, ConsoleSettings::instance().SETTINGS_TO_HOME_DELAY);
//    resume_game_from_home(console, context);
}

void day_skip_from_overworld(ConsoleHandle& console, BotBaseContext& context){
    pbf_press_button(context, BUTTON_HOME, 10, GameSettings::instance().GAME_TO_HOME_DELAY);
    home_to_date_time(context, true, true);
    ssf_press_button(context, BUTTON_A, 20, 10);
    ssf_issue_scroll(context, DPAD_RIGHT, 0);
    ssf_press_button(context, BUTTON_A, 2);
    ssf_issue_scroll(context, SSF_SCROLL_RIGHT, 3);
    ssf_issue_scroll(context, SSF_SCROLL_RIGHT, 3);
    ssf_press_button(context, BUTTON_A, 0);
    ssf_issue_scroll(context, SSF_SCROLL_RIGHT, 3);
    ssf_issue_scroll(context, SSF_SCROLL_RIGHT, 3);
    ssf_issue_scroll(context, SSF_SCROLL_RIGHT, 0);
    ssf_press_button(context, BUTTON_A, 20, 10);
    pbf_press_button(context, BUTTON_HOME, 20, ConsoleSettings::instance().SETTINGS_TO_HOME_DELAY);
    resume_game_from_home(console, context);
}

void open_map_from_overworld(ConsoleHandle& console, BotBaseContext& context){
    {
        OverworldWatcher overworld(COLOR_CYAN);
        context.wait_for_all_requests();
        int ret = wait_until(
            console, context,
            std::chrono::seconds(10),
            {overworld}
        );
        context.wait_for(std::chrono::milliseconds(100));
        if (ret == 0){
            console.log("Detected overworld.");
            pbf_press_button(context, BUTTON_Y, 20, 105); // open map
        }
        else{
            throw OperationFailedException(console.logger(), "open_map_from_overworld(): No overworld state found after 10 seconds.");
        }
    }

    WallClock start = current_time();
    while (true){
        if (current_time() - start > std::chrono::minutes(1)){
            throw OperationFailedException(console, "open_map_from_overworld(): Failed to open map after 1 minute.");
        }

        OverworldWatcher overworld(COLOR_CYAN);
        MapWatcher map(COLOR_RED);

        context.wait_for_all_requests();
        int ret = wait_until(
            console, context,
            std::chrono::seconds(30),
            {overworld, map}
        );
        context.wait_for(std::chrono::milliseconds(100));
        switch (ret){
        case 0:
            console.log("Detected overworld.");
            pbf_press_button(context, BUTTON_Y, 20, 105); // open map
            continue;
        case 1:
            console.log("Detected map.");
            console.overlay().add_log("Map opened", COLOR_WHITE);
            if (map.map_in_fixed_view()){
                return;
            }
            else{ // click R joystick to change to fixed view
                console.log("Map in rotate view, fix it");
                console.overlay().add_log("Change map to fixed view", COLOR_WHITE);
                pbf_press_button(context, BUTTON_RCLICK, 20, 105);
                continue;
            }
        default:
            throw OperationFailedException(console.logger(), "open_map_from_overworld(): No recognized state after 30 seconds.");
        }
    }
}

void fly_to_overworld_from_map(ConsoleHandle& console, BotBaseContext& context){
    context.wait_for_all_requests();
    WallClock start = current_time();
    while (true){
        if (current_time() - start > std::chrono::minutes(1)){
            throw OperationFailedException(console, "fly_to_overworld_from_map(): Failed to open map after 1 minute.");
        }

        int ret = 0;
        {
            OverworldWatcher overworld(COLOR_CYAN);
            MapWatcher map(COLOR_RED);
            GradientArrowWatcher spot_dialog_watcher(COLOR_YELLOW, GradientArrowType::RIGHT, {0.469, 0.500, 0.215, 0.150});
            PromptDialogWatcher confirm_watcher(COLOR_BLUE, {0.686, 0.494, 0.171, 0.163});

            context.wait_for_all_requests();
            ret = wait_until(
                console, context,
                std::chrono::seconds(30),
                {overworld, map, spot_dialog_watcher, confirm_watcher}
            );
        }
        context.wait_for(std::chrono::milliseconds(100));
        switch (ret){
        case 0:
            console.log("Detected overworld.");
            return;
        case 1:
            console.log("Detected map.");
            // Press A to bring up the promp dialog on choosing "Fly here", "Set as destination", "Never mind".
            pbf_press_button(context, BUTTON_A, 20, 130);
            continue;
        case 2:
            console.log("Detected fly here prompt dialog.");
            console.overlay().add_log("Fly here", COLOR_WHITE);
            pbf_press_button(context, BUTTON_A, 20, 130);
            continue;
        case 3:
            console.log("Detected fly confirmation prompt.");
            pbf_press_button(context, BUTTON_A, 20, 130);
            continue;
            
        default:
            throw OperationFailedException(console.logger(), "fly_to_overworld_from_map(): No recognized state after 30 seconds.");
        }
    }
}


void picnic_from_overworld(ConsoleHandle& console, BotBaseContext& context){
    context.wait_for_all_requests();
    console.log("Start picnic from overworld...");
    WallClock start = current_time();
    while (true){
        if (current_time() - start > std::chrono::minutes(3)){
            throw OperationFailedException(console, "picnic_from_overworld(): Failed to start picnic after 3 minutes.");
        }

        OverworldWatcher overworld(COLOR_CYAN);
        MainMenuWatcher main_menu(COLOR_RED);
        PicnicWatcher picnic;
        context.wait_for_all_requests();
        int ret = wait_until(
            console, context,
            std::chrono::seconds(30),
            {overworld, main_menu, picnic}
        );
        context.wait_for(std::chrono::milliseconds(100));
        switch (ret){
        case 0:
            console.log("Detected overworld.");
            pbf_press_button(context, BUTTON_X, 20, 105); // open menu 
            continue;
        case 1:
            console.log("Detected main menu.");
            main_menu.move_cursor(console, context, MenuSide::RIGHT, 2, true);
            pbf_mash_button(context, BUTTON_A, 125); // mash button A to enter picnic mode
            continue;
        case 2:
            console.log("Detected picnic.");
            console.overlay().add_log("Start picnic", COLOR_WHITE);
            // extra wait to make sure by the end the player can move.
            // the player throwing out pokeballs animation is long.
            pbf_wait(context, 1000);
            context.wait_for_all_requests();
            return;
        default:
            throw OperationFailedException(console.logger(), "picnic_from_overworld(): No recognized state after 60 seconds.");
        }
    }
}

void leave_picnic(ConsoleHandle& console, BotBaseContext& context){
    context.wait_for_all_requests();
    console.log("Leaving picnic...");
    console.overlay().add_log("Leaving picnic", COLOR_WHITE);

    pbf_press_button(context, BUTTON_Y, 30, 100);
    for(int i = 0; i < 5; i++){
        PromptDialogWatcher prompt(COLOR_RED, {0.595, 0.517, 0.273, 0.131});
        context.wait_for_all_requests();
        int ret = wait_until(
            console, context,
            std::chrono::seconds(5),
            {prompt}
        );

        if (ret == 0){
            console.log("Detected end picnic prompt");
            break;
        }

        if (i == 4){
            throw OperationFailedException(console, "leave_picnic(): Failed to leave picnic after 5 tries.");
        }

        // prompt not found, maybe button Y dropped?
        pbf_press_button(context, BUTTON_Y, 30, 100);
    }

    // We have now the prompt to asking for confirmation of leaving picnic.
    // Mash A to confirm
    pbf_mash_button(context, BUTTON_A, 150);
    context.wait_for_all_requests();
    
    // Wait for overworld:
    OverworldWatcher overworld(COLOR_CYAN);
    int ret = wait_until(
        console, context,
        std::chrono::seconds(20),
        {overworld}
    );
    if (ret < 0){
        throw OperationFailedException(console, "leave_picnic(): Failed to detecxt overworld after 20 seconds.");
    }
    // Wait three more seconds to make sure the player character is free to operate:
    context.wait_for(std::chrono::seconds(3));
}


}
}
}
