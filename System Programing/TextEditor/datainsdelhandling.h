//HEADER for insert delete and rendering
//required while horizantal scrlling only t
int editorRowCxToRx(editorRow *row, int cx) {
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (TABSPACES - 1) - (rx % TABSPACES);
    rx++;
  }
  return rx;
}
//updating the editor row datastructure
void editorUpdateRow(editorRow *row) {
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t') tabs++;

  free(row->render);
  row->render = malloc(row->size + tabs*(TABSPACES - 1) + 1);

  int idx = 0;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';
      while (idx % TABSPACES != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;
}
//addition of new row
void editorInsertRow(int at, char *s, size_t len) {
  if (at < 0 || at > EDIT_global.numrows) return;
  EDIT_global.row = realloc(EDIT_global.row, sizeof(editorRow) * (EDIT_global.numrows + 1));
  memmove(&EDIT_global.row[at + 1], &EDIT_global.row[at], sizeof(editorRow) * (EDIT_global.numrows - at));
  EDIT_global.row[at].size = len;
  EDIT_global.row[at].chars = malloc(len + 1);
  memcpy(EDIT_global.row[at].chars, s, len);
  EDIT_global.row[at].chars[len] = '\0';
  EDIT_global.row[at].rsize = 0;
  EDIT_global.row[at].render = NULL;
  editorUpdateRow(&EDIT_global.row[at]);
  EDIT_global.numrows++;
  EDIT_global.dirty++;
}
//clear a row
void editorFreeRow(editorRow *row) {
  free(row->render);
  free(row->chars);
}
//delete a row
void editorDelRow(int at) {
  if (at < 0 || at >= EDIT_global.numrows) return;
  editorFreeRow(&EDIT_global.row[at]);
  memmove(&EDIT_global.row[at], &EDIT_global.row[at + 1], sizeof(editorRow) * (EDIT_global.numrows - at - 1));
  EDIT_global.numrows--;
  EDIT_global.dirty++;
}
//insert characters in an existing row
void editorRowInsertChar(editorRow *row, int at, int c) {
  if (at < 0 || at > row->size) at = row->size;
  row->chars = realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  editorUpdateRow(row);
  EDIT_global.dirty++;
}
//add strings at end
void editorRowAppendString(editorRow *row, char *s, size_t len) {
  row->chars = realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  EDIT_global.dirty++;
}
//deletion of character
void editorRowDelChar(editorRow *row, int at) {
  if (at < 0 || at >= row->size) return;
  memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
  row->size--;
  editorUpdateRow(row);
  EDIT_global.dirty++;
}