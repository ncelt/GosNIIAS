#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

enum cription
{
    encryption = 0,
    decryption = 1
};

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void slot_button_encryptionData(); //слот обработки клавиши "шифрование"
    void slot_button_decryptionData(); //слот обработки клавиши "расшифрование"


private:
    QLabel *label_HelpText;
    QLineEdit *lineEdit_Data;
    QLabel *label_Offset;
    QLineEdit *lineEdit_Offset;
    QPushButton *pushButton_encryption;
    QPushButton *pushButton_decryption;


    QString cryptionData(QString &data, int offset, cription crip); //зашифровка данных
    bool getData(QString &Data, int &offset);//сбор данных с формы
    bool saveLog(QString file_name, QString &input_text, QString &output_text, int offset, cription crip); //сохранение лога
};
#endif // MAINWINDOW_H
