/**
 * Hercules P32 DJ controller script for Mixxx 2.1
 * Thanks to Hercules for supporting the development of this mapping by providing a controller
 * See http://mixxx.org/wiki/doku.php/hercules_p32_dj for instructions on how to use this mapping
 *
 * Copyright (C) 2017 Be <be.0@gmx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/


//function XoneK2_4Fx() {}
var XoneK2_4Fx = {};

XoneK2_4Fx.leds =  [0x34,   0x35,   0x36,   0x37,
                0x58,	0x59,   0x5A,	0x5B,
                0x7C,	0x7d,	0x7e,	0x7f,
                0x30,	0x31,	0x32,	0x33,
                0x54,	0x55,	0x56,	0x57,
                0x78,	0x79,	0x7A,	0x7b,
                0x2c,	0x2d,	0x2e,	0x2f,
                0x50,	0x51,	0x52,	0x53,
                0x74,	0x75,	0x76,	0x77,
                0x28,	0x29,	0x2a,	0x2b,
                0x4c,	0x4d,	0x4e,	0x4f,
                0x70,	0x71,	0x72,	0x73,
                0x24,	0x25,	0x26,	0x27,
                0x48,	0x49,	0x4a,	0x4b,
                0x6c,	0x6d,	0x6e,	0x6f,
                0x20,	0x21,	0x22,	0x23,
                0x44,	0x45,	0x46,	0x47,
                0x68,	0x69,	0x6a,	0x6b,
                0x1c,	0x1d,	0x1e,	0x1f,
                0x40,	0x41,	0x42,	0x43,
                0x64,	0x65,	0x66,	0x67,
                0x18,	0x19,	0x1A,	0x1b,
                0x3c,	0x3d,	0x3e,	0x3f,
                0x60,	0x61,	0x62,	0x63];

XoneK2_4Fx.init = function (id) {    // called when the MIDI device is opened & set up
    print ("XoneK2_4Fx id: \""+id+"\" initialized.");
    // one-shot to clear lights.  If we don't delay this call, nothing happens
    if (engine.beginTimer(2000,"XoneK2_4Fx.clearlights()",true) == 0) {
        print("Clearlights timer setup failed");
    }
    XoneK2_4Fx.clearlights();
}

XoneK2_4Fx.shutdown = function(id) {
	XoneK2_4Fx.clearlights();
}

XoneK2_4Fx.clearlights = function () {
	for ( var LED in XoneK2_4Fx.leds ) {
        print("Clear LED: #" + LED +" --> "+XoneK2_4Fx.leds[LED]);
        midi.sendShortMsg(0x9F, XoneK2_4Fx.leds[LED], 0x0);
    }
}


XoneK2_4Fx.effectUnit1 = new EffectUnit(1);
XoneK2_4Fx.effectUnit1.enableButtons[1].midi = [0x9F, 0x30];
XoneK2_4Fx.effectUnit1.enableButtons[2].midi = [0x9F, 0x2C];
XoneK2_4Fx.effectUnit1.enableButtons[3].midi = [0x9F, 0x28];
XoneK2_4Fx.effectUnit1.showParametersButton.midi = [0x9F, 0x58];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel1 = [0x9F, 0x24];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel2 = [0x9F, 0x20];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel3 = [0x9F, 0x1C];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel4 = [0x9F, 0x18];
XoneK2_4Fx.effectUnit1.init();

