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
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.pipacs.ionic.Book 1.0
import com.pipacs.ionic.Preferences 1.0

Flickable {
    // Target reading position, within the current part of the book. After loading the part, BookView will jump to this position.
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
    interactive: prefs.useSwipe

    WebView {
        id: webView
        transformOrigin: Item.TopLeft
        pressGrabTime: 9999
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
        Keys.enabled: true

        property bool loading: false

        onLoadFailed: {
            loading = false
            flickable.targetPos = -1
            styleCover.visible = false
        }

        onLoadFinished: {
            console.log("* BookView.WebView.onLoadFinished")
            setStyle(prefs.style)
            loading = false
            bookView.jump()
            styleCover.visible = false
        }

        onLoadStarted: {
            loading = true
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

    Rectangle {
        id: styleCover
        anchors.fill: parent
        border.width: 0
        color: "white"
        visible: false
        z: 1
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
        load(library.nowReading.urlFromPart(flickable.part))
    }

    function goToNextPart() {
        if (flickable.part >= (library.nowReading.partCount - 1)) {
            return;
        }
        flickable.part += 1
        flickable.targetPos = 0
        load(library.nowReading.urlFromPart(flickable.part))
    }

    // Update book's last reading position
    function updateLastBookmark() {
        var currentPosition = flickable.contentY / webView.contentsSize.height
        var book = library.nowReading
        if ((Math.abs(book.lastBookmark.position - currentPosition) > 0.0005) || (book.lastBookmark.part != flickable.part)) {
            console.log("* BookView.updateLastBookmark: Needs update")
            book.setLastBookmark(flickable.part, currentPosition)
            book.save()
        }
    }

    // Jump to a new location specified in flickable.targetPos or flickable.targetUrlFragment
    function jump() {
        console.log("* BookView.jump targetPos " + flickable.targetPos)
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
        var styles = new Object
        styles.day = "document.body.style.background = '#F7F7F7'; document.body.style.color = '#000000'"
        styles.night = "document.body.style.background = '#000009'; document.body.style.color = '#F7F7F7'"
        styles.sand = "document.body.style.background = '#EDC9AF'; document.body.style.color = '#000000'"
        var backgrounds = new Object
        backgrounds.day = "#F7F7F7"
        backgrounds.night = "#000009"
        backgrounds.sand = "#EDC9AF"
        console.log("* BookView.setStyle " + style + " -> " + styles[style] + ", color " + backgrounds[style])
        styleCover.color = backgrounds[style]
        webView.evaluateJavaScript(styles[style])
    }

    function load(url) {
        styleCover.visible = true;
        webView.url = url
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
