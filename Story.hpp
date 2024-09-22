#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Story {
    std::unordered_map<std::string, std::string> texts; // text id -> text
    std::unordered_map<std::string, std::vector<std::string> > choices; // choice id -> choice consequence text id
    
    const uint32_t WORK_REQUIRED_PER_DAY = 2;
    
    bool has_ai = false;

    const uint32_t TOTAL_HELP_DONE_TO_ESCAPE = 4;
    const uint32_t TOTAL_HELP_DONE_TO_GET_AI = 2;
    uint32_t help_done = 0;

    const uint32_t TOTAL_EXPLORE_DONE_TO_ESCAPE = 6;
    uint32_t explore_done = 0;

public:
    uint32_t work_done = 0;
    uint32_t day = 1;
    bool is_end = false;

    Story();

    std::string get_choice_outcome_text(std::string choice_id);

    std::string get_next_state(std::string state_id="");

    std::string get_text(std::string text_id);

    void reset();

    bool is_escapable();
};