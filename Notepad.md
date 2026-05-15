# Notepad — Implementation Notes

## Required Features

### 1. Exception Handling

Created `notepad_exception.h` with a hierarchy of four exception classes:
`notepad_exception` as the base (extends `std::runtime_error`), and three
derived classes: `file_not_found_exception`, `file_read_exception`, and
`file_write_exception`. Each carries a descriptive message with the file path.

Both `open_file()` and `save_file()` are wrapped in `try / catch` blocks that
catch `notepad_exception` and display the error message via `QMessageBox::critical`.

### 2. Spell Checker

Implemented in two classes:

- `spell_checker` — loads `data/words.txt` into a `std::set<std::string>` at
  startup. `is_correct()` lowercases and strips non-alphabetic characters before
  looking up the word. `suggestions()` returns up to 5 candidates using prefix
  matching and edit-distance-1 search.

- `spell_checker_highlighter` — a `QSyntaxHighlighter` subclass that scans each
  block for words with `QRegularExpression`, calls `is_correct()`, and applies
  `QTextCharFormat::SpellCheckUnderline` in red to misspelled words.

Right-clicking a misspelled word shows a context menu with up to 5 suggestions.
Clicking a suggestion replaces the word via `QTextCursor::insertText()`.
`Tools > Check Spelling...` calls `rehighlight()` to re-run the pass.

## Optional Features

### 1. Cursor Line / Column Indicator (Feature #1)

The status bar displays the current cursor position as `Ln X, Col Y` alongside
word and line counts. Implemented in `update_status_bar()` using
`QTextCursor::blockNumber()` and `columnNumber()`. The slot is connected to both
`textChanged` and `cursorPositionChanged` signals so the indicator updates in
real time.

### 2. Text Color Picker (Feature #3)

`Format > Text Color...` opens a `QColorDialog`. If text is selected, the color
is applied only to the selection; otherwise it is applied to the whole document.
The color is set via `QTextCharFormat::setForeground()` and merged with
`QTextCursor::mergeCharFormat()`.

### 3. Line Numbers (Feature #6)

Implemented with two classes in `line_number_area.h` and `line_number_area.cpp`:

- `line_number_area` — a thin `QWidget` placed in the left margin of the editor.
- `line_number_editor` — a `QPlainTextEdit` subclass that owns the margin widget,
  computes its width based on the digit count of the last line number, and
  overrides `resizeEvent` and `paintEvent` to keep the margin in sync with
  scrolling and block changes.
