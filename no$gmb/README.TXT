NO$GMB nocash gameboy emulator  copyright 1997-2000 martin korth
----------------------------------------------------------------

FREQUENTLY ASKED QUESTIONS AND SOME BASIC INFO

Can you help me per email?

     Nope, sorry! I already get more emails than I can reply, if you have
     general questions about downloading, installing, registering, or using
     the emulator, then there is one important thing that you should note
     first: PLEASE NO EMAILS.
     Otherwise you could try to read this text file, and the help text in
     the emulator itself. Sorry if that doesn't help or if you do not
     understand it - In that case ask your friends for help, or try to find
     info in newsgroups or webpages - I can't give any further help.

The color disappears after 5 minutes!
I get a battery error message!

     That is not a bug, in fact, if you would have read the complete message
     on the screen, then you would have seen that the message said that you
     should send some money in order to buy a registration key if you want to
     play color games for more than 5 minutes.
     Read the section about 'registering' (below) for details.

How do I register?

     That is done very easy, and would be very nice. In short words: Send
     some cash to my address, that's it. For more details, read the help
     text (in the emulator itself), or print the included ORDER.FRM file,
     and then just do it!

A message says ILLEGAL COPY and beeps whenever I turn on my computer!

     Yup, that's what happens to people like you. However, as that message
     said, you can still use your computer (after hitting space 500 times),
     and after doing that five times (ie. 2500 keystrokes alltogether) the
     program will assume that you have realized your fault, and delete your
     harddisk, no, no, just joking, it'll stop bothering you at that time.
     Additionally you are encouraged to report your actions to your local
     police office.
     Note: That happens -only- to people whom have actually stolen a
     registration key, not to normal registered or unregistered users.

How does the Link function work?

     Make sure that you are using no$gmb v1.9 or newer.
     Enter EMUlation setup (Press F11-key or mouseclick 'options')
     Select 'Two Machines' instead of 'Single Machine'
     There is NO WAY to load different cartridges at the same time.
     All gameboys are emulated on the same computer, there is NO WAY to
     link to another computer, not via cable or network or whatever.

How can I play Pokemon Red / Blue?

     As described above, there is NO WAY to load two different cartridges
     at the same time. You may want to try to rename/copy POKERED.SAV to
     POKEBLUE.SV2 (both files in BATTERY subdirectory) I haven't tested
     that, but people claim it would allow trading. (See also information
     about linking above.)

     The game hangs at the professor? This happens with people who
     downloaded a corrupted pokemon game file, download a clean version
     somewhere else.

     For video problems, see below.
     For memory problems, see below.

Where can I download your pokemon game?

     It is NOT MY pokemon game, no$gmb emulates a gameboy, so it allows
     you to play games on it, as on a real gameboy. The game cartridges
     are usually sold in shops, you might also find a place where you
     can download a copy, which is definitely illegal as long as you do
     not own the real cartridge.

In SGB (older color) games, my screen looks like a mess!

     Your computer is unable to display SGB graphics in a resolution of
     256x240 pixels. Enter EMUsetup and select 'Debugscreen' as Emulation
     background. This way it uses 640x480 pixel mode. Sorry if you don't
     like numbers.

