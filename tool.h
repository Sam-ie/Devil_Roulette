#ifndef TOOL_H
#define TOOL_H

#include <QDialog>
#include "model.h"

namespace Ui {
class Tool;
}

class Tool : public QDialog
{
    Q_OBJECT

public:
    explicit Tool(QWidget *parent = nullptr);
    ~Tool();

signals:
    void closed(int exitCode);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::Tool *ui;
};

#endif // TOOL_H
