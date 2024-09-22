#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"
#include "Font.hpp"
#include "Story.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	float COOLDOWN_TIME = 0.1f;
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
		float cooldown = 0.0f;
	} one, two, three, four, enter;

	// font:
	std::shared_ptr<Font> font = nullptr;
	unsigned int texture;

	// story:
	std::shared_ptr<Story> story = nullptr;
	std::string curr_state = "start";

	// text:
	std::shared_ptr<Text> manual = nullptr;
	std::shared_ptr<Text> title = nullptr;
	std::shared_ptr<Text> description = nullptr;
	std::shared_ptr<Text> choice1 = nullptr;
	std::shared_ptr<Text> choice2 = nullptr;
	std::shared_ptr<Text> choice3 = nullptr;
	std::shared_ptr<Text> choice4 = nullptr;
	std::shared_ptr<Text> outcome = nullptr;
	std::shared_ptr<Text> status = nullptr;
	std::shared_ptr<Text> gameover = nullptr;


	std::vector<std::shared_ptr<Text> > texts;

	std::array<std::string, 4> choice_ids = {"work", "help", "explore", "escape"};
	bool choice_made = true;

	void show_next_state();
	void show_choice_outcome(uint32_t choice_idx);
	void restart();
};
