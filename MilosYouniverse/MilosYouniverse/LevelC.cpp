#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 10
#define OBJECT_COUNT 4

unsigned int LEVELC_DATA[] =
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

LevelC::~LevelC() {
    delete [] m_game_state.object;
    delete m_game_state.player;
    delete m_game_state.map;
    Mix_FreeMusic(m_game_state.bgm);
    Mix_FreeChunk(m_game_state.win_sfx);
}

void LevelC::initialise() {
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/images/grass_tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 11, 7);
    
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
        
    m_game_state.player->set_position(glm::vec3(1.0f, -1.0f, 0.0f));
    
    m_game_state.object = new Entity[OBJECT_COUNT];
    
    // ————— CHEST ————— //
    GLint chest_texture_id = Utility::load_texture("assets/images/chest.png");
    m_game_state.object[0] = Entity(
       chest_texture_id,         // texture id
       0.0f,                      // speed
       0.25f,                      // width
       0.25f,                       // height
       FINISH
    );
    
    m_game_state.object[0].set_position(glm::vec3(7.25f, -7.25f, 0.0f));
    
    // ————— SLIMES ————— //
    GLint pslime_texture_id = Utility::load_texture("assets/images/pslime.png");
    GLint gslime_texture_id = Utility::load_texture("assets/images/gslime.png");
    GLint bslime_texture_id = Utility::load_texture("assets/images/bslime.png");
    
    int slime_walking_animation[2][7] =
    {
        { 0, 1, 2, 3, 4, 5, 6 },
        { 7, 8, 9, 10, 11, 12, 13 }
    };
    
    m_game_state.object[1] = Entity(
        pslime_texture_id,         // texture id
        1.0f,                     // speed
        glm::vec3(0.0f, 0.0f, 0.0f),              // acceleration
        player_walking_animation,  // animation index sets
        0.0f,                     // animation time
        7,                        // animation frame amount
        0,                        // current animation index
        7,                        // animation column amount
        2,                        // animation row amount
        0.5f,                     // width
        0.5f,                     // height
        ENEMY                    // type
    );

    m_game_state.object[1].set_walk1(slime_walking_animation);
    m_game_state.object[1].set_position(glm::vec3(7.0f, -4.5f, 0.0f));
    m_game_state.object[1].set_entity_type(ENEMY);
    m_game_state.object[1].set_ai_type(GUARD);
    m_game_state.object[1].set_ai_state(IDLE);
    
    m_game_state.object[2] = Entity(
        gslime_texture_id,         // texture id
        1.25f,                     // speed
        glm::vec3(0.0f, 0.0f, 0.0f),              // acceleration
        player_walking_animation,  // animation index sets
        0.0f,                     // animation time
        7,                        // animation frame amount
        0,                        // current animation index
        7,                        // animation column amount
        2,                        // animation row amount
        0.5f,                     // width
        0.5f,                     // height
        ENEMY                    // type
    );

    m_game_state.object[2].set_walk1(slime_walking_animation);
    m_game_state.object[2].set_position(glm::vec3(4.0f, -7.0f, 0.0f));
    m_game_state.object[2].set_entity_type(ENEMY);
    m_game_state.object[2].set_ai_type(GUARD);
    m_game_state.object[2].set_ai_state(IDLE);
    
    m_game_state.object[3] = Entity(
        bslime_texture_id,         // texture id
        1.5f,                     // speed
        glm::vec3(0.0f, 0.0f, 0.0f),              // acceleration
        player_walking_animation,  // animation index sets
        0.0f,                     // animation time
        7,                        // animation frame amount
        0,                        // current animation index
        7,                        // animation column amount
        2,                        // animation row amount
        0.5f,                     // width
        0.5f,                     // height
        ENEMY                    // type
    );

    m_game_state.object[3].set_walk1(slime_walking_animation);
    m_game_state.object[3].set_position(glm::vec3(2.0f, -2.5f, 0.0f));
    m_game_state.object[3].set_entity_type(ENEMY);
    m_game_state.object[3].set_ai_type(GUARD);
    m_game_state.object[3].set_ai_state(IDLE);

    
    // ————— BGM + SFX ————— //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/audio/morning.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
    
    m_game_state.win_sfx = Mix_LoadWAV("assets/win.wav");
}

void LevelC::update(float delta_time) {
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
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE]) {
        m_space_pressed = true;
    } else {
        m_space_pressed = false;
    }
    
    if (abs(m_game_state.player->get_position().x - m_game_state.object[0].get_position().x) < 0.40f &&
        abs(m_game_state.player->get_position().y - m_game_state.object[0].get_position().y) < 0.40f) {
        m_game_over = true;
        m_game_state.player->set_speed(0.0f);
    }
}

void LevelC::render(ShaderProgram *program) {
    GLuint text_texture_id = Utility::load_texture("assets/font/font1.png");
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].render(program);
    }
    
    std::cout << "berry count: " << m_berry_counter << std::endl;
    std::cout << "egg count: " << m_egg_counter << std::endl;
    std::cout << "slime count: " << m_game_state.player->m_slime_counter << std::endl;
    
    if (m_game_over) {
        if (m_berry_counter == 5 && m_egg_counter == 5 && m_game_state.player->m_slime_counter == 3) {
            Utility::draw_text(program, text_texture_id, "YOU WIN!", 0.5f, -0.1f, 
                               glm::vec3(m_game_state.player->get_position().x - 1.25f, 
                                         m_game_state.player->get_position().y - 1.5f, 0.0f));
            Mix_PlayChannel(-1, m_game_state.win_sfx, 0);
        } else {
            Utility::draw_text(program, text_texture_id, "YOU LOSE!", 0.5f, -0.1f, 
                               glm::vec3(m_game_state.player->get_position().x - 1.25f, 
                                         m_game_state.player->get_position().y - 1.5f, 0.0f));
        }
    }
}
