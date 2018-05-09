	public _BITMAP_PolluxVGA
	public _BITMAP_Fixedsys12
	public _BITMAP_8bitpusab12

	code

	even

_BITMAP_PolluxVGA:
	incbin "pollux.bin"

_BITMAP_Fixedsys12:
	incbin "fixedsys.bin"

_BITMAP_8bitpusab12:
	incbin "8bitpusab.bin"

