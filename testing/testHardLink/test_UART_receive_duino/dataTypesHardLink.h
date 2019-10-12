// This file must be kept in sync with the datatypesShared.h file in the Simblee code folder

typedef enum Mode_tag
{
  OFF = 0x00,
  SOLID = 0x01,
  FADE = 0x02,
  FLASH = 0x03,
  THEATER_CHASE = 0x04,
  TWO_COLOR_THEATER_CHASE = 0x05,
  TIME_RAINBOW = 0x06,
  SPACE_RAINBOW = 0x07,
  CHRISTMAS = 0x08,
  INDEPENDENCE_DAY = 0x09,
  SCANNER = 0x0A,
  CANDLE = 0x0B
} Mode_t;

typedef enum Action_tag 
{
  SET_MODE = 0x00,
  SET_BRIGHTNESS = 0x01,
  SET_SPEED = 0x02,
  SET_COLOR1 = 0x03,
  SET_COLOR2 = 0x04
} Action_t;

// Including the Mode byte
typedef enum ModeDataLength_tag 
{
  SET_MODE_LEN = 2,
  SET_BRIGHTNESS_LEN = 2,
  SET_SPEED_LEN = 2,
  SET_COLOR1_LEN = 4,
  SET_COLOR2_LEN = 4
} ModeDataLength_t;

#define MAX_MESSAGE_LENGTH 4 // Is there a better way?
#define SERIAL_BAUD 9600

