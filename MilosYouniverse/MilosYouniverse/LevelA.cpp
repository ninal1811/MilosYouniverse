#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 10
#define OBJECT_COUNT 3

unsigned int LEVELA_DATA[] =
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

LevelA::~LevelA() {
    delete m_game_state.player;
    delete [] m_game_state.object;
    delete m_game_state.map;
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise() {
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/images/grass_tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 11, 7);
    
    // ————— MILO ————— //
    int player_walking_animation[4][4] =
    {
        { 8, 9, 10, 11 },  // left
        { 12, 13, 14, 15 }, // right
        { 4, 5, 6, 7 }, // up
        { 0, 1, 2, 3 }   // down
    };
    
    GLuint player_texture_id = Utility::load_texture("assets/images/milo.png");
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
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
    
    m_game_state.object = new Entity[OBJECT_COUNT];
    
    // ————— CHICKEN HOUSE ————— //
    GLuint chicken_house_texture_id = Utility::load_texture("assets/images/chicken_house.png");
    
    m_game_state.object[0] = Entity (
        chicken_house_texture_id,         // texture id
        0.0f,                      // speed
        0.5f,                      // width
        0.5f,                       // height
        CHICKENHOUSE
    );
    
    m_game_state.object[0].set_position(glm::vec3(3.0f, -7.0f, 0.0f));
    
    // ————— CHICKEN ————— //
    int chicken_walking_animation[2][4] =
    {
        { 0, 1, 0, 1 },  // first row (e.g., walking left)
        { 4, 5, 6, 7 }   // down
    };
    
    GLuint chicken_texture_id = Utility::load_texture("assets/images/chicken.png");
    m_game_state.object[1] = Entity(
        chicken_texture_id,         // texture id
        2.0f,                      // speed
        glm::vec3(0.0f, 0.0f, 0.0f),              // acceleration
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        2,                         // animation row amount
        0.125f,                      // width
        0.125f,                       // height
        CHICKEN
    );
    
    m_game_state.object[1].set_walk(chicken_walking_animation);
    m_game_state.object[1].set_position(glm::vec3(5.0f, -5.0f, 0.0f));
    m_game_state.object[1].set_ai_type(WALKER);
    m_game_state.object[1].set_ai_state(IDLE);
    
    m_game_state.object[2] = Entity(
        chicken_texture_id,         // texture id
        2.0f,                      // speed
        glm::vec3(0.0f, 0.0f, 0.0f),              // acceleration
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        2,                         // animation row amount
        0.125f,                      // width
        0.125f,                       // height
        CHICKEN
    );
    
    m_game_state.object[2].set_walk(chicken_walking_animation);
    m_game_state.object[2].set_position(glm::vec3(5.0f, -3.0f, 0.0f));
    m_game_state.object[2].set_ai_type(WALKER);
    m_game_state.object[2].set_ai_state(IDLE);

    // ————— BGM + SFX ————— //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/audio/morning.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
}

void LevelA::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.object, OBJECT_COUNT, m_game_state.map);
    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    if (abs(m_game_state.player->get_position().x - m_game_state.object[0].get_position().x) < 0.75f &&
        abs(m_game_state.player->get_position().y - m_game_state.object[0].get_position().y) < 0.75f) {
        m_game_state.next_scene_id = 3;
    }
    
    if (m_game_state.player->get_position().y < -20.0f) m_game_state.next_scene_id = 2;
}

void LevelA::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].render(program);
    }

}
