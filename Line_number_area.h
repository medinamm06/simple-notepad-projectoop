#ifndef LINE_NUMBER_AREA_H
#define LINE_NUMBER_AREA_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QWidget>

class line_number_area;

// ── Editor with line-number margin ───────────────────────────────────────────

class line_number_editor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit line_number_editor(QWidget* parent = nullptr);

    [[nodiscard]] int line_number_width() const;
    void line_number_paint(QPaintEvent* event);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void update_line_number_width(int);
    void update_line_number_area(const QRect& rect, int dy);

private:
    line_number_area* number_area { nullptr };
};

// ── Thin widget that sits in the margin ──────────────────────────────────────

class line_number_area : public QWidget {
public:
    explicit line_number_area(line_number_editor* editor)
        : QWidget(editor)
        , code_editor(editor)
    {
    }

    [[nodiscard]] QSize sizeHint() const override
    {
        return { code_editor->line_number_width(), 0 };
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        code_editor->line_number_paint(event);
    }

private:
    line_number_editor* code_editor;
};

// Inline implementations (need both classes complete)
inline line_number_editor::line_number_editor(QWidget* parent)
    : QPlainTextEdit(parent)
    , number_area(new line_number_area(this))
{
    connect(this, &QPlainTextEdit::blockCountChanged,
        this, &line_number_editor::update_line_number_width);
    connect(this, &QPlainTextEdit::updateRequest,
        this, &line_number_editor::update_line_number_area);

    update_line_number_width(0);
}

inline int line_number_editor::line_number_width() const
{
    int digits = 1;
    int max = std::max(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    return 6 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

inline void line_number_editor::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);
    const QRect cr = contentsRect();
    number_area->setGeometry(QRect(cr.left(), cr.top(), line_number_width(), cr.height()));
}

inline void line_number_editor::update_line_number_width(int)
{
    setViewportMargins(line_number_width(), 0, 0, 0);
}

inline void line_number_editor::update_line_number_area(const QRect& rect, int dy)
{
    if (dy) {
        number_area->scroll(0, dy);
    } else {
        number_area->update(0, rect.y(), number_area->width(), rect.height());
    }
    if (rect.contains(viewport()->rect())) {
        update_line_number_width(0);
    }
}

#endif // LINE_NUMBER_AREA_H