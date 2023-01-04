#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>
#include <QSyntaxHighlighter>
#include <QtGui>

#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    connect(ui->actionNew, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::exit);
    connect(ui->actionUndo, &QAction::triggered, this, &Notepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &Notepad::redo);
    connect(ui->actionFont, &QAction::triggered, this, &Notepad::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &Notepad::setFontBold);
    connect(ui->actionUnderline, &QAction::triggered, this, &Notepad::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &Notepad::setFontItalic);
    connect(ui->actionCor, &QAction::triggered, this, &Notepad::setColour);
    connect(ui->actionEsquerda, &QAction::triggered, this, &Notepad::setEsquerda);
    connect(ui->actionDireita, &QAction::triggered, this, &Notepad::setDireita);
    connect(ui->actionCentro, &QAction::triggered, this, &Notepad::setCentro);

// Desabilitar o menu para ações não disponíveis
#if !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

Notepad::~Notepad()
{
    delete ui;
}

//Novo Documento
void Notepad::newDocument()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

//Abrir um Documento existente
void Notepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir o arquivo");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Aviso", "O arquivo não pode ser aberto: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

//Salvar documento atual
void Notepad::save()
{
    QString fileName;
    // Escolher um nome caso não tenha.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Aviso", "O arquivo não pode ser salvo: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

//Escolher onde salvar
void Notepad::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Salvar como");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Avisog", "O arquivo não pode ser salvo: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

//Sair do app
void Notepad::exit()
{
    QCoreApplication::quit();
}


//Desfazer
void Notepad::undo()
{
     ui->textEdit->undo();
}

//Refazer
void Notepad::redo()
{
    ui->textEdit->redo();
}

//Escolher fonte
void Notepad::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

//Sublinhar
void Notepad::setFontUnderline(bool underline)
{
    ui->textEdit->setFontUnderline(underline);
}

//Italico
void Notepad::setFontItalic(bool italic)
{
    ui->textEdit->setFontItalic(italic);
}

//Negrito
void Notepad::setFontBold(bool bold)
{
    bold ? ui->textEdit->setFontWeight(QFont::Bold) :
           ui->textEdit->setFontWeight(QFont::Normal);
}

//Escolher a cor
void Notepad::setColour()
{

    QColor txtColour = QColorDialog::getColor();
    ui->textEdit->setTextColor(txtColour);

}

void Notepad::setEsquerda()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}

void Notepad::setDireita()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}

void Notepad::setCentro()
{
    ui->textEdit->setAlignment(Qt::AlignCenter);
}
