#define MIN_A_Z_UNICODE 65
#define MAX_A_Z_UNICODE 90
#define MIN_a_z_UNICODE 97
#define MAX_a_z_UNICODE 122

#include "MainWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    //1
    label_HelpText = new QLabel("Введите текст для шифровки/расшифровки:");
    label_HelpText->setMinimumSize(30, 30);
    label_HelpText->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    //2
    lineEdit_Data = new QLineEdit();
    lineEdit_Data->setMinimumSize(500, 30);
    lineEdit_Data->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    lineEdit_Data->setValidator(new QRegExpValidator(QRegExp(tr("[a-zA-Z]+")), this));

    //3
    label_Offset = new QLabel("Смещение: ");
    label_Offset->setMinimumSize(100, 30);
    lineEdit_Offset = new QLineEdit();
    lineEdit_Offset->setMinimumSize(200, 30);
    lineEdit_Offset->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]+")), this));

    //4
    pushButton_encryption = new QPushButton("Зашифровать");
    pushButton_encryption->setMinimumSize(100, 30);
    pushButton_decryption = new QPushButton("Расшифровать");
    pushButton_decryption->setMinimumSize(100, 30);


    QHBoxLayout *layout_btn = new QHBoxLayout;
    layout_btn->addWidget(pushButton_encryption);
    layout_btn->addWidget(pushButton_decryption);

    QHBoxLayout *layout_offset = new QHBoxLayout;
    layout_offset->addWidget(label_Offset);
    layout_offset->addWidget(lineEdit_Offset);


    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addWidget(label_HelpText);//1
    layout_main->addWidget(lineEdit_Data);//2
    layout_main->addLayout(layout_offset);//3
    layout_main->addLayout(layout_btn);//4

    setLayout(layout_main);
    setWindowTitle(tr("Шифр Цезаря"));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    connect(pushButton_encryption,SIGNAL(clicked()), this, SLOT(slot_button_encryptionData()));
    connect(pushButton_decryption,SIGNAL(clicked()), this, SLOT(slot_button_decryptionData()));
}


MainWindow::~MainWindow()
{
}


void MainWindow::slot_button_encryptionData()
{
    QString data;
    int offset = 0;

    if (!getData(data, offset)) return;
    QString encryptionData = cryptionData(data, offset, cription::encryption);
    if (encryptionData.isNull()) return;
    qDebug() << encryptionData;

    if(!saveLog("log.txt", data, encryptionData, offset, cription::decryption))
    {
        QMessageBox::warning(this,
                             "Внимание",
                             "Данные не были записаны в лог файл",
                             QMessageBox::Ok);
    }
}


void MainWindow::slot_button_decryptionData()
{
    QString data;
    int offset = 0;

    if (!getData(data, offset)) return;
    QString decryptionData = cryptionData(data, offset, cription::decryption);
    if (decryptionData.isNull()) return;
    qDebug() << decryptionData;

    if(!saveLog("log.txt", data, decryptionData, offset, cription::decryption))
    {
        QMessageBox::warning(this,
                             "Внимание",
                             "Данные не были записаны в лог файл",
                             QMessageBox::Ok);
    }
}


QString MainWindow::cryptionData(QString &data, int offset, cription crip)
{
    offset = offset % 25;
    if (crip == cription::decryption)
    {
        offset = -offset;
    }

    QString criptionData;
    for (int i = 0; i < data.size(); i++)
    {
        int curentUnicode = static_cast<int>(data.at(i).unicode());
        int newUnicode = curentUnicode + offset;
        if ((curentUnicode >= MIN_a_z_UNICODE) && (curentUnicode <= MAX_a_z_UNICODE))
        {
            if (newUnicode < MIN_a_z_UNICODE)
            {
                newUnicode = MAX_a_z_UNICODE - MIN_a_z_UNICODE + newUnicode + 1;
            }
            else if (newUnicode > MAX_a_z_UNICODE)
            {
                newUnicode = MIN_a_z_UNICODE - MAX_a_z_UNICODE + newUnicode - 1;
            }
        }
        else if((curentUnicode >= MIN_A_Z_UNICODE) && (curentUnicode <= MAX_A_Z_UNICODE))
        {
            if (newUnicode < MIN_A_Z_UNICODE)
            {
                newUnicode = MAX_A_Z_UNICODE - MIN_A_Z_UNICODE + newUnicode + 1;
            }
            else if (newUnicode > MAX_A_Z_UNICODE)
            {
                newUnicode = MIN_A_Z_UNICODE - MAX_A_Z_UNICODE + newUnicode - 1;
            }
        }
        else
        {
            qDebug() << "Где-то обсчитался!";
            criptionData = nullptr;
            return criptionData;
        }

        criptionData.append(QString(QChar(newUnicode)));
    }
    return criptionData;
}


bool MainWindow::getData(QString &Data, int &offset)
{
    QString offset_str = lineEdit_Offset->text();
    if (offset_str.isEmpty())
    {
        QMessageBox::warning(this,
                             "Внимание",
                             "Не заполнено поле \"смещение\"",
                             QMessageBox::Ok);
        return false;
    }


    bool ok;
    offset = offset_str.toInt(&ok, 10);
    if (!ok)
    {
        QMessageBox::warning(this,
                             "Внимание",
                             "Не заполнено поле \"смещение\"",
                             QMessageBox::Ok);
        return false;
    }

    Data = lineEdit_Data->text();
    return true;
}


bool MainWindow::saveLog(QString file_name, QString &input_text, QString &output_text, int offset, cription crip)
{
    QFile file(file_name);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return false;
    }

    QString out_text = QString("Дата: %1\n"
                               "Время: %2\n"
                               "Входные данные: %3\n"
                               "Операция: %4\n"
                               "Смещение: %5\n"
                               "Выходные данные: %6\n\n\n")
            .arg((QDate::currentDate()).toString())
            .arg((QTime::currentTime()).toString())
            .arg(input_text)
            .arg(crip==cription::encryption?"Шифрование":"Расшифрование")
            .arg(offset)
            .arg(output_text);

    QTextStream writeStream(&file);
    writeStream << out_text;
    file.close();

    return true;
}
