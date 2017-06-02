import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQml.StateMachine 1.0


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
    }

    Action {
        id: prevPage
        text: "Prev Page"
        onTriggered: {ldafbrowser.prevPage()}
    }

    Action {
        id: closeWindow
        text: "Close Window"
        onTriggered: {close()}
    }

    toolBar: ToolBar {
           Row{
               anchors.fill: parent
               ToolButton { action: nextPage}
               ToolButton { action: prevPage}
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
    }

    StateMachine{
        id: naviStates
        initialState: both_closed
        running: true

        State {
            id: both_closed
        }
    }
}
