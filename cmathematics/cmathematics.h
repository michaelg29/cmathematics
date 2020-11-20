typedef unsigned char bool;
#define false 0
#define true !false

/**
 * calculates the number of arguments in a __VA_ARGS__ list
 * @param type the type of the argument in the list
 * @param ... the list
 * @return the number of arguments in the list
 */
#define NUMARGS(type, ...) (sizeof((type[]){0.0f, ##__VA_ARGS__}) / sizeof(type) - 1)