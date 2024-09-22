#include "PlayMode.hpp"

#include "TextTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <string>


PlayMode::PlayMode() {
	// initialize font
	std::string font_path = data_path("Open_Sans/OpenSans-VariableFont_wdth,wght.ttf");
	// font = std::make_shared<Font>(font_path, 
	// 							/*font_size*/18, 
	// 							/*width*/150, 
	// 							/*height*/150, 
	// 							/*margin*/glm::vec2(10,10), 
	// 							/*line_height*/20);
	font = std::make_shared<Font>(font_path, 
								/*font_size*/30, 
								/*width*/1280,
								/*height*/720);

	story = std::make_shared<Story>();
	description = std::make_shared<Text>(story->get_text(curr_state), 
						/*line height*/35, 
						/*line length*/85, 
						/*start pos*/glm::vec2(30, 120));
	manual = std::make_shared<Text>("Press 'return' to continue, 123 to make choices", 
						/*line height*/35, 
						/*line length*/85, 
						/*start pos*/glm::vec2(590, 690));
	title = std::make_shared<Text>("Escape RTO 5 days", 
						/*line height*/35, 
						/*line length*/85, 
						/*start pos*/glm::vec2(500, 60));
	choice1 = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/70, 
				/*start pos*/glm::vec2(100, 300));
	choice2 = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/70, 
				/*start pos*/glm::vec2(100, 380));
	choice3 = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/70, 
				/*start pos*/glm::vec2(100, 460));
	choice4 = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/70, 
				/*start pos*/glm::vec2(100, 540));
	outcome = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/70, 
				/*start pos*/glm::vec2(100, 500));
	status = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/150, 
				/*start pos*/glm::vec2(60, 80));
	gameover = std::make_shared<Text>("", 
				/*line height*/35, 
				/*line length*/70, 
				/*start pos*/glm::vec2(500, 300));
	texts = {description, manual, title, choice1, choice2, choice3, choice4, outcome, status, gameover};
	font->gen_texture(texture, texts);
	
	//get pointer to camera for convenience:
	// if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	// camera = &scene.cameras.front();

	//start music loop playing:
	// (note: position will be over-ridden in update())
	// leg_tip_loop = Sound::loop_3D(*dusty_floor_sample, 1.0f, get_leg_tip_position(), 10.0f);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_1 && one.cooldown <= 0.0f) {
			one.downs += 1;
			one.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_2 && two.cooldown <= 0.0f) {
			two.downs += 1;
			two.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_3 && three.cooldown <= 0.0f) {
			three.downs += 1;
			three.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_4 && four.cooldown <= 0.0f) {
			four.downs += 1;
			four.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RETURN && enter.cooldown <= 0.0f) {
			enter.downs += 1;
			enter.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_1) {
			one.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_2) {
			two.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_3) {
			three.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_4) {
			four.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RETURN) {
			enter.pressed = false;
			return true;
		}
	} 

	return false;
}

void PlayMode::update(float elapsed) {

	// { //update listener to camera position:
	// 	glm::mat4x3 frame = camera->transform->make_local_to_parent();
	// 	glm::vec3 frame_right = frame[0];
	// 	glm::vec3 frame_at = frame[3];
	// 	Sound::listener.set_position_right(frame_at, frame_right, 1.0f / 60.0f);
	// }
	bool need_update_texture = false;

	if(!story->is_end) {
		if(!choice_made) {
			if(one.downs) {
				one.cooldown = COOLDOWN_TIME;
				need_update_texture = true;
				show_choice_outcome(0);
			}
			if(two.downs) {
				two.cooldown = COOLDOWN_TIME;
				need_update_texture = true;
				show_choice_outcome(1);
			}
			if(three.downs) {
				three.cooldown = COOLDOWN_TIME;
				need_update_texture = true;
				show_choice_outcome(2);
			}
			if(four.downs) {
				four.cooldown = COOLDOWN_TIME;
				need_update_texture = true;
				show_choice_outcome(3);
			}
		}
		
		if(choice_made && enter.downs) {
			enter.cooldown = COOLDOWN_TIME;
			need_update_texture = true;
			choice_made = false;
			show_next_state();
		}

		
	} else {
		if(enter.downs) {
			enter.cooldown = COOLDOWN_TIME;
			need_update_texture = true;
			choice_made = false;
			restart();
		}
	}
	
	if(need_update_texture) {
		font->gen_texture(texture, texts);
		need_update_texture = false;
	}

	if(one.cooldown > 0.0f) {
		one.cooldown -= elapsed;
	}
	if(two.cooldown > 0.0f) {
		two.cooldown -= elapsed;
	}
	if(three.cooldown > 0.0f) {
		three.cooldown -= elapsed;
	}
	if(four.cooldown > 0.0f) {
		four.cooldown -= elapsed;
	}
	if(enter.cooldown > 0.0f) {
		enter.cooldown -= elapsed;
	}
	
	//reset button press counters:
	one.downs = 0;
	two.downs = 0;
	three.downs = 0;
	four.downs = 0;
	enter.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(text_texture_program->program);
	
	// glUseProgram(0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	if(!story->is_end) {
		glUniform3f(glGetUniformLocation(text_texture_program->program, "textColor"), 1.0f, 1.0f, 1.0f);
	} else {
		glUniform3f(glGetUniformLocation(text_texture_program->program, "textColor"), 1.0f, 0.0f, 0.0f);
	}
	glBindVertexArray(text_texture_program->VAO);
	GL_ERRORS();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	GL_ERRORS();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	GL_ERRORS();

	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	GL_ERRORS();
}

void PlayMode::show_choice_outcome(uint32_t choice_idx){
	std::string& choice_id = choice_ids[choice_idx];

	if(choice_idx == 3) {
		// if escape, show outcome and end game
		description->text = story->get_choice_outcome_text(choice_id);
		choice1->text = "";
		choice2->text = "";
		choice3->text = "";
		choice4->text = "";
		gameover->text = "Game Over";
		return;
	}
	
	outcome->text = "> "+story->get_choice_outcome_text(choice_id);
	choice1->text = std::to_string(choice_idx+1)+". "+story->get_text(choice_id);
	choice2->text = "";
	choice3->text = "";
	choice4->text = "";
	status->text = "Day "+std::to_string(story->day)+"                                                                                                           Work done: "+std::to_string(story->work_done);
	choice_made = true;
}

void PlayMode::show_next_state() {
	curr_state = story->get_next_state(curr_state);
	description->text = story->get_text(curr_state);

	// hide outcome
	outcome->text = "";

	// reset choices
	if(story->is_end) {
		choice1->text = "";
		choice2->text = "";
		choice3->text = "";
		choice4->text = "";
		gameover->text = "Game Over";
	} else {
		choice1->text = "1. "+story->get_text("work");
		choice2->text = "2. "+story->get_text("help");
		choice3->text = "3. "+story->get_text("explore");
		choice4->text = "4. "+story->get_text("escape");
	}
	status->text = "Day "+std::to_string(story->day)+"                                                                                                           Work done: "+std::to_string(story->work_done);
}

void PlayMode::restart(){
	story->reset();
	curr_state = "start";
	description->text = story->get_text(curr_state);
	choice1->text = "";
	choice2->text = "";
	choice3->text = "";
	choice4->text = "";
	outcome->text = "";
	status->text = "";
	gameover->text = "";
	choice_made = true;
}