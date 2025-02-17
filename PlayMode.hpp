#include "PPU466.hpp"
#include "Mode.hpp"

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
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	glm::vec2 poke_at = glm::vec2(100.0f, 100.0f);

	//started offscreen
	glm::vec2 ball_at = glm::vec2(260.0f, 250.0f);
	size_t playerdir = 0;
	size_t balldir = 0;
	//bool drawBall = false;

	size_t score = 0;

	glm::vec2 score1_at = glm::vec2(240.0f, 232.0f);
	glm::vec2 score2_at = glm::vec2(248.0f, 232.0f);

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
