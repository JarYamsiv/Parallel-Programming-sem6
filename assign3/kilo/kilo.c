/**** includes ****/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

/**** defines ****/
#define CTRL_KEY(k) ((k) & 0x1f)


/**** data ****/
struct editorConfig {
  int screenrows;
  int screencols;
  struct termios orig_termios;
};
struct editorConfig E;



/**** function definitions ****/

/**** terminal ****/
void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}

void disableRawMode() {
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios)==-1){
    die("tcsetattr");
  }
}

void enableRawMode() {

  if(tcgetattr(STDIN_FILENO, &E.orig_termios)==-1)die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.orig_termios; //to set the raw attributes
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //fixes ctrl+m , disable ctrl+s , ctrl+q 
  raw.c_oflag &= ~(OPOST); //turns off o/p processing now we need to use /r while printing
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN); // turns off echo , cononical mode(now it reads byte by byte instead of line) , ctrl+c , ctrl+z
                                                   //disables  ctrl+v

  raw.c_cc[VMIN] = 0; //time out
  raw.c_cc[VTIME] = 10;

  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1)die("tcsetattr");
  //setting the attributes previuosly defined
}

char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}


/**** input processing ****/


void editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}


/**** output processing ****/
void editorDrawRows() { 
  //drwas tild on the begining of all lines
  int y;
  for (y = 0; y < E.screenrows; y++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}


void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  editorDrawRows();

  write(STDOUT_FILENO, "\x1b[H", 3);
  //clears screen
}

/**** init ****/
void initEditor() {
  //get window size
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}


/**** function def end ****/

/**** main function ****/
int main() {

  enableRawMode();
  initEditor();

   while (1) {
    editorRefreshScreen();
    editorProcessKeypress();

  }
  disableRawMode();
  return 0;
}