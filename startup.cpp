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
    update_shown_text("",QString("<center>第%1关<br/>第%2局</center>").arg(Model::getInstance().level).arg(Model::getInstance().round));
    set_level_hint();
    if (Model::getInstance().whos_turn==0)
        get_AI_move();
}

Startup::~Startup()
{
    delete ui;
}

void Startup::on_pushButton_12_clicked()
{
    QDialog dialog(this);
    // 设置窗口标题
    dialog.setWindowTitle("帮助");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QTextEdit *textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true); // 设置只读，防止编辑
    textEdit->setText("在自己血量不为0的前提下，将对手血量削减到0！\n对手AI水平随关数越来越高，看看你能坚持几关？\n\n"
                      "道具说明：\n【放大镜】检查弹膛中当前的子弹。\n【香烟】消除压力，恢复1点生命值。\n【手铐】对方下一回合无法行动（不可叠加，和射自己的跳过效果覆盖）。\n【护木】弹出霰弹枪膛内当前的子弹。\n"
                      "【手锯】霰弹枪造成2点伤害（不可叠加）。\n【逆转器】切换弹膛内当前子弹为实弹或空包弹(实弹换空包弹，空包弹换实弹)。\n【过期药品】有40%的几率恢复2点生命值，否则将损失1点生命值。\n"
                      "【窃贼手套】偷窃一件物品并立即使用。\n【小灵通】一个神秘的声音，让你窥探未来(告诉你枪里第几颗子弹是实弹还是空包弹，不能查看当前子弹，告诉的数量随枪中子弹数变化，最多3个)。\n\n"
                      "策略说明：\n向自己开枪，如果枪中为空包弹，对手跳过下一回合。\n向对手开枪，如果枪中为实弹，对手损失相应伤害的生命值。\n"
                      "霰弹枪每轮重新装弹，会分发一定数量的道具。\n在自己回合内，可以自由使用自己持有的任意数量道具。\n各局之间，道具不继承，先手顺序将会继承。\n霰弹枪会重复装弹，直到一方血量为0。");

    // 设置文本交互标志为 Qt::NoTextInteraction
    textEdit->setTextInteractionFlags(Qt::NoTextInteraction);

    layout->addWidget(textEdit);

    dialog.setFixedSize(660, 560); // 设置对话框的固定大小

    dialog.exec();
}

