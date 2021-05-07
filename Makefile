SHELL := /bin/bash
SOURCE_FILES=$(wildcard src/*)

SIZE=1440k
LINKFILE=config/linker.ld
TARGET=floppy_boot.img
BINARY=bin/kernel.img
LINK_DEBUG=debug/kernel-elf.o

ASM_FILES=$(filter %.asm, $(SOURCE_FILES))
  C_FILES=$(filter %.c,   $(SOURCE_FILES))
CPP_FILES=$(filter %.cpp, $(SOURCE_FILES))
  HEADERS=$(filter %.h,   $(SOURCE_FILES))

GCC_SOURCE=$(C_FILES) $(CPP_FILES)
SOURCE_CODE=$(ASM_FILES) $(C_FILES) $(CPP_FILES)
OBJECT_FILES=$(patsubst src/%, bin/%.o, $(SOURCE_CODE))
DEPEND_FILES=$(patsubst src/%, .depend/%.d, $(GCC_SOURCE))
DEBUG_ASM_FILES=$(patsubst src/%, debug/asm/%.asm, $(GCC_SOURCE))
OUTPUT_LIST=$(TARGET) bin/ .depend/ debug/asm/ $(LINK_DEBUG)
GENERATED=$(OUTPUT_LIST) debug/

GCC_NO_EXTENSIONS=-mno-mmx -mno-sse -mno-sse2 -mno-red-zone
GCC_COMPILE_FLAGS=-fleading-underscore -fno-asynchronous-unwind-tables -ffreestanding -nostdlib -fno-builtin -fno-pie

GCC_CORE_FLAGS=$(GCC_COMPILE_FLAGS) $(GCC_NO_EXTENSIONS)
ASM_DEBUG_FLAGS=-S -masm=intel
ELF_DEBUG_FLAGS=--oformat=elf64-x86-64

COMPILE_ASM=nasm -f elf64
COMPILE_C=  gcc -std=c99   -c $(GCC_CORE_FLAGS)
COMPILE_CPP=g++ -std=c++11 -c $(GCC_CORE_FLAGS)

ECHO_TAG=\033[$(1);1m[MAKE]\033[m

.PHONY: all clean purge debug run runb runq
.SUFFIXES:

all: $(TARGET)

clean:
	@printf "$(call ECHO_TAG, 31) Removing output files (excluding debug/).\n"
	@printf "$(call ECHO_TAG, 33) (Use 'make purge' to include debug/.)\n"
	@rm -rf $(OUTPUT_LIST)

purge:
	@printf "$(call ECHO_TAG, 33) This operation will remove debug/ which could contain\n"
	@printf "$(call ECHO_TAG, 33) valuable/non-reproducible data. Proceed anyway? [y/N] "
	@read ans; if [[ $${ans:-N} =~ [Yy].* ]]; then                      \
        printf "$(call ECHO_TAG, 31) Removing all generated files.\n";  \
        rm -rf $(GENERATED);                                            \
    fi

run: runb

runb: config/bochs_cfg.bxrc config/bochs_dbg.bxrc all
	@mkdir -p debug/
	@printf "$(call ECHO_TAG, 32) Starting bochs simulation.\n"
	@bochs -qf $(word 1, $^) -rc $(word 2, $^)

runq: all
	@printf "$(call ECHO_TAG, 32) Starting qemu simulation.\n"
	@qemu-system-x86_64 -drive format=raw,file=$(TARGET)

debug: all $(DEBUG_ASM_FILES) $(LINK_DEBUG)

$(TARGET): $(BINARY)
	@printf "$(call ECHO_TAG, 34) Copying $< into $@\n"
	@cp $< $@; chmod 644 $@
	@truncate $@ -s ">$(SIZE)"
	@printf "$(call ECHO_TAG, 34) Checking resultant size of $@\n"
	@if [[ $$(find $@ -size "+$(SIZE)") ]]; then rm -f $@; false; fi

$(BINARY): $(LINKFILE) $(OBJECT_FILES)
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Linking...\n"
	@ld -T $< -o $@ $(OBJECT_FILES)

$(LINK_DEBUG): $(LINKFILE) $(OBJECT_FILES)
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 36) Linking [ELF DEBUG]...\n"
	@ld -T $< $(ELF_DEBUG_FLAGS) -o $@ $(OBJECT_FILES)

bin/%.asm.o: src/%.asm
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Compiling (asm) $< --> $@\n"
	@$(COMPILE_ASM) -o $@ $<

bin/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Compiling ( c ) $< --> $@\n"
	@$(COMPILE_C)   -o $@ $<

bin/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Compiling (c++) $< --> $@\n"
	@$(COMPILE_CPP) -o $@ $<

debug/asm/%.c.asm: src/%.c
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 36) Compiling ( c ) [ASM DEBUG] $< --> $@\n"
	@$(COMPILE_C)   $(ASM_DEBUG_FLAGS) -o $@ $<

debug/asm/%.cpp.asm: src/%.cpp
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 36) Compiling (c++) [ASM DEBUG] $< --> $@\n"
	@$(COMPILE_CPP) $(ASM_DEBUG_FLAGS) -o $@ $<

.depend/%.d: src/% $(HEADERS)
	@mkdir -p $(dir $@)
	@gcc -MM -MT bin/%.o $< > $@
	@gcc -MM -MT debug/asm/%.asm $< >> $@

include $(DEPEND_FILES)
