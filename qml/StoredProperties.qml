/**
 *  OSM
 *  Copyright (C) 2018  Pavel Smokotnin

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.13
import QtQuick.Dialogs 1.2

import QtQuick.Controls.Material 2.12

Item {
    property var dataObject

    RowLayout
    {
        spacing: 0
        anchors.fill: parent


        ColumnLayout {
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            RowLayout {
                ColorPicker {
                    id: colorPicker

                    Layout.preferredWidth: 25
                    Layout.preferredHeight: 25
                    Layout.margins: 5

                    onColorChanged: {
                        dataObject.color = color
                    }
                    Component.onCompleted: {
                        colorPicker.color = dataObject.color
                    }
                }

                TextField {
                    placeholderText: qsTr("title")
                    width: 135
                    text: dataObject.name
                    onTextEdited: dataObject.name = text
                }
            }
            RowLayout {

                ComboBox {
                    displayText: qsTr("Save data as");

                    implicitWidth: 170
                    model: ["osm", "cal", "frd"]

                    onActivated: function() {
                        fileDialog.saveas = currentText;
                        fileDialog.open();
                    }

                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("export data")
                }
            }
        }

        ScrollView {
            id: scrollTextArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            ScrollBar.vertical.interactive: false

            TextArea {
                id:ta
                padding: 5
                placeholderText: qsTr("notes")
                text: dataObject.notes;
                onTextChanged: dataObject.notes = text;
                font.italic: true
                wrapMode: TextEdit.WrapAnywhere
                background: Rectangle{
                    height: scrollTextArea.height
                    width:  scrollTextArea.width
                    border.color: ta.activeFocus ? ta.Material.accentColor : ta.Material.hintTextColor
                    border.width: ta.activeFocus ? 2 : 1
                    color: "transparent"
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        property string saveas: "osm"
        selectExisting: false
        title: "Please choose a file's name"
        folder: shortcuts.home
        defaultSuffix: saveas
        onAccepted: {
            switch (saveas) {
                case "osm":
                    dataObject.save(fileDialog.fileUrl);
                    break;
                case "cal":
                    dataObject.saveCal(fileDialog.fileUrl);
                    break;
                case "frd":
                    dataObject.saveFRD(fileDialog.fileUrl);
                    break;
            }
        }
    }
}
