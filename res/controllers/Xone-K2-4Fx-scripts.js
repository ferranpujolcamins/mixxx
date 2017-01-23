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
'use strict';

/**
A Control is a JavaScript object that represents a physical component on a controller, such as a
button, knob, encoder, or fader. It encapsulates all the information needed to receive MIDI input
from that component and send MIDI signals out to the controller to activate its LED(s). It provides
generic functions that can be made to work for most use cases just by changing some attributes
of the Control, without having to write many or any custom functions.

Controls should generally be properties of a ControlContainer object, which provides functions for
conveniently iterating over a collection of related Controls. Most Controls should be properties of
a custom Deck object, which is a derivative of ControlContainer. Refer to the Deck documentation for
more details and an example.

The input function needs to be mapped to the incoming MIDI signals in the XML file. For example:
<control>
    <group>[Channel1]</group>
    <!-- MyController.leftDeck would be an instance of a custom Deck. -->
    <key>MyController.leftDeck.quantizeButton.input</key>
    <status>0x90</status>
    <midino>0x01</midino>
    <options>
        <script-binding/>
    </options>
</control>
The output does not need to be mapped in XML. It is handled by the library in JavaScript.

A handful of derivative Control objects are available that are more convenient for common use cases.
These derivative objects will cover most use cases. In practice, most Controls are derivatives
of the Button or Pot Controls. Only if you need to make a lot of changes to the default Control
attributes should you use the Control constructor directly.

Create Controls by calling the constructor with JavaScript's "new" keyword. The Control constructor
takes a single argument. This is an options object containing properties that get merged with the
Control when it is created, making it easy to customize the functionality of the Control. Most
Controls will need at least their midi, group, inCo, and outCo attributes specified.

The midi attribute is a two member array corresponding to the first two MIDI bytes that the
controller sends/receives when the physical component changes state. Currently, this is only used
to send out MIDI messages and is not relevant for receiving input because that is handled by
the XML file. The group property specifies the group that both the inCo and outCo manipulate, for
example '[Channel1]' for deck 1. The inCo property is the name of the Mixxx ControlObject (see
http://mixxx.org/wiki/doku.php/mixxxcontrols for a list of them) that this JavaScript Control
manipulates when it receives a MIDI input signal. When the Mixxx CO specified by outCo changes, this
JavaScript Control sends MIDI signals back out to the controller. For example:

var quantizeButton = new Button({
    midi: [0x91, 0x01],
    group: '[Channel1]'
    inCo: 'quantize',
    outCo: 'quantize'
});

The output callback is automatically connected by the constructor function if the outCo, group,
and midi properties are specified to the constructor (unless the outConnect property is set to false
to intentionally avoid that). This makes it easy to map the controller so its LEDs stay synchronized
with the status of Mixxx, whether the outCo changes because of the Control receiving MIDI input or
the user changing it with the keyboard, mouse, or another controller. The output callback can be
easily connected and disconnected by calling the Control's connect() and disconnect() functions. The
output callback can also be manually run with the appropriate arguments simply by calling the
Control's trigger() function. The connect(), disconnect(), and trigger() functions are automatically
called by ControlContainer's reconnectControls and applyLayer functions to make activating different
layers of functionality easy.

Controls can be used to manage alternate behaviors in different conditions. The most common use case
for this is for shift buttons. For that case, assign functions to the shift and unshift properties
that manipulate the Control appropriately. In some cases, using the shift/unshift functions to
change the Control's inCo, outCo, or group properties will be sufficient. Refer to HotcueButton for
an example. In more complex cases, changing input() and output() may be required. Refer to
SamplerButton and EffectUnit for examples. To avoid redundancy (like typing the name of the inCo
both as the inCo property and in the unshift function), the Control constructor will automatically
call the unshift function if it exists. The shift() and unshift() functions of ControlContainer will
call the appropriate function of all the Controls within it that have that function defined
and will recursively decend into ControlContainers that are properties of the parent
ControlContainer.


Control and its derivative objects use constructor functions with a minimal amount of logic. Most of
the functionality of Controls comes from their prototype objects. In JavaScript, making a change to
an object's prototype immediately changes all existing and future objects that have it in their
prototype chain (regardless of the context in which the derivative objects were created). This
makes it easy to change the behavior for all (of a subtype) of Control to accomodate the MIDI
signals used by a particular controller. For example, the Hercules P32 controller sends and receives
two sets of MIDI signals for most physical components, one for when the shift button is pressed and
one for when the shift button is not pressed. The controller changes the state of its LEDs when the
shift buttons are pressed, which is controlled by the alternate set of MIDI signals. These alternate
MIDI signals are the same as the unshifted ones, but the MIDI channel is 3 higher. So, to avoid
having the LEDs flicker when the shift button is pressed or having to define separate JavaScript
Controls for every physical controller component in its shifted and unshifted state, the P32's
init function has this code:

Control.prototype.shiftOffset = 3;
Control.prototype.shiftChannel = true;
Button.prototype.sendShifted = true;
This causes the Control.prototype.send function to send both the shifted and unshifted MIDI
signals when the Control's outCo changes. If your controller uses the same MIDI channel but
different MIDI control numbers when a shift button is pressed, set Control.prototype.shiftControl to
true instead of Control.prototype.shiftChannel.


This library provides more convenient shortcuts for common situations. If inCo and outCo are the
same, you can specify 'co' in the options object for the constructor to set both inCo and outCo.
For example:

var quantizeButton = new Button({
    midi: [0x91, 0x01],
    group: '[Channel1]'
    co: 'quantize'
});

Setting the co property after calling the constructor will not automatically set inCo and outCo;
you would need to do that manually if necessary.

Also, if a Control only needs its midi property specified for its constructor, this can be provided
simply as an array without wrapping it in an object. For example:

var playButton = new PlayButton([0x90 + channel, 0x0A]);
instead of
var playButton = new PlayButton({
    midi: [0x90 + channel, 0x0A]
});

To avoid typing out the group for the constructor of each Control, Controls that share a group can
be part of a ControlContainer and the ControlContainer's reconnectControls method can assign the
group to all of them. Refer to the Deck ControlContainer documentation for an example.
**/
//
var Control = function (options) {
    if (Array.isArray(options) && typeof options[0] === 'number') {
        this.midi = options;
    } else {
        _.assign(this, options);
    }

    if (typeof this.unshift === 'function') {
        this.unshift();
    }
    // These cannot be in the prototype; they must be unique to each instance.
    this.isShifted = false;
    this.connections = [];

    if (options !== undefined && typeof options.co === 'string') {
        this.inCo = options.co;
        this.outCo = options.co;
    }

    if (this.outConnect && this.group !== undefined && this.outCo !== undefined) {
        this.connect();
        if (this.outTrigger) {
            this.trigger();
        }
    }
};
Control.prototype = {
    // default attributes
    // You should probably overwrite at least some of these.
    inValueScale: function (value) {
        return value / this.max;
    },
    // map input in the XML file, not inValueScale
    input: function (channel, control, value, status, group) {
        this.setParameter(this.inValueScale(value));
    },
    max: 127, // for MIDI. When adapting for HID this may change.
    outValueScale: function (value) {return value * this.max;},
    output: function (value, group, control) {
        this.send(this.outValueScale(value));
    },
    outConnect: true,
    outTrigger: true,

    // common functions
    // In most cases, you should not overwrite these.
    setValue: function (value) {
        engine.setValue(this.group, this.inCo, value);
    },
    setParameter: function (value) {
        print (this.inCo);
        engine.setParameter(this.group, this.inCo, value);
    },
    // outCo value generally shouldn't be set directly,
    // only by the output() callback when its value changes,
    // or by calling trigger()
    // so don't provide separate setValueIn/setValueOut functions.
    getValueIn: function () {
        return engine.getValue(this.group, this.inCo);
    },
    getValueOut: function () {
        return engine.getValue(this.group, this.outCo);
    },
    getParameterIn: function () {
        return engine.getParameter(this.group, this.inCo);
    },
    getParameterOut: function () {
        return engine.getParameter(this.group, this.outCo);
    },
    toggle: function () {
        this.setValue( ! this.getValueIn());
    },
    connect: function () {
        /**
        Override this method with a custom one to connect multiple Mixxx COs for a single Control.
        Add the connection objects to the this.connections array so they all get disconnected just
        by calling this.disconnect(). This can be helpful for multicolor LEDs that show a
        different color depending on the state of different Mixxx COs. See SamplerButton.connect()
        and SamplerButton.output() for an example.
        **/
        if (undefined !== this.group &&
            undefined !== this.outCo &&
            undefined !== this.output &&
            typeof this.output === 'function') {
            this.connections[0] = engine.connectControl(this.group, this.outCo, this.output);
        }
    },
    disconnect: function () {
        if (this.connections[0] !== undefined) {
            this.connections.forEach(function (connection) {
                connection.disconnect();
            });
        }
    },
    trigger: function() {
        engine.trigger(this.group, this.outCo);
    },
    shiftOffset: 0,
    sendShifted: false,
    shiftChannel: false,
    shiftControl: false,
    send: function (value) {
        if (this.midi === undefined || this.midi[0] === undefined || this.midi[1] === undefined) {
            return;
        }
        midi.sendShortMsg(this.midi[0], this.midi[1], value);
        if (this.sendShifted) {
            if (this.shiftChannel) {
                midi.sendShortMsg(this.midi[0] + this.shiftOffset, this.midi[1], value);
            } else if (this.shiftControl) {
                midi.sendShortMsg(this.midi[0], this.midi[1] + this.shiftOffset, value);
            }
        }
    },
};

