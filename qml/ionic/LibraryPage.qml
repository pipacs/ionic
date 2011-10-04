import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: commonTools

    ToolBarLayout {
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {myMenu.close(); pageStack.pop()}
        }
    }
}
