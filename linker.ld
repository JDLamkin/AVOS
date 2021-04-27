OUTPUT_FORMAT("binary")
/*
MEMORY
{
    boot_usable (rwx) : ORIGIN = 0x500, LENGTH = 0x7FB00
}*/

SECTIONS
{
    .bss.low 0x500 (NOLOAD) : {
        * ( .bss.low )
    }

    /* Boot Stage 0. This section must be exactly 512 bytes starting at 0x7C00 (in memory) and zero
     * in the output binary. Boot Stage 0 is responsible for loading boot stages 1 and 2.
     */
    .boot_stage_0 0x7C00 : {
        bin/boot_0.asm.o ( .text* )
        bin/boot_0.asm.o ( .rodata* .rdata* .data* )
        . = 0x1FE;
        SHORT(0xAA55);
    } = 0x00

    .boot_stage_1_2 : ALIGN(0x200) {
        BOOT_1_START = ABSOLUTE( . );
        * ( .text* )
        * ( .rodata* .rdata* .data* )
    }

    .boot_stage_3 : {
        BOOT_3_START = ABSOLUTE( . );   /*
        * ( .text* )
        * ( .rodata* .rdata* .data* )   */
    }
    /* Number of disk segments in Boot Stages 1, 2 and 3 */
    BOOT_SEGMENTS = ALIGN( SIZEOF( .boot_stage_3 ) + BOOT_3_START - BOOT_1_START , 0x200 ) / 0x200;
    ASSERT( BOOT_SEGMENTS < 0x80 , "Error: Combined Boot Stages 1 and 2 are too large" )

    .bss (NOLOAD) : ALIGN(0x1000) {
        * ( .bss.paged )
        * ( .bss* )
    }

    /DISCARD/ : {
        * (*)
    }
}
