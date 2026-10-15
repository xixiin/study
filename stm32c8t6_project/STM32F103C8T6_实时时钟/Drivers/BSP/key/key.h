#ifndef __KEY_H__
#define __KEY_H__

enum key_num
{
	KEY_SET = 1,
	KEY_SHIFT,
	KEY_UP,
	KEY_DOWN

};

void key_init(void);
uint8_t key_scand(void);



#endif
