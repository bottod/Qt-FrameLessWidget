#include "framelesswidget.h"
#include <QDesktopWidget>
#include <QApplication>

FrameLessWidget::FrameLessWidget(QWidget *parent)
    : QWidget(parent)
{
    DefaultSet();                               //设置默认数据
    InitWindowLayout();                         //初始化窗口布局
    BindAction();                               //绑定动作到控件
}

FrameLessWidget::~FrameLessWidget()
{
}

//设置标题栏上的按钮类型 ，可自定义标题栏上的按钮
//需要其他按钮可以自己添加
void FrameLessWidget::SetButtonType(ButtonType buttonType)
{
    m_ButtonType = buttonType;

    //根据按钮类型设置按钮可见性
    switch(buttonType)
    {
        case MIN_BUTTON:
            {
                m_pButtonRestore->setVisible(false);
                m_pButtonMax->setVisible(false);
             }
            break;
        case MIN_MAX_BUTTON:
            {
                m_pButtonRestore->setVisible(false);
            }
            break;
        case ONLY_CLOSE_BUTTON:
            {
                m_pButtonMax->setVisible(false);
                m_pButtonMin->setVisible(false);
                m_pButtonRestore->setVisible(false);
            }
            break;
        default:
            break;
    }
}

//初始化窗口控件以及布局
void  FrameLessWidget::InitWindowLayout()
{
    //整体背景色 当设置了spacing时显示出的底部的颜色 现在没设置spacing
    this->setStyleSheet("background-color:rgb(3, 204, 255);");

    //标题栏窗口 限制高度TitleHeight 背景色
    m_pTitleWidget->setFixedHeight(TitleHeight);
    m_pTitleWidget->setStyleSheet("background-color:rgb(153, 204, 255);");

    //内容窗口 背景色
    m_pBottomWidget->setStyleSheet("background-color:rgb(153, 255, 102);");

    //设置按钮大小
    m_pButtonMin->setFixedSize(QSize(41, 40));
    m_pButtonRestore->setFixedSize(QSize(41, 40));
    m_pButtonMax->setFixedSize(QSize(41, 40));
    m_pButtonClose->setFixedSize(QSize(41, 40));

    //标题栏布局管理器
    m_pHTopLayout->addWidget(m_pTitleLabel);
    m_pHTopLayout->addSpacing(0);
    m_pHTopLayout->addWidget(m_pButtonMin);
    m_pHTopLayout->addWidget(m_pButtonRestore);
    m_pHTopLayout->addWidget(m_pButtonMax);
    m_pHTopLayout->addWidget(m_pButtonClose);

    //标题栏设置布局
    m_pTitleWidget->setLayout(m_pHTopLayout);

    //添加标题栏窗口和内容窗口进竖直布局管理器
    m_pVLayout->addWidget(m_pTitleWidget);
    m_pVLayout->addWidget(m_pBottomWidget);

    //设置与窗体边距为0 使其看着是一个窗口而不是内嵌子窗口
    m_pVLayout->setSpacing(0);
    m_pVLayout->setMargin(0);

    //设置布局
    this->setLayout(m_pVLayout);
}

//设置默认数据 初始化变量 位置最好别变(看懂依赖的情况下再变)
void FrameLessWidget::DefaultSet()
{
    m_pTitleWidget = new QWidget();                                                 //标题栏窗口
    m_pBottomWidget = new QWidget();                                                //标题栏下面内容窗口

    //标题栏label和按钮button 并且设置名字用于在css里设置样式
    m_pButtonMin = new QPushButton();m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestore = new QPushButton();m_pButtonRestore->setObjectName("ButtonRestore");
    m_pButtonMax = new QPushButton();m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose = new QPushButton();m_pButtonClose->setObjectName("ButtonClose");
    m_pTitleLabel = new QLabel("Title");

    //标题栏水平布局 内容窗口什么都没加，可以自己添加
    m_pHTopLayout = new QHBoxLayout();
    m_pVLayout = new QVBoxLayout();                                                 //标题栏和内容窗口的竖直布局管理器

    TitleHeight = 60;                                                               //标题栏窗口高度
    this->resize(900,500);                                                          //设置窗口默认大小
    SetButtonType(MIN_MAX_BUTTON);                                                  //设置窗口按钮类型
    m_Moving = false;                                                               //鼠标按下状态
    is_MaxType = false;                                                             //当前窗口是否为最大状态
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);         //取消边框标题栏 (自绘)
}

