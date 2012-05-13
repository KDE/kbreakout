import QtQuick 1.1

Image {
    property string spriteKey

    source: spriteKey=="" ? "" : "image://svgitem/"+spriteKey
}
