#ifndef _CONSTANTS_INCLUDE
#define _CONSTANTS_INCLUDE

#define JUMP_HEIGHT 175.f
#define JUMP_TIME .62f
#define N_FALL_GRAVITY 3.f
#define FALLING_TERMINAL_VEL 500.f
#define GRAVITY_ACC ((-2*JUMP_HEIGHT)/(JUMP_TIME*JUMP_TIME))
#define JUMP_VEL sqrtf(-2.f * GRAVITY_ACC * JUMP_HEIGHT)

#endif // _CONSTANTS_INCLUDE
