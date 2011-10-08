import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: aboutTools

    Flickable {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5
        contentWidth: Math.max(parent.width, column.width)
        contentHeight: Math.max(parent.height, column.height)
        flickableDirection: Flickable.VerticalFlick
        id: about
        Column {
            id: column
            Row {
                Image {source: "qrc:/ionic80.png"}
            }
            Row {
                Label {
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    width: aboutTools.width - 10
                    text: "\nIonic version " + version + "\n\nCopyright (c) 2009-2011 by Akos Polster. All rights reserved\n\nAcknowledgements:\n\nSplash images and book PNG icons are by Mikey (mikey@orcap.net), licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License\n\nSample book \"2 B R 0 2 B\" by Kurt Vonnegut, digitized by Project Gutenberg (http://www.gutenberg.org/)\n\nMinizip library copyright (C) 1998-2010 Gilles Vollant, Even Rouault and Mathias Svensson (http://www.winimage.com/zLibDll/minizip.html)\n\nBook SVG icon by Chris Down of the Tango project: http://tango.freedesktop.org/Tango_Desktop_Project. The icon is released into the public domain. The Tango! Desktop Project grants anyone the right to use this work for any purpose, without any conditions, unless such conditions are required by law\n\nRabbit image is based on the cover image of Uncle Remus, His Songs and His Sayings: The Folk-Lore of the Old Plantation, by Joel Chandler Harris. Illustrations by Frederick S. Church and James H. Moser. New York: D. Appleton and Company, 1881. The cover image is in the public domain in the United States: http://commons.wikimedia.org/wiki/File:Brer_Rabbit_cover,_1881.jpg"}}
        }
    }

    ScrollDecorator {
        flickableItem: about
    }

    ToolBarLayout {
        id: aboutTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop()
            }
        }
    }
}