void Startup::change_btn_enable()
{
    // 两种寻找指针方式
    for (int i = 1; i <= 9; i++) {
        QPushButton *button = ui->groupBox_3->findChild<QPushButton*>(QString("pushButton_%1").arg(i));
            button->setEnabled(false);
    }
    QPushButton *buttons[] = {ui->pushButton_10, ui->pushButton_11};
    for (QPushButton *button : buttons) {
        button->setEnabled(false);
    }

    QString btnText[9]={"放大镜","香烟","手铐","护木","手锯","逆转器","过期药品","窃贼手套","小灵通"};
    if (Model::getInstance().whos_turn==1&&!Model::getInstance().jump_next[1])
    {
        for (int i = 1; i <= 9; i++) {
            QPushButton *button = ui->groupBox_3->findChild<QPushButton*>(QString("pushButton_%1").arg(i));
            if (Model::getInstance().player_items[i-1]>0) {
                button->setEnabled(true);
                button->setText(btnText[i-1]+QString(" %1").arg(Model::getInstance().player_items[i-1]));
            }
            else
                button->setText(btnText[i-1]);
        }
        if (Model::getInstance().gun_status.size()<=1)
            ui->pushButton_9->setEnabled(false);
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

void Startup::update_shown_text(QString rival_ation,QString text_log_addition)
{
    QString text=QString("<center>第%1关<br/>第%2局</center>").arg(Model::getInstance().level).arg(Model::getInstance().round);
    ui->textEdit->setText(text);

    // 创建表格内容
    QStringList cells = {QString("放大镜 : %1").arg(Model::getInstance().rival_items[0]), QString("香烟 : %1").arg(Model::getInstance().rival_items[1]), QString("手铐 : %1").arg(Model::getInstance().rival_items[2]),
                         QString("护木 : %1").arg(Model::getInstance().rival_items[3]), QString("手锯 : %1").arg(Model::getInstance().rival_items[4]), QString("逆转器 : %1").arg(Model::getInstance().rival_items[5]),
                         QString("过期药品 : %1").arg(Model::getInstance().rival_items[6]), QString("窃贼手套 : %1").arg(Model::getInstance().rival_items[7]), QString("小灵通 : %1").arg(Model::getInstance().rival_items[8])};
    // 生成表格 HTML 字符串
    QString html = createTable(cells, rival_ation);
    ui->textEdit_2->setText(html);

    int count_true = 0,count_false=0;
    for (int element : Model::getInstance().gun_status) {
        if (element > 0)
            count_true++;
        if (element == 0)
            count_false++;
    }
    QString text_3=QString("<center>对手血量：%1<br/>玩家血量：%2<br/>满血血量：%3<br/>霰弹枪状态：<br/>剩余：<br/>%4发实弹<br/>%5发空包弹</center>").
                     arg(Model::getInstance().blood[0]).arg(Model::getInstance().blood[1]).arg(std::min(5,2+Model::getInstance().level/2)).arg(count_true).arg(count_false);
    if (use_swap)
        text_3=QString("<center>对手血量：%1<br/>玩家血量：%2<br/>满血血量：%3<br/>霰弹枪状态：<br/>剩余：<br/>%4发子弹</center>").
                 arg(Model::getInstance().blood[0]).arg(Model::getInstance().blood[1]).arg(std::min(5,2+Model::getInstance().level/2)).arg(Model::getInstance().gun_status.size());
    ui->textEdit_3->setText(text_3);

    // 插入红色文本
    ui->textEdit_4->setText(text_log+QString("<font color='red'>%1</font>").arg(text_log_addition));
    // 更新文本
    text_log += text_log_addition;
    // 移动光标到最后一行
    QTextCursor cursor = ui->textEdit_4->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_4->setTextCursor(cursor);
}

void Startup::set_level_hint()
{
    switch (Model::getInstance().level) {
    case 1:
        update_shown_text("","<br/>欢迎！本游戏改编自steam游戏《Buckshot Roulette》，你的对手是一名来自深渊的恶魔！第一关只开放了开枪按钮，如果对自己开空枪，可以获得额外的开枪回合。"
                              "如果对对手射击实弹，可以削减对手的血量。一方血量为0，本局结束。更多关于如何操作，请查看“帮助”。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 2:
        update_shown_text("","<br/>从本关开始，你将获得前五个道具中的若干个。注意每局间道具不继承，所以请尽量使用道具，避免浪费。关于道具的详细功能，请查看“帮助”。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 3:
        update_shown_text("","<br/>警告！恶魔对手变聪明了，他不再胡乱开枪，而是会预测下一发为实弹的概率。他不会使用道具，局势仍然对你有利。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 4:
        update_shown_text("","<br/>随着关数的增加，每关局数、双方血量上限、每轮子弹数量、每轮道具数量会逐渐增加，可操作性和策略性会逐渐加强。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 5:
        update_shown_text("","<br/>从本关开始，开放了全部道具的获取。关于额外道具的详细功能，请查看“帮助”。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 6:
        update_shown_text("","<br/>警告！恶魔对手变得更聪明了，他短暂获得了预知子弹的能力，这意味着你每次将枪交到他的手里，你必然会损失1点血量。"
                              "但他还不会使用道具，充分利用道具来击败他吧！<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 7:
        update_shown_text("","<br/>如果你在关卡中失败，你不会从头重新开始，而是回溯时间到三关前。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 8:
        update_shown_text("","<br/>现在开始，每关局数、双方血量上限、每轮子弹数量、每轮道具数量不再增加，请充分享受完整的游戏。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 9:
        update_shown_text("","<br/>警告！恶魔对手的智商已经接近人类水平，他失去了预知能力，但学会了使用道具。但他仍然偶有失误，会记错子弹状态，做出愚蠢的决策。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 13:
        update_shown_text("","<br/>警告！恶魔对手的智商已经升级到人类的最高水平，他会充分利用道具和自己获得的每一条信息，做出最符合逻辑的动作。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 17:
        update_shown_text("","<br/>警告！恶魔对手已经进入神之领域，他的动作优雅而致命，并且绝不会失误。和他对决是绝对愚蠢的行为，你只能祈求于逆天的气运。这是一场不公平的豪赌！<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 20:
        update_shown_text("","<br/>恭喜！神已被你击败，触发隐藏彩蛋，作者的QQ号：1163429473。接下来是无尽模式，你将和接近人类最高水平的对手公平对决。<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 21:
        update_shown_text("","<br/>为了避免玩家输了之后回到17-20关渡劫，作者设置了无尽模式掉段最多就掉到20关，是不是很贴心? ^_^<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    case 22:
        update_shown_text("","<br/>接下来将不再有作者的话，请以更高的关卡数为目标努力吧！<br/>");
        QApplication::processEvents();
        QThread::msleep(2400);
        break;
    default:
        break;
    }
}

void Startup::end_level()
{
    int flag=Model::getInstance().determined_winner();
    change_btn_enable();
    if (flag==0)
        return;

    QApplication::processEvents();
    QThread::msleep(1600);
    int count_true = 0,count_false=0;
    for (int element : Model::getInstance().gun_status) {
        if (element > 0)
            count_true++;
        if (element == 0)
            count_false++;
    }
    switch (flag) {
    case -2:
        update_shown_text("",QString("你死了，被踢回了低级关卡，本关%1局%2胜<br/>").arg(std::min(Model::getInstance().level/5*2+3,5)).arg(std::min(Model::getInstance().level/5*2+3,5)/2+1));
        break;
    case -1:
        update_shown_text("",QString("你输掉一局，当前%1局%2胜<br/>").arg(Model::getInstance().round-1).arg(Model::getInstance().winning_round));
        break;
    case 1:
        update_shown_text("",QString("你赢下一局，当前%1局%2胜<br/>").arg(Model::getInstance().round-1).arg(Model::getInstance().winning_round));
        break;
    case 2:
        update_shown_text("",QString("你过关！进入下一关卡，本关%1局%2胜<br/>").arg(std::min(Model::getInstance().level/5*2+3,5)).arg(std::min(Model::getInstance().level/5*2+3,5)/2+1));
        break;
    default:
        break;
    }
    QApplication::processEvents();
    QThread::msleep(800);
    update_shown_text("",QString("<center>第%1关<br/>第%2局</center>").arg(Model::getInstance().level).arg(Model::getInstance().round));
    if (flag==-2||flag==2)
    {
        QApplication::processEvents();
        QThread::msleep(800);
        set_level_hint();
    }
    update_shown_text("",QString("霰弹枪装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
}

void Startup::on_pushButton_1_clicked()
{
    Model::getInstance().use_item(1,0);
    use_swap=false;
    update_shown_text("",QString("你查看了当前子弹，它是%1<br/>").arg(Model::getInstance().gun_status.head()==0?"空包弹":"实弹"));
    change_btn_enable();
}


void Startup::on_pushButton_2_clicked()
{
    Model::getInstance().use_item(1,1);
    update_shown_text("",QString("你休息了片刻，恢复一点生命值，当前生命值%1点<br/>").arg(Model::getInstance().blood[1]));
    change_btn_enable();
}


void Startup::on_pushButton_3_clicked()
{
    Model::getInstance().use_item(1,2);
    update_shown_text("","你铐住了对方，下回合对方跳过<br/>");
    change_btn_enable();
}


void Startup::on_pushButton_4_clicked()
{
    int gun_size=Model::getInstance().gun_status.size();
    Model::getInstance().use_item(1,3);
    update_shown_text("",QString("你退出了当前子弹，枪内还有%1发子弹<br/>").arg(Model::getInstance().gun_status.size()));
    change_btn_enable();
    if (gun_size==1)
    {
        QApplication::processEvents();
        QThread::msleep(800);
        int count_true = 0,count_false=0;
        for (int element : Model::getInstance().gun_status) {
            if (element > 0)
                count_true++;
            if (element == 0)
                count_false++;
        }
        update_shown_text("",QString("弹匣已空，重新装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
    }
}


void Startup::on_pushButton_5_clicked()
{
    Model::getInstance().use_item(1,4);
    update_shown_text("","你锯短了枪管，现在霰弹枪威力翻倍<br/>");
    change_btn_enable();
}


void Startup::on_pushButton_6_clicked()
{
    Model::getInstance().use_item(1,5);
    use_swap=!use_swap;
    update_shown_text("","你呼叫主持人，切换了枪中当前子弹<br/>");
    change_btn_enable();
}


void Startup::on_pushButton_7_clicked()
{
    int origin_blood = Model::getInstance().blood[1];
    Model::getInstance().use_item(1,6);
    if (origin_blood>Model::getInstance().blood[1])
        update_shown_text("",QString("你药物中毒了，损失一点生命值，当前生命值%1点<br/>").arg(Model::getInstance().blood[1]));
    else
        update_shown_text("",QString("你感觉精神抖擞，恢复两点生命值，当前生命值%1点<br/>").arg(Model::getInstance().blood[1]));
    change_btn_enable();
    end_level();
}


void Startup::on_pushButton_8_clicked()
{
    Tool tool(this);
    connect(&tool, &Tool::closed, [this](int exitCode) {
        if (exitCode != 0)  {
            QString btnText[9]={"放大镜","香烟","手铐","护木","手锯","逆转器","过期药品","窃贼手套","小灵通"};
            Model::getInstance().use_item(1,7,exitCode-1);
            update_shown_text("",QString("你趁对手不注意，偷走了对方的%1，并立即使用<br/>").arg(btnText[exitCode-1]));
            change_btn_enable();

            exit_code=exitCode;
        }
    });
    tool.exec();
    if (exit_code != 0)
    {
        QApplication::processEvents();
        QThread::msleep(800);

        // 构建槽函数名称
        QString slotName = "on_pushButton_" + QString::number(exit_code) + "_clicked()"; // 包括括号

        exit_code=0;

        // 获取元对象
        const QMetaObject *metaObj = metaObject();

        // 查找对应的槽函数
        int methodIndex = metaObj->indexOfSlot(slotName.toLatin1().data());
        if (methodIndex != -1) {
            QMetaMethod method = metaObj->method(methodIndex);
            method.invoke(this);
        } else {
            qDebug() << "Slot not found:" << slotName;
        }
    }
}


void Startup::on_pushButton_9_clicked()
{
    Model::getInstance().use_item(1,8);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::vector<int> values(Model::getInstance().gun_status.size());
    for (int i = 0; i < Model::getInstance().gun_status.size(); ++i) {
        values[i] = i + 1;
    }

    // 打乱序列
    std::shuffle(values.begin(), values.end(), generator);
    std::vector<int> random_numbers(values.begin(), values.begin() + (Model::getInstance().gun_status.size() + 3) / 4);
    std::sort(random_numbers.begin(), random_numbers.end());

    QString str="你呼叫了场外，场外告诉你";
    for (int random_number : random_numbers)
    {
        str+=QString("，第%1发子弹是%2").arg(random_number).arg(Model::getInstance().gun_status[random_number-1]==0?"空包弹":"实弹");
    }
    str+="<br/>";
    update_shown_text("",str);
    change_btn_enable();
}


void Startup::on_pushButton_10_clicked()
{
    use_swap=false;
    int gun_size=Model::getInstance().gun_status.size();
    int bullet=Model::getInstance().gun_status.head();
    Model::getInstance().shoot(1,1);
    if (bullet==0)
        update_shown_text("","你向自己开枪，枪里是空包弹，对手跳过下个回合<br/>");
    else if (bullet==1)
        update_shown_text("",QString("你向自己开枪，枪里是实弹，你损失一点血量，你当前血量是%1<br/>").arg(Model::getInstance().blood[1]));
    else if (bullet==2)
        update_shown_text("",QString("你向自己开枪，枪里是强化过的实弹，你损失两点血量，你当前血量是%1<br/>").arg(Model::getInstance().blood[1]));
    change_btn_enable();
    if (gun_size==1)
    {
        QApplication::processEvents();
        QThread::msleep(800);
        int count_true = 0,count_false=0;
        for (int element : Model::getInstance().gun_status) {
            if (element > 0)
                count_true++;
            if (element == 0)
                count_false++;
        }
        update_shown_text("",QString("弹匣已空，重新装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
    }
    end_level();
    if (Model::getInstance().whos_turn==0)
    {
        get_AI_move();
    }
}


void Startup::on_pushButton_11_clicked()
{
    use_swap=false;
    int gun_size=Model::getInstance().gun_status.size();
    int bullet=Model::getInstance().gun_status.head();
    Model::getInstance().shoot(1,0);
    if (bullet==0)
        update_shown_text("","你向对手开枪，枪里是空包弹<br/>");
    else if (bullet==1)
        update_shown_text("",QString("你向对手开枪，枪里是实弹，对手损失一点血量，对手当前血量是%1<br/>").arg(Model::getInstance().blood[0]));
    else if (bullet==2)
        update_shown_text("",QString("你向对手开枪，枪里是强化过的实弹，对手损失两点血量，对手当前血量是%1<br/>").arg(Model::getInstance().blood[0]));
    change_btn_enable();
    if (gun_size==1)
    {
        QApplication::processEvents();
        QThread::msleep(800);
        int count_true = 0,count_false=0;
        for (int element : Model::getInstance().gun_status) {
            if (element > 0)
                count_true++;
            if (element == 0)
                count_false++;
        }
        update_shown_text("",QString("弹匣已空，重新装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
    }
    end_level();
    if (Model::getInstance().whos_turn==0)
    {
        get_AI_move();
    }
}

void Startup::get_AI_move()
{
    QApplication::processEvents();
    QThread::msleep(2200);
    int move_index=AI::getInstance().get_action();

choice_switch:
    switch (move_index) {
    case 1:
        Model::getInstance().use_item(0,0);
        use_swap=false;
        update_shown_text("对手使用了放大镜","对手查看了当前子弹<br/>");
        break;
    case 2:
        Model::getInstance().use_item(0,1);
        update_shown_text("对手使用了香烟",QString("对手休息了片刻，恢复一点生命值，当前生命值%1点<br/>").arg(Model::getInstance().blood[0]));
        break;
    case 3:
        Model::getInstance().use_item(0,2);
        update_shown_text("对手使用了手铐","对手铐住了你，下回合你跳过<br/>");
        break;
    case 4:
    {
        int gun_size=Model::getInstance().gun_status.size();
        Model::getInstance().use_item(0,3);
        update_shown_text("对手使用了护木",QString("对手退出了当前子弹，枪内还有%1发子弹<br/>").arg(Model::getInstance().gun_status.size()));
        if (gun_size==1)
        {
            QApplication::processEvents();
            QThread::msleep(800);
            int count_true = 0,count_false=0;
            for (int element : Model::getInstance().gun_status) {
                if (element > 0)
                    count_true++;
                if (element == 0)
                    count_false++;
            }
            update_shown_text("",QString("弹匣已空，重新装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
        }
    }
        break;
    case 5:
        Model::getInstance().use_item(0,4);
        update_shown_text("对手使用了手锯","对手锯短了枪管，现在霰弹枪威力翻倍<br/>");
        break;
    case 6:
        Model::getInstance().use_item(0,5);
        use_swap=!use_swap;
        update_shown_text("对手使用了逆转器","对手呼叫主持人，切换了枪中当前子弹<br/>");
        break;
    case 7:
    {
        int origin_blood = Model::getInstance().blood[0];
        Model::getInstance().use_item(0,6);
        if (origin_blood>Model::getInstance().blood[0])
            update_shown_text("对手使用了过期药品",QString("对手药物中毒了，损失一点生命值，当前生命值%1点<br/>").arg(Model::getInstance().blood[0]));
        else
            update_shown_text("对手使用了过期药品",QString("对手感觉精神抖擞，恢复两点生命值，当前生命值%1点<br/>").arg(Model::getInstance().blood[0]));
        change_btn_enable();
        end_level();
    }
        break;
    case 8:
    {
        Model::getInstance().use_item(0,7,AI::getInstance().want_steal-1);
        QString btnText[9]={"放大镜","香烟","手铐","护木","手锯","逆转器","过期药品","窃贼手套","小灵通"};
        update_shown_text("对手使用了窃贼手套",QString("对手趁你不注意，偷走了你的%1，并立即使用<br/>").arg(btnText[AI::getInstance().want_steal-1]));
        if (AI::getInstance().want_steal!=8)
        {
            move_index=AI::getInstance().want_steal;
            goto choice_switch;
        }
    }
        break;
    case 9:
    {
        Model::getInstance().use_item(0,8);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::vector<int> values(Model::getInstance().gun_status.size());
        for (int i = 0; i < Model::getInstance().gun_status.size(); ++i) {
            values[i] = i + 1;
        }

        // 打乱序列
        std::shuffle(values.begin(), values.end(), generator);
        std::vector<int> random_numbers(values.begin(), values.begin() + (Model::getInstance().gun_status.size() + 3) / 4);
        std::sort(random_numbers.begin(), random_numbers.end());

        for (int random_number : random_numbers)
            AI::getInstance().bullet_known(random_number,Model::getInstance().gun_status[random_number]);

        update_shown_text("对手使用了小灵通","对手呼叫了场外，场外告诉了他某些子弹的位置");
    }
        break;
    case 10:
    {
        use_swap=false;
        int gun_size=Model::getInstance().gun_status.size();
        int bullet=Model::getInstance().gun_status.head();
        Model::getInstance().shoot(0,0);
        if (bullet==0)
            update_shown_text("对手向自己开枪","对手向自己开枪，枪里是空包弹，你跳过下个回合<br/>");
        else if (bullet==1)
            update_shown_text("对手向自己开枪",QString("对手向自己开枪，枪里是实弹，对手损失一点血量，对手当前血量是%1<br/>").arg(Model::getInstance().blood[0]));
        else if (bullet==2)
            update_shown_text("对手向自己开枪",QString("对手向自己开枪，枪里是强化过的实弹，对手损失两点血量，对手当前血量是%1<br/>").arg(Model::getInstance().blood[0]));
        change_btn_enable();
        if (gun_size==1)
        {
            QApplication::processEvents();
            QThread::msleep(800);
            int count_true = 0,count_false=0;
            for (int element : Model::getInstance().gun_status) {
                if (element > 0)
                    count_true++;
                if (element == 0)
                    count_false++;
            }
            update_shown_text("",QString("弹匣已空，重新装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
        }
        end_level();
    }
        break;
    case 11:
    {
        use_swap=false;
        int gun_size=Model::getInstance().gun_status.size();
        int bullet=Model::getInstance().gun_status.head();
        Model::getInstance().shoot(0,1);
        if (bullet==0)
            update_shown_text("对手向你开枪","对手向你开枪，枪里是空包弹<br/>");
        else if (bullet==1)
            update_shown_text("对手向你开枪",QString("对手向你开枪，枪里是实弹，你损失一点血量，你当前血量是%1<br/>").arg(Model::getInstance().blood[1]));
        else if (bullet==2)
            update_shown_text("对手向你开枪",QString("对手向你开枪，枪里是强化过的实弹，你损失两点血量，你当前血量是%1<br/>").arg(Model::getInstance().blood[1]));
        change_btn_enable();
        if (gun_size==1)
        {
            QApplication::processEvents();
            QThread::msleep(800);
            int count_true = 0,count_false=0;
            for (int element : Model::getInstance().gun_status) {
                if (element > 0)
                    count_true++;
                if (element == 0)
                    count_false++;
            }
            update_shown_text("",QString("弹匣已空，重新装弹，当前%1发实弹，%2发空包弹<br/>").arg(count_true).arg(count_false));
        }
        end_level();
    }
        break;
    default:
        break;
    }
    change_btn_enable();
    if (Model::getInstance().whos_turn==0)
    {
        get_AI_move();
    }
}

