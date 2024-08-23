#ifndef STARTUP_H
#define STARTUP_H

#include <QWidget>
#include <QMessageBox>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Startup;
}
QT_END_NAMESPACE

class Startup : public QWidget
{
    Q_OBJECT

public:
    Startup(QWidget *parent = nullptr);
    ~Startup();

private slots:
    void on_pushButton_12_clicked();

private:
    void change_btn_enable();
    void update_shown_text(QString rival_ation="");

private:
    Ui::Startup *ui;

    Model m;
};
#endif // STARTUP_H
