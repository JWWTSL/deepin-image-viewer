// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "cursortool.h"

#include <QTimer>
#include <QCursor>
#include <DGuiApplicationHelper>
#include <DLog>

Q_DECLARE_LOGGING_CATEGORY(logImageViewer)

static const int sc_ESampleInterval = 50;  // 采样间隔 50ms

CursorTool::CursorTool(QObject *parent)
    : QObject(parent)
{
    qCDebug(logImageViewer) << "CursorTool constructor called.";
    m_CaptureTimer = new QTimer(this);
    m_CaptureTimer->setInterval(sc_ESampleInterval);
    qCDebug(logImageViewer) << "CaptureTimer initialized with interval: " << sc_ESampleInterval << "ms.";

    connect(m_CaptureTimer, &QTimer::timeout, this, [this]() {
        QPoint pos = QCursor::pos();
        if (pos != m_lastPos) {
            m_lastPos = pos;
            // 发送当前光标的全局位置
            Q_EMIT this->cursorPosChanged(pos.x(), pos.y());
            qCDebug(logImageViewer) << "Cursor position changed to: (" << pos.x() << ", " << pos.y() << "), emitting cursorPosChanged.";
        }
    });
    qCDebug(logImageViewer) << "Connected CaptureTimer timeout signal.";

    connect(Dtk::Gui::DGuiApplicationHelper::instance(), &Dtk::Gui::DGuiApplicationHelper::applicationPaletteChanged, [this]() {
        auto newColor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
        emit activeColorChanged(newColor);
        qCDebug(logImageViewer) << "Application palette changed, emitting activeColorChanged with new color.";
    });
    qCDebug(logImageViewer) << "Connected applicationPaletteChanged signal.";
}

/**
   @return 返回当前鼠标光标在屏幕的位置
 */
QPoint CursorTool::currentCursorPos() const
{
    qCDebug(logImageViewer) << "CursorTool::currentCursorPos() called, returning current cursor position.";
    return QCursor::pos();
}

/**
 * @param b 设置是否启动定时查询光标位置
 *  默认间隔20ms
 */
void CursorTool::setCaptureCursor(bool b)
{
    qCDebug(logImageViewer) << "CursorTool::setCaptureCursor() called with value: " << b;
    if (b) {
        m_CaptureTimer->start();
        qCDebug(logImageViewer) << "CaptureTimer started.";
    } else {
        m_CaptureTimer->stop();
        qCDebug(logImageViewer) << "CaptureTimer stopped.";
    }
}

QColor CursorTool::activeColor()
{
    qCDebug(logImageViewer) << "CursorTool::activeColor() called.";
    return Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
}
