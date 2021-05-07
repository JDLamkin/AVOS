# A Verified Operating System
*DISCLAIMER: THIS PROJECT WILL BE UPDATED EXTREMELY SLOWLY WITH LARGE GAPS IN DEVELOPMENT. EXPECT
YEARS TO GO BY WITH NEXT TO NO UPDATES.*

*NOTE: THIS README IS A WORK IN PROGRESS. IT IS CURRENTLY UPDATED ONLY FOR CODE ARCHITECTURE
PURPOSES. THE REST OF THIS README IS TODO.*

In this project, we seek to create a kernel (complete with a custom boot loader) that interfaces
solely with verified code. The kernel itself is verified to the extent that the underlying machine
is well-defined.

## Architecture

### Boot Stages
The kernel proper can be divided functionally into four "Boot Stages", each preparing an environment
for the next one and stepping out of the way afterward.

#### Boot Stage 0 - BIOS Drop
In this stage, we only have the first sector (512 bytes) of our OS. Our first priority is to load
the code for the rest of the OS in a platform-independent manner. Since the final 2 bytes are
reserved for the magic number, we must squeeze the following into 510 bytes:
 * Establishing a common environment.
 * Loading from source disk.
 * Error handling/messaging.
 * Printing strings.

#### Boot Stage 1 - Machine Prep
Once in Boot Stage 1, we have practically unlimited space for core code (though if we try to fit
much more than a microkernel, we may find our space strained). The real challenge now is getting the
CPU out of 16-bit "real" compatibility mode and into a mode where it can be more fully used. We need
to prepare to jump out of assembly into a high-level language to prepare for Boot Stage 2.

#### Boot Stage 2 - OS Setup
This is the primary setup stage, responsible for initializing high-level constructs including the
core concept of module verification and integration. After the end of this stage, we no longer need
the one-time code from Boot Stages 0-2, so we reclaim the memory it was loaded into.

#### Boot Stage 3 - Completed
We have now fully set up the kernel and have started the root thread, loading the relevant modules
as needed. We can now turn our attention away from the kernel as it will only be needed on-demand
from now on.
