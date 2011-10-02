import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: commonTools
    BookView {
        url: library.nowReading.lastUrl
        anchors {top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom}
    }
}
