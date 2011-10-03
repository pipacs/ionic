/*
 * NPM: originally from QtSDK 1.1
 * /opt/QtSDK/Demos/4.7/declarative/webbrowser/content/FlickableWebView.qml
 *
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

    // Target reading position, within the current part url.
    // Upon successful loading, BookView will jump to this position.
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

    WebView {
        id: webView
        transformOrigin: Item.TopLeft
        pressGrabTime: 999
        settings.standardFontFamily: "Nokia Pure Text"
        smooth: false
        preferredWidth: flickable.width
        preferredHeight: flickable.height
        contentsScale: 1

        onLoadFailed: {
            flickable.interactive = true
            flickable.targetPos = 0
        }

        onLoadFinished: {
            flickable.interactive = true
            var newY = webView.contentsSize.height * flickable.targetPos - flickable.height
            if (newY < 0) {
                newY = 0
            }
            flickable.contentY = newY
            flickable.targetPos = 0
        }

        onLoadStarted: {
            console.log("BookView onLoadStarted")
            flickable.interactive = false
        }

        javaScriptWindowObjects:QtObject {
            WebView.windowObjectName: "qml"
            function consoleLog(msg) {console.log(msg);}
            function consoleInfo(msg) {console.info(msg);}
            function enableScroll(enable) {flickable.interactive = enable;}
        }

        Keys.onPressed: {
            if (event.key == 16777330) {
                // Volume up
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
            if (event.key == 16777328) {
                // Volume down
                var newY = flickable.contentY + flickable.height - 31;
                if (newY > webView.contentsSize.height) {
                    goToNextPart()
                }
                flickable.contentY = newY
            }
            // if (event.key == Qt.Key_Plus) {
            // }
        }

        // Forward signals on completion
        Component.onCompleted: {
            console.log("webView onCompleted")
            loadStarted.connect(flickable.loadStarted)
            loadFailed.connect(flickable.loadFailed)
            loadFinished.connect(flickable.loadFinished)
        }

        // Go to the previous part
        function goToPreviousPart() {
            if (flickable.part == 0) {
                return
            }
            flickable.part -= 1
            flickable.targetPos = 1
            webView.url = flickable.book.url(flickable.part)
        }

        // Go to the next part
        function goToNextPart() {
            if (flickable.part >= (flickable.book.partCount - 1)) {
                return;
            }
            flickable.part += 1
            flickable.targetPos = 0
            webView.url = flickable.book.url(flickable.part)
        }
    }
}
