
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE
#include <termios.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
//setting global variables
#define TABSPACES 8
#define NO_OF_QUIT_PRESSED 3
//funtion to process Control key input 
#define CTRL_KEY(k) ((k) & 0x1f)
//assigning high value to these keys to not mess up the switch case
//arrowkey handling code 
enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000, ARROW_RIGHT,  ARROW_UP,  ARROW_DOWN,
  DEL_KEY,  HOME_KEY,  END_KEY,
  PAGE_UP,  PAGE_DOWN
};
//data structures for global variables
//editor row save data of each line along with its length
typedef struct editorRow {
  int size;
  int rsize;
  char *chars;
  char *render;
} editorRow;
//global variables required to set up editor

/*Our next goal is to get the size of the terminal, so we know how many rows to draw in editorDrawRows(). But first, let’s set up a global struct that will contain our editor state, which we’ll use to store the width and height of the terminal. For now, let’s just put our orig_termios global into the "struct" */
struct textEditorConfiguration {
  int cx, cy;//cursor positions
  int rx;
  int rowoff;	//row offset to  enable vertical scrolling,what row of the file,user is currently scrolled to
  int coloff;
  int screenrows;//no of rows displayable at once
  int screencols;//no of columns displayable at once
  int numrows; //no of rows in files
  editorRow *row;
  int dirty; //dirty flag
  char *filename;
  char statusmsg[80]; //status bar message 
  time_t statusmsg_time; //time display in editor
  //inbuilt struct defination in <terminos.h>
  struct termios orig_termios;
};
struct textEditorConfiguration EDIT_global;

