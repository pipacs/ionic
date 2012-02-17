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

import QtQuick 1.1
import QtWebKit 1.0
import com.pipacs.ionic.Book 1.0
import com.pipacs.ionic.Preferences 1.0
import "theme.js" as Theme

Flickable {
    // Target reading position, within the current part of the book. After loading the part, BookView will jump to this position, unless it is set to -1
    property double targetPos: -1

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
    property alias updateTimer: updateTimer

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
    interactive: prefs.useSwipe

    WebView {
        id: webView
        transformOrigin: Item.TopLeft
        pressGrabTime: 9999
        focus: true
        settings.defaultFontSize: 26 + (prefs.zoom - 100) / 10
        settings.minimumFontSize: 22
        settings.javaEnabled: false
        settings.javascriptCanAccessClipboard: false
        settings.javascriptCanOpenWindows: false
        settings.javascriptEnabled: true
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
        z: 0

        property bool loading: false

        // Hide cover with a delay (prevent flicking)
        Timer {
            id: coverRemover
            interval: 200
            running: false
            repeat: false
            onTriggered: {styleCover.opacity = 0}
        }

        onLoadFailed: {
            loading = false
            flickable.targetPos = -1
            coverRemover.restart()
        }

        onLoadFinished: {
            setStyle(prefs.style)
            loading = false
            bookView.jump()
            coverRemover.restart()
            // Disable links
            webView.evaluateJavaScript("for (var i = 0; i < document.links.length; i++) {l = document.links[i]; l.disabled = true; l.onclick = new Function('return false'); l.style.textDecoration = 'none'}")
        }

        onLoadStarted: {
            loading = true
        }

        // Forward signals on completion
        Component.onCompleted: {
            loadStarted.connect(flickable.loadStarted)
            loadFailed.connect(flickable.loadFailed)
            loadFinished.connect(flickable.loadFinished)
        }
    }

    // Show bookmarks, using the bookmark list as model
    Repeater {
        z: 1
        model: library.nowReading.bookmarks
        delegate: bookmarkDelegate
    }

    // Delegate to draw a bookmark, but only if it points to the current part
    Component {
        id: bookmarkDelegate
        Item {
            Image {
                id: star
                x: webView.width - 50
                y: webView.contentsSize.height * library.nowReading.bookmarks[index].position
                width: 50
                height: 50
                visible: !webView.loading && (library.nowReading.bookmarks[index].part == flickable.part)
                source: "qrc:/icons/star.png"
                opacity: 0.75
            }
            MouseArea {
                anchors.fill: star
                enabled: star.visible
                onPressAndHold: {
                    console.log("* Bookmark image press-and-hold")
                }
            }
        }
    }

    // A rectangle to cover up the web view while it is loading
    Rectangle {
        id: styleCover
        anchors.fill: parent
        border.width: 0
        color: Theme.background(prefs.style)
        z: 1
    }

    // Periodically update last reading position
    Timer {
        id: updateTimer
        interval: 3000
        running: true
        repeat: true
        onTriggered: {if (!webView.loading) updateLastBookmark()}
    }

    // Scroll up one page
    function goToPreviousPage() {
        if (flickable.contentY == 0) {
            goToPreviousPart()
            return
        }
        var newY = flickable.contentY - flickable.height + 31;
        if (newY < 0) {
            newY = 0
        }
        flickable.contentY = newY
        updateLastBookmark()
    }

    // Scroll down one page
    function goToNextPage() {
        if (flickable.contentY + flickable.height >= webView.contentsSize.height) {
            goToNextPart()
        }
        var newY = flickable.contentY + flickable.height - 31;
        if (newY + flickable.height > webView.contentsSize.height) {
            newY = webView.contentsSize.height - flickable.height
        }
        flickable.contentY = newY
        updateLastBookmark()
    }

    // Load previous part
    function goToPreviousPart() {
        if (flickable.part == 0) {
            return
        }
        flickable.part -= 1
        flickable.targetPos = 1
        load(library.nowReading.urlFromPart(flickable.part))
    }

    // Load next part
    function goToNextPart() {
        if (flickable.part >= (library.nowReading.partCount - 1)) {
            return;
        }
        flickable.part += 1
        flickable.targetPos = 0
        load(library.nowReading.urlFromPart(flickable.part))
    }

    // Update book's last reading position, but only if it has been changed
    function updateLastBookmark() {
        if (webView.contentsSize.height > 0) {
            var currentPosition = flickable.contentY / webView.contentsSize.height
            var book = library.nowReading
            if ((Math.abs(book.lastBookmark.position - currentPosition) > 0.0005) || (book.lastBookmark.part != flickable.part)) {
                book.setLastBookmark(flickable.part, currentPosition)
                book.save()
            }
        }
    }

    // Jump to a new location within the page, specified in flickable.targetPos
    function jump() {
        if (flickable.targetPos != -1) {
            var newY = webView.contentsSize.height * flickable.targetPos
            if (flickable.targetPos == 1) {
                newY -= flickable.height
            }
            if (newY < 0) {
                newY = 0
            }
            flickable.contentY = newY
            flickable.targetPos = -1
            updateLastBookmark()
        }
    }

    // Set style
    function setStyle(style) {
        styleCover.color = Theme.background(style)
        webView.evaluateJavaScript(Theme.webTheme(style))
    }

    // Load URL while covering the web view
    function load(url) {
        styleCover.opacity = 1
        webView.url = url
    }
}
