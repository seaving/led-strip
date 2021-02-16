#include "player.h"
#include "timer.h"
#include "cache.h"
#include "color_ctrl.h"
#include "position_ctrl.h"

static data player_t _players[PLAYER_COUNT] = {0};
static data uint8_t _player_index = 0;

void play(void)
{
    data uint8_t action_index = 0, i = 0, j = 0;

    //当前播放器播放
    for (i = 0; i < PLAY_MAX_COUNT; i ++)
    {
        //if (_players[_player_index].enable == true)
        {
            action_index = _players[_player_index].action_start + i;
            if (action_index <= _players[_player_index].action_end)
            {
                color_ctrl(&actions[action_index].hsv, &actions[action_index].color_ctrl, 
                    actions[action_index].fre_index, POSITION_CTRL_LEN(&actions[action_index].position_ctrl));
                position_ctrl(&actions[action_index].hsv, &actions[action_index].position_ctrl, 
                    &actions[action_index].color_ctrl, actions[action_index].fre_index);
            }
        }

        cache_refresh(actions[action_index].position_ctrl.cache_index, j);
    }

#if 0
    //时间到了就切换到下一个播放器
    if (_players[_player_index].enable == true 
        && _players[_player_index].play_time > 0 
        && IS_TIME_TO(_players[_player_index].play_time))
    {
        //切换到下一播放器，先清空显示
        cache_clear(0);
        _player_index ++;
        if (_player_index >= PLAYER_COUNT)
        {
            _player_index = 0;
        }
    }
#endif

    timer_cnt ++;
}

void player_set(uint8_t player_index, uint8_t action_start, uint8_t action_end, uint16_t play_time, bool enable)
{
    _players[player_index].action_start = action_start;
    _players[player_index].action_end = action_end;
    _players[player_index].play_time = play_time;
    _players[player_index].enable = enable;
}

void player_clear(void)
{
    memset(_players, 0, sizeof(player_t) * PLAYER_COUNT);
    memset(actions, 0, sizeof(action_t) * ACTION_COUNT);
    cache_clear(0);
    cache_clear(1);
}
