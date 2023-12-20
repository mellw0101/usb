import QtQuick 2.0
import QtWayland.Compositor 1.4

WaylandCompositor {
    // Define a WaylandOutput to link with the QWaylandOutput in C++
    WaylandOutput {
        window: Window {
            visible: true
            width: 640
            height: 480
            color: "lightgray" // Background color of the compositor window

            // This Text element is optional, just for visual confirmation
            Text {
                text: "Wayland Compositor Running"
                anchors.centerIn: parent
            }
        }
    }

    // XdgShell for handling Wayland client surfaces
    XdgShell {
        onToplevelCreated: {
            let surfaceItem = shellSurfaceItem.createObject(window.contentItem)
            surfaceItem.toplevel = toplevel
        }

        Component {
            id: shellSurfaceItem

            // Define how each Wayland client window is displayed
            WaylandQuickItem {
                anchors.fill: parent
                surface: toplevel.surface
            }
        }
    }
}
