//HEADER file to control display part of text editor

//scrolling funcyions and handling of details
void editorScroll() {
  EDIT_global.rx = 0;
  if (EDIT_global.cy < EDIT_global.numrows) {
    EDIT_global.rx = editorRowCxToRx(&EDIT_global.row[EDIT_global.cy], EDIT_global.cx);
  }
  if (EDIT_global.cy < EDIT_global.rowoff) {
    EDIT_global.rowoff = EDIT_global.cy;
  }
  if (EDIT_global.cy >= EDIT_global.rowoff + EDIT_global.screenrows) {
    EDIT_global.rowoff = EDIT_global.cy - EDIT_global.screenrows + 1;
  }
  if (EDIT_global.rx < EDIT_global.coloff) {
    EDIT_global.coloff = EDIT_global.rx;
  }
  if (EDIT_global.rx >= EDIT_global.coloff + EDIT_global.screencols) {
    EDIT_global.coloff = EDIT_global.rx - EDIT_global.screencols + 1;
  }
}
//display screen . row wise
void editorDrawRows(struct appbuf *ab) {
  int y;
  for (y = 0; y < EDIT_global.screenrows; y++) {
    int filerow = y + EDIT_global.rowoff;
    if (filerow >= EDIT_global.numrows) {
      if (EDIT_global.numrows == 0 && y == EDIT_global.screenrows / 3) {
        //if new file is creted , display this message
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
          "Text Editor -Assignment 4- Hello ");
        if (welcomelen > EDIT_global.screencols) welcomelen = EDIT_global.screencols;
        int padding = (EDIT_global.screencols - welcomelen) / 2;
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);
        abAppend(ab, welcome, welcomelen);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      int len = EDIT_global.row[filerow].rsize - EDIT_global.coloff;
      if (len < 0) len = 0;
      if (len > EDIT_global.screencols) len = EDIT_global.screencols;
      abAppend(ab, &EDIT_global.row[filerow].render[EDIT_global.coloff], len);
    }

    abAppend(ab, "\x1b[K", 3);
    abAppend(ab, "\r\n", 2);
  }
}
//designing of status bar
void editorDrawStatusBar(struct appbuf *ab) {
  abAppend(ab, "\x1b[7m", 4);
  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
    EDIT_global.filename ? EDIT_global.filename : "[unnamedfile]", EDIT_global.numrows,
    EDIT_global.dirty ? "*" : "");
  int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d",
    EDIT_global.cy + 1, EDIT_global.numrows);
  if (len > EDIT_global.screencols) len = EDIT_global.screencols;
  abAppend(ab, status, len);
  while (len < EDIT_global.screencols) {
    if (EDIT_global.screencols - len == rlen) {
      abAppend(ab, rstatus, rlen);
      break;
    } else {
      abAppend(ab, " ", 1);
      len++;
    }
  }
  abAppend(ab, "\x1b[m", 3);
  abAppend(ab, "\r\n", 2);
}
//designing of message baar
void editorDrawMessageBar(struct appbuf *ab) {
  abAppend(ab, "\x1b[K", 3);
  int msglen = strlen(EDIT_global.statusmsg);
  if (msglen > EDIT_global.screencols) msglen = EDIT_global.screencols;
  if (msglen && time(NULL) - EDIT_global.statusmsg_time < 5)
    abAppend(ab, EDIT_global.statusmsg, msglen);
}
//function to refresh screen after any changes are made in the file
void editorRefreshScreen() {
  editorScroll();

  struct appbuf ab = APPBUF_INIT;
  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);
  editorDrawRows(&ab);
  editorDrawStatusBar(&ab);
  editorDrawMessageBar(&ab);
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (EDIT_global.cy - EDIT_global.rowoff) + 1,(EDIT_global.rx - EDIT_global.coloff) + 1);                                         
  abAppend(&ab, buf, strlen(buf));
  abAppend(&ab, "\x1b[?25h", 6);
  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}
//setting funtion of what to display in message baar
void editorSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(EDIT_global.statusmsg, sizeof(EDIT_global.statusmsg), fmt, ap);
  va_end(ap);
  EDIT_global.statusmsg_time = time(NULL);
}