/**
A Button is a Control derivative for buttons/pads.

For example:
var quantize = new Button({
    midi: [0x91, 0x01],
    group: '[Channel1]',
    co: 'quantize',
});

By default, the inCo is toggled only when the button is pressed. For buttons that activate an inCo
only while they are held down, set the onlyOnPress property to false. For example:
var tempSlow = new Button({
    midi: [0x91, 0x44],
    inCo: 'rate_temp_down',
    onlyOnPress: false,
});

The button's LED is sent the value of the "on" property when outCo > 0 and "off" when outCo <= 0.
By default, on is 127 and off is 0. For buttons/pads with multicolor LEDs, you can change the color
of the LED by defining the on and off properties to be the MIDI value to send for that state. For
example, if the LED turns red when sent a MIDI value of 127 and blue when sent a value of 126:

MyController.padColors = {
    red: 127,
    blue: 126
};
MyController.quantize = new Button({
    midi: [0x91, 0x01],
    group: '[Channel1]',
    co: 'quantize',
    on: MyController.padColors.red,
    off: MyController.padColors.blue,
});

Derivative Buttons are provided for many common use cases, including:
PlayButton
CueButton
SyncButton
LoopToggleButton
HotcueButton
SamplerButton
These make it easy to map those kinds of buttons without having to worry about particularities
of Mixxx's ControlObjects that can make mapping them not so straightforward. The PlayButton,
SyncButton, HotcueButton, and SamplerButton objects also provide alternate functionality for when a
shift button is pressed.

By default, this works for controllers that send MIDI messages with a different 3rd byte of the
MIDI message (value) to indicate the button being pressed/released, with the first two bytes
(status and control) remaining the same for both press and release. If your controller sends
separate MIDI note on/off messages with on indicated by the first nybble (hexadecimal digit) of
the first (status) byte being 9 and note off with the first nybble being 8, in your script's init
function, set Button.prototype.separateNoteOnOff to true and map both the note on and off messages
in XML to the Button object's input property.
**/
var Button = function (options) {
    Control.call(this, options);
};
Button.prototype = new Control({
    onlyOnPress: true,
    on: 127,
    off: 0,
    inValueScale: function () { return ! this.getValueIn(); },
    separateNoteOnOff: false,
    input: function (channel, control, value, status, group) {
        if (this.onlyOnPress) {
            var pressed;
            if (this.separateNoteOnOff) {
                // Does the first nybble of the first MIDI byte indicate a
                // note on or note off message?
                pressed = (status & 0xF0) === 0x90;
            } else {
                pressed = value > 0;
            }
            if (pressed) {
                this.setValue(this.inValueScale(value));
            }
        } else {
            this.setValue(this.inValueScale(value));
        }
    },
    outValueScale: function() {
        return (this.getValueOut()) ? this.on : this.off;
    },
});

