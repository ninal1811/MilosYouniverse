#include "Menu.h"
#include "Utility.h"

#define MENU_WIDTH 10
#define MENU_HEIGHT 10

unsigned int MENU_DATA[] = 
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 77, 1, 1, 1, 2, 0, 0,
    0, 0, 77, 28, 12, 12, 12, 27, 2, 0,
    0, 77, 28, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 16, 24, 0,
    0, 22, 17, 12, 12, 16, 23, 24, 0, 0,
    0, 0, 22, 23, 23, 24, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

Menu::~Menu() {
    delete m_game_state.player;
    delete m_game_state.map;
    Mix_FreeMusic(m_game_state.bgm);
}

void Menu::initialise() {
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/images/grass_tileset.png");
    m_game_state.map = new Map(MENU_WIDTH, MENU_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 11, 7);
    
    // ————— MILO ————— //
    int player_walking_animation[4][4] =
    {
        { 0, 1, 2, 3 },  // left
        { 0, 1, 2, 3 }, // right
        { 0, 1, 2, 3 }, // up
        { 0, 1, 2, 3 }   // down
    };
    
    GLuint player_texture_id = Utility::load_texture("assets/images/milo.png");
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        0.0f,                      // speed
        glm::vec3(0.0f, 0.0f, 0.0f),              // acceleration
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        0.5f,                      // width
        0.5f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(3.75f, -1.5f, 0.0f));
    
    // ————— BGM + SFX ————— //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/audio/morning.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
}

void Menu::update(float delta_time) {
    if (delta_time == -1) {
        m_game_state.next_scene_id = 1;
    }
    
    m_game_state.player->update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
}

void Menu::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    GLuint text_texture_id = Utility::load_texture("assets/font/font1.png");
    Utility::draw_text(program, text_texture_id, "Milo's Youniverse", 0.5f, -0.1f, glm::vec3(0.6f, 1.0f, 0.0f));
    Utility::draw_text(program, text_texture_id, "PRESS ENTER TO START", 0.5f, -0.1f, glm::vec3(0.1f, 0.0f, 0.0f));
}
