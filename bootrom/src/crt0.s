	global _bios_start
	
VEC_RESET_INITIAL_SSP:		* $000000 */
	dc.l	$1FFF00	* Stack pointer - last page of RAM. */

VEC_RESET_INITIAL_PC:		* $000004 */
	dc.l	_bios_start	* BIOS entry point. */

VEC_BUS_ERROR:			* $000008 */
	dc.l	_bios_start	* BIOS entry point. */

VEC_ADDRESS_ERROR:		* $00000C */
	dc.l	_bios_start

VEC_ILLEGAL_INSTRUCTION:	* $000010 */
	dc.l	_bios_start

VEC_DIVISION_BY_ZERO:		* $000014 */
	dc.l	_bios_start

VEC_CHK_INSTRUCTION:		* $000018 */
	dc.l	_bios_start

VEC_TRAPV_INSTRUCTION:		* $00001C */
	dc.l	_bios_start

VEC_PRIVILEGE_VIOLATION:	* $000020 */
	dc.l	_bios_start

VEC_TRACE_INSTRUCTION:		* $000024 */
	dc.l	_bios_start

VEC_LINE_A_INSTRUCTION:		* $000028 */
	dc.l	LineA_Dispatcher

VEC_LINE_F_INSTRUCTION:		* $00002C */
	dc.l	_bios_start

* 3 reserved vectors
	dc.l	_bios_start	* $000030 */
	dc.l	_bios_start	* $000034 */
	dc.l	_bios_start	* $000038 */

VEC_UNINITIALIZED_INTERRUPT:	* $00003C */
	dc.l	_bios_start

* 8 reserved vectors
	dc.l	_bios_start	* $000040 */
	dc.l	_bios_start	* $000044 */
	dc.l	_bios_start	* $000048 */
	dc.l	_bios_start	* $00004C */
	dc.l	_bios_start	* $000050 */
	dc.l	_bios_start	* $000054 */
	dc.l	_bios_start	* $000058 */
	dc.l	_bios_start	* $00005C */

VEC_SPURIOUS_INTERRUPT:		* $000060 */
	dc.l	_bios_start

VEC_LEVEL_1_INTERRUPT:		* $000064 */
	dc.l	_bios_start

VEC_LEVEL_2_INTERRUPT:		* $000068 */
	dc.l	_bios_start

VEC_LEVEL_3_INTERRUPT:		* $00006C */
	dc.l	_bios_start

VEC_LEVEL_4_INTERRUPT:		* $000070 */
	dc.l	_isr_com1

VEC_LEVEL_5_INTERRUPT:		* $000074 */
	dc.l	_bios_start

VEC_LEVEL_6_INTERRUPT:		* $000078 */
	dc.l	_bios_start

VEC_LEVEL_7_INTERRUPT:		* $00007C */
	dc.l	_bios_start

VEC_TRAP_0:			* $000080 */
	dc.l	_bios_start

VEC_TRAP_1:
	dc.l	_bios_start

_bios_start:
	jmp	$1000

_isr_com1:
	jsr	_MOUSE_PacketISR
	rte