/**
PlayButton
Default behavior: play/pause
Shift behavior: go to start of track and stop

LED behavior depends on cue mode selected by the user in the preferences
Refer to http://mixxx.org/manual/latest/chapters/user_interface.html#interface-cue-modes
**/
var PlayButton = function (options) {
    Button.call(this, options);
};
PlayButton.prototype = new Button({
    unshift: function () {
        this.inCo = 'play';
    },
    shift: function () {
        this.inCo = 'start_stop';
    },
    outCo: 'play_indicator'
});

/**
CueButton
Behavior depends on cue mode configured by the user in the preferences
Refer to http://mixxx.org/manual/latest/chapters/user_interface.html#interface-cue-modes
**/
var CueButton = function (options) {
    Button.call(this, options);
};
CueButton.prototype = new Button({
    inCo: 'cue_default',
    outCo: 'cue_indicator',
    onlyOnPress: false
});

/**
SyncButton
Default behavior: momentary sync without toggling sync lock
Shift behavior: toggle sync lock (master sync)
**/
var SyncButton = function (options) {
    Button.call(this, options);
};
SyncButton.prototype = new Button({
    unshift: function () {
        this.inCo = 'beatsync';
    },
    shift: function () {
        this.inCo = 'sync_enabled';
    },
    outCo: 'sync_enabled'
});

// Toggle a loop on/off
var LoopToggleButton = function (options) {
    Button.call(this, options);
};
LoopToggleButton.prototype = new Button({
    inCo: 'reloop_exit',
    inValueScale: function () {
        return 1;
    },
    outCo: 'loop_enabled',
    outValueScale: function (value) {
        return (value) ? this.on : this.off;
    }
});

