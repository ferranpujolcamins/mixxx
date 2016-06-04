/****************************************************************/
/*             Xone K2 MIDI controller script v1.20             */
/*          Copyright (C) 2012, Owen Williams                   */
/*      but feel free to tweak this to your heart's content!    */
/*      For Mixxx version 1.11                                  */
/****************************************************************/

function XoneK2() {}



/**********************************************************************/
/*                     I n i t i a l i z a t i o n                    */
/**********************************************************************/

XoneK2.shift_status = false;
XoneK2.channelOrder = [1, 2, 3, 4];


// Called when the MIDI device is opened & set up
XoneK2.init = function(id) {
    print ("XoneK2 id: \""+id+"\" initializing...");

    // Timer needed because otherwise some methods don't get called.
    if (engine.beginTimer(2000,"XoneK2.customInit()",true) == 0) {
        print("initializer timer setup failed");
    }
}

XoneK2.customInit = function () {
    XoneK2.bindOutputsToFunctions();
    // TODO: read and send initial state for leds

    XoneK2.createMagicButtonTable()

    engine.setValue('[EffectRack1_EffectUnit1]','clear',1);
    engine.setValue('[EffectRack1_EffectUnit1]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit1]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit1]','prev_chain',1);

    engine.setValue('[EffectRack1_EffectUnit2]','clear',1);
    engine.setValue('[EffectRack1_EffectUnit2]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit2]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit2]','prev_chain',1);

    engine.setValue('[EffectRack1_EffectUnit3]','clear',1);
    engine.setValue('[EffectRack1_EffectUnit3]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit3]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit3]','prev_chain',1);

    engine.setValue('[EffectRack1_EffectUnit4]','clear',1);
    engine.setValue('[EffectRack1_EffectUnit4]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit4]','prev_chain',1);
    engine.setValue('[EffectRack1_EffectUnit4]','prev_chain',1);

    engine.setValue('[EffectRack1_EffectUnit1]','group_[Channel1]_enable',1);
    engine.setValue('[EffectRack1_EffectUnit2]','group_[Channel2]_enable',1);
    engine.setValue('[EffectRack1_EffectUnit3]','group_[Channel3]_enable',1);
    engine.setValue('[EffectRack1_EffectUnit4]','group_[Channel4]_enable',1);

    engine.setValue('[Channel1]','quantize',1);
    engine.setValue('[Channel2]','quantize',1);
    engine.setValue('[Channel3]','quantize',1);
    engine.setValue('[Channel4]','quantize',1);

    engine.setValue('[Channel1]','sync_mode',1);
    engine.setValue('[Channel2]','sync_mode',1);
    engine.setValue('[Channel3]','sync_mode',1);
    engine.setValue('[Channel4]','sync_mode',1);

}

XoneK2.magicButtonTable = {}

XoneK2.createMagicButtonTable = function () {
    for (var midiCode in XoneK2.functionForMidiControl) {
        var func = XoneK2.functionForMidiControl[midiCode];
        if (func.magic) {
            XoneK2.magicButtonTable[func.name] = false;
        }
    }
}


XoneK2.midiControl = function midiControl(channel, code) {
    this.channel = channel;
    this.code = code;

    this.toString = function() {
        return this.channel.toString() + ", " + this.code.toString();
    }

    this.fromString = function(str) {
        results = str.split(", ");
        this.channel = results[0];
        this.code = results[1];
    }
}

