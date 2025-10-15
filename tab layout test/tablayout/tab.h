#ifndef TAB_H
#define TAB_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class tab; }
QT_END_NAMESPACE

class tab : public QWidget
{
    Q_OBJECT

public:
    tab(QWidget *parent = nullptr);
    ~tab();

private:
    Ui::tab *ui;
};
#endif // TAB_H
