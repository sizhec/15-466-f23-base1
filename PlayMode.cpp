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
	load_png(data_path("ball.png"), &player_size, &player_data, LowerLeftOrigin);
	

	assert(player_size == glm::uvec2(8,8));

	size_t colorSeen = 0;

	ppu.tile_table[1].bit0 = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
	};

	ppu.tile_table[1].bit1 = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
	};
	
	
	


	// a naive loading that assume png are in 8*8 with less than 4 colours including transparent
	// Assume tile table are init with all 0 vector (turn out to be bad assumption please zero out first)
	for(size_t i =0; i < 8; i++){
		for (size_t j = 0; j < 8; j++){
			//getting data
			glm::u8vec4 curColor = player_data[i*8 + j];
			// if already in palette_table1
			if (std::find(std::begin(ppu.palette_table[1]), std::end(ppu.palette_table[1]), curColor)!= std::end(ppu.palette_table[1])){
				size_t pos = std::find(std::begin(ppu.palette_table[1]), std::end(ppu.palette_table[1]), curColor) - std::begin(ppu.palette_table[1]);
				if (pos == 0){
						continue;
				}else if(pos == 1){
						ppu.tile_table[1].bit1[i] |= (1 << j);
				}else if(pos == 2){
						ppu.tile_table[1].bit0[i] |= (1 << j);
				}else{
						ppu.tile_table[1].bit1[i] |= (1 << j);
						ppu.tile_table[1].bit0[i] |= (1 << j);
				}
			// if not 
			}else{
				if (colorSeen < 4){
					ppu.palette_table[1][colorSeen] = curColor;
					if (colorSeen == 0){
						continue;
					}else if(colorSeen == 1){
						ppu.tile_table[1].bit1[i] |= (1 << j);
					}else if(colorSeen == 2){
						ppu.tile_table[1].bit0[i] |= (1 << j);
					}else{
						ppu.tile_table[1].bit1[i] |= (1 << j);
						ppu.tile_table[1].bit0[i] |= (1 << j);
					}
				}
				colorSeen++;
			}
		}
	}

	

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
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// (will be used to set background color)
	//background_fade += elapsed / 10.0f;
	//background_fade -= std::floor(background_fade);

	constexpr float PlayerSpeed = 60.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//background color will be some hsv-like fade:
	// ppu.background_color = glm::u8vec4(
	// 	std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 0.0f / 3.0f) ) ) ))),
	// 	std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 1.0f / 3.0f) ) ) ))),
	// 	std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 2.0f / 3.0f) ) ) ))),
	// 	0xff	
	// );

	

	//tilemap gets recomputed every frame as some weird plasma thing:
	//NOTE: don't do this in your game! actually make a map or something :-)
	// for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
	// 	for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
	// 		//TODO: make weird plasma thing
	// 		ppu.background[x+PPU466::BackgroundWidth*y] = ((x+y)%16);
	// 	}
	// }

	//background scroll:
	ppu.background_position.x = int32_t(-0.5f * player_at.x);
	ppu.background_position.y = int32_t(-0.5f * player_at.y);

	//player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	ppu.sprites[0].index = 1;
	ppu.sprites[0].attributes = 1;

	ppu.sprites[1].x = int8_t(player_at.x);
	ppu.sprites[1].y = int8_t(player_at.y);
	ppu.sprites[1].index = 1;
	ppu.sprites[1].attributes = 1;

	

	//--- actually draw ---
	ppu.draw(drawable_size);
}