XoneK2.leds = {
    "encoder1"      : {red:0x34, yellow:0x58, green:0x7C},
    "encoder2"      : {red:0x35, yellow:0x59, green:0x7D},
    "encoder3"      : {red:0x36, yellow:0x5A, green:0x7E},
    "encoder4"      : {red:0x37, yellow:0x5B, green:0x7F},

    "topKnob1"      : {red:0x30, yellow:0x54, green:0x7C},
    "topKnob2"      : {red:0x31, yellow:0x55, green:0x7D},
    "topKnob3"      : {red:0x32, yellow:0x56, green:0x7E},
    "topKnob4"      : {red:0x33, yellow:0x57, green:0x7F},

    "midKnob1"      : {red:0x2C, yellow:0x50, green:0x7C},
    "midKnob2"      : {red:0x2D, yellow:0x51, green:0x7D},
    "midKnob3"      : {red:0x2E, yellow:0x52, green:0x7E},
    "midKnob4"      : {red:0x2F, yellow:0x53, green:0x7F},

    "bottomKnob1"   : {red:0x28, yellow:0x4C, green:0x7C},
    "bottomKnob2"   : {red:0x29, yellow:0x4D, green:0x7D},
    "bottomKnob3"   : {red:0x2A, yellow:0x4E, green:0x7E},
    "bottomKnob4"   : {red:0x2B, yellow:0x4F, green:0x7F},

    "A"             : {red:0x24, yellow:0x48, green:0x7C},
    "B"             : {red:0x25, yellow:0x49, green:0x7C},
    "C"             : {red:0x26, yellow:0x4A, green:0x7C},
    "D"             : {red:0x27, yellow:0x4B, green:0x7C},

    "E"             : {red:0x20, yellow:0x44, green:0x7C},
    "F"             : {red:0x21, yellow:0x45, green:0x7C},
    "G"             : {red:0x22, yellow:0x46, green:0x7C},
    "H"             : {red:0x23, yellow:0x47, green:0x7C},

    "I"             : {red:0x1C, yellow:0x40, green:0x7C},
    "J"             : {red:0x1D, yellow:0x41, green:0x7C},
    "K"             : {red:0x1E, yellow:0x42, green:0x7C},
    "L"             : {red:0x1F, yellow:0x43, green:0x7C},

    "M"             : {red:0x18, yellow:0x3C, green:0x7C},
    "N"             : {red:0x19, yellow:0x3D, green:0x7C},
    "O"             : {red:0x1A, yellow:0x3E, green:0x7C},
    "P"             : {red:0x1B, yellow:0x3F, green:0x7C},

    "layer"         : {red:0x0C, yellow:0x10, green:0x14},
    "setup"         : {red:0x0F, yellow:0x13, green:0x17},

}


XoneK2.shutdown = function(id) {
}










/**********************************************************************/
/*                  M e s s a g e    d i s p a t c h                  */
/**********************************************************************/

XoneK2.magicShift = false;
XoneK2.magicStack = [];

XoneK2.execMidi = function (midichannel, control, value, status) {
    var midi_control = new XoneK2.midiControl(XoneK2.convertDecToHexString(status), XoneK2.convertDecToHexString(control));
    var func = XoneK2.functionForMidiControl[midi_control.toString()];
    func.channel = XoneK2.channelOrder[func.channel-1];
    func.value = value;
    if (XoneK2.magicShift && func.magic) {
        XoneK2.magicStack.push(func);
    } else {
        XoneK2[func.name].call(this, func.channel, value);
    }
}

XoneK2.magicButton = function(channel, value) {
    if (value == 0) {
        // Is a note off.
        return;
    }
    var l = XoneK2.magicStack.length;
    for (var i=0; i < l; i++) {
        var func = XoneK2.magicStack.pop();
        XoneK2[func.name].call(this, func.channel, func.value);
    }
}

XoneK2.magicShiftButton = function(channel, value) {
    if (value == 127) {
        XoneK2.magicShift = true;
    } else {
        XoneK2.magicShift = false;
    }
}