/**
HotcueButton
Default behavior: set hotcue if it is not set. If it is set, jump to it.
Shift behavior: delete hotcue

The LED indicates whether the hotcue is set.

Pass the number of the hotcue as the number property of the options argument for the constructor.
For example:

var hotcues = [];
for (var i = 1; i <= 8; i++) {
    hotcues[i] = new HotcueButton({
        number: i,
        group: '[Channel1]',
        midi: [0x91, 0x26 + i],
    });
}

**/
var HotcueButton = function (options) {
    if (options.number === undefined) {
        print('WARNING: No hotcue number specified for new HotcueButton.');
    }
    this.number = options.number;
    this.outCo = 'hotcue_' + this.number + '_enabled';
    Button.call(this, options);
};
HotcueButton.prototype = new Button({
    unshift: function () {
        this.inCo = 'hotcue_' + this.number + '_activate';
    },
    shift: function () {
        this.inCo = 'hotcue_' + this.number + '_clear';
    },
    onlyOnPress: false
});

/**
SamplerButton
Default behavior:
Press the button to load the track selected in the library into an empty sampler. Press a loaded
sampler to play it from its cue point. Press again while playing to jump back to the cue point.
Shift behavior:
If the sampler is playing, stop it. If the sampler is stopped, eject it.

Specify the sampler number as the number property of the object passed to the constructor. There
is no need to manually specify the group. For example:

var samplerButtons = [];
for (var n = 1; n <= 8; n++) {
    samplerButtons[n] = new SamplerButton({
        number: n,
        midi: [0x91, 0x02],
    });
)};

When the sampler is loaded, the LED will be set to the value of the "on" property. When the sampler
is empty, the LED will be set to the value of the "off" property. These are inherited from
Button.prototype if they are not manually specified. If your controller's pads have multicolor LEDs,
specify the value to send for a different LED color with the playing property to set the LED to a
different color while the sampler is playing. For example:

MyController.padColors = {
// These values are just examples, consult the MIDI documentation from your controller's
manufacturer to find the values for your controller. If that information is not available,
guess and check to find the values.
    red: 125,
    blue: 126,
    purple: 127,
    off: 0
};
var samplerButton = [];
var samplerButton[1] = new SamplerButton(
    midi: [0x91, 0x02],
    number: 1,
    on: MyController.padColors.blue,
    playing: MyController.padColors.red,
    // off is inherited from Button.prototype
)};
**/
var SamplerButton = function (options) {
    if (options.number === undefined) {
        print('WARNING: No sampler number specified for new SamplerButton.');
    }
    this.number = options.number;
    this.group = '[Sampler' + this.number + ']';
    Button.call(this, options);
};
SamplerButton.prototype = new Button({
    unshift: function () {
        this.input = function (channel, control, value, status, group) {
            if (value > 0) {
                // track_samples is 0 when the sampler is empty and > 0 when a sample is loaded
                if (engine.getValue(this.group, 'track_samples') === 0) {
                    engine.setValue(this.group, 'LoadSelectedTrack', 1);
                } else {
                    engine.setValue(this.group, 'cue_gotoandplay', 1);
                }
            }
        };
    },
    shift: function() {
        this.input = function (channel, control, value, status, group) {
            if (value > 0) {
                if (engine.getValue(this.group, 'play') === 1) {
                    engine.setValue(this.group, 'play', 0);
                } else {
                    engine.setValue(this.group, 'eject', 1);
                }
            }
        };
    },
    output: function (value, group, control) {
        if (engine.getValue(this.group, 'track_samples') > 0) {
            if (this.playing === undefined) {
                this.send(this.on);
            } else {
                if (engine.getValue(this.group, 'play') === 1) {
                    this.send(this.on);
                } else {
                    this.send(this.playing);
                }
            }
        } else {
            this.send(this.off);
        }
    },
    connect: function() {
        this.connections[0] = engine.connectControl(this.group, 'track_samples', this.output);
        if (this.playing !== undefined) {
            this.connections[1] = engine.connectControl(this.group, 'play', this.output);
        }
    },
    outCo: null, // hack to get Control constructor to call connect()
});