//function definations for later use
void editorSetStatusMessage(const char *fmt, ...);
void editorRefreshScreen();	//It’s possible that the cursor might be displayed in the middle of the screen somewhere for a split second while the terminal is drawing to the screen. To make sure that doesn’t happen, let’s hide the cursor before refreshing the screen, and show it again immediately after the refresh finishes.
char *editorPrompt(char *prompt);
//function to end program incase of an error
void end_n_error(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}
//sets the terminal in mode to stop echo and other commards like control keys to work 
//whenever the text editor is running
#include "terminal.h"
//read the key from keyboard
int readNewKey() {
  int nread;
  char c;
  //read key from keyboard
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) end_n_error("Error: ReadKey not working");
  }
  if (c == '\x1b')//for special characters keys to be used
  {
    char seq[3];	//f we read an escape character, we immediately read two more bytes into the seq buffer. If either of these reads time out (after 0.1 seconds), then we assume the user just pressed the Escape key and return that. Otherwise we look to see if the escape sequence is an arrow key escape sequence. If it is, we just return the corresponding wasd character, for now. If it’s not an escape sequence we recognize, we just return the escape character.

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';   
    if (seq[0] == '[') 
    {
      if (seq[1] >= '0' && seq[1] <= '9') 
      {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') 
        {
          switch (seq[1]) 
          {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } 
      else 
      {
        switch (seq[1]) 
        {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } 
    else if (seq[0] == 'O') 
    {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }

    return '\x1b';
  } 
  //for normal alphanumeric keys including backspace and enter and tabs and shifts
  else 
  {
    return c;
  }
}
//get cursor positions and put in global variables cx and cy
int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;	// The n command (Device Status Report) can be used to query the terminal for status information. We want to give it an argument of 6 to ask for the cursor position. Then we can read the reply from the standard input.
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;	//We’re going to have to parse this response. But first, let’s read it into a buffer. We’ll keep reading characters until we get to the R character.
    if (buf[i] == 'R') break;
    i++;
  }
//When we print out the buffer, we don’t want to print the '\x1b' character, because the terminal would interpret it as an escape sequence and wouldn’t display it. So we skip the first character in buf by passing &buf[1] to printf(). printf() expects strings to end with a 0 byte, so we make sure to assign '\0' to the final byte of buf.
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;	//First we make sure it responded with an escape sequence. Then we pass a pointer to the third character of buf to sscanf(), skipping the '\x1b' and '[' characters. So we are passing a string of the form 24;80 to sscanf(). We are also passing it the string %d;%d which tells it to parse two integers separated by a ;, and put the values into the rows and cols variables.

  return 0;
}
//get window size of terminal
//added a failsafe method if ioctl doesnt work
//i.e. move the cursor to end point and get cx and cy positions
int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {	//On most systems, you should be able to get the size of the terminal by simply calling ioctl() with the TIOCGWINSZ request
//On success, ioctl() will place the number of columns wide and the number of rows high the terminal is into the given winsize struct. On failure, ioctl() returns -1. 
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;	//B-down shift ,C-right shift of cursor
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}
//SECTION for datahandling in rows structure

#include "datainsdelhandling.h"

//other ins del operations

void editorInsertChar(int c) {
  if (EDIT_global.cy == EDIT_global.numrows) {
    editorInsertRow(EDIT_global.numrows, "", 0);
  }
  editorRowInsertChar(&EDIT_global.row[EDIT_global.cy], EDIT_global.cx, c);
  EDIT_global.cx++;
}

void editorInsertNewline() {
  if (EDIT_global.cx == 0) {
    editorInsertRow(EDIT_global.cy, "", 0);
  } else {
    editorRow *row = &EDIT_global.row[EDIT_global.cy];
    editorInsertRow(EDIT_global.cy + 1, &row->chars[EDIT_global.cx], row->size - EDIT_global.cx);
    row = &EDIT_global.row[EDIT_global.cy];
    row->size = EDIT_global.cx;
    row->chars[row->size] = '\0';
    editorUpdateRow(row);
  }
  EDIT_global.cy++;
  EDIT_global.cx = 0;
}

void editorDelChar() {
  if (EDIT_global.cy == EDIT_global.numrows) return;
  if (EDIT_global.cx == 0 && EDIT_global.cy == 0) return;

  editorRow *row = &EDIT_global.row[EDIT_global.cy];
  if (EDIT_global.cx > 0) {
    editorRowDelChar(row, EDIT_global.cx - 1);
    EDIT_global.cx--;
  } else {
    EDIT_global.cx = EDIT_global.row[EDIT_global.cy - 1].size;
    editorRowAppendString(&EDIT_global.row[EDIT_global.cy - 1], row->chars, row->size);
    editorDelRow(EDIT_global.cy);
    EDIT_global.cy--;
  }
}

//FILE MENU OPTIONS _ SAVE _ NEW _OPEN

char *editorRowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < EDIT_global.numrows; j++)
    totlen += EDIT_global.row[j].size + 1;
  *buflen = totlen;

  char *buf = malloc(totlen);
  char *p = buf;
  for (j = 0; j < EDIT_global.numrows; j++) {
    memcpy(p, EDIT_global.row[j].chars, EDIT_global.row[j].size);
    p += EDIT_global.row[j].size;
    *p = '\n';
    p++;
  }
  return buf;
}
//open file prsennt in arguements
void editorOpen(char *filename) {
  free(EDIT_global.filename);
  EDIT_global.filename = strdup(filename);

  FILE *fp = fopen(filename, "r");
  if (!fp) end_n_error("fopen");

  char *line = NULL;
  size_t linecap = 0;	//line capacity.if -1,end of file no more line to read
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    editorInsertRow(EDIT_global.numrows, line, linelen);
  }
  free(line);
  fclose(fp);
  EDIT_global.dirty = 0;
}
//file save mechanismes
void editorSave() {
  if (EDIT_global.filename == NULL) {
    EDIT_global.filename = editorPrompt("Save as: %s (ESC to cancel)");
    if (EDIT_global.filename == NULL) {
      editorSetStatusMessage("Save cancelled.");
      return;
    }
  }

  int len;
  char *buf = editorRowsToString(&len);

  int fd = open(EDIT_global.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        EDIT_global.dirty = 0;
        editorSetStatusMessage("File saved. File size: %d bytes", len);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage("Save declined . Input/Output section malfunction.: %s", strerror(errno));
}

//bufferdatastucture to append data and frree it
//It’s not a good idea to make a whole bunch of small write()’s every time we refresh the screen. It would be better to do one big write(), to make sure the whole screen updates at once. Otherwise there could be small unpredictable pauses between write()’s, which would cause an annoying flicker effect.

/*We want to replace all our write() calls with code that appends the string to a buffer, and then write() this buffer out at the end. Unfortunately, C doesn’t have dynamic strings, so we’ll create our own dynamic string type that supports one operation: appending.This acts as a constructor for our abuf type.*/
struct appbuf {
  char *b;
  int len;
};
#define APPBUF_INIT {NULL, 0}
void abAppend(struct appbuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);	//to append a new string first we r ensuring enough space for that string by realloc.realloc gives us current string size+size of the new string.

  if (new == NULL) return;
  memcpy(&new[ab->len], s, len);	//to copy the string s after the end of the current data in the buffer and we update the pointer length 
  ab->b = new;
  ab->len += len;
}
void abFree(struct appbuf *ab) {	//is a destructor that deallocates the dynamic memory used by an appbuf.
  free(ab->b);
}

//the output system functions
#include "outputsection.h"
//search functions
    //not added