XoneK2.functionForMidiControl = {
    "0x90, 0x0C": {name:"magicButton", channel:1, magic:false},
    "0x80, 0x0C": {name:"magicButton", channel:1, magic:false},
    "0x90, 0x0F": {name:"magicShiftButton", channel:1, magic:false},
    "0x80, 0x0F": {name:"magicShiftButton", channel:1, magic:false},

    "0xB0, 0x11": {name:"kickFilter", channel:1, magic:false},
    "0xB0, 0x13": {name:"kickFilter", channel:2, magic:false},
    "0xB1, 0x11": {name:"kickFilter", channel:3, magic:false},
    "0xB1, 0x13": {name:"kickFilter", channel:4, magic:false},

    "0xB0, 0x10": {name:"fader", channel:1, magic:false},
    "0xB0, 0x12": {name:"fader", channel:2, magic:false},
    "0xB1, 0x10": {name:"fader", channel:3, magic:false},
    "0xB1, 0x12": {name:"fader", channel:4, magic:false},

    "0xB0, 0x0D": {name:"highPassFilter", channel:1, magic:false},
    "0xB0, 0x0F": {name:"highPassFilter", channel:2, magic:false},
    "0xB1, 0x0D": {name:"highPassFilter", channel:3, magic:false},
    "0xB1, 0x0F": {name:"highPassFilter", channel:4, magic:false},

    "0x90, 0x29": {name:"highPassFilterButton", channel:1, magic:true},
    "0x80, 0x29": {name:"highPassFilterButton", channel:1, magic:true},
    "0x90, 0x2B": {name:"highPassFilterButton", channel:2, magic:true},
    "0x80, 0x2B": {name:"highPassFilterButton", channel:2, magic:true},
    "0x91, 0x29": {name:"highPassFilterButton", channel:3, magic:true},
    "0x81, 0x29": {name:"highPassFilterButton", channel:3, magic:true},
    "0x91, 0x2B": {name:"highPassFilterButton", channel:4, magic:true},
    "0x81, 0x2B": {name:"highPassFilterButton", channel:4, magic:true},

    "0xB0, 0x09": {name:"lowPassFilter", channel:1, magic:false},
    "0xB0, 0x0B": {name:"lowPassFilter", channel:2, magic:false},
    "0xB1, 0x09": {name:"lowPassFilter", channel:3, magic:false},
    "0xB1, 0x0B": {name:"lowPassFilter", channel:4, magic:false},

    "0x90, 0x2D": {name:"lowPassFilterButton", channel:1, magic:true},
    "0x80, 0x2D": {name:"lowPassFilterButton", channel:1, magic:true},
    "0x90, 0x2F": {name:"lowPassFilterButton", channel:2, magic:true},
    "0x80, 0x2F": {name:"lowPassFilterButton", channel:2, magic:true},
    "0x91, 0x2D": {name:"lowPassFilterButton", channel:3, magic:true},
    "0x81, 0x2D": {name:"lowPassFilterButton", channel:3, magic:true},
    "0x91, 0x2F": {name:"lowPassFilterButton", channel:4, magic:true},
    "0x81, 0x2F": {name:"lowPassFilterButton", channel:4, magic:true},

    "0xB0, 0x05": {name:"resonance", channel:1, magic:false},
    "0xB0, 0x07": {name:"resonance", channel:2, magic:false},
    "0xB1, 0x05": {name:"resonance", channel:3, magic:false},
    "0xB1, 0x07": {name:"resonance", channel:4, magic:false},

    "0xB0, 0x0C": {name:"lowEQ", channel:1, magic:false},
    "0xB0, 0x0E": {name:"lowEQ", channel:2, magic:false},
    "0xB1, 0x0C": {name:"lowEQ", channel:3, magic:false},
    "0xB1, 0x0E": {name:"lowEQ", channel:4, magic:false},

    "0xB0, 0x08": {name:"midEQ", channel:1, magic:false},
    "0xB0, 0x0A": {name:"midEQ", channel:2, magic:false},
    "0xB1, 0x08": {name:"midEQ", channel:3, magic:false},
    "0xB1, 0x0A": {name:"midEQ", channel:4, magic:false},

    "0xB0, 0x04": {name:"highEQ", channel:1, magic:false},
    "0xB0, 0x06": {name:"highEQ", channel:2, magic:false},
    "0xB1, 0x04": {name:"highEQ", channel:3, magic:false},
    "0xB1, 0x06": {name:"highEQ", channel:4, magic:false},

    "0xB0, 0x00": {name:"gain", channel:1, magic:false},
    "0xB0, 0x02": {name:"gain", channel:2, magic:false},
    "0xB1, 0x00": {name:"gain", channel:3, magic:false},
    "0xB1, 0x02": {name:"gain", channel:4, magic:false},

    "0x90, 0x34": {name:"gainButton", channel:1, magic:false},
    "0x90, 0x36": {name:"gainButton", channel:2, magic:false},
    "0x91, 0x34": {name:"gainButton", channel:3, magic:false},
    "0x91, 0x36": {name:"gainButton", channel:4, magic:false},
}

XoneK2.bindOutputsToFunctions = function () {
    for (i = 1; i <= 4; i++) {
        script.bindConnections("[Channel"+i+"]", XoneK2.outputsToFunctions, false);
        script.bindConnections("[EqualizerRack1_[Channel"+i+"]_Effect1]", XoneK2.eqOutputsToFunctions, false);
    }
}

