#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_ADDRESS 0x3C   // try 0x3D if needed

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// states for demo 
int demo_mode = 1;
static const int max_animation_index = 8;
int current_animation_index = 0;

// reference state
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;

// current state of the eyes
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;

int right_eye_x = 32 + ref_eye_width + ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;

void draw_eyes(bool update = true)
{
  display.clearDisplay();

  int x = int(left_eye_x - left_eye_width / 2);
  int y = int(left_eye_y - left_eye_height / 2);
  display.fillRoundRect(x, y, left_eye_width, left_eye_height,
                        ref_corner_radius, SH110X_WHITE);

  x = int(right_eye_x - right_eye_width / 2);
  y = int(right_eye_y - right_eye_height / 2);
  display.fillRoundRect(x, y, right_eye_width, right_eye_height,
                        ref_corner_radius, SH110X_WHITE);

  if (update) display.display();
}

void center_eyes(bool update = true)
{
  left_eye_height = ref_eye_height;
  left_eye_width  = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width  = ref_eye_width;

  left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
  left_eye_y = SCREEN_HEIGHT / 2;
  right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
  right_eye_y = SCREEN_HEIGHT / 2;

  draw_eyes(update);
}

void blink(int speed = 12)
{
  draw_eyes();
  for (int i = 0; i < 3; i++) {
    left_eye_height -= speed;
    right_eye_height -= speed;
    draw_eyes();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_height += speed;
    right_eye_height += speed;
    draw_eyes();
    delay(1);
  }
}

void sleep()
{
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);
}

void wakeup()
{
  sleep();
  for (int h = 0; h <= ref_eye_height; h += 2) {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
  }
}

void happy_eye()
{
  center_eyes(false);
  int offset = ref_eye_height / 2;

  for (int i = 0; i < 10; i++) {
    display.fillTriangle(
      left_eye_x - left_eye_width / 2 - 1, left_eye_y + offset,
      left_eye_x + left_eye_width / 2 + 1, left_eye_y + 5 + offset,
      left_eye_x - left_eye_width / 2 - 1, left_eye_y + left_eye_height + offset,
      SH110X_BLACK);

    display.fillTriangle(
      right_eye_x + right_eye_width / 2 + 1, right_eye_y + offset,
      right_eye_x - right_eye_width / 2 - 1, right_eye_y + 5 + offset,
      right_eye_x + right_eye_width / 2 + 1, right_eye_y + right_eye_height + offset,
      SH110X_BLACK);

    offset -= 2;
    display.display();
    delay(1);
  }
  delay(1000);
}

void saccade(int direction_x, int direction_y)
{
  int dx = 8;
  int dy = 6;
  int blink_amp = 8;

  left_eye_x += dx * direction_x;
  right_eye_x += dx * direction_x;
  left_eye_y += dy * direction_y;
  right_eye_y += dy * direction_y;
  left_eye_height -= blink_amp;
  right_eye_height -= blink_amp;
  draw_eyes();
  delay(1);

  left_eye_height += blink_amp;
  right_eye_height += blink_amp;
  draw_eyes();
  delay(1);
}

void move_big_eye(int direction)
{
  int move_amp = 2;
  int blink_amp = 5;

  for (int i = 0; i < 3; i++) {
    left_eye_x += move_amp * direction;
    right_eye_x += move_amp * direction;
    left_eye_height -= blink_amp;
    right_eye_height -= blink_amp;
    draw_eyes();
    delay(1);
  }

  for (int i = 0; i < 3; i++) {
    left_eye_x -= move_amp * direction;
    right_eye_x -= move_amp * direction;
    left_eye_height += blink_amp;
    right_eye_height += blink_amp;
    draw_eyes();
    delay(1);
  }

  center_eyes();
}

void setup()
{
  Serial.begin(115200);

  if (!display.begin(I2C_ADDRESS, true)) {
    Serial.println("SH1106 not found");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Robot Eyes");
  display.display();
  delay(2000);

  sleep();
  delay(2000);
}

void launch_animation_with_index(int index)
{
  if (index > max_animation_index) index = 0;

  switch (index) {
    case 0: wakeup(); break;
    case 1: center_eyes(true); break;
    case 2: move_big_eye(1); break;
    case 3: move_big_eye(-1); break;
    case 4: blink(10); break;
    case 5: blink(20); break;
    case 6: happy_eye(); break;
    case 7: sleep(); break;
    case 8:
      center_eyes(true);
      for (int i = 0; i < 20; i++) {
        int dx = random(-1, 2);
        int dy = random(-1, 2);
        saccade(dx, dy);
        delay(1);
        saccade(-dx, -dy);
        delay(1);
      }
      break;
  }
}

void loop()
{
  if (demo_mode == 1) {
    launch_animation_with_index(current_animation_index++);
    if (current_animation_index > max_animation_index)
      current_animation_index = 0;
  }
}
