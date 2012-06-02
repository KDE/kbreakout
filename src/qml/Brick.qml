import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

CanvasItem {
    id: brick
    property int blockWidth: giftType!="" && spriteKey==giftType ? Globals.GIFT_WIDTH : Globals.BRICK_WIDTH
    property int blockHeight: giftType!="" && spriteKey==giftType ? Globals.GIFT_HEIGHT : Globals.BRICK_HEIGHT

    width: m_scale * blockWidth
    height: m_scale * blockHeight

    property string type
    property string giftType

    onTypeChanged: show();
    //onGiftTypeChanged: showGift();

    function isHidden() { return spriteKey==""; }
    function hide() { spriteKey = ""; }
    function show() { spriteKey = type; }

    function showGift() { spriteKey = giftType; }
    function hasGift() { return giftType!=""; }
}
