Thank you for using this Dunfield Development Services product.

To install this package:

[NOTE: For products downloaded from the web, "INSTALL" in the text below
 refers to the .EXE file that you downloaded - for example, the 8051
 Developers Kit is provided in the file DK51.EXE - use "DK51" as the
 name of the install program.]

From DOS/Command prompt
   - Select the drive containing the distribution.
   - CD to the subdirectory containing the distribution (if necessary)
   - Run the INSTALL program

From Windows:
   - From "My Computer", open the drive containing the distribution.
   - Locate and open the subdirectory containing the distribution.
   - Double-Click on the INSTALL program icon.

NOTE: INSTALL is menu driven, and allows you to choose what portions of
the distribution package to install, and where to put them (directory).
It will warn you before installing over an existing directory, and does
not modify AUTOEXEC.BAT or CONFIG.SYS. It simply copies/dearchives the
files from the distribution diskette into appropriate directories on your
hard disk.

Please refer to the various README.*, *.TXT, and *.DOC files installed
with the package for more information.

Once you have installed the system, refer to the section entitled "THE
COMMAND CO-ORDINATOR" in the MC85.TXT file for information on how to compile
programs using the 'CC85' command. You should either include the MICRO-C
directory (\MC) in your PATH, or copy the CC85.COM file to a directory which
is already in your PATH. The MCDIR and TEMP environment variables should
be set up as described in the document before attempting to use the compiler.

Note that some DDS source files ('.c', '.h' etc.) are written using tab
stops at 4 character intervals, which is much more convenient for 'C'
development than the usual MS-DOS tab stops at 8 character intervals.
The DDSIDE and EDT editors from Dunfield Development Services correctly
support these 4 character tab stop.

For information on obtaining technical help, see the TECHSUPP.TXT file
installed with the package documentation.

Contents of this distribution:
-----------------------------------------------------------------------------
DDS MICRO-C 8085 Developers Kit:

    These are complete PC based cross development systems which include
    EVERYTHING you need to develop 'C' and 'ASM' software for one CPU.
    - Includes MICRO-C compiler, optimizer, assembler, and related utilities.
    - Integrated Development Environment. Edit, Compile, Download and Debug
      from one application. Configurable with lots of options. Many extra
      features like: multi-file browser, "pop-down" HEX calculator and
      ASCII character set chart. Command line tools are also included.
    - Hand coded (efficent ASM) standard library for CPU (Source included).
    - Includes Monitor/Debugger for CPU  - ASM source (See XTOOLS) *
    - Includes text editor, telecomm program, MAKE and many other utilities.
    - Complete documentation (on disk).
    - Lots of "extras".
    - Available for 68HC08, 6809, 68HC11, 68HC12, 68HC16, 8051/52, 8080/85,
      8086, 8096, AVR and C-FLEA **
    [SUPER Developers kit contains all of the above]
    *  68HC16 Developers Kit does not include a monitor/debugger.
       C-FLEA kit includes a simulator instead of monitor/debugger.
    ** C-FLEA is a "virtual machine" designed to run 'C' in situations
       not suited to a native implementation. See the FAQ on our website.
       Sample VM implementations are provided for:
        6800/01/02/03/08/11/12, 6805, 8051/52, 8086 and PIC (16C57/58).

