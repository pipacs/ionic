/*
 * NPM: originally from QtSDK 1.1
 * /opt/QtSDK/Demos/4.7/declarative/webbrowser/content/FlickableWebView.qml
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2011 Niels Mayer <NielsMayer _AT_ gmail _DOT_ com>
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

Flickable {
    property alias title:         webView.title
    property alias icon:          webView.icon
    property alias progress:      webView.progress
    property alias url:           webView.url
    property alias back:          webView.back
    property alias stop:          webView.stop
    property alias reload:        webView.reload
    property alias forward:       webView.forward
    property alias scale:         webView.contentsScale
    property alias sx:            flickable.contentX
    property alias sy:            flickable.contentY

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
    interactive: false

    WebView {
        id: webView
        transformOrigin: Item.TopLeft
        pressGrabTime: 999
        settings.standardFontFamily: "Nokia Pure Text"
        smooth: false
        focus: true
        onAlert: console.log("WebView onAlert: " + message);
        onLoadFailed: root.webViewReady(false);
        onLoadFinished: {
            flickable.interactive = true;
        }
        javaScriptWindowObjects:QtObject {
            WebView.windowObjectName: "qml"
            function consoleLog(msg)      {console.log(msg);}
            function consoleInfo(msg)     {console.info(msg);}
            function enableScroll(enable) {flickable.interactive = enable;}
        }

        function doZoom(zoom,centerX,centerY) {
            if (centerX) {
                var sc = zoom*contentsScale;
                scaleAnim.to = sc;
                flickVX.from = flickable.contentX
                flickVX.to = Math.max(0,Math.min(centerX-flickable.width/2,webView.width*sc-flickable.width))
                finalX.value = flickVX.to
                flickVY.from = flickable.contentY
                flickVY.to = Math.max(0,Math.min(centerY-flickable.height/2,webView.height*sc-flickable.height))
                finalY.value = flickVY.to
                quickZoom.start()
            }
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Plus) {
            }
            if (event.key == Qt.Key_Minus) {
            }
            if (event.key == Qt.Key_0) {
            }
        }

        preferredWidth: flickable.width
        preferredHeight: flickable.height
        contentsScale: 1
        onUrlChanged: {
            // got to topleft
            flickable.contentX = 0
            flickable.contentY = 0
        }

        SequentialAnimation {
            id: quickZoom

            PropertyAction {
                target: webView
                property: "renderingEnabled"
                value: false
            }
            ParallelAnimation {
                NumberAnimation {
                    id: scaleAnim
                    target: webView
                    property: "contentsScale"
                    // the to property is set before calling
                    easing.type: Easing.Linear
                    duration: 200
                }
                NumberAnimation {
                    id: flickVX
                    target: flickable
                    property: "contentX"
                    easing.type: Easing.Linear
                    duration: 200
                    from: 0 // set before calling
                    to: 0 // set before calling
                }
                NumberAnimation {
                    id: flickVY
                    target: flickable
                    property: "contentY"
                    easing.type: Easing.Linear
                    duration: 200
                    from: 0 // set before calling
                    to: 0 // set before calling
                }
            }
            // Have to set the contentXY, since the above 2
            // size changes may have started a correction if
            // contentsScale < 1.0.
            PropertyAction {
                id: finalX
                target: flickable
                property: "contentX"
                value: 0 // set before calling
            }
            PropertyAction {
                id: finalY
                target: flickable
                property: "contentY"
                value: 0 // set before calling
            }
            PropertyAction {
                target: webView
                property: "renderingEnabled"
                value: true
            }
        }
        onZoomTo: doZoom(zoom,centerX,centerY)
    }
}
