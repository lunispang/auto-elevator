#include <stdio.h>
#include <time.h>
#include <winbase.h>
#include <windows.h>
#include <winuser.h>

char anyKeyDown() {
  for (int i = 1; i < 256; i++) {
    if (GetAsyncKeyState(i) < 0)
      return i;
  }
  return 0;
}

void click() {
  INPUT input = {0};
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  SendInput(1, &input, sizeof(input));
  input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(1, &input, sizeof(input));
}

void reload() {
  INPUT reload_input[2] = {0};
  reload_input[0].type = INPUT_KEYBOARD;
  reload_input[1].type = INPUT_KEYBOARD;

  reload_input[0].ki.wVk = VK_CONTROL;
  reload_input[1].ki.wVk = 'R';

  SendInput(2, reload_input, sizeof(*reload_input));

  reload_input[1].ki.dwFlags = KEYEVENTF_KEYUP;
  reload_input[0].ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(2, reload_input, sizeof(*reload_input));

  Sleep(1000);

  click();
}

int main(int argc, char **argv) {
  int wait_time = 120;
  if (argc == 2) {
    int arg = atoi(argv[1]);
    if (arg) {
      wait_time = arg;
    } else {
      printf("Invalid wait time argument, defaulting to 120\n");
    }
  } else {
    printf("Invalid wait time argument, defaulting to 120\n");
  }

  Sleep(4000);

  click();

  long latestpress = time(0);
  char reloaded = 0;
  while (1) {
    char key = anyKeyDown();
    long now = time(0);
    if (key) {
      latestpress = now;
      reloaded = 0;
    }
    if ((latestpress + wait_time < now) & !reloaded) {
      reload();
      latestpress = now;
      reloaded = 1;
      printf("Reloaded!\n");
    }
  }
}
