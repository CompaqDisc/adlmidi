<?php
require_once '/WWW/email.php';

//TITLE=OPL3 MIDI player for Linux and Cygwin

$title = 'ADLMIDI: OPL3 MIDI player for Linux and Cygwin';
$progname = 'adlmidi';
$git = 'git://bisqwit.iki.fi/adlmidi.git';

$text = array(
   'purpose:1. Purpose' => "

<div style=\"float:right;width:542px;text-align:justify;margin-left:30px\">
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/programming_examples/midiplay.gif\"
alt=\"adlmidi screenshot\"
title=\"adlmidi playing Tifa theme from FF7\">
<br>
<span style=\"font-size:90%\"
>This screenshot shows ADLMidi running on Linux, seen through
a <a href=\"http://en.wikipedia.org/wiki/Secure_Shell\">SSH</a>
session that runs in <a href=\"http://www.cygwin.com/\">Cygwin</a>
<a href=\"http://en.wikipedia.org/wiki/Xterm\">XTerm</a> on Windows 7.<br></span>
<small style=\"color:#444\">The lovely dithering pattern is generated by
<a href=\"http://bisqwit.iki.fi/source/animmerger.html\">animmerger</a>,
another one of my pet projects&hellip;(The image above has only 16 colors)</small></div>

AdlMIDI is a commandline program that plays MIDI files
using software OPL3 emulation (FM synthesis).

", 'features:1. Key features' => "

<ul>
 <li>OPL3 emulation with four-operator mode support</li>
 <li>FM patches from a number of known PC games, copied from
   files typical to
   AIL = Miles Sound System / DMX / HMI = Human Machine Interfaces
   / Creative IBK.</li>
 <li>Stereo sound</li>
 <li>Reverb filter based on code from <a href=\"http://sox.sourceforge.net/\">SoX</a>,
     based on code from <a href=\"http://freeverb3.sourceforge.net/\">Freeverb</a>.
     A copy of either project is not needed.</li>
 <li>Number of simulated soundcards can be specified as 1-100 (maximum channels 1800!)</li>
 <li>xterm-256color support</li>
 <li>WIN32 console support (also tested with <a href=\"http://www.japheth.de/HX.html\">HXRT / MS-DOS</a>)</li>
 <li>Pan (binary panning, i.e. left/right side on/off)</li>
 <li>Pitch-bender with adjustable range</li>
 <li>Vibrato that responds to RPN/NRPN parameters</li>
 <li>Sustain enable/disable</li>
 <li>MIDI and RMI file support</li>
 <li>loopStart / loopEnd tag support (Final Fantasy VII)</li>
 <li>Use automatic arpeggio with chords to relieve channel pressure</li>
</ul>

<hr>
This project is <b>developed in C++</b>, but a <i>GW-BASIC version</i> is also provided
(<a href=\"http://bisqwit.iki.fi/jutut/kuvat/programming_examples/midiplay.html\"
>MIDIPLAY.BAS</a>).
This player was
<a href=\"http://www.youtube.com/watch?v=ZwcFV3KrnQA\"
>first implemented in GW-BASIC for a Youtube demonstration video</a>!
With alterations that take 15 seconds to implement,
it can be run in QBasic and QuickBASIC too.
Note: The GW-BASIC version does not contain all of the same features that the C++ version does.

", 'usage: 1. Usage' => "

<pre>Usage: midiplay &lt;midifilename> [ &lt;banknumber> [ &lt;numcards> [ &lt;numfourops>] ] ]
    Banks: 0 = AIL (Star Control 3, Albion, Empire 2, Sensible Soccer, Settlers 2, many others)
           1 = HMI (Descent, Asterix)
           2 = HMI (Descent:: Int)
           3 = HMI (Descent:: Ham)
           4 = HMI (Descent:: Rick)
           5 = DMX (Doom           :: partially 4op)
           6 = DMX (Hexen, Heretic :: partially 4op)
           7 = AIL (Warcraft 2)
           8 = AIL (SimFarm, SimHealth :: 4op)
           9 = AIL (SimFarm, Settlers, Serf City)
          10 = AIL (Air Bucks, Blue And The Gray, America Invades, Terminator 2029)
          11 = AIL (Ultima Underworld 2)
          12 = AIL (Caesar 2 :: partially 4op, MISSING INSTRUMENTS)
          13 = AIL (Death Gate)
          14 = AIL (Kasparov's Gambit)
          15 = AIL (High Seas Trader :: MISSING INSTRUMENTS)
          16 = AIL (Discworld, Grandest Fleet, Pocahontas, Slob Zone 3d, Ultima 4, Zorro)
          17 = AIL (Syndicate)
          18 = AIL (Guilty, Orion Conspiracy, Terra Nova Strike Force Centauri :: 4op)
          19 = AIL (Magic Carpet 2)
          20 = AIL (Jagged Alliance)
          21 = AIL (When Two Worlds War :: 4op, MISSING INSTRUMENTS)
          22 = AIL (Bards Tale Construction :: MISSING INSTRUMENTS)
          23 = AIL (Return to Zork)
          24 = AIL (Theme Hospital)
          25 = AIL (Inherit The Earth)
          26 = AIL (Inherit The Earth, file two)
          27 = AIL (Little Big Adventure :: 4op)
          28 = AIL (Wreckin Crew)
          29 = AIL (FIFA International Soccer)
          30 = AIL (Starship Invasion)
          31 = AIL (Super Street Fighter 2 :: 4op)
          32 = AIL (Lords of the Realm :: MISSING INSTRUMENTS)
          33 = AIL (Syndicate Wars)
          34 = AIL (Bubble Bobble Feat. Rainbow Islands, Z)
          35 = AIL (Warcraft)
          36 = AIL (Terra Nova Strike Force Centuri :: partially 4op)
          37 = AIL (System Shock :: partially 4op)
          38 = AIL (Advanced Civilization)
          39 = AIL (Battle Chess 4000 :: partially 4op, melodic only)
          40 = AIL (Ultimate Soccer Manager :: partially 4op)
          41 = HMI (Theme Park)
          42 = HMI (3d Table Sports, Battle Arena Toshinden)
          43 = HMI (Aces of the Deep)
          44 = HMI (Earthsiege)
          45 = HMI (Anvil of Dawn)
          46 = AIL (Master of Magic, Master of Orion 2 :: 4op, std percussion)
          47 = AIL (Master of Magic, Master of Orion 2 :: 4op, orchestral percussion)
          48 = SB (Action Soccer)
          49 = SB (3d Cyberpuck :: melodic only)
          50 = SB (Simon the Sorcerer :: melodic only)
     Use banks 1-4 to play Descent \"q\" soundtracks.
     Look up the relevant bank number from descent.sng.

     The fourth parameter can be used to specify the number
     of four-op channels to use. Each four-op channel eats
     the room of two regular channels. Use as many as required.
     The Doom & Hexen sets require one or two, while
     Miles four-op set requires the maximum of numcards*6.</pre>

", 'copying:1. Copying and contributing' => "

cromfs has been written by Joel Yliluoma, a.k.a.
<a href=\"http://iki.fi/bisqwit/\">Bisqwit</a>,<br />
and is distributed under the terms of the
<a href=\"http://www.gnu.org/licenses/gpl-3.0.html\">General Public License</a>
version 3 (GPL3).
 <br/>
The OPL3 emulator within is from DOSBox is licensed under GPL2+.
 <br/>
The FM soundfonts (patches) used in the program are
imported from various PC games without permission
from their respective authors. The question of copyright, when
it comes to sets of 11 numeric bytes, is somewhat vague, especially
considering that most of those sets are simply descendants of the
patch sets originally published by AdLib Inc. for everyone's free use.
 <p/>
Patches (as in source code modifications)
and other related material can be submitted
to the author
".GetEmail('by e-mail at:', 'Joel Yliluoma', 'bisqwi'. 't@iki.fi')."
 <p/>
The author also wishes to hear if you use adlmidi, and for what you
use it and what you think of it.

");
include '/WWW/progdesc.php';