/**
A Pot is a Control for potentiometers (faders and knobs) with finite ranges, although it can be
adapted for infintely turning encoders. Using a Pot Control is helpful because Pot.connect() and
Pot.disconnect() take care of soft takeover when switching layers with
ControlContainer.reconnectControls() and ControlContainer.applyLayer(). Soft takeover is not
activated until the first input is received so it does not interfere with setting initial values
for controllers that can report that information.

The midi attribute does not need to be specified because Pots do not send any MIDI output. You may
want to specify it anyway to make the code self-documenting.

To adapt a Pot for an infinitely rotating encoder, replace its inValueScale() function with a
function that increments or decrements the parameter depending on the direction the encoder is
turned. For example, if the encoder sends a MIDI value of 1 for a left turn and 127 for a right
turn:

MyController.SomePot.inValueScale = function (value) {
    if (value === 1) {
        return this.getParameterIn() - .05;
    } else if (value === 127) {
        return this.getParameterIn() + .05;
    }
}
**/
var Pot = function (options) {
    Control.call(this, options);

    this.firstValueReceived = false;
};
Pot.prototype = new Control({
    inValueScale: function (value) { return value / this.max; },
    input: function (channel, control, value, status, group) {
        this.setParameter(this.inValueScale(value));
        if (! this.firstValueReceived) {
            this.firstValueReceived = true;
            this.connect();
        }
    },
    connect: function () {
        if (this.firstValueReceived) {
            engine.softTakeover(this.group, this.inCo, true);
        }
    },
    disconnect: function () {
        engine.softTakeoverIgnoreNextValue(this.group, this.inCo);
    },
    trigger: function () {},
});

/**
RingEncoder is a Control for encoders with LED rings around them. These are different from Pots
because they are sent MIDI messages to keep their LED rings in sync with the state of Mixxx and
do not require soft takeover.

These encoders can often be pushed like a button. Usually, it is best to use a separate Button
Control to handle the MIDI signals from pushing it.

The generic Control code provides everything to implement a RingEncoder. This RingEncoder Control
to be able to use instanceof to separate RingEncoders from other Controls and make code more
self-documenting.
**/
var RingEncoder = function (options) {
    Control.call(this, options);
};
RingEncoder.prototype = new Control();

/**
A ControlContainer is an object that contains Controls as properties, with methods to help
iterate over those Controls. Documentation for each method is inline below.
**/
var ControlContainer = function (initialLayer) {
    if (typeof initialLayer === 'object') {
        this.applyLayer(initialLayer);
    }
};
ControlContainer.prototype = {
    /**
    forEachControl
    Iterate over all Controls in this ControlContainer and perform an operation on them.

    operation, function that takes 1 argument:
    the function to call for each Control. Takes each Control as its first argument.
    "this" in the context of the function refers to the ControlContainer.
    recursive, boolean, optional:
    whether to call forEachControl recursively for each ControlContainer within this
    ControlContainer. Defaults to true if ommitted.
    **/
    forEachControl: function (operation, recursive) {
        if (typeof operation !== 'function') {
            print('ERROR: ControlContainer.forEachContainer requires a function argument');
            return;
        }
        if (recursive === undefined) { recursive = true; }

        var that = this;
        var applyOperationTo = function (obj) {
            if (obj instanceof Control) {
                operation.call(that, obj);
            } else if (recursive && obj instanceof ControlContainer) {
                obj.forEachControl(operation);
            } else if (Array.isArray(obj)) {
                obj.forEach(function (element) {
                    applyOperationTo(element);
                });
            }
        };

        for (var memberName in this) {
            if (this.hasOwnProperty(memberName)) {
                applyOperationTo(this[memberName]);
            }
        }
    },
    /**
    reconnectControls
    Disconnect and reconnect output callbacks for each Control. Optionally perform an operation
    on each Control between disconnecting and reconnecting the output callbacks. Arguments are
    the same as forEachControl().
    **/
    reconnectControls: function (operation, recursive) {
        this.forEachControl(function (control) {
            control.disconnect();
            if (typeof operation === 'function') {
                operation.call(this, control);
            }
            control.connect();
            control.trigger();
        }, recursive);
    },
    isShifted: false,
    /**
    shift
    Call each Control's shift() function if it exists. This iterates recursively on any
    Controls in ControlContainers that are properties of this, so there is no need to call
    shift() on each child ControlContainer.
    **/
    shift: function () {
        this.forEachControl(function (control) {
            if (typeof control.shift === 'function') {
                control.shift();
            }
            // Set isShifted for child ControlContainers forEachControl is iterating through recursively
            this.isShifted = true;
        });
    },
    /**
    unshift
    Call each Control's unshift() function if it exists. This iterates recursively on any
    Controls in ControlContainers that are properties of this, so there is no need to call
    unshift() on each child ControlContainer.
    **/
    unshift: function () {
        this.forEachControl(function (control) {
            if (typeof control.unshift === 'function') {
                control.unshift();
            }
            // Set isShifted for child ControlContainers forEachControl is iterating through recursively
            this.isShifted = false;
        });
    },
    /**
    applyLayer
    Activate a new layer of functionality. Layers are merely objects with properties to overwrite
    the properties of the Controls within this ControlContainer. Layer objects are deeply merged.
    If a new layer does not define a property for a Control, the Control's old property will be
    retained.

    In the most common case, for providing alternate functionality when a shift button is pressed,
    using applyLayer() is likely overcomplicated and may be slow. Use shift()/unshift() instead.
    applyLayer() may be useful for cycling through more than two alternate layers.

    For example:
    someControlContainer.applyLayer({
        someButton: { inCo: 'alternate inCo' },
        anotherButton: { outCo: 'alternate outCo' }
    });

    By default, the old layer's output callbacks are disconnected and the new layer's output
    callbacks are connected. To avoid this behavior, which would be desirable if you are not
    changing any output functionality, pass false as the second argument to applyLayer().
    **/
    applyLayer: function (newLayer, reconnectControls) {
        if (reconnectControls !== false) {
            reconnectControls = true;
        }
        if (reconnectControls === true) {
            this.forEachControl(function (control) {
                control.disconnect();
            });
        }

        _.merge(this, newLayer);

        if (reconnectControls === true) {
            this.forEachControl(function (control) {
                control.connect();
                control.trigger();
            });
        }
    },
};

