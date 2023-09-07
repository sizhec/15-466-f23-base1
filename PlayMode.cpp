#include "PlayMode.hpp"


//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

//added files
#include "data_path.hpp"
#include "load_save_png.hpp"

PlayMode::PlayMode() {
	//TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.

	//Also, *don't* use these tiles in your game:

	//data_path

	glm::uvec2 player_size;
	std::vector<glm::u8vec4> player_data;
	//According to load png it takes in
	// 1. string filename used datapath
	// 2. glm::uvec2 *size w
	// 3. vector< glm::u8vec4 > *data
	// 4. origin where is defined as either LowerLeftOrigin or  UpperLeftOrigin
	load_png(data_path("player.png"), &player_size, &player_data, LowerLeftOrigin);



	glm::uvec2 poke1_size;
	std::vector<glm::u8vec4> poke1_data;	
	load_png(data_path("poke1.png"), &poke1_size, &poke1_data, LowerLeftOrigin);

	glm::uvec2 zero_size;
	std::vector<glm::u8vec4> zero_data;
	load_png(data_path("zero.png"), &zero_size, &zero_data, LowerLeftOrigin);

	glm::uvec2 one_size;
	std::vector<glm::u8vec4> one_data;
	load_png(data_path("one.png"), &one_size, &one_data, LowerLeftOrigin);

	glm::uvec2 two_size;
	std::vector<glm::u8vec4> two_data;
	load_png(data_path("two.png"), &two_size, &two_data, LowerLeftOrigin);

	glm::uvec2 three_size;
	std::vector<glm::u8vec4> three_data;
	load_png(data_path("three.png"), &three_size, &three_data, LowerLeftOrigin);

	glm::uvec2 four_size;
	std::vector<glm::u8vec4> four_data;
	load_png(data_path("four.png"), &four_size, &four_data, LowerLeftOrigin);
	
	glm::uvec2 five_size;
	std::vector<glm::u8vec4> five_data;
	load_png(data_path("five.png"), &five_size, &five_data, LowerLeftOrigin);
	
	glm::uvec2 six_size;
	std::vector<glm::u8vec4> six_data;
	load_png(data_path("six.png"), &six_size, &six_data, LowerLeftOrigin);

	glm::uvec2 seven_size;
	std::vector<glm::u8vec4> seven_data;
	load_png(data_path("seven.png"), &seven_size, &seven_data, LowerLeftOrigin);

	glm::uvec2 eight_size;
	std::vector<glm::u8vec4> eight_data;
	load_png(data_path("eight.png"), &eight_size, &eight_data, LowerLeftOrigin);

	glm::uvec2 nine_size;
	std::vector<glm::u8vec4> nine_data;
	load_png(data_path("nine.png"), &nine_size, &nine_data, LowerLeftOrigin);

	glm::uvec2 ball_size;
	std::vector<glm::u8vec4> ball_data;
	load_png(data_path("ball.png"), &ball_size, &ball_data, LowerLeftOrigin);
	
	auto load_from_png_data = [&](size_t tile_index, size_t palette_index, std::vector<glm::u8vec4> data){

		ppu.tile_table[tile_index].bit0 = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		};

		ppu.tile_table[tile_index].bit1 = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		};

		//ppu.palette_table[palette_index] ={};

		size_t colorSeen = 0;

		// a naive loading that assume png are in 8*8 with less than 4 colours including transparent
	    // Assume tile table are init with all 0 vector (turn out to be bad assumption please zero out first)
		for(size_t i =0; i < 8; i++){
			for (size_t j = 0; j < 8; j++){
				//getting data
				glm::u8vec4 curColor = data[i*8 + j];
				// if already in palette_table1
				if (std::find(std::begin(ppu.palette_table[palette_index]), std::end(ppu.palette_table[palette_index]), curColor)!= std::end(ppu.palette_table[palette_index])){
					size_t pos = std::find(std::begin(ppu.palette_table[palette_index]), std::end(ppu.palette_table[palette_index]), curColor) - std::begin(ppu.palette_table[palette_index]);
					if (pos == 0){
							continue;
					}else if(pos == 1){
							ppu.tile_table[tile_index].bit1[i] |= (1 << j);
					}else if(pos == 2){
							ppu.tile_table[tile_index].bit0[i] |= (1 << j);
					}else if(pos == 3){
							ppu.tile_table[tile_index].bit1[i] |= (1 << j);
							ppu.tile_table[tile_index].bit0[i] |= (1 << j);
					}
				// if not 
				}else{
					colorSeen++;
					if (colorSeen <= 4){
						ppu.palette_table[palette_index][colorSeen-1] = curColor;
						if (colorSeen == 1){
							continue;
						}else if(colorSeen == 2){
							ppu.tile_table[tile_index].bit1[i] |= (1 << j);
						}else if(colorSeen == 3){
							ppu.tile_table[tile_index].bit0[i] |= (1 << j);
						}else{
							ppu.tile_table[tile_index].bit1[i] |= (1 << j);
							ppu.tile_table[tile_index].bit0[i] |= (1 << j);
						}
					}
		
				}
			}
		}

	};

	load_from_png_data(1,1, player_data);

	load_from_png_data(2,2,poke1_data);

	load_from_png_data(3,3,zero_data);

	load_from_png_data(4,3,one_data);

	load_from_png_data(5,3,two_data);

	load_from_png_data(6,3,three_data);

	load_from_png_data(7,3,four_data);

	load_from_png_data(8,3,five_data);

	load_from_png_data(9,3,six_data);

	load_from_png_data(10,3,seven_data);

	load_from_png_data(11,3,eight_data);

	load_from_png_data(12,3,nine_data);

	load_from_png_data(13,4,ball_data);

	



	

	//first palette table, the first color is white
	//the rest doesnt matter for now

	ppu.palette_table[0] = {
		glm::u8vec4(0xff, 0xff, 0xff, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};

	//while backrgound tile

	uint16_t white_palette_index = 0;
	uint16_t white_tile_index = 0;



	PPU466::Tile curTile;
	for (uint32_t x = 0; x < 8; ++x) {
		curTile.bit0[x] = 0;
		curTile.bit1[x] = 0; 
	}
	ppu.tile_table[white_tile_index] = curTile;


	uint16_t white_background = (white_palette_index << 8) | (white_tile_index);

	//setting background to all white tile
	size_t width = ppu.BackgroundWidth;
	size_t height = ppu.BackgroundHeight;

	for(size_t i =0; i < width; i++){
		for (size_t j = 0; j < height; j++){
			ppu.background[i+width * j] = white_background;
		}
	}

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.downs += 1;
			space.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		}
	}

	return false;
}



