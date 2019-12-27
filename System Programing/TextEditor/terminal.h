//HEADER to initialise terminal for starting text editor

//this resets the command to revert chnages made to terminal back to normal
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &EDIT_global.orig_termios) == -1)
    end_n_error("tcsetattr. type 'reset' to correct terminal ");
}
//make sthe terminal go into raw mode so thta no other functions of terminal take place
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &EDIT_global.orig_termios) == -1) end_n_error("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = EDIT_global.orig_termios;
  //disabling of keys o be echoed, control signs, backspaces, extra keys and all
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) end_n_error("tcsetattr");
}