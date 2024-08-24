#include "tool.h"
#include "ui_tool.h"

Tool::Tool(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Tool)
{
    ui->setupUi(this);

    QString btnText[9]={"放大镜","香烟","手铐","护木","手锯","逆转器","过期药品","窃贼手套","小灵通"};
    for (int i = 1; i <= 9; i++) {
        QPushButton *button = ui->groupBox->findChild<QPushButton*>(QString("pushButton_%1").arg(i));
        if (Model::getInstance().rival_items[i-1]>0) {
            button->setEnabled(true);
            button->setText(btnText[i-1]+QString(" %1").arg(Model::getInstance().rival_items[i-1]));
        }
        else
        {
            button->setEnabled(false);
            button->setText(btnText[i-1]);
        }
    }
}

Tool::~Tool()
{
    delete ui;
}

void Tool::on_pushButton_1_clicked()
{
    emit closed(1); // 发送信号，表示通过按钮关闭
    accept(); // 关闭对话框
}


void Tool::on_pushButton_2_clicked()
{
    emit closed(2);
    accept();
}


void Tool::on_pushButton_3_clicked()
{
    emit closed(3);
    accept();
}


void Tool::on_pushButton_4_clicked()
{
    emit closed(4);
    accept();
}


void Tool::on_pushButton_5_clicked()
{
    emit closed(5);
    accept();
}


void Tool::on_pushButton_6_clicked()
{
    emit closed(6);
    accept();
}


void Tool::on_pushButton_7_clicked()
{
    emit closed(7);
    accept();
}


void Tool::on_pushButton_8_clicked()
{
    emit closed(8);
    accept();
}


void Tool::on_pushButton_9_clicked()
{
    emit closed(9);
    accept();
}

void Tool::closeEvent(QCloseEvent *event)
{
    emit closed(0); // 发送信号，表示通过关闭按钮关闭
    QDialog::closeEvent(event); // 调用基类的 closeEvent 方法
}
