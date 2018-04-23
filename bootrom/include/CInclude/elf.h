#ifndef ELF_H
#define ELF_H

#include <stdlib.h>
#include <stdio.h>

#include "memmgr.h"

/* https://en.wikipedia.org/wiki/Executable_and_Linkable_Format */
typedef struct elf_ident_t {
  uint8_t magic[4];
  uint8_t class;
  uint8_t data;
  uint8_t version;
  uint8_t osabi;
  uint8_t abi_version;
  uint8_t padding[7];
} ELF_Ident;

typedef struct elf_header_t {
  struct elf_ident_t e_ident;
  uint16_t e_type;
  uint16_t e_machine;
  uint32_t e_version;
  uint32_t e_entry;
  uint32_t e_phoff;
  uint32_t e_shoff;
  uint32_t e_flags;
  uint16_t e_ehsize;
  uint16_t e_phentsize;
  uint16_t e_phnum;
  uint16_t e_shentsize;
  uint16_t e_shnum;
  uint16_t e_shstrndx;
} ELF_Header;

typedef struct elf_program_header_t {
  uint32_t p_type;
  uint32_t p_offset;
  uint32_t p_vaddr;
  uint32_t p_paddr;
  uint32_t p_filesz;
  uint32_t p_memsz;
  uint32_t p_flags;
  uint32_t p_align;
} ELF_ProgramHeader;

typedef struct elf_section_header_t {
  uint32_t sh_name;
  uint32_t sh_type;
  uint32_t sh_flags;
  uint32_t sh_addr;
  uint32_t sh_offset;
  uint32_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint32_t sh_addralign;
  uint32_t sh_entsize;
} ELF_SectionHeader;

typedef enum {
  SHT_NULL,
  SHT_PROGBITS,
  SHT_SYMTAB,
  SHT_STRTAB,
  SHT_RELA,
  SHT_HASH,
  SHT_DYNAMIC,
  SHT_NOTE,
  SHT_NOBITS,
  SHT_REL,
  SHT_SHLIB,
  SHT_DYNSIM,
} ELF_SHTYPE;

// Relocation
typedef struct elf_rel_t {
  void *r_offset;
  uint32_t r_info;
} ELF_REL;

typedef struct elf_rela_t {
  void *r_offset;
  uint32_t r_info;
  int32_t r_addend;
} ELF_RELA;

// Symbols
typedef struct elf_symbol_t {
  uint32_t st_name;
  void *st_value;
  uint32_t st_size;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
} ELF_SYMBOL;

int16_t ELF_LoadExecutable(char *buffer);
void ELF_Relocate(ELF_RELA relocation,
				  ELF_SectionHeader *sh_table,
				  void *load_addr);
void ELF_RunProgram(void *entry);
void ELF_JSRProgram(__reg("a1") void *) = "\tjsr (a1)\n";

// Errors
#define ELF_ERROR_BAD_MAGIC_NUMBER -1
#define ELF_ERROR_NOT_32_BIT       -2
#define ELF_ERROR_LITTLE_ENDIAN    -3

#endif
