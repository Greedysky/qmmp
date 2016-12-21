/***************************************************************************
 *   Copyright (C) 2016 The Qt Company Ltd.                                *
 *   Copyright (C) 2016 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

//Based on QTabWidget class copyrighted by Qt Company Ltd.

#include <QStylePainter>
#include <QSettings>
#include <QApplication>
#include <QStyleOptionTabWidgetFrame>
#include <qmmpui/playlistmanager.h>
#include "qsuitabwidget.h"

QSUiTabWidget::QSUiTabWidget(QWidget *parent) : QWidget(parent)
{
    m_tabBar = new QSUiTabBar(this);
    m_tabBar->setMovable(true);
    m_tabBar->setExpanding(false);
    m_listWidget = new ListWidget(PlayListManager::instance()->selectedPlayList(), this);
    m_listWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred,
                                            QSizePolicy::TabWidget));
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding,
                              QSizePolicy::TabWidget));


    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(m_tabBar);
    m_rightCornerWidget = 0;
    m_leftCornerWidget = 0;

    m_menu = new QMenu(this);
    m_group = new QActionGroup(this);
    m_group->setExclusive(true);
    connect(m_tabBar, SIGNAL(tabMoved(int,int)), SIGNAL(tabMoved(int,int)));
    connect(m_tabBar, SIGNAL(tabMoved(int,int)), SLOT(updateActions()));
    connect(m_tabBar, SIGNAL(tabCloseRequested(int)), SIGNAL(tabCloseRequested(int)));
    connect(m_tabBar, SIGNAL(currentChanged(int)), SLOT(onCurrentChanged(int)));
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(onActionTriggered(QAction*)));
    readSettings();
}

QSUiTabBar *QSUiTabWidget::tabBar() const
{
    return m_tabBar;
}

ListWidget *QSUiTabWidget::listWidget() const
{
    return m_listWidget;
}

void QSUiTabWidget::setCornerWidget(QWidget *widget, Qt::Corner corner)
{
    if (widget && widget->parentWidget() != this)
            widget->setParent(this);

    if (corner & Qt::TopRightCorner)
    {
        if (m_rightCornerWidget)
            m_rightCornerWidget->hide();
        m_rightCornerWidget = widget;
    }
    else
    {
        if (m_leftCornerWidget)
            m_leftCornerWidget->hide();
        m_leftCornerWidget = widget;
    }
    setUpLayout();
}

void QSUiTabWidget::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    m_tabBar->setTabsClosable(settings.value("pl_tabs_closable", false).toBool());
    QFont tab_font = qApp->font(m_tabBar);
    if(!settings.value("use_system_fonts", true).toBool())
    {
        tab_font.fromString(settings.value("pl_tabs_font", tab_font.toString()).toString());
    }
    m_tabBar->setFont(tab_font);
    settings.endGroup();
    m_tabBar->updateGeometry();
    setUpLayout();
}

QMenu *QSUiTabWidget::menu() const
{
    return m_menu;
}

void QSUiTabWidget::setTabText(int index, const QString &text)
{
    QString tabText = text;
    tabText.replace("&","&&");
    m_tabBar->setTabText(index, tabText);
    m_menu->actions().at(index)->setText(tabText);
}

int QSUiTabWidget::addTab(const QString &label)
{
    return insertTab(-1, label);
}

int QSUiTabWidget::addTab(const QIcon &icon, const QString &label)
{
    return insertTab(-1, icon, label);
}

int QSUiTabWidget::insertTab(int index, const QString &label)
{
    return insertTab(index, QIcon(), label);
}

int QSUiTabWidget::insertTab(int index, const QIcon &icon, const QString &label)
{
    QString tabText = label;
    index = m_tabBar->insertTab(index, icon, tabText.replace("&", "&&"));
    setUpLayout();
    tabInserted(index);
    return index;
}

void QSUiTabWidget::removeTab(int index)
{
    m_tabBar->removeTab(index);
    setUpLayout();
    tabRemoved(index);
}

void QSUiTabWidget::setTabsVisible(bool visible)
{
    m_tabBar->setVisible(visible);
}

void QSUiTabWidget::setCurrentIndex(int index)
{
    m_tabBar->setCurrentIndex(index);
}

void QSUiTabWidget::onCurrentChanged(int index)
{
    emit currentChanged(index);
    if(index >= m_menu->actions().count())
        return;
    m_menu->actions().at(index)->setChecked(true);
}

void QSUiTabWidget::onActionTriggered(QAction *action)
{
    m_tabBar->setCurrentIndex(m_menu->actions().indexOf(action));
}

void QSUiTabWidget::updateActions()
{
    for(int i = 0; i < m_menu->actions().size(); ++i)
    {
        m_menu->actions().at(i)->setText(m_tabBar->tabText(i));
    }
    m_menu->actions().at(m_tabBar->currentIndex())->setChecked(true);
}

void QSUiTabWidget::initStyleOption(QStyleOptionTabWidgetFrame *option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, 0, this);

    int exth = style()->pixelMetric(QStyle::PM_TabBarBaseHeight, 0, this);
    QSize t(0, 0);
    if (m_tabBar->isVisibleTo(const_cast<QSUiTabWidget *>(this)))
        t = m_tabBar->sizeHint();

    if (m_rightCornerWidget)
    {
        const QSize rightCornerSizeHint = m_rightCornerWidget->sizeHint();
        const QSize bounds(rightCornerSizeHint.width(), t.height() - exth);
        option->rightCornerWidgetSize = rightCornerSizeHint.boundedTo(bounds);
    }
    else
    {
        option->rightCornerWidgetSize = QSize(0, 0);
    }

    if (m_leftCornerWidget)
    {
        const QSize leftCornerSizeHint = m_leftCornerWidget->sizeHint();
        const QSize bounds(leftCornerSizeHint.width(), t.height() - exth);
        option->leftCornerWidgetSize = leftCornerSizeHint.boundedTo(bounds);
    }
    else
    {
        option->leftCornerWidgetSize = QSize(0, 0);
    }


    option->shape = QTabBar::RoundedNorth;

    option->tabBarSize = t;

#if QT_VERSION >= 0x050000
    QRect tbRect = m_tabBar->geometry();
    QRect selectedTabRect = tabBar()->tabRect(tabBar()->currentIndex());
    option->tabBarRect = tbRect;
    selectedTabRect.moveTopLeft(selectedTabRect.topLeft() + tbRect.topLeft());
    option->selectedTabRect = selectedTabRect;
#else
    if(QStyleOptionTabWidgetFrameV2 *tabframe = qstyleoption_cast<QStyleOptionTabWidgetFrameV2*>(option))
    {
        QRect tbRect = tabBar()->geometry();
        QRect selectedTabRect = tabBar()->tabRect(tabBar()->currentIndex());
        tabframe->tabBarRect = tbRect;
        selectedTabRect.moveTopLeft(selectedTabRect.topLeft() + tbRect.topLeft());
        tabframe->selectedTabRect = selectedTabRect;
    }
#endif
}

void QSUiTabWidget::setUpLayout()
{
    QStyleOptionTabWidgetFrame option;
    initStyleOption(&option);

    QRect tabRect = style()->subElementRect(QStyle::SE_TabWidgetTabBar, &option, this);
    m_panelRect = style()->subElementRect(QStyle::SE_TabWidgetTabPane, &option, this);
    QRect contentsRect = style()->subElementRect(QStyle::SE_TabWidgetTabContents, &option, this);
    QRect leftCornerRect = style()->subElementRect(QStyle::SE_TabWidgetLeftCorner, &option, this);
    QRect rightCornerRect = style()->subElementRect(QStyle::SE_TabWidgetRightCorner, &option, this);

    m_tabBar->setGeometry(tabRect);
    m_listWidget->setGeometry(contentsRect);
    if (m_leftCornerWidget)
        m_leftCornerWidget->setGeometry(leftCornerRect);
    if (m_rightCornerWidget)
        m_rightCornerWidget->setGeometry(rightCornerRect);

    updateGeometry();
}

void QSUiTabWidget::showEvent(QShowEvent *)
{
    setUpLayout();
}

void QSUiTabWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    setUpLayout();
}

void QSUiTabWidget::paintEvent(QPaintEvent *)
{
    if(!m_tabBar->isVisibleTo(this))
        return;
    QStylePainter p(this);

    QStyleOptionTabWidgetFrame opt;
    initStyleOption(&opt);
    opt.rect = m_panelRect;
    p.drawPrimitive(QStyle::PE_FrameTabWidget, opt);
}

void QSUiTabWidget::changeEvent(QEvent *ev)
{
    if (ev->type() == QEvent::StyleChange
#ifdef Q_OS_MAC
            || ev->type() == QEvent::MacSizeChange
#endif
            )
        setUpLayout();
    QWidget::changeEvent(ev);
}

bool QSUiTabWidget::event(QEvent *ev)
{
    if (ev->type() == QEvent::LayoutRequest)
        setUpLayout();
    return QWidget::event(ev);
}

void QSUiTabWidget::tabInserted(int index)
{
    QAction *action = new QAction(m_menu);
    action->setCheckable(true);
    action->setActionGroup(m_group);
    action->setText(m_tabBar->tabText(index));

    if(m_menu->actions().isEmpty() || index == m_menu->actions().count())
    {
        m_menu->addAction(action);
    }
    else
    {
        QAction *before = m_menu->actions().at(index);
        m_menu->insertAction(before, action);
    }
    if(m_tabBar->currentIndex() == index)
        action->setChecked(true);
}

void QSUiTabWidget::tabRemoved(int index)
{
    QAction *a = m_menu->actions().at(index);
    m_menu->removeAction(a);
    delete a;
}

void QSUiTabWidget::mousePressEvent(QMouseEvent *e)
{
    if((e->button() == Qt::MidButton) &&
            (childAt(e->pos()) == 0))
    {
        e->accept();
        emit createPlayListRequested();
    }
    QWidget::mousePressEvent(e);
}

void QSUiTabWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if((e->button() == Qt::LeftButton) &&
            (childAt(e->pos()) == 0))
    {
        e->accept();
        emit createPlayListRequested();
    }
    QWidget::mouseDoubleClickEvent(e);
}
