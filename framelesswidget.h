#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QMouseEvent>

//窗口按钮类型 默认是MIN_MAX_BUTTON
enum ButtonType
{
    MIN_BUTTON = 0,			// 最小化和关闭按钮;
    MIN_MAX_BUTTON ,		// 最小化、最大化和关闭按钮;
    ONLY_CLOSE_BUTTON		// 只有关闭按钮;
};

class FrameLessWidget : public QWidget
{
    Q_OBJECT

public:
    FrameLessWidget(QWidget *parent = 0);

    void SetButtonType(ButtonType buttonType);      //设置标题栏上的按钮类型
    void InitWindowLayout();                        //初始化窗口控件以及布局
    void DefaultSet();                              //设置一些默认的数据
    void BindAction();                              //绑定动作到控件
    void SetTitle(int title_height);                //设置标题栏信息

    void SaveRestoreInfo(const QPoint point, const QSize size);         //保存变大前的窗口信息

    ~FrameLessWidget();
private slots:
    void OnButtonMinClicked();                      //最小化按钮按下
    void onButtonRestoreClicked();                  //恢复按钮按下
    void onButtonMaxClicked();                      //最大化按钮按下
    void onButtonCloseClicked();                    //关闭按钮按下
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);        //鼠标响应事件
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
private:
    int TitleHeight;                                 //标题栏窗口高度
    QWidget * m_pTitleWidget;                        //标题栏窗口widget
    QWidget * m_pBottomWidget;                       //内容窗口widget

    QLabel * m_pTitleLabel;                          //标题栏标签
    //按钮类型
    ButtonType m_ButtonType;
    QPushButton * m_pButtonMin;                     //最小化按钮
    QPushButton * m_pButtonMax;                     //最大化按钮
    QPushButton * m_pButtonRestore;                 //恢复原始大小按钮
    QPushButton * m_pButtonClose;                   //关闭按钮

    QHBoxLayout * m_pHTopLayout;                    //标题栏窗口的布局
    QVBoxLayout * m_pVLayout;                       //标题栏和内容窗口的布局 竖直布局

    bool m_Moving;                                  //鼠标按下状态
    QPoint m_MovePosition;                          //鼠标相对于窗口的位置

    QPoint m_restorePos;                            //存储窗口变大前的位置
    QSize m_restoreSize;                            //存储窗口变大前的大小
    bool is_MaxType;                                //记录当前窗口是否为最大化的 (保证窗口最大化时无法移动)

};

#endif // FRAMELESSWIDGET_H
