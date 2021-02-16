#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "typedef.h"
#include "action.h"
#include "cache.h"

#define PLAYER_COUNT        1
#define PLAY_MAX_COUNT      7

typedef struct __player__
{
    uint8_t             action_start;
    uint8_t             action_end;
    uint16_t            play_time;
    bool                enable;
} player_t;

void play(void);

#define play_init()    hal_led_init(/*ic_type*/)

void player_set(uint8_t player_index, uint8_t action_start, uint8_t action_end, uint16_t play_time, bool enable);
void player_clear(void);

#endif