XoneK2.outputsToFunctions = {
    "PeakIndicator" : "XoneK2.gainLeds",
    "pregain" : "XoneK2.gainLeds",
    "beat_active" : "XoneK2.blink",
}

XoneK2.eqOutputsToFunctions = {
    "parameter3" : "XoneK2.gainLeds",
    "parameter2" : "XoneK2.gainLeds",
    "parameter1" : "XoneK2.gainLeds",
}










/**********************************************************************/
/*                              B l i n k                             */
/**********************************************************************/

XoneK2.blink = function(value, group, control) {
    // Check which deck to trust.
    var master = 0;
    var master_bpm = 0;
    for (var i = 1; i <= 4; i++) {
        var play = engine.getValue("[Channel"+i+"]", "play");
        master_bpm = engine.getValue("[Channel"+i+"]", "bpm");
        var volume = engine.getParameter("[Channel"+i+"]", "volume");
        if (play && master_bpm >= 30 && master_bpm <= 240 && volume >= 0.25) {
            master = i;
            break;
        }
    }
    var channel = XoneK2.channelGroupToChannel(group);
    if (master == 0) {
        // put everything to its static led correct value
    } else {
        if (master == channel && value == 1) {
            //do things


            if (engine.beginTimer(60000/master_bpm/2,"XoneK2.blinkOff()",true) == 0) {
                print("blink off timer setup failed");
            }
            return;
        }
    }
}

XoneK2.blinkOff = function() {
    for (var led in XoneK2.leds) {
        if (led.blink) {
            midi.sendShortMsg(led.midichannel, led.midicode, 0x00);
        }
    }
}










/**********************************************************************/
/*                            F i l t e r s                           */
/**********************************************************************/

XoneK2.kickFilter = function (channel, value) {
    var parameter = XoneK2.expCurve(value, 0, 127, 0.5, 0.65, 1);
    engine.setParameter("[QuickEffectRack1_[Channel" + channel + "]]", "super1", parameter);
}

XoneK2.highPassFilterEnabled = [false, false, false, false];
XoneK2.highPassFilterValue = [0, 0, 0, 0];

XoneK2.highPassFilter = function (channel, value) {
    XoneK2.highPassFilterValue[channel-1] = XoneK2.expCurve(value, 0, 127, 0, 1, 1);
    if (XoneK2.highPassFilterEnabled[channel-1]) {
        engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter3", XoneK2.highPassFilterValue[channel-1]);
    }
}

XoneK2.highPassFilterLeds = function(channel) {
    var midichannel, midicode;
    if (channel == 1 || channel == 2) {
        midichannel = 1;
    } else {
        midichannel = 2;
    }
    if (channel == 1 || channel == 3) {
        midicode = 0x29;
    } else {
        midicode = 0x2B;
    }
    return {"midichannel":midichannel, "midicode":midicode};
}

XoneK2.highPassFilterButton = function(channel, value) {
    if (value == 0) {
        // Is Note off
        return;
    }
    channel = XoneK2.channelOrder[channel-1];
    XoneK2.highPassFilterEnabled[channel-1] = !XoneK2.highPassFilterEnabled[channel-1];
    var led = XoneK2.highPassFilterLeds(channel);
    if (XoneK2.highPassFilterEnabled[channel-1]) {
        engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter3", XoneK2.highPassFilterValue[channel-1]);
        midi.sendShortMsg(0x90 + led.midichannel - 1, led.midicode, 0x7F);
    } else {
        engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter3", 0);
        midi.sendShortMsg(0x80 + led.midichannel - 1, led.midicode, 0x00);
    }
}

XoneK2.lowPassFilterEnabled = [false, false, false, false];
XoneK2.lowPassFilterValue = [0, 0, 0, 0];

XoneK2.lowPassFilter = function (channel, value) {
    XoneK2.lowPassFilterValue[channel-1] = XoneK2.expCurve(value, 0, 127, 0, 1, 1);
    if (XoneK2.lowPassFilterEnabled[channel-1]) {
        engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter1", XoneK2.lowPassFilterValue[channel-1]);
    }
}

XoneK2.lowPassFilterLeds = function(channel) {
    var midichannel, midicode;
    if (channel == 1 || channel == 2) {
        midichannel = 1;
    } else {
        midichannel = 2;
    }
    if (channel == 1 || channel == 3) {
        midicode = 0x2D;
    } else {
        midicode = 0x2F;
    }
    return {"midichannel":midichannel, "midicode":midicode};
}

