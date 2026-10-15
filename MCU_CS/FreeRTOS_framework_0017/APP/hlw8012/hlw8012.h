#ifndef __HLW8012__H
#define __HLW8012__H

#include "framework.h"

typedef struct{
	void (*init)(void);
	void (*work)(void);


}hwl_opt_t;


extern hwl_opt_t* hwl_opt ;

#endif
