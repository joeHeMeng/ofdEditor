#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"
#include "Widget/ParagraphFormatDialog.h"
#include "Widget/FontSettingDialog.h"

#include <QTextCursor>
#include <QPalette>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QDebug>
#include <QFontDialog>
#include <QColorDialog>


DocTextBlock::DocTextBlock(QWidget *parent)
    :QTextEdit(parent)
{
//    QTextCursor cursor(this->textCursor());
//    cursor.insertText(tr("testsesetstsetestes"));

//    this->setBackgroundRole(QPalette::Dark);
    this->init();   // 调用初始化函数

}

DocTextBlock::~DocTextBlock()
{

}

/**
 * @Author Chaoqun
 * @brief  设置文本内容
 * @param  QString str
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::setContent(QString str)
{
    QTextCursor cursor(this->textCursor());
    cursor.insertText(str);
}

/**
 * @Author Chaoqun
 * @brief  用来合并格式
 * @param  const QTextCharFormat &format
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::mergeFormatOnWordOrSelection(
        QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor(); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }
    cursor.mergeCharFormat(format);         // 合并光标下的 QTextCharFormat
    this->mergeCurrentCharFormat(format);   // 合并当前的 QTextCharFormat

}

/**
 * @Author Chaoqun
 * @brief  当前CharFormat样式发上改变的响应函数，
 *         当增加了一个字体控制的widget时，可以使用
 * @param  const QTextCharFormat &format
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::currentCharFormatChangedEvent(
        const QTextCharFormat &format)
{

}

/**
 * @Author Chaoqun
 * @brief  光标位置发生改变时的响应函数
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::cursorPositionChangedEvent()
{
    //    qDebug() << "Cursor Position Changed!";
}

void DocTextBlock::setFont(const QFont &font)
{
//    QTextCharFormat currentFormat =
//            this->currentCharFormat();      // 当前选择文字的样式
    QTextCursor cursor = this->textCursor();
    QTextCharFormat currentFormat = cursor.charFormat();

    currentFormat.setFont(font);            // 设置字体

//    mergeCurrentCharFormat(currentFormat);
    mergeFormatOnWordOrSelection(currentFormat);
}



/**
 * @Author Chaoqun
 * @brief  文字加粗
 * @param  void
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::textBold()
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式
    fmt.setFontWeight(currentFormat.fontWeight() != QFont::Bold ?   // 设置粗细
                          QFont::Bold : QFont::Normal);

    mergeFormatOnWordOrSelection(fmt);      // 合并格式

}

/**
 * @Author Chaoqun
 * @brief  设置下划线
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textUnderline()
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式

    fmt.setFontUnderline(currentFormat.fontUnderline()?
                             false:true);

    mergeFormatOnWordOrSelection(fmt);      // 合并格式
}

/**
 * @Author Chaoqun
 * @brief  设置斜体
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textItalic()
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式
    fmt.setFontItalic(currentFormat.fontItalic() == true?                  // 设置斜体
                          false:true);

    mergeFormatOnWordOrSelection(fmt);      // 合并格式
}

/**
 * @Author Chaoqun
 * @brief  弹出一个段落属性设置的QDialog，设置好了后，
 *          将该属性应用在选中的段落上
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textParagraph()
{
    QTextCursor cursor = this->textCursor();
    QTextBlockFormat blockFormat = cursor.blockFormat();

    ParagraphFormatDialog * para = new ParagraphFormatDialog(
                blockFormat,this);

    para->exec();
}
/**
 * @Author Chaoqun
 * @brief  弹出一个框，设置字体颜色，设置好了以后，将该属性应用到选中的文字中
 * @param  参数
 * @return 返回值
 * @date   2017/05/27
 */
void DocTextBlock::setTextColor()
{

    QColor color = QColorDialog::getColor(this->textColor(),
                                          NULL,
                                          tr("Choose a Color"),
                                          QColorDialog::ShowAlphaChannel);

    if(!color.isValid())
        return;
    QTextCharFormat charFormat;
    charFormat.setForeground(color);        // 设置颜色
    mergeCurrentCharFormat(charFormat);

}

/**
 * @Author Chaoqun
 * @brief  通过字体小窗口设置字体
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textFontDialog()
{
    bool btn_ok;    // 确认按键
    QTextCursor cursor = this->textCursor();
    QTextCharFormat currentFormat = cursor.charFormat();
//    QTextCharFormat currentFormat =
//            this->currentCharFormat();      // 当前选择文字的样式
    QFont oldFont = currentFormat.font();   // 获取之前的字体样式

    QFont newFont = QFontDialog::getFont(
                &btn_ok, oldFont,NULL,tr("Set the font")
                );    // 选择字体框

    if(btn_ok)
    {
        this->setFont(newFont);     // 设置字体
    }
    else
    {
        // 用户取消了操作，不做处理
        qDebug() << "Cancel select Font!";
    }


}

void DocTextBlock::customFontDialog()
{

    FontSettingDialog * font = new FontSettingDialog(this,0);
    font->exec();
}

/**
 * @Author Chaoqun
 * @brief  设置当前选中的段落的格式
 * @param  参数
 * @return 返回值
 * @date   2017/05/22
 */
void DocTextBlock::setTextBlockFormat(QTextBlockFormat &blockFormat)
{
    QTextCursor cursor = this->textCursor(); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }

    cursor.setBlockFormat(blockFormat);
    qDebug()<<"Change the selected QTextBlock' Format. Test";


}

