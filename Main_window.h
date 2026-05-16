#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "line_number_area.h"
#include "spell_checker.h"
#include "spell_checker_highlighter.h"
#include "text_transform.h"

#include <QDialog>
#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <memory>
#include <vector>

namespace Ui {
class find_replace_dialog;
class word_frequency_dialog;
}

class main_window : public QMainWindow {
  Q_OBJECT

public:
  main_window();
  ~main_window() override;

private:
  // Menu setup
  void setup_file_menu();
  void setup_edit_menu();
  void setup_format_menu();
  void setup_tools_menu();
  void setup_search_menu();

  // Toolbar
  void setup_format_toolbar();

  // File operations
  void open_file();
  void save_file();
  void save_file_as();
  void update_title();

  // Status bar
  void update_status_bar() const;

  // Text transforms
  void apply_transform(const text_transform& transform) const;

  // Find / Replace
  void show_find_replace_dialog();
  void find_next(const QString& term,
      QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
  void replace_current(const QString& term, const QString& replacement,
      QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
  void replace_all(const QString& term, const QString& replacement,
      QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;

  // Tools
  void show_word_frequency();
  void run_spell_check();


  // Spell check context menu



  // Widgets
  line_number_editor* editor { nullptr };
  spell_checker checker;
  spell_checker_highlighter* highlighter { nullptr };

  QString current_file;
  std::vector<std::unique_ptr<text_transform>> transforms;

  QDialog* find_replace_dlg { nullptr };
  std::unique_ptr<Ui::find_replace_dialog> find_replace_ui;
};

#endif // MAIN_WINDOW_H