//最小化按钮按下
void FrameLessWidget::OnButtonMinClicked()
{
    showMinimized();
}

//最大化按钮按下
void FrameLessWidget::onButtonMaxClicked()
{
    m_pButtonRestore->setVisible(true);
    m_pButtonMax->setVisible(false);
    is_MaxType = true;

    this->SaveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();       //获取可用桌面大小 不适用与多个屏幕
    setGeometry(desktopRect);
}

//恢复按钮按下
void FrameLessWidget::onButtonRestoreClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    is_MaxType = false;

    this->setGeometry(QRect(m_restorePos, m_restoreSize));
}

//关闭按钮按下
void FrameLessWidget::onButtonCloseClicked()
{
    this->close();
}

//保存变大前的窗口信息
void FrameLessWidget::SaveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

//重写鼠标按下事件
void FrameLessWidget::mousePressEvent(QMouseEvent *event)
{
    m_Moving = true;
    //记录下鼠标相对于窗口的位置
    //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
    //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
    m_MovePosition = event->globalPos() - pos();
    return QWidget::mousePressEvent(event);
}

//重写鼠标移动事件
void FrameLessWidget::mouseMoveEvent(QMouseEvent *event)
{
    //(event->buttons() && Qt::LeftButton)按下是左键
    //鼠标移动事件需要移动窗口，窗口移动到哪里呢？就是要获取鼠标移动中，窗口在整个屏幕的坐标，然后move到这个坐标，怎么获取坐标？
    //通过事件event->globalPos()知道鼠标坐标，鼠标坐标减去鼠标相对于窗口位置，就是窗口在整个屏幕的坐标
    //如果不是最大化窗口，并且按下了，按的是鼠标左键 并且窗口相对屏幕的位置发生了变化 即经过了大于0个像素点 并且鼠标位置必须在标题栏里
    if (!is_MaxType&&m_Moving && (event->buttons() && Qt::LeftButton)
        && (event->globalPos()-m_MovePosition).manhattanLength() > 0
        && m_MovePosition.y() < TitleHeight)
    {
        move(event->globalPos()-m_MovePosition);
    }
    return QWidget::mouseMoveEvent(event);
}

//重写鼠标释放事件
void FrameLessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_Moving = false;
}


//重写鼠标双击事件
void FrameLessWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 只有存在最大化、还原按钮时双击才有效;
    if (m_ButtonType == MIN_MAX_BUTTON && m_MovePosition.y() < TitleHeight)
    {
        // 通过最大化按钮的状态判断当前窗口是处于最大化还是原始大小状态;
        // 或者通过单独设置变量来表示当前窗口状态;
        if (m_pButtonMax->isVisible())
        {
            onButtonMaxClicked();
        }
        else
        {
            onButtonRestoreClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}

//绑定消息到控件
void FrameLessWidget::BindAction()
{
    //信号绑定
    connect(m_pButtonMin,&QPushButton::clicked,this,&FrameLessWidget::OnButtonMinClicked);
    connect(m_pButtonMax,&QPushButton::clicked,this,&FrameLessWidget::onButtonMaxClicked);
    connect(m_pButtonRestore,&QPushButton::clicked,this,&FrameLessWidget::onButtonRestoreClicked);
    connect(m_pButtonClose,&QPushButton::clicked,this,&FrameLessWidget::onButtonCloseClicked);
}

//设置标题栏相关信息，待补充
void FrameLessWidget::SetTitle(int title_height)
{
    TitleHeight = title_height;                 //限高
    m_pTitleWidget->setFixedHeight(TitleHeight);
}
