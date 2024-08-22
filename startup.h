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
    void on_pushButton_11_clicked();

private:
    Ui::Startup *ui;

    Model m;
};
#endif // STARTUP_H