I get a not enough memory error message!

     That should be related only to the DOS version, if you have windows
     and a relative fast computer, then download the no$gmb windows version
     from http://www.work.de/nocash/gmb.htm that should bypass memory
     problems, otherwise read on below.

        The following are instructions how you get your operating system
        installed properly, so that it allows software to use EMS memory.

        If you still get problems, contact your dealer, a friend, or the
        microsoft support line (no joke).
        I CANNOT GIVE YOU MORE THAN THE FOLLOWING INSTRUCTIONS.
        DO NOT ASK ME FOR MORE INFORMATION.
        DO NOT EVEN TRY TO ASK ME.

     UNDER WINDOWS:
     Start the emulator, press ALT+ENTER to toggle from full-screen mode
     to a small window (if necessary). Click the MSDOS symbol in upper left
     of this window, select the last item (properties/options).
     In the following screen, click 'Memory'.
     In the 'Expanded Memory (EMS)' field, select either 'Automatic' or at
     least 2048 (assuming that your biggest cartridge is sized 2048 KBytes).
        If Windows complains about 'NOEMS' being specified in config.sys,
        load the file C:\CONFIG.SYS into the editor and remove the word
        'NOEMS' from the file, reboot the computer, and retry the above.
     Otherwise press OK. If necessary use ALT+ENTER again, probably you
     will also have to restart the emulator (use ALT+X to exit).

     UNDER DOS:
     Install EMM386 (your standard EMS memory manager) in your config.sys.
     In general, add the following two lines at the head of C:\CONFIG.SYS
        device=windows\himem.sys
        device=windows\emm386.exe
     Please refer to your DOS manual for details. The above lines are
     windows user specific, I doubt that other people will have any
     problems about EMS.
     If you don't have a DOS manual, read the .TXT files in your \WINDOWS
     directory or ask somebody who knows a bit more about computers.
     If you already had EMM386 installed in config.sys, remove the word
     'NOEMS' from config.sys.

     EMM386 IS INCLUDED IN YOUR OPERATING SYSTEM ON YOUR HARDDISK! I give
     a fuck on lamers asking me where they can download EMM386!
     If none of the above helps, you may also have to append FRAME=D000
     (or similiar) to the EMM386 line in config.sys.

I can't get sound working!

     If you have a plug and play card, make sure that you have sound drivers
     installed. If your soundcard is using a different DMA channel than 1,
     make sure that the BLASTER environment variable is set up correctly.
       set BLASTER=A220 I5 D1
     would mean port 220h, IRQ 5, DMA channel 1

The games run at 50% speed only!

     A bug somehow related to certain soundcards, choose "SB Mono" instead
     "SB stereo" in setup to bypass it.

How does game saving work?
     A lot of games use battery ram, no$gmb automatically saves battery
     ram for all emulated gameboys. For battery-less games, you could
     also use the snapshot feature of the emulator, this won't work with
     more than one gameboy though.

Finally, the most important question, answered for the masses:
What is the difference between a Gameboy and Pokemon?
     A gameboy is a game console. Pokemon is a game for this game console
     (see above). And by the way, do NOT send any mails regarding Pokemon,
     I won't reply to such mails anyways.

I have a copy of the full version of Windows 95b which I no longer need
since I'm running with Win 98.
     This mis-information, of course, got into this FAQ by mistake.


-------------------

More Help
     this file is just to getting started. more help is available by
     pressing F1 when you have loaded the emulator. detailed debugger-help
     is included in the special EXTENDED debugger version.
     The manual can be extracted to file by pressing 'X' in F1-help screen.

Starting the emulator
     Just enter something like the following:
     gmb
     gmb tetris
     gmb rtype.zip   ;provided that pkunzip.exe is somewhere in your path
     gmb /?
     When you download the emulator it is compressed in a ZIP file, before
     you can use it you must decompress it by using a program like PKUNZIP,
     or WINZIP. If you do not have such a program, then you should be able
     to download it somewhere.

Cartridge Files
     there are files in the net (*). download these & copy them to SLOT
     subdirectory. you can put cartridges into ZIP or ARJ files. please
     use same name for archieve and cartridge (ie. tetris.zip contains
     tetris.gb). PKUNZIP.EXE has to be present in your path, ie. for example
     in your C:\WINDOWS directory.

     (*) some freeware games/links can be found at my webpage
         I expect that everybody knows that most other games are
         copyrighted and that it's illegal to play these games without
         buying them first.

environment failure (buggy or strange operating systems)
     try start either "gmb.bat" or "no$gmb.exe" or get rid of your
     operating system and please contact anyway

