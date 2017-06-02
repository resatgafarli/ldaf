import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

ApplicationWindow {
    id: ldafbrowser_ui
    visible: true
    objectName: "ldafbrowser_appwindow"
    visibility: "FullScreen"
    title: "LDAFBrowser"



    Action {
        id: nextPage
        text: "Next Page"
        onTriggered: {ldafbrowser.nextPage()}
        iconSource: {"images/Next.png"}
    }

    Action {
        id: prevPage
        text: "Prev Page"
        onTriggered: {ldafbrowser.prevPage()}
        iconSource: {"images/Previous.png"}
    }

    Action {
        id: closeWindow
        text: "Close Window"
        onTriggered: {close()}
        iconSource: {"images/Close.png"}
    }

    toolBar: ToolBar {
           Row{
               anchors.fill: parent
               ToolButton { action: prevPage}
               ToolButton { action: nextPage}
               ToolButton { action: closeWindow}
           }
       }

    Loader {
      id: contentLoader
      anchors.fill: parent
    }

    function browserContentLoader(url) {
        console.log(url)
        contentLoader.source = url
        contentLoader.asynchronous = true
        nextPage.enabled = ldafbrowser.hasNext()
        prevPage.enabled = ldafbrowser.hasPrev()
    }


}
