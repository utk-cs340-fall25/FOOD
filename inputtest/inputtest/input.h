#ifndef INPUT_H
#define INPUT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Input; }
QT_END_NAMESPACE

class Input : public QWidget
{
    Q_OBJECT

public:
    Input(QWidget *parent = nullptr);
    ~Input();

private:
    Ui::Input *ui;
};
#endif // INPUT_H
