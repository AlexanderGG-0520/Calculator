#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>
#include <QPushButton>
#include <QComboBox>
#include <QJSEngine>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->button_2nd, &QPushButton::clicked, this, &MainWindow::on_button_2nd_clicked);
    update2ndButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_factorial_clicked()
{
    bool ok;
    int n = ui->display->text().toInt(&ok);
    if (!ok || n < 0) {
        QMessageBox::warning(this, "エラー", "正の整数を入力してください。");
        return;
    }
    int result = 1;
    for (int i = 1; i <= n; ++i) result *= i;
    ui->display->setText(QString::number(result));
}

void MainWindow::on_button_inverse_clicked()
{
    bool ok;
    double n = ui->display->text().toDouble(&ok);
    if (!ok || n == 0.0) {
        QMessageBox::warning(this, "エラー", "0以外の数値を入力してください。");
        return;
    }
    double result = 1.0 / n;
    ui->display->setText(QString::number(result));
}

void MainWindow::on_button_backspace_clicked()
{
    QString text = ui->display->text();
    if (!text.isEmpty() && text != "0") {
        text.chop(1);
        if (text.isEmpty()) text = "0";
        ui->display->setText(text);
    }
}

void MainWindow::on_button_2nd_clicked()
{
    is2nd = !is2nd;
    update2ndButtons();
    // 三角関数ドロップダウンの表示も切り替え
    QComboBox* trig = ui->combo_trig;
    for (int i = 1; i < trig->count(); ++i) { // 0は2ndなので除外
        QString text = trig->itemText(i);
        if (is2nd) {
            if (text == "sin") trig->setItemText(i, "sin⁻¹");
            else if (text == "cos") trig->setItemText(i, "cos⁻¹");
            else if (text == "tan") trig->setItemText(i, "tan⁻¹");
            else if (text == "sec") trig->setItemText(i, "sec⁻¹");
            else if (text == "csc") trig->setItemText(i, "csc⁻¹");
            else if (text == "cot") trig->setItemText(i, "cot⁻¹");
            // hypはそのまま
        } else {
            if (text.contains("sin")) trig->setItemText(i, "sin");
            else if (text.contains("cos")) trig->setItemText(i, "cos");
            else if (text.contains("tan")) trig->setItemText(i, "tan");
            else if (text.contains("sec")) trig->setItemText(i, "sec");
            else if (text.contains("csc")) trig->setItemText(i, "csc");
            else if (text.contains("cot")) trig->setItemText(i, "cot");
            // hypはそのまま
        }
    }
    // 関数ドロップダウンの範囲外アクセス防止
    QComboBox* func = ui->combo_func;
    int idx = func->currentIndex();
    if (idx < 0 || idx >= func->count()) {
        func->setCurrentIndex(0);
    }
    // combo_funcの内容に応じた分岐は不要なので、ここで終了
}

void MainWindow::update2ndButtons()
{
    // 2nd列のボタン取得
    QPushButton* btn_pow2 = ui->button_pow2;
    QPushButton* btn_sqrt = ui->button_sqrt;
    QPushButton* btn_powxy = ui->button_powxy;
    QPushButton* btn_pow10 = ui->button_pow10;
    QPushButton* btn_log = ui->button_log;
    QPushButton* btn_exp = ui->button_exp;

    if (is2nd) {
        btn_pow2->setText("x³");
        btn_sqrt->setText("∛x");
        btn_powxy->setText("ʸ√x");
        btn_pow10->setText("2ˣ");
        btn_log->setText("logʸx");
        btn_exp->setText("eˣ");
    } else {
        btn_pow2->setText("x²");
        btn_sqrt->setText("√x");
        btn_powxy->setText("xʸ");
        btn_pow10->setText("10ˣ");
        btn_log->setText("log");
        btn_exp->setText("exp");
    }
    // 三角関数ドロップダウンの範囲外アクセス防止
    QComboBox* func = ui->combo_trig;
    int idx = func->currentIndex();
    if (idx < 0 || idx >= func->count()) {
        func->setCurrentIndex(0);
    }
    // combo_funcの内容に応じた分岐は不要なので、ここで終了
}

void MainWindow::on_button_0_clicked() { appendToDisplay("0"); }
void MainWindow::on_button_1_clicked() { appendToDisplay("1"); }
void MainWindow::on_button_2_clicked() { appendToDisplay("2"); }
void MainWindow::on_button_3_clicked() { appendToDisplay("3"); }
void MainWindow::on_button_4_clicked() { appendToDisplay("4"); }
void MainWindow::on_button_5_clicked() { appendToDisplay("5"); }
void MainWindow::on_button_6_clicked() { appendToDisplay("6"); }
void MainWindow::on_button_7_clicked() { appendToDisplay("7"); }
void MainWindow::on_button_8_clicked() { appendToDisplay("8"); }
void MainWindow::on_button_9_clicked() { appendToDisplay("9"); }
void MainWindow::on_button_dot_clicked() { appendToDisplay("."); }
void MainWindow::on_button_plus_clicked() { appendToDisplay("+"); }
void MainWindow::on_button_minus_clicked() { appendToDisplay("-"); }
void MainWindow::on_button_multiply_clicked() { appendToDisplay("*"); }
void MainWindow::on_button_divide_clicked() { appendToDisplay("/"); }
void MainWindow::on_button_sign_clicked() {
    QString current = ui->display->text();
    if (current.startsWith("-"))
        current.remove(0, 1);
    else if (current != "0")
        current = "-" + current;
    ui->display->setText(current);
}

void MainWindow::appendToDisplay(const QString& text) {
    QString current = ui->display->text();
    if (current == "0") current = "";
    ui->display->setText(current + text);
}

void MainWindow::on_button_clear_clicked() {
    ui->display->setText("0");
}

void MainWindow::on_button_pow2_clicked() {
    bool ok;
    double n = ui->display->text().toDouble(&ok);
    if (ok) {
        double result = n * n;
        ui->display->setText(QString::number(result));
    }
}

void MainWindow::on_button_equal_clicked() {
    QString expr = ui->display->text();
    expr.replace("×", "*");
    expr.replace("÷", "/");
    expr.replace(QRegularExpression("\\bsin\\b"), "Math.sin");
    expr.replace(QRegularExpression("\\bcos\\b"), "Math.cos");
    expr.replace(QRegularExpression("\\btan\\b"), "Math.tan");
    expr.replace(QRegularExpression("\\blog\\b"), "Math.log10");
    expr.replace(QRegularExpression("\\bln\\b"), "Math.log");
    expr.replace(QRegularExpression("\\bexp\\b"), "Math.exp");
    expr.replace(QRegularExpression("\\bsqrt\\b"), "Math.sqrt");
    expr.replace(QRegularExpression("\\babs\\b"), "Math.abs");
    expr.replace(QRegularExpression("\\bpow\\b"), "Math.pow");
    expr.replace(QRegularExpression("(\\d+)\\s*\\^\\s*(\\d+)"), "Math.pow($1,$2)");
    QJSEngine engine;
    QJSValue result = engine.evaluate(expr);
    if (!result.isError() && result.isNumber() && result.toNumber() == result.toNumber()) {
        double val = result.toNumber();
        if (std::isinf(val)) {
            // 0除算などは「不定」と表示
            if (expr.contains(QRegularExpression("/0(?![\\d.])"))) {
                ui->display->setText(QStringLiteral("不定"));
            } else {
                ui->display->setText(QString::fromUtf8("\u221E")); // ∞
            }
        } else {
            ui->display->setText(result.toString());
        }
    } else {
        ui->display->setText("Error");
    }
}
