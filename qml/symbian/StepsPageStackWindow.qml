import QtQuick 1.1
import com.nokia.symbian 1.1

PageStackWindow {
    property bool toolBar: false // Ignored on Symbian
    platformSoftwareInputPanelEnabled: true
    // showToolBar: !inputContext.visible;
    // showStatusBar: !inputContext.visible;
}
