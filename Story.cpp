#include "Story.hpp"

Story::Story() {
    //states
    texts["start"] = "You are a software engineer. One day, you receive a dreaded notice from your boss: you are required to return to the office five days a week. Reluctantly, you comply, but when you arrive at the office, something isn't right. The doors lock behind you, and you realize you are trapped. Your boss demands that you work around the clock, 24 hours a day, without rest. The office has become a prison, and the workload is inhumane. You know you cannot survive in this suffocating environment for long. Now, you must find a way to escape before it is too late. You have only 5 days.";

    texts["daily1"] = "It's the start of a new day, 00:00. Midnight strikes, but sleep is a luxury you can’t afford. You sit at your desk, the clock ticking loudly in the silence. Time is limited, but so are your choices.";
    texts["daily2"] = "It's 06:00. Usually, you wouldn't be awake at this hour. But here you are, having already worked six hours, with a long day still ahead. You sit at your desk, the clock ticking loudly in the silence. Time is limited, but so are your choices.";
    texts["daily3"] = "It's 12:00. You are hungry and tired. Yet, the day isn’t over. You sit at your desk, the clock ticking loudly in the silence. Time is limited, but so are your choices.";
    texts["daily4"] = "It's 18:00. You are exhausted, but rest is not an option. You know you have to keep working. ou sit at your desk, the clock ticking loudly in the silence. Time is limited, but so are your choices.";

    // endings
    texts["be1"] = "You fails to escape the office. You are now a prisoner of your boss. You will never see the light of day again.";
    texts["gg"] = "You boss catches you slacked off at work. He is furious and you will now suffer the consequence.";

    // choices
    texts["work"] = "Work: Focus on completing your tasks.";
    texts["explore"] = "Explore: The office is unusually quiet. Perhaps you can slip away and search for an exit or clue.";
    texts["help"] = "Help your colleague: Your colleague is struggling with a bug. Do you want to spend the time helping him?";
    texts["escape"] = "Escape: You cannot bear it anymore. You want to make a way out.";

    // map choices to outcomes
    choices["work"].push_back("You complete some of your assigned tasks for the day. You feel the pressure ease slightly, but the clock is ticking.");
    choices["work"].push_back("With the help of AI assistant, you finish twice amount of the tasks this time.");

    choices["explore"].push_back("You wander around the office, searching for a way out. You find a locked door, but no key. You return to your desk, feeling more trapped than ever.");
    choices["explore"].push_back("You haven’t found much yet, but maybe there’s something hidden in the breakroom or storage area.");
    choices["explore"].push_back("You manage to sneak into the storage room and find a keycard.");
    choices["explore"].push_back("The keycard works! You’ve unlocked the locked door which leads to the control room. Perhaps you can dig into the security system and find a weakness. But it needs some time.");
    choices["explore"].push_back("You hacks into the security system and find logs showing the security camera schedule. This could help you avoid detection later when you try to escape.");
    choices["explore"].push_back("You use the camera schedule to avoid detection and discover an emergency exit plan pinned in the HR office.");
    choices["explore"].push_back("You have already explored the office and found nothing new.");

    choices["help"].push_back("You help your colleague with the bug. But he does not express any appreciation.");
    choices["help"].push_back("You help your colleague with another bug. He thanks you this time and in return he teaches you to use a powerful AI assistant that can double your work efficiency.");
    choices["help"].push_back("You help your colleague fix a critical bug, and he seems genuinely relieved. He starts to complain about the workload and the boss with you.");
    choices["help"].push_back("You quickly help your colleague fix the bug. The two of you spend the remaining time cursing the boss and the company.");

    choices["escape"].push_back("Your colleague reports your escape plan to the boss. As you make your move, you're caught right by the boss. Now, there's no way out and you'll face the harshest punishment!");
    choices["escape"].push_back("You escape the office with your colleague's help. You finally breaks free from the prison. The nightmare has ended. But who knows what awaits you outside.");
    choices["escape"].push_back("You are desparate and you attempt to escape. But you are too imprudent and get caught by the security guard. Now, the worst punishment awaits you.");
}

std::string Story::get_choice_outcome_text(std::string choice_id) {
    if (choice_id == "work") {
        if(has_ai) {
            work_done += 2;
            return choices["work"][1];
        } else {
            work_done++;
            return choices["work"][0];
        }
    } else if (choice_id == "explore") {
        if(explore_done >= TOTAL_EXPLORE_DONE_TO_ESCAPE) {
            return choices["explore"].back();
        }
        return choices["explore"][explore_done++];
    } else if (choice_id == "help") {
        if (help_done+1 == TOTAL_HELP_DONE_TO_GET_AI) {
            has_ai = true;
        }
        if (help_done == TOTAL_HELP_DONE_TO_ESCAPE) {
            return choices["help"].back();
        }
        return choices["help"][help_done++];
    } else if (choice_id == "escape") {
        is_end = true;
        if (explore_done >= TOTAL_EXPLORE_DONE_TO_ESCAPE) {
            if(help_done >= TOTAL_HELP_DONE_TO_ESCAPE) {
                // escape successfully
                return choices["escape"][1];
            } else {
                // reported by colleague, fails
                return choices["escape"][0];
            }
        } else {
            // not enough exploration
            return choices["escape"][2];
        }
    }
    return "Invalid choice";
}

std::string Story::get_next_state(std::string state_id) {
    std::string next_state_id;
    if (state_id == "") {
        next_state_id = "start";
    } else if (state_id == "start") {
        next_state_id = "daily1";
    } else if (state_id == "daily1") {
        next_state_id = "daily2";
    } else if (state_id == "daily2") {
        next_state_id = "daily3";
    } else if (state_id == "daily3") {
        next_state_id = "daily4";
    } else if (state_id == "daily4") {
        if(day == 5) {
            is_end = true;
            next_state_id = "be1";
        } else {
            if (work_done >= WORK_REQUIRED_PER_DAY * day) {
                day++;
                next_state_id = "daily1";
            } else {
                is_end = true;
                next_state_id = "gg";
            }
        } 
        
    } 
    return next_state_id;
}

std::string Story::get_text(std::string text_id) {
    return texts[text_id];
}

void Story::reset() {
    work_done = 0;
    day = 1;
    is_end = false;
    has_ai = false;
    help_done = 0;
    explore_done = 0;
}