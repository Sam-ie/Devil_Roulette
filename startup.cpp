#include "startup.h"
#include "ui_startup.h"

Startup::Startup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Startup)
{
    ui->setupUi(this);

    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    // 设置文本居中
    ui->lineEdit->setAlignment(Qt::AlignCenter);
    ui->lineEdit_2->setAlignment(Qt::AlignCenter);
}

Startup::~Startup()
{
    delete ui;
}

void Startup::on_pushButton_11_clicked()
{
    // 创建一个消息对话框
    QMessageBox msgBox(this);

    // 设置对话框标题
    msgBox.setWindowTitle("帮助");

    // 设置对话框的正文内容，包括两行文本
    msgBox.setText("在自己血量不为0的前提下，将对手血量削减到0！\n\n对手AI水平随局数越来越高，看看你能坚持几轮？\n\n"
                   "道具说明：\n【放大镜】检查弹膛中当前的子弹。\n【香烟】消除压力，恢复1点生命值。\n【手铐】对方下一回合无法行动。\n【啤酒】弹出霰弹枪膛内当前的子弹。\n"
                   "【手锯】霰弹枪造成2点伤害。\n【逆转器】切换弹膛内当前子弹为实弹或空包弹(实弹换空包弹，空包弹换实弹)。\n【过期药品】有50%的几率恢复2点生命值，否则将损失1点生命值。\n【肾上腺素】偷窃一件物品并立即使用。\n\n"
                   "策略说明：\n向自己开枪，如果枪中为空包弹，对手跳过下一回合。\n在自己回合内，可以自由使用自己持有的任意数量道具。\n");

    // 设置默认图标为Information
    msgBox.setIcon(QMessageBox::Information);

    // 显示消息对话框
    msgBox.exec();
}
