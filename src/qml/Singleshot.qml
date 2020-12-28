/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.3

Timer {
    signal timeout(variant target)

    property variant target

    onTriggered: {
        timeout(target);
        destroy(10);
    }
}