script.samplerRegEx = /\[Sampler(\d+)\]/ ;
script.channelRegEx = /\[Channel(\d+)\]/ ;
script.eqKnobRegEx = /\[EqualizerRack1_\[(.*)\]_Effect1\]/ ;
script.quickEffectRegEx = /\[QuickEffectRack1_\[(.*)\]\]/ ;

/**
Deck
This is a ControlContainer with methods for conveniently changing the group attributes of
contained Controls to switch the deck that a set of Controls is manipulating. The setCurrentDeck()
method takes the new deck as a string and sets the Controls' group property appropriately, including
for equalizer knobs and QuickEffect (filter) knobs.

The Deck constructor takes one argument, which is an array of deck numbers to cycle through with the\
toggle() method. Typically this will be [1, 3] or [2, 4].

To map your own controller, create a custom derivative of Deck and create instances of your custom
Deck objects in your controller's init() function. Use a constructor function to create all the
Controls you need for your particular controller and assign your custom derivative's prototype
to Deck. For example:

MyController.init = function () {
    this.leftDeck = new MyController.Deck([1, 2]);
    this.rightDeck = new MyController.Deck([2, 4]);
};
MyController.Deck = function (deckNumbers, midiChannel) {
    // Call the Deck constructor to setup the currentDeck and deckNumbers properties.
    Deck.call(this, deckNumbers);
    this.playButton = new PlayButton([0x90 + midiChannel, 0x01]);
    this.CueButton = new CueButton([0x90 + midiChannel, 0x02]);
    this.hotcueButtons = [];
    for (var i = 1; i <= 8; i++) {
        this.hotcueButtons[i] = new HotcueButton({
            midi: [0x90 + midiChannel, 0x10 + i],
            number: i
        });
    }
    // ... define as many other Controls as necessary ...

    // Set the group properties of the above Controls and connect their output callback functions
    // Without this, the group property for each Control would have to be specified to its
    // constructor.
    this.reconnectControls(function (c) {
        if (c.group === undefined) {
            // 'this' inside a function passed to reconnectControls refers to the ControlContainer.
            c.group = this.currentDeck;
        }
    });
};
MyController.Deck.prototype = new Deck();
**/
var Deck = function (deckNumbers) {
    if (deckNumbers !== undefined && Array.isArray(deckNumbers)) {
        // These must be unique to each instance, so they cannot be in the prototype.
        this.currentDeck = '[Channel' + deckNumbers[0] + ']';
        this.deckNumbers = deckNumbers;
    }
};
Deck.prototype = new ControlContainer({
    setCurrentDeck: function (newGroup) {
        this.currentDeck = newGroup;
        this.reconnectControls(function (control) {
            if (control.group.search(script.channelRegEx) !== -1) {
                control.group = this.currentDeck;
            } else if (control.group.search(script.eqKnobRegEx) !== -1) {
                control.group = '[EqualizerRack1_' + this.currentDeck + '_Effect1]';
            } else if (control.group.search(script.quickEffectRegEx) !== -1) {
                control.group = '[QuickEffectRack1_' + this.currentDeck + ']';
            }
            // Do not alter the Control's group if it does not match any of those RegExs because
            // that could break effects Controls.
        });
    },
    toggle: function () {
        var index = this.deckNumbers.indexOf(parseInt(
            script.channelRegEx.exec(this.currentDeck)[1]
        ));
        if (index === (this.deckNumbers.length - 1)) {
            index = 0;
        } else {
            index += 1;
        }
        this.setCurrentDeck("[Channel" + this.deckNumbers[index] + "]");
    }
});