XoneK2.lowPassFilterButton = function(channel, value) {
    if (value == 0) {
        // Is Note off
        return;
    }
    channel = XoneK2.channelOrder[channel-1];
    XoneK2.lowPassFilterEnabled[channel-1] = !XoneK2.lowPassFilterEnabled[channel-1];
    var led = XoneK2.lowPassFilterLeds(channel);
    if (XoneK2.lowPassFilterEnabled[channel-1]) {
        engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter1", XoneK2.lowPassFilterValue[channel-1]);
        midi.sendShortMsg(0x90 + led.midichannel - 1, led.midicode, 0x7F);
    } else {
        engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter1", 1);
        midi.sendShortMsg(0x80 + led.midichannel - 1, led.midicode, 0x00);
    }
}

XoneK2.resonance = function (channel, value) {
    var parameter = XoneK2.expCurve(value, 0, 127, 0, 1, -1.7);
    engine.setParameter("[EffectRack1_EffectUnit" +  channel + "_Effect1]", "parameter2", parameter);
}










/**********************************************************************/
/*                                  E Q                               */
/**********************************************************************/

XoneK2.lowEQ = function (channel, value) {
    var threshold = 1;
    var curve = 1.7;
    if (value >= 63-threshold && value <= 64+threshold) {
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter1", 0.5);
    } else if (value < 63-threshold) {
        var parameter = XoneK2.expCurve(value, 0, 63-threshold, 0, 0.5, -curve);
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter1", parameter);
    } else {
        var parameter = XoneK2.expCurve(value, 64+threshold, 127, 0.5, 1, curve);
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter1", parameter);
    }
}

XoneK2.midEQ = function (channel, value) {
    var threshold = 1;
    var curve = 1.7;
    if (value >= 63-threshold && value <= 64+threshold) {
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter2", 0.5);
    } else if (value < 63-threshold) {
        var parameter = XoneK2.expCurve(value, 0, 63-threshold, 0, 0.5, -curve);
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter2", parameter);
    } else {
        var parameter = XoneK2.expCurve(value, 64+threshold, 127, 0.5, 1, curve);
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter2", parameter);
    }
}

XoneK2.highEQ = function (channel, value) {
    var threshold = 1;
    var curve = 1.7;
    if (value >= 63-threshold && value <= 64+threshold) {
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter3", 0.5);
    } else if (value < 63-threshold) {
        var parameter = XoneK2.expCurve(value, 0, 63-threshold, 0, 0.5, -curve);
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter3", parameter);
    } else {
        var parameter = XoneK2.expCurve(value, 64+threshold, 127, 0.5, 1, curve);
        engine.setParameter("[EqualizerRack1_[Channel" +  channel + "]_Effect1]", "parameter3", parameter);
    }
}

XoneK2.gain = function (channel, value) {
    var step = 0.01;
    var jogValue;
    if (value == 127) {
        jogValue = -1;
    } else {
        jogValue = 1;
    }
    var parameter = engine.getParameter("[Channel" +  channel + "]", "pregain");
    parameter += jogValue*step;
    if (parameter > 1) {
        parameter = 1;
    } else if (parameter < 0) {
        parameter = 0;
    }
    engine.setParameter("[Channel" +  channel + "]", "pregain", parameter);
}

XoneK2.gainButton = function (channel, value) {
    engine.setParameter("[Channel" +  channel + "]", "pregain", 0.5);
}

XoneK2.gainControlToLed = {
    pregain    : {red:"0x34", yellow:"0x58", green:"0x7C",},
    parameter3 : {red:"0x30", yellow:"0x54", green:"0x78",},  // High
    parameter2 : {red:"0x2C", yellow:"0x50", green:"0x74",},  // Mid
    parameter1 : {red:"0x28", yellow:"0x4C", green:"0x70",},  // Low
}

