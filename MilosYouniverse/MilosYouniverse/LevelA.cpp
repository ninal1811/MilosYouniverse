#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 10
#define OBJECT_COUNT 13

unsigned int LEVELA_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 77, 1, 1, 1, 1, 1, 1, 2, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 11, 12, 12, 12, 12, 12, 12, 13, 0,
    0, 22, 23, 23, 23, 23, 23, 23, 24, 0,
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
    m_berry_counter = 0;
    
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
        2.0f,                      // speed
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
        
    m_game_state.player->set_position(glm::vec3(4.5f, -1.5f, 0.0f));
    
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
    
    m_game_state.object[0].set_position(glm::vec3(7.0f, -7.0f, 0.0f));
    
    // ————— BUSH ————— //
    GLuint bush_texture_id = Utility::load_texture("assets/images/bush.png");
    GLuint berry_texture_id = Utility::load_texture("assets/images/berry.png");
    
    m_game_state.object[1] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[1].set_position(glm::vec3(6.6f, -1.3f, 0.0f));
    
    m_game_state.object[2] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[2].set_position(glm::vec3(7.28f, -1.5f, 0.0f));
    
    m_game_state.object[3] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[3].set_position(glm::vec3(6.74f, -1.75f, 0.0f));
    
    m_game_state.object[4] = Entity (
        berry_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BERRY
    );
    m_game_state.object[4].set_position(glm::vec3(6.67f, -1.87f, 0.0f));
    
    m_game_state.object[5] = Entity (
        berry_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BERRY
    );
    m_game_state.object[5].set_position(glm::vec3(7.40f, -1.57f, 0.0f));
    
    m_game_state.object[6] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[6].set_position(glm::vec3(3.0f, -4.45f, 0.0f));
    
    m_game_state.object[7] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[7].set_position(glm::vec3(3.65f, -4.8f, 0.0f));
    
    m_game_state.object[8] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[8].set_position(glm::vec3(2.45f, -4.8f, 0.0f));
    
    m_game_state.object[9] = Entity (
        bush_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BUSH
    );
    m_game_state.object[9].set_position(glm::vec3(3.0f, -5.2f, 0.0f));
    
    m_game_state.object[10] = Entity (
        berry_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BERRY
    );
    m_game_state.object[10].set_position(glm::vec3(3.70f, -4.85f, 0.0f));
    
    m_game_state.object[11] = Entity (
        berry_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BERRY
    );
    m_game_state.object[11].set_position(glm::vec3(2.30f, -4.85f, 0.0f));
    
    m_game_state.object[12] = Entity (
        berry_texture_id,         // texture id
        0.0f,                      // speed
        0.75f,                      // width
        0.75f,                       // height
        BERRY
    );
    m_game_state.object[12].set_position(glm::vec3(3.0f, -5.25f, 0.0f));

    // ————— BGM + SFX ————— //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/audio/morning.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
}

void LevelA::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.object, OBJECT_COUNT, m_game_state.map);
    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    float min_x = 1.5f;
    float max_x = 7.5f;
    float min_y = -7.5f;
    float max_y = -1.5f;
    
    glm::vec3 player_position = m_game_state.player->get_position();
    
    if (player_position.x < min_x) {
        player_position.x = min_x;
    } else if (player_position.x > max_x) {
        player_position.x = max_x;
    }
    
    if (player_position.y < min_y) {
        player_position.y = min_y;
    } else if (player_position.y > max_y) {
        player_position.y = max_y;
    }
    
    m_game_state.player->set_position(player_position);
    
    if (abs(m_game_state.player->get_position().x - m_game_state.object[0].get_position().x) < 0.75f &&
        abs(m_game_state.player->get_position().y - m_game_state.object[0].get_position().y) < 0.75f) {
        m_game_state.next_scene_id = 2;
    }
    
    if (m_game_state.player->get_position().y < -20.0f) m_game_state.next_scene_id = 2;
    
    m_berry_counter = m_game_state.player->m_berry_counter;
}

void LevelA::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].render(program);
    }

}