/**
 * @Author Chaoqun
 * @brief  直接修改选中的文字的CharFormat
 * @param  const QTextCharFormat &format
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::setCharFormatOnWordOrSelection(
         QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor(); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }
    cursor.setCharFormat(format);         // 设置光标下的 QTextCharFormat
    this->setCurrentCharFormat(format);   // 合并当前的 QTextCharFormat
}

/**
 * @Author Chaoqun
 * @brief  显示右键菜单
 * @param  QContextMenuEvent *event
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::contextMenuEvent(QContextMenuEvent *event)
{

    this->ContextMenu = createStandardContextMenu();     // 拓展标准菜单
    this->ContextMenu->addAction(this->actionBold);      // 加粗
    this->ContextMenu->addAction(this->actionItalic);    // 斜体
    this->ContextMenu->addAction(this->actionUnderline); // 下划线
    this->ContextMenu->addAction(this->actionColor);     // 颜色
    this->ContextMenu->addSeparator();  // 分界线
    this->ContextMenu->addAction(this->actionFontSet);   // 字体设置
    this->ContextMenu->addAction(this->actionParagraph); // 段落设置
    this->ContextMenu->addAction(this->actionFontSetTest);// 字体

    // 展示菜单
    this->ContextMenu->exec(event->globalPos());

}

/**
 * @Author Chaoqun
 * @brief  焦点聚焦，显示边框
 * @param  QFocusEvent *e
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::focusInEvent(QFocusEvent *e)
{
    this->setFrameStyle(QFrame::Box);
    QTextEdit::focusInEvent(e);
}

/**
 * @Author Chaoqun
 * @brief  焦点移出，隐藏边框
 * @param  QFocusEvent *e
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::focusOutEvent(QFocusEvent *e)
{
    this->setFrameStyle(QFrame::NoFrame);
    QTextEdit::focusOutEvent(e);
}

/**
 * @Author Chaoqun
 * @brief  初始化函数
 * @param  void
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::init()
{
    this->setMinimumSize(5,5);        // 为了正确显示缩放标识

    // 关闭滑动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 设置为背景透明
    this->viewport()->setAttribute(Qt::WA_TranslucentBackground, true);
//    // 无边框
    this->setFrameStyle(QFrame::NoFrame);

    this->initFormat();         // 初始化格式



    // 连接当前charFormat改变函数
    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChangedEvent(QTextCharFormat)));

    // 连接当前光标位置改变事件
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChangedEvent()));

    this->initAcitons();    // 初始化QAction相关
}

/**
 * @Author Chaoqun
 * @brief  初始化按钮 ， QAction
 * @param  void
 * @return 返回值
 * @date   2017/05/21
 */
void DocTextBlock::initAcitons()
{
    // 粗体
    this->actionBold = new QAction(tr("Bold"),NULL);
    this->actionBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    this->actionBold->setFont(bold);

    this->connect(this->actionBold,SIGNAL(triggered()),
                  this,SLOT(textBold()));

    // 斜体
    this->actionItalic = new QAction(tr("Italic"),NULL);
    this->actionItalic->setPriority(QAction::LowPriority);
    QFont italic;
    italic.setItalic(true);
    this->actionItalic->setFont(italic);

    this->connect(this->actionItalic,SIGNAL(triggered()),
                  this,SLOT(textItalic()));

    // 下划线
    this->actionUnderline = new QAction(tr("Underline"),NULL);
    this->actionUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    this->actionUnderline->setFont(underline);

    this->connect(this->actionUnderline, SIGNAL(triggered()),
                  this,SLOT(textUnderline()));

    // 设置字体颜色
    this->actionColor = new QAction(tr("Color"),NULL);
    this->actionColor->setPriority(QAction::LowPriority);

    this->connect(this->actionColor,SIGNAL(triggered()),
                  this,SLOT(setTextColor()));

    // 字体
    this->actionFontSet = new QAction(tr("Font"),NULL);
    this->actionFontSet->setPriority(QAction::LowPriority);

    this->connect(this->actionFontSet,SIGNAL(triggered()),
                  this,SLOT(textFontDialog()));

    // 段落
    this->actionParagraph = new QAction(tr("Paragraph"),NULL);
    this->connect(this->actionParagraph,SIGNAL(triggered()),
                  this,SLOT(textParagraph()));


    // 字体窗口测试
    this->actionFontSetTest = new QAction(tr("FontDialogTest"),NULL);
    this->connect(this->actionFontSetTest, SIGNAL(triggered()),
                  this, SLOT(customFontDialog()));

    // 右键菜单
//    this->ContextMenu = createStandardContextMenu();     // 拓展标准菜单
//    this->ContextMenu->addAction(this->actionBold);      // 加粗
//    this->ContextMenu->addAction(this->actionItalic);    // 斜体
//    this->ContextMenu->addAction(this->actionUnderline); // 下划线
//    this->ContextMenu->addAction(this->actionColor);     // 颜色
//    this->ContextMenu->addSeparator();  // 分界线
//    this->ContextMenu->addAction(this->actionFontSet);   // 字体设置
//    this->ContextMenu->addAction(this->actionParagraph); // 段落设置
//    this->ContextMenu->addAction(this->actionFontSetTest);// 字体

}


/**
 * @Author Chaoqun
 * @brief  初始化文字的样式
 * @param  void
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::initFormat()
{
    QTextCursor cursor = this->textCursor();            // 获得当前光标

    QTextCharFormat charFormat = cursor.charFormat();   // 字符格式
    charFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    this->mergeCurrentCharFormat(charFormat);

}
