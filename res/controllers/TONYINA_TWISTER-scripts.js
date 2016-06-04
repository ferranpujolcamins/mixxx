/****************************************************************/
/*             Twister K2 MIDI controller script v1.20             */
/*          Copyright (C) 2012, Owen Williams                   */
/*      but feel free to tweak this to your heart's content!    */
/*      For Mixxx version 1.11                                  */
/****************************************************************/

function Twister() {}



/**********************************************************************/
/*                     I n i t i a l i z a t i o n                    */
/**********************************************************************/

// Called when the MIDI device is opened & set up
Twister.init = function(id) {
    print ("Twister id: \""+id+"\" initializing...");

    // Timer needed because otherwise some methods don't get called.
    if (engine.beginTimer(2000,"Twister.customInit()",true) == 0) {
        print("initializer timer setup failed");
    }
}

Twister.customInit = function () {
    engine.setValue("[Channel1]", "bpm_change_time", 30);
    engine.setValue("[Channel1]", "bpm_target", 128);

}

Twister.midiControl = function midiControl(channel, code) {
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

Twister.shutdown = function(id) {
}










/**********************************************************************/
/*                  M e s s a g e    d i s p a t c h                  */
/**********************************************************************/

Twister.execMidi = function (midichannel, control, value, status) {
    var midi_control = new Twister.midiControl(Twister.convertDecToHexString(status), Twister.convertDecToHexString(control));
    var func = Twister.functionForMidiControl[midi_control.toString()];
    func.value = value;
    Twister[func.name].call(this, func.channel, value);
}

Twister.functionForMidiControl = {
    "0xB0, 0x0E": {name:"setBPMChangeTime", channel:1},
    "0xB0, 0x0F": {name:"setTargetBPM", channel:1},

    "0x91, 0x0F": {name:"changeBPMButton", channel:1},
    "0x81, 0x0F": {name:"changeBPMButton", channel:1},
}






/**********************************************************************/
/*                         B p m  c h a n g e r                       */
/**********************************************************************/

Twister.bpmTargetEncoderFilter = 0;
Twister.bpmTargetEncoderSensitivity = 3;
Twister.setBPMTarget = function(channel, value) {
    if (engine.getValue("[Channel1]", "changingBPM")) {
        return;
    }

    var target = engine.getValue("[Channel1]", "bpm_target");
    if (value < 64) {
        if (Twister.bpmTargetEncoderFilter > 0) {
            Twister.bpmTargetEncoderFilter = 0;
        } else {
            Twister.bpmTargetEncoderFilter--;
        }
        if (Twister.bpmTargetEncoderFilter == -Twister.bpmTargetEncoderSensitivity) {
            target--;
            if (target < 60) {
                target = 60;
            }
            engine.setValue("[Channel1]", "bpm_target", target);
            Twister.bpmTargetEncoderFilter = 0;
        }
    } else {
        if (Twister.bpmTargetEncoderFilter < 0) {
            Twister.bpmTargetEncoderFilter = 0;
        } else {
            Twister.bpmTargetEncoderFilter++;
        }
        if (Twister.bpmTargetEncoderFilter == Twister.bpmTargetEncoderSensitivity) {
            target++;
            if (target > 240) {
                target = 240;
            }
            engine.setValue("[Channel1]", "bpm_target", target);
            Twister.bpmTargetEncoderFilter = 0;
        }
    }
}

Twister.changeTimeEncoderFilter = 0;
Twister.bpmChangeTimeEncoderSensitivity = 3;
Twister.setBPMChangeTime = function(channel, value) {
    if (engine.getValue("[Channel1]", "changingBPM")) {
        return;
    }

    var time = engine.getValue("[Channel1]", "bpm_change_time");
    if (value < 64) {
        if (Twister.changeTimeEncoderFilter > 0) {
            Twister.changeTimeEncoderFilter = 0;
        } else {
            Twister.changeTimeEncoderFilter--;
        }
        if (Twister.changeTimeEncoderFilter == -Twister.bpmChangeTimeEncoderSensitivity) {
            time--;
            if (time < 1) {
                time = 1;
            }
            engine.setValue("[Channel1]", "bpm_change_time", time);
            Twister.changeTimeEncoderFilter = 0;
        }
    } else {
        if (Twister.changeTimeEncoderFilter < 0) {
            Twister.changeTimeEncoderFilter = 0;
        } else {
            Twister.changeTimeEncoderFilter++;
        }
        if (Twister.changeTimeEncoderFilter == Twister.bpmChangeTimeEncoderSensitivity) {
            time++;
            if (time > 240) {
                time = 240;
            }
            engine.setValue("[Channel1]", "bpm_change_time", time);
            Twister.changeTimeEncoderFilter = 0;
        }
    }
}

Twister.bpmChangeTimerID = 0;
Twister.bpmChangeTimerResolution = 50;
Twister.changeBPMButton = function(channel, value) {
    if (value == 0) {
        // Is note off
        return;
    }

    if (engine.getValue("[Channel1]", "changingBPM")) {
        engine.stopTimer(Twister.bpmChangeTimerID);
    } else {
        Twister.bpmChangeTimerID = engine.beginTimer(Twister.bpmChangeTimerResolution,"Twister.bpmChanger()",false) == 0);
        if (Twister.bpmChangeTimerID == 0) {
            print("bpm changer timer setup failed");
        }
        engine.setValue("[Channel1]", "changingBPM", true);
    }
}

Twister.bpmChanger = function() {
    var time = engine.getValue("[Channel1]", "bpm_change_time");
    time -= Twister.bpmChangeTimerResolution;

    if (time < 0) {
        // TODO reset time to initial value
        engine.setValue("[Channel1]", "bpm_change_countdown", 0);
        engine.stopTimer(Twister.bpmChangeTimerID);
    }
}








/**********************************************************************/
/*                         A u x  m e t h o d s                       */
/**********************************************************************/

Twister.convertDecToHexString = function(dec) {
    hex = dec.toString(16).toUpperCase();
    if (hex.length == 1) {
        hex = "0" + hex;
    }
    return "0x" + hex;
}


Twister.channelGroupToChannel = function (group) {
    var channel = parseInt(group.charAt(8));
    return channel;
}

Twister.eqGroupToChannel = function (group) {
    var channel = parseInt(group.charAt(24));
    return channel;
}


Twister.sleepFor = function(sleepDuration) {
    var now = new Date().getTime();
    while(new Date().getTime() < now + sleepDuration){ /* do nothing */ }
}

