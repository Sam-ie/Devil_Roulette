#ifndef STARTUP_H
#define STARTUP_H

#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QMetaMethod>
#include <QThread>
#include "model.h"
#include "tool.h"
#include "ai.h"

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

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    void change_btn_enable();
    void set_level_hint();
    void update_shown_text(QString rival_ation="",QString text_log_addition="");
    void end_level();
    void get_AI_move();

private:
    Ui::Startup *ui;

    int exit_code=0;
    bool use_swap=false;
    QString text_log="";
};
#endif // STARTUP_H