XoneK2.gainLeds = function (value, group, control) {
    var channel = XoneK2.channelGroupToChannel(group);
    if (!channel) {
        channel = XoneK2.eqGroupToChannel(group);
    }
    var clip = engine.getParameter("[Channel"+channel+"]", "PeakIndicator");
    if (clip == 1) {
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["pregain"].red, 0x7F);
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["parameter3"].red, 0x7F);
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["parameter2"].red, 0x7F);
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["parameter1"].red, 0x7F);
    } else {
        var parameter = engine.getParameter("[Channel"+channel+"]", "pregain");
        if (parameter == 0.5) {
            midi.sendShortMsg(0x80 + channel-1, XoneK2.gainControlToLed["pregain"].red, 0x00);
        } else if (parameter < 0.5) {
            midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["pregain"].green, 0x7F);
        } else {
            midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["pregain"].yellow, 0x7F);
        }

        XoneK2.gainLedsEQ(channel, "parameter3");
        XoneK2.gainLedsEQ(channel, "parameter2");
        XoneK2.gainLedsEQ(channel, "parameter1");
    }
}

XoneK2.gainLedsEQ = function(channel, control) {
    var parameter = engine.getParameter("[EqualizerRack1_[Channel"+channel+"]_Effect1]", control);
    var t = 0.05;
    if (parameter >= 0.5-t && parameter <= 0.5+t) {
        midi.sendShortMsg(0x80 + channel-1, XoneK2.gainControlToLed[control].red, 0x00);
    } else if (parameter < 0.5-t) {
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed[control].green, 0x7F);
    } else {
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed[control].yellow, 0x7F);
    }
}

XoneK2.gainLedsClip = function (value, group, control) {
    return;
    var channel = XoneK2.channelGroupToChannel(group);
    if (!channel) {
        channel = XoneK2.eqGroupToChannel(group);
    }
    if (value == 0) {
        var eqGroup = "[EqualizerRack1_[Channel"+channel+"]_Effect1]";

        var currentValue = engine.getValue(eqGroup, "parameter1");
        XoneK2.gainLedsGain(currentValue, eqGroup, "parameter1");

        currentValue = engine.getValue(eqGroup, "parameter2");
        XoneK2.gainLedsGain(currentValue, eqGroup, "parameter2");

        currentValue = engine.getValue(eqGroup, "parameter3");
        XoneK2.gainLedsGain(currentValue, eqGroup, "parameter3");

        currentValue = engine.getValue(group, "pregain");
        XoneK2.gainLedsGain(currentValue, group, "pregain");
    } else {
        // TODO make clip leds fast blinking
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["pregain"].red, 0x7F);
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["parameter3"].red, 0x7F);
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["parameter2"].red, 0x7F);
        midi.sendShortMsg(0x90 + channel-1, XoneK2.gainControlToLed["parameter1"].red, 0x7F);
    }
}

XoneK2.fader = function (channel, value) {
    var parameter = XoneK2.expCurve(value, 0, 127, 0, 1, -1.2);
    engine.setParameter("[Channel" + channel + "]", "volume", parameter);
    engine.setValue("[Channel" + 1 + "]", "test", parameter);
}










/**********************************************************************/
/*                          A u x  m e t h o d s                      */
/**********************************************************************/











/**********************************************************************/
/*                          A u x  m e t h o d s                      */
/**********************************************************************/

XoneK2.expCurve = function (x, x0, x1, y0, y1, a) {
    if (!x0) {
        x0 = 0;
    }
    if (!x1) {
        x1 = 127;
    }
    if (!y0) {
        y0 = 0;
    }
    if (!y1) {
        y1 = 1;
    }
    if (!a) {
        a = 1;
    }
    if (x <= x0) {
        return y0;
    } else if (x >= x1) {
        return y1;
    } else {
        dx = x1-x0;
        dy = y1-y0;
        k = (x-x0)/dx;
        result = dy*(Math.exp(a*k)-1)/(Math.exp(a)-1) + y0;
        return result;
    }
}

XoneK2.convertDecToHexString = function(dec) {
    hex = dec.toString(16).toUpperCase();
    if (hex.length == 1) {
        hex = "0" + hex;
    }
    return "0x" + hex;
}


XoneK2.channelGroupToChannel = function (group) {
    var channel = parseInt(group.charAt(8));
    return XoneK2.channelOrder[channel-1];
}

XoneK2.eqGroupToChannel = function (group) {
    var channel = parseInt(group.charAt(24));
    return XoneK2.channelOrder[channel-1];
}


XoneK2.sleepFor = function(sleepDuration) {
    var now = new Date().getTime();
    while(new Date().getTime() < now + sleepDuration){ /* do nothing */ }
}

