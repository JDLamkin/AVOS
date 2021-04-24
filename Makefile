SOURCE_FILES=$(wildcard src/*)

SIZE=1440k
LINKFILE=linker.ld
TARGET=floppy_boot.img
BINARY=bin/kernel.img
LINK_DEBUG=debug/kernel-elf.o

ASM_FILES=$(filter %.asm, $(SOURCE_FILES))
  C_FILES=$(filter %.c,   $(SOURCE_FILES))
CPP_FILES=$(filter %.cpp, $(SOURCE_FILES))

SOURCE_CODE=$(ASM_FILES) $(C_FILES) $(CPP_FILES)
OBJECT_FILES=$(patsubst src/%, bin/%.o, $(SOURCE_CODE))
OUTPUT_LIST=$(TARGET) bin/
GENERATED=$(OUTPUT_LIST) debug/

GCC_COMPILE_FLAGS=-fleading-underscore -fno-asynchronous-unwind-tables

COMPILE_ASM=nasm -f elf64
COMPILE_C=  gcc -c $(GCC_COMPILE_FLAGS)
COMPILE_CPP=g++ -c $(GCC_COMPILE_FLAGS)

ECHO_TAG=\033[$(1);1m[MAKE]\033[m

.PHONY: all clean purge run debug
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

run: bochsrc.bxrc all
	@mkdir -p debug/
	@printf "$(call ECHO_TAG, 32) Starting simulation.\n"
	@bochs -f $<

debug: $(LINK_DEBUG)

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
	@printf "$(call ECHO_TAG, 34) Linking [ELF DEBUG]...\n"
	@ld -T $< --oformat=elf64-x86-64 -o $@ $(OBJECT_FILES)

bin/%.asm.o: src/%.asm
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Compiling (asm) $<\n"
	@$(COMPILE_ASM) -o $@ $<

bin/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Compiling ( c ) $<\n"
	@$(COMPILE_C)   -o $@ $<

bin/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	@printf "$(call ECHO_TAG, 34) Compiling (c++) $<\n"
	@$(COMPILE_CPP) -o $@ $<