/**
EffectUnit
This ControlContainer provides Controls designed to be mapped to the common arrangement of 4 knobs
and 4 buttons for controlling effects. 3 knobs are used for controlling effect metaknobs
or parameters, depending on whether the effects' parameters are shown. The other knob is used for
the dry/wet knob of the whole chain or the superknob when shift is pressed. 3 buttons are used for
enabling effects and the other button toggles the effect unit between hiding and showing effect
parameters. The Controls provided are:

dryWetKnob (Pot)
showParametersButton (Button)
enableButtons[1-3] (ControlContainer of Buttons)
knobs[1-3] (ControlContainer of Pots)
enableOnChannelButtons (ControlContainer of Buttons)

When the effect unit is showing the metaknobs of the effects but not each parameter, the knobs
control the metaknobs. The enableButtons control whether each effect is enabled. Pressing an
enableButton with shift switches to the next available effect.

When the effect unit is showing all the parameters, the knobs behave differently depending on
whether an effect is focused. When there is no focused effect (the default state), the knobs control
the effect metaknobs like they do when parameters are not showing. When an effect is focused, the
knobs control the first 3 parameters of the focused effect. An effect can be focused by pressing
shift + its enableButton or clicking the focus button on screen. Pressing shift + the enableButton
for the focused effect again unfocuses the effect.

The enableOnChannelButtons allow assigning the effect unit to different channels and are named after
the Mixxx channel they affect. Not all controllers have buttons to map these. The following Buttons
are provided by default:
Channel1
Channel2
Channel3
Channel4
Headphones
Master
Microphone
Auxiliary1
You can easily add more, for example for additional microphones, auxiliary inputs, or samplers by
calling enableOnChannelButtons.addButton('CHANNEL_NAME') (do not put brackets around the
CHANNEL_NAME).

To map an EffectUnit for your controller, call the constructor with the unit number of the effect
unit as the only argument. Then, set the midi attributes for the showParametersButton,
enableButtons[1-3], and optionally enableOnChannelButtons (setting the midi attributes for the Pots
is not necessary because they do not send any output). After the midi attributes are set up, call
EffectUnit.init() to set up the output callbacks. For example:

MyController.effectUnit = new EffectUnit(1);
MyController.effectUnit.enableButtons[1].midi = [0x90, 0x01];
MyController.effectUnit.enableButtons[2].midi = [0x90, 0x02];
MyController.effectUnit.enableButtons[3].midi = [0x90, 0x03];
MyController.effectUnit.showParametersButton.midi = [0x90, 0x04];
MyController.effectUnit.enableOnChannelButtons.Channel1 = [0x90, 0x05];
MyController.effectUnit.enableOnChannelButtons.Channel2 = [0x90, 0x06];
MyController.effectUnit.init();

Controllers designed for Serato and Rekordbox often have an encoder instead of a dry/wet knob
(labeled "Beats" for Serato or "Release FX" for Rekordbox) and a button labeled "Tap". If the
encoder sends a MIDI signal when pushed, it is recommended to map the encoder push to the
EffectUnit's showParametersButton, otherwise map that to the "Tap" button. To use the dryWetKnob
Pot with an encoder, replace its inValueScale() function with a function that can appropriately
handle the signals sent by your controller. Refer to the Pot documentation for an example.

For the shift functionality to work, the shift button of your controller must be mapped to a
function that calls the shift()/unshift() functions of the EffectUnit on button press/release. If
the EffectUnit is a property of another ControlContainer (for example a Deck), calling shift()
and unshift() on the parent ControlContainer will recursively call it on the EffectUnit too (just
like it will for any other ControlContainer).
**/
EffectUnit = function (unitNumber) {
    var eu = this;
    this.group = '[EffectRack1_EffectUnit' + unitNumber + ']';

    this.dryWetKnob = new Pot({
        group: this.group,
        unshift: function () {
            this.inCo = 'mix';
            // for soft takeover
            this.disconnect();
            this.connect();
        },
        shift: function () {
            this.inCo = 'super1';
            // for soft takeover
            this.disconnect();
            this.connect();
            eu.knobs.reconnectControls();
        },
        outConnect: false,
    });

    this.enableOnChannelButtons = new ControlContainer();
    this.enableOnChannelButtons.addButton = function (channel) {
        this[channel] = new Button({
            group: eu.group,
            co: 'group_[' + channel + ']_enable',
            outConnect: false,
        });
    };
    this.enableOnChannelButtons.addButton('Channel1');
    this.enableOnChannelButtons.addButton('Channel2');
    this.enableOnChannelButtons.addButton('Channel3');
    this.enableOnChannelButtons.addButton('Channel4');
    this.enableOnChannelButtons.addButton('Headphone');
    this.enableOnChannelButtons.addButton('Master');
    this.enableOnChannelButtons.addButton('Microphone');
    this.enableOnChannelButtons.addButton('Auxiliary1');

    this.EffectUnitKnob = function (number) {
        this.number = number;
        Pot.call(this);
    };
    this.EffectUnitKnob.prototype = new Pot({
        onParametersHide: function () {
            this.group = '[EffectRack1_EffectUnit' + unitNumber + '_Effect' + this.number + ']';
            this.inCo = 'meta';
        },
        onParametersShow: function () {
            var focused_effect = engine.getValue(eu.group, "focused_effect");
            if (focused_effect === 0) {
                // manipulate metaknobs
                this.onParametersHide();
            } else {
                this.group = '[EffectRack1_EffectUnit' + unitNumber + '_Effect' +
                              focused_effect + ']';
                this.inCo = 'parameter' + this.number;
            }
        },
    });

    this.EffectEnableButton = function (number) {
        this.number = number;
        this.group = '[EffectRack1_EffectUnit' + unitNumber + '_Effect' + number + ']';
        Button.call(this);
    };
    this.EffectEnableButton.prototype = new Button({
        onParametersHide: function () {
            this.input = Button.prototype.input;
            this.outCo = 'enabled';
            this.unshift = function () {
                this.isShifted = false;
                this.inCo = 'enabled';
                this.onlyOnPress = true;
            };
            this.shift = function () {
                this.isShifted = true;
                this.inCo = 'next_effect';
                this.onlyOnPress = false;
            };
            if (this.isShifted) {
                this.shift();
            } else {
                this.unshift();
            }
        },
        onParametersShow: function () {
            this.inCo = 'enabled';
            this.outCo = 'enabled';
            this.unshift = function () {
                this.isShifted = false;
                this.input = Button.prototype.input;
                this.onlyOnPress = true;
            };
            this.shift = function () {
                this.isShifted = true;
                this.input = function (channel, control, value, status, group) {
                    if (value > 0) {
                        if (engine.getValue(eu.group, "focused_effect") === this.number) {
                            // focus this effect
                            engine.setValue(eu.group, "focused_effect", 0);
                        } else {
                            // unfocus and make knobs control metaknobs
                            engine.setValue(eu.group, "focused_effect", this.number);
                        }
                    }
                };
            };
            this.connect = function () {
                this.connections[0] = engine.connectControl(this.group, "enabled", Button.prototype.output);
                this.connections[1] = engine.connectControl(eu.group, "focused_effect", this.onFocusChanged);
            };
            this.onFocusChanged = function (value, group, control) {
                if (value === this.number) {
                    // make knobs control first 3 parameters of the focused effect
                    eu.knobs.reconnectControls(function (knob) {
                        if (typeof knob.onParametersShow === 'function') {
                            knob.onParametersShow(); // to set new group property
                        }
                    });
                } else if (value === 0) {
                    // make knobs control metaknobs
                    eu.knobs.reconnectControls(function (knob) {
                        if (typeof knob.onParametersShow === 'function') {
                            knob.onParametersHide(); // to set new group property
                        }
                    });
                }
            };
            if (this.isShifted) {
                this.shift();
            } else {
                this.unshift();
            }
        },
    });

    this.knobs = new ControlContainer();
    this.enableButtons = new ControlContainer();
    for (var n = 1; n <= 3; n++) {
        this.knobs[n] = new this.EffectUnitKnob(n);
        this.enableButtons[n] = new this.EffectEnableButton(n);
    }

    this.showParametersButton = new Button({
        group: this.group,
        co: 'show_parameters',
        output: function (value, group, control) {
            this.send((value > 0) ? this.on : this.off);
            if (value === 0) {
                engine.setValue(this.group, "show_focus", 0);
                // NOTE: calling eu.reconnectControls() here would cause an infinite loop when
                // calling EffectUnit.reconnectControls().
                eu.forEachControl(function (c) {
                    if (typeof c.onParametersHide === 'function') {
                        c.disconnect();
                        c.onParametersHide();
                        c.connect();
                        c.trigger();
                    }
                });
            } else {
                engine.setValue(this.group, "show_focus", 1);
                eu.forEachControl(function (c) {
                    if (typeof c.onParametersShow === 'function') {
                        c.disconnect();
                        c.onParametersShow();
                        c.connect();
                        c.trigger();
                    }
                });
            }
        },
        outConnect: false,
    });

    this.init = function () {
        this.showParametersButton.connect();
        this.showParametersButton.trigger();

        this.enableOnChannelButtons.forEachControl(function (button) {
            if (button.midi !== undefined) {
                button.disconnect();
                button.connect();
                button.trigger();
            }
        });

        this.forEachControl(function (control) {
            if (control.group === undefined) {
                control.group = eu.group;
            }
        });
    };
};
EffectUnit.prototype = new ControlContainer();




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
XoneK2_4Fx.effectUnit1.showParametersButton.midi = [0x9F, 0x34];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel1 = [0x9F, 0x24];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel2 = [0x9F, 0x20];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel3 = [0x9F, 0x1C];
XoneK2_4Fx.effectUnit1.enableOnChannelButtons.Channel4 = [0x9F, 0x18];
XoneK2_4Fx.effectUnit1.init();

