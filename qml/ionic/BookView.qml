/*
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2011 Niels Mayer <NielsMayer _AT_ gmail _DOT_ com>
 * Copyright (C) 2011 Akos Polster <akos@pipacs.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

import QtQuick 1.0
import QtWebKit 1.0

import com.pipacs.ionic.Book 1.0

Flickable {
    // Book to display
    property Book book: emptyBook

    // Target reading position, within the current part of the book. After loading the part, BookView will jump to this position.
    property double targetPos: 0

    // Current part index
    property int part: 0

    property alias title: webView.title
    property alias icon: webView.icon
    property alias progress: webView.progress
    property alias url: webView.url
    property alias back: webView.back
    property alias stop: webView.stop
    property alias reload: webView.reload
    property alias forward: webView.forward
    property alias scale: webView.contentsScale
    property alias sx: flickable.contentX
    property alias sy: flickable.contentY
    property alias webView: webView

    signal loadStarted
    signal loadFinished
    signal loadFailed

    id: flickable
    width: parent.width
    contentWidth: Math.max(parent.width, webView.width)
    contentHeight: Math.max(parent.height, webView.height)
    anchors.top: parent.top
    anchors.bottom: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    pressDelay: 0
    flickableDirection: Flickable.VerticalFlick
    // focus: true
    // Keys.enabled: true

    WebView {
        id: webView
        transformOrigin: Item.TopLeft
        pressGrabTime: 9999
        // settings.standardFontFamily: "Nokia Pure Text"
        settings.defaultFontSize: 26
        settings.javaEnabled: false
        settings.javascriptCanAccessClipboard: false
        settings.javascriptCanOpenWindows: false
        settings.javascriptEnabled: false
        settings.linksIncludedInFocusChain: false
        settings.localContentCanAccessRemoteUrls: false
        settings.localStorageDatabaseEnabled: false
        settings.offlineStorageDatabaseEnabled: false
        settings.offlineWebApplicationCacheEnabled: false
        settings.pluginsEnabled: false
        smooth: false
        preferredWidth: flickable.width
        preferredHeight: flickable.height
        contentsScale: 1
        // enabled: true
        Keys.enabled: true

        onLoadFailed: {
            flickable.interactive = true
            flickable.targetPos = 0
        }

        onLoadFinished: {
            flickable.interactive = true
            var newY = webView.contentsSize.height * flickable.targetPos
            if (flickable.targetPos == 1) {
                newY -= flickable.height
            }
            if (newY < 0) {
                newY = 0
            }
            flickable.contentY = newY
            flickable.targetPos = 0
            updateLastBookmark()
        }

        onLoadStarted: {
            console.log("* BookView.WebView onLoadStarted")
            flickable.interactive = false
        }

        Keys.onPressed: {
            console.log("* BookView.WebView.Keys.onPressed " + event.key)
            if ((event.key == Qt.Key_VolumeUp) || (event.key == Qt.Key_Up) || (event.key == Qt.Key_PageUp)) {
                goToPreviousPage()
            } else if ((event.key == Qt.Key_VolumeDown) || (event.key == Qt.Key_Down) || (event.key == Qt.Key_PageDown)) {
                goToNextPage()
            }
        }

        // Forward signals on completion
        Component.onCompleted: {
            console.log("* webView onCompleted")
            loadStarted.connect(flickable.loadStarted)
            loadFailed.connect(flickable.loadFailed)
            loadFinished.connect(flickable.loadFinished)
        }
    }

    function goToPreviousPage() {
        if (flickable.contentY == 0) {
            goToPreviousPart()
            return
        }
        var newY = flickable.contentY - flickable.height + 31;
        if (newY < 0) {
            newY = 0;
        }
        flickable.contentY = newY;
    }

    function goToNextPage() {
        if (flickable.contentY + flickable.height >= webView.contentsSize.height) {
            goToNextPart()
        }
        var newY = flickable.contentY + flickable.height - 31;
        if (newY + flickable.height > webView.contentsSize.height) {
            newY = webView.contentsSize.height - flickable.height
        }
        flickable.contentY = newY
    }

    function goToPreviousPart() {
        if (flickable.part == 0) {
            return
        }
        flickable.part -= 1
        flickable.targetPos = 1
        webView.url = flickable.book.url(flickable.part)
    }

    function goToNextPart() {
        if (flickable.part >= (flickable.book.partCount - 1)) {
            return;
        }
        flickable.part += 1
        flickable.targetPos = 0
        webView.url = flickable.book.url(flickable.part)
    }

    // Update book's last reading position
    function updateLastBookmark() {
        var currentPos = flickable.contentY / webView.contentsSize.height
        if ((Math.abs(flickable.book.lastBookmark.pos - currentPos) > 0.0005) || (flickable.book.lastBookmark.part != flickable.part)) {
            console.log("* webView.updateLastBookmark: Needs update")
            flickable.book.lastBookmark.pos = currentPos
            flickable.book.lastBookmark.part = flickable.part
            flickable.book.save()
        }
    }

    Timer {
        interval: 3000
        running: true
        repeat: true
        onTriggered: {
            updateLastBookmark()
        }
    }
}
