#ifndef LINE_NUMBER_AREA_H
#define LINE_NUMBER_AREA_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QWidget>

class line_number_area;


// редактор с областью для номеров строк сбоку
class line_number_editor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit line_number_editor(QWidget* parent = nullptr);

    // считает ширину области под цифры
    [[nodiscard]] int line_number_width() const;

    // функция которая потом рисует сами номера
    void line_number_paint(QPaintEvent* event);

protected:
    // отслеживаем изменение размера окна
    void resizeEvent(QResizeEvent* event) override;

private slots:

    // обновляет размер области с номерами
    void update_line_number_width(int);

    // обновляет область когда скроллим или что-то меняется
    void update_line_number_area(
        const QRect& rect,
        int dy);

private:

    // указатель на область с цифрами
    line_number_area* number_area { nullptr };
};


// отдельный маленький виджет сбоку нужен чисто под номера строк
class line_number_area : public QWidget {
public:
    explicit line_number_area(
        line_number_editor* editor)

        : QWidget(editor)
        , code_editor(editor)
    {
    }

    // подсказываем какой размер нужен
    [[nodiscard]] QSize sizeHint() const override
    {
        return {
            code_editor->line_number_width(),
            0
        };
    }

protected:

    // когда надо рисовать область вызываем функцию редактора
    void paintEvent(
        QPaintEvent* event) override
    {
        code_editor
            ->line_number_paint(event);
    }

private:
    line_number_editor* code_editor;
};


// дальше inline функции типа чтоб все было сразу тут

inline line_number_editor::
line_number_editor(QWidget* parent)

    : QPlainTextEdit(parent)

    , number_area(
        new line_number_area(this))
{
    // соединяем сигналы, если количество строк изменилось обновляем область

    connect(
        this,
        &QPlainTextEdit::blockCountChanged,
        this,
        &line_number_editor::
        update_line_number_width);

    // если скроллим или что-то двигается
    connect(
        this,
        &QPlainTextEdit::updateRequest,
        this,
        &line_number_editor::
        update_line_number_area);

    update_line_number_width(0);
}


inline int
line_number_editor::
line_number_width() const
{
    // минимум одна цифра
    int digits = 1;

    // берем количество строк
    int max =
        std::max(1, blockCount());

    // считаем сколько цифр надо
    // типа 9=одна цифра
    // 100= уже три
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    // считаем итоговую ширину
    return 6 +
        fontMetrics()
        .horizontalAdvance(
        QLatin1Char('9'))
        * digits;
}


inline void
line_number_editor::
resizeEvent(
QResizeEvent* event)
{
    // стандартное изменение размера
    QPlainTextEdit::resizeEvent(event);

    const QRect cr =
        contentsRect();

    // двигаем область номеров
    number_area->setGeometry(
        QRect(
            cr.left(),
            cr.top(),
            line_number_width(),
            cr.height()));
}


inline void
line_number_editor::
update_line_number_width(int)
{
    // добавляем отступ слева
    // чтоб текст не залезал на цифры
    setViewportMargins(
        line_number_width(),
        0,0,0);
}


inline void
line_number_editor::
update_line_number_area(
const QRect& rect,
int dy)
{
    // если скроллим
    if (dy) {
        number_area->scroll(
            0,dy);
    }
    else {

        // иначе обновляем область
        number_area->update(
            0,
            rect.y(),
            number_area->width(),
            rect.height());
    }

    // если обновилось все окно
    if(rect.contains(
        viewport()->rect()))
    {
        update_line_number_width(0);
    }
}

#endif // LINE_NUMBER_AREA_H