void PlayMode::update(float elapsed) {


	//credit to https://stackoverflow.com/questions/5743678/generate-random-number-between-0-and-10
	std::random_device rdev;
	std::mt19937 gen(rdev());
	std::uniform_int_distribution<> distrib(0, 230);

	constexpr float PlayerSpeed = 60.0f;
	constexpr float BallSpeed = 100.0f;
	if (left.pressed) {
		player_at.x -= PlayerSpeed * elapsed;
		playerdir = 0;
	}
	if (right.pressed){
		player_at.x += PlayerSpeed * elapsed;
		playerdir = 1;
	}
	if (down.pressed){
		player_at.y -= PlayerSpeed * elapsed;
		playerdir = 2;
	}
	if (up.pressed){
		player_at.y += PlayerSpeed * elapsed;
		playerdir = 3;
	}if (space.pressed){
		ball_at.x = player_at.x;
		ball_at.y = player_at.y;
		balldir = playerdir;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	space.downs = 0;


	
	//implementaion of naive update ball
	if (ball_at.x < 257 && ball_at.x > -12 && ball_at.y > 0 && ball_at.y <241){
		if (balldir == 0){
			ball_at.x -= BallSpeed * elapsed;
		}else if(balldir == 1){
			ball_at.x += BallSpeed * elapsed;
		}else if(balldir == 2){
			ball_at.y -= BallSpeed * elapsed;
		}else if(balldir == 3){
			ball_at.y += BallSpeed * elapsed;
		}
		
	}else{
		ball_at = glm::vec2(260.0f, 250.0f);
	}

	//implementaion of naive collision

	if ((std::abs(ball_at.x - poke_at.x) <= 3) && (std::abs(ball_at.y - poke_at.y) <= 3)){
		poke_at.x = (float)distrib(gen);
		poke_at.y = (float)distrib(gen);
		score++;
		ball_at = glm::vec2(260.0f, 250.0f);
	}


}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---


	//background scroll:
	ppu.background_position.x = int32_t(-0.5f * player_at.x);
	ppu.background_position.y = int32_t(-0.5f * player_at.y);


	//player sprite:
	ppu.sprites[32].x = int8_t(player_at.x);
	ppu.sprites[32].y = int8_t(player_at.y);
	ppu.sprites[32].index = 1;
	ppu.sprites[32].attributes = 1;
	
	ppu.sprites[31].x = int8_t(ball_at.x);
	ppu.sprites[31].y = int8_t(ball_at.y);
	ppu.sprites[31].index = 13;
	ppu.sprites[31].attributes = 4;

	
	ppu.sprites[1].x = int8_t(poke_at.x);
	ppu.sprites[1].y = int8_t(poke_at.y);
	ppu.sprites[1].index = 2;
	ppu.sprites[1].attributes = 2;


	ppu.sprites[60].x = int8_t(score1_at.x);
	ppu.sprites[60].y = int8_t(score1_at.y);
	ppu.sprites[60].index = uint8_t((score/10) + 3);
	ppu.sprites[60].attributes = 3;

	ppu.sprites[61].x = int8_t(score2_at.x);
	ppu.sprites[61].y = int8_t(score2_at.y);
	ppu.sprites[61].index = int8_t((score%10) + 3);
	ppu.sprites[61].attributes = 3;

	

	//--- actually draw ---
	ppu.draw(drawable_size);
}
