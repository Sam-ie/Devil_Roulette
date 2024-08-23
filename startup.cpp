#include "startup.h"
#include "ui_startup.h"

Startup::Startup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Startup)
{
    ui->setupUi(this);

    // 禁用窗口的最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    // 禁止窗口调整大小
    setFixedSize(geometry().width(), geometry().height());

    ui->textEdit->setReadOnly(true);
    ui->textEdit_2->setReadOnly(true);
    ui->textEdit_3->setReadOnly(true);
    ui->textEdit_4->setReadOnly(true);

    change_btn_enable();
    update_shown_text();
    update_shown_text();
}

Startup::~Startup()
{
    delete ui;
}

void Startup::on_pushButton_12_clicked()
{
    // 创建一个消息对话框
    QMessageBox msgBox(this);

    // 设置对话框标题
    msgBox.setWindowTitle("帮助");

    // 设置对话框的正文内容，包括两行文本
    msgBox.setText("在自己血量不为0的前提下，将对手血量削减到0！\n对手AI水平随关数越来越高，看看你能坚持几轮？\n\n"
                   "道具说明：\n【放大镜】检查弹膛中当前的子弹。\n【香烟】消除压力，恢复1点生命值。\n【手铐】对方下一回合无法行动。\n【啤酒】弹出霰弹枪膛内当前的子弹。\n"
                   "【手锯】霰弹枪造成2点伤害（不可叠加）。\n【逆转器】切换弹膛内当前子弹为实弹或空包弹(实弹换空包弹，空包弹换实弹)。\n【过期药品】有40%的几率恢复2点生命值，否则将损失1点生命值。\n"
                   "【窃贼手套】偷窃一件物品并立即使用。\n【小灵通】一个神秘的声音，让你窥探未来(告诉你枪里第几颗子弹是实弹还是空包弹)。\n\n"
                   "策略说明：\n向自己开枪，如果枪中为空包弹，对手跳过下一回合。\n向对手开枪，如果枪中为实弹，对手损失相应伤害的生命值。\n"
                   "霰弹枪每轮重新装弹，会分发一定数量的道具。\n在自己回合内，可以自由使用自己持有的任意数量道具。\n各轮之间，道具不继承，先手顺序将会继承。\n霰弹枪会重复装弹，直到一方血量为0。");
    // 继承会极大影响策略，此处可修改。

    // 设置默认图标为Information
    msgBox.setIcon(QMessageBox::Information);

    // 显示消息对话框
    msgBox.exec();
}

void Startup::change_btn_enable()
{
    // 两种寻找指针方式
    QPushButton *buttons[] = {ui->pushButton_1, ui->pushButton_2, ui->pushButton_3,
                              ui->pushButton_4, ui->pushButton_5, ui->pushButton_6,
                              ui->pushButton_7, ui->pushButton_8, ui->pushButton_9,
                              ui->pushButton_10, ui->pushButton_11};
    for (QPushButton *button : buttons) {
        button->setEnabled(false);
    }

    if (m.whos_turn==1&&!m.jump_next[1])
    {
        for (int i = 1; i <= 9; i++) {
            QPushButton *button = ui->groupBox_3->findChild<QPushButton*>(QString("pushButton_%1").arg(i));
            if (m.player_items[i-1]>0) {
                button->setEnabled(true);
            }
        }
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
    }
}

QString createTable(const QStringList &cells, QString action="")
{
    QString html =
        "<div style='text-align: center;'>当前动作：<br/>" + action + "</div>";

    // 创建表格标题
    html += "<table style='border-collapse: collapse; width: 100%; margin: 0 auto;'>"
            "<tr><th colspan='3' style='text-align: center;'>持有道具：</th></tr>";

    // 创建表格内容
    for (int i = 0; i < cells.size(); i++) {
        int row = i / 3;
        int col = i % 3;

        if (col == 0 && row != 0) {
            html += "</tr><tr>";
        }

        html +=
            "<td style='border: none; text-align: center;'>"
            + cells[i] +
            "</td>";
    }

    // 结束表格
    html += "</tr></table>";

    return html;
}

void Startup::update_shown_text(QString rival_ation)
{
    QString text=QString("<center>第%1关<br/>第%2轮</center>").arg(m.level).arg(m.round);
    ui->textEdit->setText(text);

    // 创建表格内容
    QStringList cells = {QString("放大镜 : %1").arg(m.rival_items[0]), QString("香烟 : %1").arg(m.rival_items[1]), QString("手铐 : %1").arg(m.rival_items[2]),
                         QString("啤酒 : %1").arg(m.rival_items[3]), QString("手锯 : %1").arg(m.rival_items[4]), QString("逆转器 : %1").arg(m.rival_items[5]),
                         QString("过期药品 : %1").arg(m.rival_items[6]), QString("窃贼手套 : %1").arg(m.rival_items[7]), QString("小灵通 : %1").arg(m.rival_items[8])};
    // 生成表格 HTML 字符串
    QString html = createTable(cells, rival_ation);
    ui->textEdit_2->setText(html);

    int count_true = 0,count_false=0;
    for (int element : m.gun_status) {
        if (element > 0)
            count_true++;
        if (element == 0)
            count_false++;
    }
    QString text_3=QString("<center>对手血量：%1<br/>玩家血量：%2<br/><br/>霰弹枪状态：<br/>剩余：<br/>%3发实弹<br/>%4发空包弹</center>").arg(m.blood[0]).arg(m.blood[1]).arg(count_true).arg(count_false);
    ui->textEdit_3->setText(text_3);

    QString text_4=QString("<center>第%1关<br/>第%2轮</center>").arg(m.level).arg(m.round);
    ui->textEdit_4->setText(text_4);
}
