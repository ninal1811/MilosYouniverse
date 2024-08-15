#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 10

#define OBJECT_COUNT 159

unsigned int LEVELB_DATA[] =
{
    77, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    11, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    22, 23, 23, 23, 23, 23, 23, 23, 23, 24
};

LevelB::~LevelB() {
    delete [] m_game_state.object;
    delete m_game_state.player;
    delete m_game_state.map;
    Mix_FreeChunk(m_game_state.collect_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise() {
    m_game_state.next_scene_id = -1;
    m_egg_counter = 0;
    
    GLuint map_texture_id = Utility::load_texture("assets/images/grass_tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 11, 7);
    
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
    
    // ————— FENCE ————— //
    GLint fence_texture_id = Utility::load_texture("assets/images/fence.png");
    
    glm::vec3 fence_positions[153] = {
//        top across
        glm::vec3(1.5f, -0.5f, 0.0f), glm::vec3(2.0f, -0.5f, 0.0f), glm::vec3(2.5f, -0.5f, 0.0f), glm::vec3(3.0f, -0.5f, 0.0f), glm::vec3(3.5f, -0.5f, 0.0f), glm::vec3(4.0f, -0.5f, 0.0f), glm::vec3(4.5f, -0.5f, 0.0f), glm::vec3(5.0f, -0.5f, 0.0f), glm::vec3(5.5f, -0.5f, 0.0f), glm::vec3(6.0f, -0.5f, 0.0f), glm::vec3(6.5f, -0.5f, 0.0f), glm::vec3(7.0f, -0.5f, 0.0f), glm::vec3(7.5f, -0.5f, 0.0f), glm::vec3(8.0f, -0.5f, 0.0f), glm::vec3(8.5f, -0.5f, 0.0f),
//        left down
        glm::vec3(0.5f, -1.0f, 0.0f), glm::vec3(0.5f, -1.5f, 0.0f), glm::vec3(0.5f, -2.0f, 0.0f), glm::vec3(0.5f, -2.5f, 0.0f), glm::vec3(0.5f, -3.0f, 0.0f), glm::vec3(0.5f, -3.5f, 0.0f), glm::vec3(0.5f, -4.0f, 0.0f), glm::vec3(0.5f, -4.5f, 0.0f), glm::vec3(0.5f, -5.0f, 0.0f), glm::vec3(0.5f, -5.5f, 0.0f), glm::vec3(0.5f, -6.0f, 0.0f), glm::vec3(0.5f, -6.5f, 0.0f), glm::vec3(0.5f, -7.0f, 0.0f), glm::vec3(0.5f, -7.5f, 0.0f), glm::vec3(0.5f, -8.0f, 0.0f), glm::vec3(0.5f, -8.5f, 0.0f),
//        right down
        glm::vec3(8.5f, -1.0f, 0.0f), glm::vec3(8.5f, -1.5f, 0.0f), glm::vec3(8.5f, -2.0f, 0.0f), glm::vec3(8.5f, -2.5f, 0.0f), glm::vec3(8.5f, -3.0f, 0.0f), glm::vec3(8.5f, -3.5f, 0.0f), glm::vec3(8.5f, -4.0f, 0.0f), glm::vec3(8.5f, -4.5f, 0.0f), glm::vec3(8.5f, -5.0f, 0.0f), glm::vec3(8.5f, -5.5f, 0.0f), glm::vec3(8.5f, -6.0f, 0.0f), glm::vec3(8.5f, -6.5f, 0.0f), glm::vec3(8.5f, -7.0f, 0.0f), glm::vec3(8.5f, -7.5f, 0.0f), glm::vec3(8.5f, -8.0f, 0.0f),
//        bottom across
        glm::vec3(1.0f, -8.5f, 0.0f), glm::vec3(1.5f, -8.5f, 0.0f), glm::vec3(2.0f, -8.5f, 0.0f), glm::vec3(2.5f, -8.5f, 0.0f), glm::vec3(3.0f, -8.5f, 0.0f), glm::vec3(3.5f, -8.5f, 0.0f), glm::vec3(4.0f, -8.5f, 0.0f), glm::vec3(4.5f, -8.5f, 0.0f), glm::vec3(5.0f, -8.5f, 0.0f), glm::vec3(5.5f, -8.5f, 0.0f), glm::vec3(6.0f, -8.5f, 0.0f), glm::vec3(6.5f, -8.5f, 0.0f), glm::vec3(7.0f, -8.5f, 0.0f), glm::vec3(7.5f, -8.5f, 0.0f),
//        middle
        glm::vec3(1.5f, -1.0f, 0.0f), glm::vec3(1.5f, -1.5f, 0.0f), glm::vec3(2.0f, -1.5f, 0.0f), glm::vec3(2.5f, -1.5f, 0.0f), glm::vec3(3.0f, -1.5f, 0.0f), glm::vec3(3.5f, -1.5f, 0.0f), glm::vec3(4.0f, -1.5f, 0.0f), glm::vec3(4.5f, -1.5f, 0.0f), glm::vec3(5.5f, -1.0f, 0.0f), glm::vec3(5.5f, -1.5f, 0.0f), glm::vec3(5.5f, -2.0f, 0.0f), glm::vec3(5.5f, -2.5f, 0.0f), glm::vec3(6.5f, -1.0f, 0.0f), glm::vec3(6.5f, -1.5f, 0.0f), glm::vec3(6.5f, -2.0f, 0.0f), glm::vec3(6.5f, -2.5f, 0.0f), glm::vec3(6.5f, -3.0f, 0.0f), glm::vec3(6.5f, -3.5f, 0.0f), glm::vec3(7.0f, -3.5f, 0.0f), glm::vec3(7.5f, -3.5f, 0.0f), glm::vec3(7.5f, -3.0f, 0.0f), glm::vec3(7.5f, -2.5f, 0.0f), glm::vec3(7.5f, -2.0f, 0.0f), glm::vec3(7.5f, -1.5f, 0.0f),
        
        glm::vec3(4.0f, -2.5f, 0.0f), glm::vec3(4.5f, -2.5f, 0.0f), glm::vec3(5.0f, -2.5f, 0.0f), glm::vec3(1.0f, -2.5f, 0.0f), glm::vec3(1.5f, -2.5f, 0.0f), glm::vec3(2.0f, -2.5f, 0.0f), glm::vec3(2.5f, -2.5f, 0.0f), glm::vec3(2.5f, -3.0f, 0.0f), glm::vec3(2.5f, -3.5f, 0.0f), glm::vec3(2.5f, -4.0f, 0.0f), glm::vec3(2.5f, -4.5f, 0.0f), glm::vec3(1.5f, -3.5f, 0.0f), glm::vec3(1.5f, -4.0f, 0.0f), glm::vec3(1.5f, -4.5f, 0.0f), glm::vec3(1.5f, -5.0f, 0.0f), glm::vec3(1.5f, -5.5f, 0.0f), glm::vec3(1.5f, -6.0f, 0.0f), glm::vec3(1.5f, -6.5f, 0.0f), glm::vec3(1.5f, -7.0f, 0.0f), glm::vec3(1.5f, -7.5f, 0.0f), glm::vec3(2.0f, -7.5f, 0.0f), glm::vec3(2.5f, -7.5f, 0.0f), glm::vec3(3.0f, -7.5f, 0.0f), glm::vec3(3.0f, -8.0f, 0.0f), glm::vec3(7.5f, -7.5f, 0.0f), glm::vec3(7.5f, -8.0f, 0.0f), glm::vec3(6.0f, -7.5f, 0.0f), glm::vec3(6.5f, -7.5f, 0.0f), glm::vec3(7.0f, -7.5f, 0.0f), glm::vec3(6.0f, -4.5f, 0.0f), glm::vec3(6.5f, -4.5f, 0.0f), glm::vec3(7.0f, -4.5f, 0.0f), glm::vec3(7.5f, -4.5f, 0.0f), glm::vec3(8.0f, -4.5f, 0.0f),
        
        glm::vec3(3.0f, -6.5f, 0.0f), glm::vec3(3.5f, -6.5f, 0.0f), glm::vec3(4.0f, -6.5f, 0.0f), glm::vec3(4.0f, -7.0f, 0.0f),  glm::vec3(4.25f, -7.5f, 0.0f), glm::vec3(4.5f, -7.0f, 0.0f), glm::vec3(4.5f, -6.5f, 0.0f), glm::vec3(4.5f, -5.5f, 0.0f), glm::vec3(4.5f, -6.0f, 0.0f), glm::vec3(5.0f, -5.5f, 0.0f), glm::vec3(5.5f, -5.5f, 0.0f), glm::vec3(5.5f, -6.0f, 0.0f), glm::vec3(5.5f, -6.5f, 0.0f), glm::vec3(5.5f, -7.0f, 0.0f), glm::vec3(5.5f, -7.5f, 0.0f), glm::vec3(3.5f, -3.0f, 0.0f), glm::vec3(3.5f, -3.5f, 0.0f), glm::vec3(4.0f, -3.5f, 0.0f), glm::vec3(4.5f, -3.5f, 0.0f), glm::vec3(5.0f, -3.5f, 0.0f), glm::vec3(5.5f, -4.5f, 0.0f), glm::vec3(6.5f, -6.5f, 0.0f), glm::vec3(7.0f, -6.5f, 0.0f), glm::vec3(7.5f, -6.5f, 0.0f), glm::vec3(8.0f, -6.5f, 0.0f), glm::vec3(5.0f, -4.5f, 0.0f), glm::vec3(2.5f, -6.5f, 0.0f), glm::vec3(4.0f, -4.5f, 0.0f), glm::vec3(4.5f, -4.5f, 0.0f), glm::vec3(3.5f, -5.0f, 0.0f), glm::vec3(3.5f, -5.5f, 0.0f), glm::vec3(3.0f, -5.5f, 0.0f), glm::vec3(6.5f, -6.0f, 0.0f), glm::vec3(7.0f, -5.5f, 0.0f), glm::vec3(7.5f, -5.5f, 0.0f),
    };

    for (int i = 0; i < 153; i++) {
        m_game_state.object[i] = Entity(
            fence_texture_id,         // texture id
            0.0f,                      // speed
            0.5f,                      // width
            0.5f,                       // height
            FENCE
        );
        m_game_state.object[i].set_position(fence_positions[i]);
    }
    
    // ————— EGGS ————— //
    GLint egg_texture_id = Utility::load_texture("assets/images/egg.png");
    
    m_game_state.object[153] = Entity(
       egg_texture_id,         // texture id
       0.0f,                      // speed
       0.125f,                      // width
       0.125f,                       // height
       EGG
    );
    
    m_game_state.object[153].set_position(glm::vec3(2.5f, -7.75f, 0.0f));
    
    m_game_state.object[154] = Entity(
       egg_texture_id,         // texture id
       0.0f,                      // speed
       0.125f,                      // width
       0.125f,                       // height
       EGG
    );
    
    m_game_state.object[154].set_position(glm::vec3(3.25f, -4.0f, 0.0f));
    
    m_game_state.object[155] = Entity(
       egg_texture_id,         // texture id
       0.0f,                      // speed
       0.125f,                      // width
       0.125f,                       // height
       EGG
    );
    
    m_game_state.object[155].set_position(glm::vec3(7.0f, -2.75f, 0.0f));
    
    m_game_state.object[156] = Entity(
       egg_texture_id,         // texture id
       0.0f,                      // speed
       0.125f,                      // width
       0.125f,                       // height
       EGG
    );
    
    m_game_state.object[156].set_position(glm::vec3(5.0f, -5.75f, 0.0f));
    
    m_game_state.object[157] = Entity(
       egg_texture_id,         // texture id
       0.0f,                      // speed
       0.125f,                      // width
       0.125f,                       // height
       EGG
    );
    
    m_game_state.object[157].set_position(glm::vec3(3.5f, -0.75f, 0.0f));
    
    // ————— CHEST ————— //
    GLint chest_texture_id = Utility::load_texture("assets/images/chest.png");
    m_game_state.object[158] = Entity(
       chest_texture_id,         // texture id
       0.0f,                      // speed
       0.125f,                      // width
       0.125f,                       // height
       CHEST
    );
    
    m_game_state.object[158].set_position(glm::vec3(8.0f, -8.5f, 0.0f));

    // ————— BGM + SFX ————— //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/audio/morning.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
    
    m_game_state.collect_sfx = Mix_LoadWAV("assets/audio/collect.wav");
}

void LevelB::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.object, OBJECT_COUNT, m_game_state.map);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
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
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (m_game_state.object[i].get_entity_type() == EGG) {
            if (m_game_state.object[i].collect) {
                m_game_state.object[i].collect = false;
                Mix_PlayChannel(-1, m_game_state.collect_sfx, 0);
                Mix_Volume(-1, 50);
            }
        }
    }

    if (abs(m_game_state.player->get_position().x - m_game_state.object[158].get_position().x) < 0.75f &&
        abs(m_game_state.player->get_position().y - m_game_state.object[158].get_position().y) < 0.75f) {
        m_game_state.next_scene_id = 3;
    }
    
    if (m_game_state.player->get_position().y < -20.0f) m_game_state.next_scene_id = 3;
    
    m_egg_counter = m_game_state.player->m_egg_counter;
}

void LevelB::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    m_game_state.player->render(program);

    for (int i = 0; i < OBJECT_COUNT; i++) {
        m_game_state.object[i].render(program);
    }
}
