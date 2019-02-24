/**** includes ****/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

/**** defines ****/
#define CTRL_KEY(k) ((k) & 0x1f)


/**** data ****/
struct termios orig_termios; //the original one which later can be used 



/**** function definitions ****/

/**** terminal ****/
void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios)==-1){
    die("tcsetattr");
  }
}

void enableRawMode() {

  if(tcgetattr(STDIN_FILENO, &orig_termios)==-1)die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = orig_termios; //to set the raw attributes
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //fixes ctrl+m , disable ctrl+s , ctrl+q 
  raw.c_oflag &= ~(OPOST); //turns off o/p processing now we need to use /r while printing
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN); // turns off echo , cononical mode(now it reads byte by byte instead of line) , ctrl+c , ctrl+z
                                                   //disables  ctrl+v

  raw.c_cc[VMIN] = 0; //time out
  raw.c_cc[VTIME] = 10;

  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1)die("tcsetattr");
  //setting the attributes previuosly defined
}

/**** input processing ****/
char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

void editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      exit(0);
      break;
  }
}


/**** output processing ****/
void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  //clears screen
}


/**** function def end ****/

/**** main function ****/
int main() {

  enableRawMode();

   while (1) {
    editorRefreshScreen();
    editorProcessKeypress();

  }
  disableRawMode();
  return 0;
}