//display prompt messages in satus bar if required and reading other keys to enter new file names or search funtions
char *editorPrompt(char *prompt) {
  size_t bufsize = 128;
  char *buf = malloc(bufsize);
  size_t buflen = 0;
  buf[0] = '\0';
  while (1) {
    editorSetStatusMessage(prompt, buf);
    editorRefreshScreen();
    int c = readNewKey();
    if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (buflen != 0) buf[--buflen] = '\0';
    } else if (c == '\x1b') {
      editorSetStatusMessage("");
      free(buf);
      return NULL;
    } else if (c == '\r') {
      if (buflen != 0) {
        editorSetStatusMessage("");
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }
  }
}
//handling of arrrow keys to move through the files
void editorMoveCursor(int key) {
  editorRow *row = (EDIT_global.cy >= EDIT_global.numrows) ? NULL : &EDIT_global.row[EDIT_global.cy];

  switch (key) {
    case ARROW_LEFT:
      if (EDIT_global.cx != 0) {
        EDIT_global.cx--;
      } else if (EDIT_global.cy > 0) {
        EDIT_global.cy--;
        EDIT_global.cx = EDIT_global.row[EDIT_global.cy].size;
      }
      break;
    case ARROW_RIGHT:
      if (row && EDIT_global.cx < row->size) {
        EDIT_global.cx++;
      } else if (row && EDIT_global.cx == row->size) {
        EDIT_global.cy++;
        EDIT_global.cx = 0;
      }
      break;
    case ARROW_UP:
      if (EDIT_global.cy != 0) {
        EDIT_global.cy--;
      }
      break;
    case ARROW_DOWN:
      if (EDIT_global.cy < EDIT_global.numrows) {
        EDIT_global.cy++;
      }
      break;
  }

  row = (EDIT_global.cy >= EDIT_global.numrows) ? NULL : &EDIT_global.row[EDIT_global.cy];
  int rowlen = row ? row->size : 0;
  if (EDIT_global.cx > rowlen) {
    EDIT_global.cx = rowlen;
  }
}
//handling of keys input
void editorProcessKeypress() {
  static int quit_times = NO_OF_QUIT_PRESSED;
  int c = readNewKey();
  switch (c) {
    case '\r':
      editorInsertNewline();
      break;

    case CTRL_KEY('q'):
      if (EDIT_global.dirty && quit_times > 0) {
        editorSetStatusMessage("CAUTION. Unsaved Changes. please save "
          "Press Ctrl-Q %d more times to quit without saving.", quit_times);
        quit_times--;
        return;
      }
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;

    case CTRL_KEY('s'):
      editorSave();
      break;

    case HOME_KEY:
      EDIT_global.cx = 0;
      break;

    case END_KEY:
      if (EDIT_global.cy < EDIT_global.numrows)
        EDIT_global.cx = EDIT_global.row[EDIT_global.cy].size;
      break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
      if (c == DEL_KEY) editorMoveCursor(ARROW_RIGHT);
      editorDelChar();
      break;

    case PAGE_UP:
    case PAGE_DOWN:
      {
        if (c == PAGE_UP) {
          EDIT_global.cy = EDIT_global.rowoff;
        } else if (c == PAGE_DOWN) {
          EDIT_global.cy = EDIT_global.rowoff + EDIT_global.screenrows - 1;
          if (EDIT_global.cy > EDIT_global.numrows) EDIT_global.cy = EDIT_global.numrows;
        }

        int times = EDIT_global.screenrows;
        while (times--)
          editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;

    case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      editorInsertChar(c);
      break;
  }

  quit_times = NO_OF_QUIT_PRESSED;
}

//initialising editor

void initEditor() {
  EDIT_global.cx = 0;
  EDIT_global.cy = 0;
  EDIT_global.rx = 0;
  EDIT_global.rowoff = 0;	//we will be scrolled to the top of the file by default.
  EDIT_global.coloff = 0;
  EDIT_global.numrows = 0;
  EDIT_global.row = NULL;
  EDIT_global.dirty = 0;
  EDIT_global.filename = NULL;
  EDIT_global.statusmsg[0] = '\0';
  EDIT_global.statusmsg_time = 0;

  if (getWindowSize(&EDIT_global.screenrows, &EDIT_global.screencols) == -1) end_n_error("getWindowSize");
  EDIT_global.screenrows -= 2;
}

int main(int argc, char *argv[]) {
  enableRawMode();
  initEditor();		//initEditor()’s job will be to initialize all the fields in the E struct.
  if (argc >= 2) {
    editorOpen(argv[1]);
  }

  editorSetStatusMessage("Help Commands: Ctrl-S for Save | Ctrl-Q for Quit");

  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }

  return 0;
}
