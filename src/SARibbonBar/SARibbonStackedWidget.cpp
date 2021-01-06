﻿#include "SARibbonStackedWidget.h"
#include <QEventLoop>
#include <QMouseEvent>
class SARibbonStackedWidgetPrivate
{
public:
    SARibbonStackedWidget *Parent;
    QEventLoop *eventLoop;
    bool isAutoResize;
    SARibbonStackedWidgetPrivate(SARibbonStackedWidget *p)
        : Parent(p)
        , eventLoop(nullptr)
        , isAutoResize(true)
    {
    }


    void init()
    {
        //Parent->setFocusPolicy(Qt::StrongFocus);
    }
};



SARibbonStackedWidget::SARibbonStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , m_d(new SARibbonStackedWidgetPrivate(this))
{
    m_d->init();
    setNormalMode();
}


SARibbonStackedWidget::~SARibbonStackedWidget()
{
    if (m_d->eventLoop) {
        m_d->eventLoop->exit();
    }
    delete m_d;
}


void SARibbonStackedWidget::setPopupMode()
{
    setWindowFlags(Qt::Popup|Qt::FramelessWindowHint);
    setFrameShape(QFrame::Panel);
}


bool SARibbonStackedWidget::isPopupMode() const
{
    return (windowFlags()&Qt::Popup);
}


void SARibbonStackedWidget::setNormalMode()
{
    if (m_d->eventLoop) {
        m_d->eventLoop->exit();
        m_d->eventLoop = nullptr;
    }
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setFrameShape(QFrame::NoFrame);
}


bool SARibbonStackedWidget::isNormalMode() const
{
    return (!isPopupMode());
}


void SARibbonStackedWidget::exec()
{
    show();
    if (!isPopupMode()) {
        m_d->eventLoop = nullptr;
        return;
    }
    QEventLoop event;

    m_d->eventLoop = &event;
    event.exec();
    m_d->eventLoop = nullptr;
}


void SARibbonStackedWidget::setAutoResize(bool autoresize)
{
    m_d->isAutoResize = autoresize;
}


bool SARibbonStackedWidget::isAutoResize() const
{
    return (m_d->isAutoResize);
}


void SARibbonStackedWidget::hideEvent(QHideEvent *e)
{
    if (isPopupMode()) {
        if (m_d->eventLoop) {
            m_d->eventLoop->exit();
        }
    }
    setFocus();
    emit hidWindow();

    QStackedWidget::hideEvent(e);
}


void SARibbonStackedWidget::resizeEvent(QResizeEvent *event)
{
    if (m_d->isAutoResize) {
        int c = count();
        for (int i = 0; i < c; ++i)
        {
            QWidget *w = widget(i);
            if (w) {
                w->resize(event->size());
            }
        }
    }
    return (QStackedWidget::resizeEvent(event));
}
