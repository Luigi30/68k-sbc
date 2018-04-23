#include "elf.h"

int16_t ELF_LoadExecutable(char *buffer)
{
  printf("Reading ELF header for file at $%06X\n", buffer);

  ELF_Header header;
  memcpy(&header, buffer, sizeof(ELF_Header));

  if(header.e_ident.magic[0] != 0x7F || header.e_ident.magic[1] != 0x45 ||
	 header.e_ident.magic[2] != 0x4C || header.e_ident.magic[3] != 0x46)
	{
	  printf("ELF: ELF header is invalid, abort\n");
	  return ELF_ERROR_BAD_MAGIC_NUMBER;
	}

  printf("Found a valid ELF magic number, processing header\n");

  // Make sure we can execute this file.
  if(header.e_ident.class != 0x01)
	{
	  printf("ELF: Header says this is not a 32-bit executable.\n");
	  return ELF_ERROR_NOT_32_BIT;
	}

  if(header.e_ident.data != 0x02)
	{
	  printf("ELF: Header says this is not a big-endian executable.\n");
	  return ELF_ERROR_LITTLE_ENDIAN;
	}

  // look for a program header
  printf("ELF: file has %d program headers\n", header.e_phnum);

  ELF_ProgramHeader *ph_table = (ELF_ProgramHeader *)((uint32_t)buffer + (uint32_t)header.e_phoff);
  ELF_ProgramHeader *current_ph = ph_table;

  for(int i=0; i<header.e_phnum; i++)
	{
	  printf("*** PROGRAM HEADER: type %d, image offset %06X, %d bytes\n",
			 current_ph[i].p_type,
			 current_ph[i].p_offset,
			 current_ph[i].p_filesz);
	}

  // look for section headers
  printf("ELF: file has %d section headers\n", header.e_shnum);
  
  ELF_SectionHeader *sh_table = (ELF_SectionHeader *)((uint32_t)buffer + (uint32_t)header.e_shoff);
  ELF_SectionHeader *current_section = sh_table;
  
  char *section_names = current_section[header.e_shstrndx].sh_offset + buffer;
	printf("section_names is at $%06X (%06X + %06X)\n",
		   section_names,
		   current_section[header.e_shstrndx].sh_offset,
		   buffer);

  for(int i=0; i<header.e_shnum; i++)
	{ 
	  printf("*** SECTION: name '%s', type %02X, offset $%06X ***\n",
			 section_names + current_section[i].sh_name,
			 current_section[i].sh_type,
			 current_section[i].sh_offset);
	}

  ELF_RELA *relocation_table;
  uint32_t relocation_count;

  for(int i=0; i<header.e_shnum; i++)
	{
	  if(current_section[i].sh_type == SHT_RELA)
		{
		  relocation_table = (ELF_RELA *)(buffer + current_section[i].sh_offset);
		  relocation_count = current_section[i].sh_size / current_section[i].sh_entsize;

		  printf("Processing RELA table is at offset $%06X, contains %d entries\n",
				 relocation_table,
				 relocation_count);

		  for(int j=0; j<relocation_count; j++)
			{
			  ELF_Relocate(relocation_table[j], sh_table, buffer);
			}
		}
	}

  ELF_RunProgram(buffer + current_section[1].sh_offset);
}

void ELF_Relocate(ELF_RELA relocation, ELF_SectionHeader *sh_table, void *load_addr)
{
  printf("r_info: %08X\n", relocation.r_info);
  int applies_to_section = (uint8_t)((relocation.r_info & 0xFF00) >> 8);
  int target_section = (uint8_t)(relocation.r_info & 0xFF);
  
  uint32_t addend = (uint32_t)load_addr + sh_table[applies_to_section].sh_offset + relocation.r_addend;
  uint32_t address_to_relocate = ((uint32_t)load_addr + sh_table[target_section].sh_offset + (uint32_t)relocation.r_offset);
  
  printf("ELF_RELOCATE: Adding $%06X+$%06X to $%06X (offset $%06X in section %d)\n",
		 (char *)load_addr + sh_table[applies_to_section].sh_offset,
		 relocation.r_addend,
		 address_to_relocate,
		 relocation.r_offset,
		 target_section);

  MMIO32(address_to_relocate) = MMIO32(address_to_relocate) + addend;
}

void ELF_RunProgram(void *entry)
{
  printf("Running program...\n");
  ELF_JSRProgram(entry);
  printf("\n\n\n");
  printf("Program complete...\n");
}