it says illegal or damaged registration key
     1) ILLEGAL: if you have stolen a copy of a reg key, please buy your own
     key, i have put a lot of work into the program, and a few dollars
     shouldn't be too much, in fact it'd be worth much more.

     2) DAMAGED: some mail programs somehow damage the keys, so it might
     frequently happen that your brandnew key doesn't work. if so, please
     reply to the email in which you got the key, and I'll send a new copy.

     I WILL REPLY TO SUCH MAILS ONLY IF YOU HAVE ACTUALLY BOUGHT A REG KEY.
     PEOPLE WHO WANT TO CHEAT ME: THANK YOU FOR WASTING MY TIME GO TO HELL.

it does not run on my XT/AT (8086/80286)
     check out www.work.de/nocash/ and download XT/AT special version

i got a black screen, no debug or setup screens!
     try running with switch /t for plain 80x25 text mode

how can i design a sound adapter for the lpt port?
     use following schematic (taken from MODPLAY docs)
     (that'll work in the DOS version only)

     You need one plug for your amp, one for your parallel port and
     9 20k and 9 10K resistors
     Use 1% resistors if possible Ä and 20k sound much better than 22k

          Printer Port:

          signal   pin        20k   20k
          D0       2      >ÄÄÄ°°°ÄÂÄ°°°ÄÄ0v
                              20k ° 10k
          D1       3      >ÄÄÄ°°°Ä´
                              20k ° 10k
          D2       4      >ÄÄÄ°°°Ä´
                              20k ° 10k
          D3       5      >ÄÄÄ°°°Ä´
                              20k ° 10k
          D4       6      >ÄÄÄ°°°Ä´
                              20k ° 10k
          D5       7      >ÄÄÄ°°°Ä´
                              20k ° 10k
          D6       8      >ÄÄÄ°°°Ä´
                              20k ° 10k
          D7       9      >ÄÄÄ°°°Ä´
                              20k ° 10k  100nF
                                  ÃÄÄÄÄÄÄ´ÛÄÄÄÄ> TO AMPLIFIER
                                  ° 10k
          GND      20     >ÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄ>
                                  0v
          
      Thanks to Harry Stox for the above circuit.  Again, make sure all
      the 0v lines are connected together.  The 100nF capacitor may not
      be needed, try removing it if this circuit sounds poor.

      Please note that an amplifer is REALLY needed, you will hear
      practically nothing if you just connect a speaker or headphones.

how can i design a snespad adapter for the lpt port?
      use following schematic (taken from http://welcome.to/snespad)

      The 5 diodes should be germanium or something, whatever it means.
      Alternately use "schottky diodes such as 1N5817". Never tried either,
      guess any normal diodes would do it, apparently the things are used
      only to protect the parallel port from damage, less sensitive people
      would probably just shortcut the vcc-lines and use no diodes at all.

      d3   5 ÄÄÄ|>|ÄÄ¿            ÚÄÄÄ¿
      d4   6 ÄÄÄ|>|ÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄ³ O ³ vcc
      d5   7 ÄÄÄ|>|ÄÄ´  ÚÄÄÄÄÄÄÄÄÄ³ O ³ clk    ;XX = 10,12,13,15 for pad 1..4
      d6   8 ÄÄÄ|>|ÄÄ´  ³ ÚÄÄÄÄÄÄÄ³ O ³ reset  ;(lpt status register bit 6..3)
      d7   9 ÄÄÄ|>|ÄÄÙ  ³ ³ ÚÄÄÄÄÄ³_O_³ data
      d0   2 ÄÄÄÄÄÄÄÄÄÄÄÙ ³ ³     ³ O ³
      d1   3 ÄÄÄÄÄÄÄÄÄÄÄÄÄÙ ³     ³ O ³
          XX ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ ÚÄÄÄ³ O ³ gnd
      gnd 18,19 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ   `ÄÄÄ'


Where can I get the latest Updates?
      check out http://www.work.de/nocash/
      also visit for nocash amstrad cpc emulator and other projects.

have fun, nocash
