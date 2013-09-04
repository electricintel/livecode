/* Copyright (C) 2003-2013 Runtime Revolution Ltd.

This file is part of LiveCode.

LiveCode is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License v3 as published by the Free
Software Foundation.

LiveCode is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with LiveCode.  If not see <http://www.gnu.org/licenses/>.  */

//
// MCImage class
//
#ifndef	IMAGE_H
#define	IMAGE_H

#include "control.h"
#include "imagebitmap.h"

#define MAG_WIDTH 8
#define MAX_PLANES 8
#define MAX_CMASK 7

typedef struct
{
	int2 y;
	int2 lx;
	int2 rx;
	int2 direction;
} MCstacktype;

typedef struct
{
	uint1 *data;
	uint2 width;
	uint2 height;
	uint2 bytes;
	int2 xhot;
	int2 yhot;
} MCbrushmask;

////////////////////////////////////////////////////////////////////////////////

bool MCImageBitmapApplyColorTransform(MCImageBitmap *p_bitmap, MCColorTransformRef p_transform);

bool MCImageQuantizeImageBitmap(MCImageBitmap *p_bitmap, MCColor *p_colors, uindex_t p_color_count, bool p_dither, bool p_add_transparency_index, MCImageIndexedBitmap *&r_indexed);
// create a new colour palette and map image pixels to palette colours
bool MCImageQuantizeColors(MCImageBitmap *p_bitmap, MCImagePaletteSettings *p_palette_settings, bool p_dither, bool p_transparency_index, MCImageIndexedBitmap *&r_indexed);

bool MCImageScaleBitmap(MCImageBitmap *p_src_bitmap, uindex_t p_width, uindex_t p_height, uint8_t p_quality, MCImageBitmap *&r_scaled);
bool MCImageRotateBitmap(MCImageBitmap *p_src, real64_t p_angle, uint8_t p_quality, uint32_t p_backing_color, MCImageBitmap *&r_rotated);

// Image format encode / decode function
bool MCImageEncodeGIF(MCImageBitmap *p_image, IO_handle p_stream, bool p_dither, uindex_t &r_bytes_written);
bool MCImageEncodeGIF(MCImageIndexedBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageDecodeGIF(IO_handle p_stream, MCImageFrame *&r_frames, uindex_t &r_frame_count);

bool MCImageEncodeJPEG(MCImageBitmap *p_image, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageDecodeJPEG(IO_handle p_stream, MCImageBitmap *&r_image);

bool MCImageEncodePNG(MCImageBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageEncodePNG(MCImageIndexedBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageDecodePNG(IO_handle p_stream, MCImageBitmap *&r_bitmap);

bool MCImageEncodeBMP(MCImageBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageDecodeBMPStruct(IO_handle p_stream, uindex_t &x_bytes_read, MCImageBitmap *&r_bitmap);
bool MCImageDecodeBMP(IO_handle p_stream, MCPoint &r_hotspot, MCImageBitmap *&r_bitmap);

bool MCImageEncodeRawTrueColor(MCImageBitmap *p_bitmap, IO_handle p_stream, Export_format p_format, uindex_t &r_bytes_written);
bool MCImageEncodeRawIndexed(MCImageBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageEncodeRawIndexed(MCImageIndexedBitmap *p_indexed, IO_handle p_stream, uindex_t &r_bytes_written);

bool MCImageEncodePBM(MCImageBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageEncodePPM(MCImageBitmap *p_bitmap, IO_handle p_stream, uindex_t &r_bytes_written);
bool MCImageDecodeNetPBM(IO_handle p_stream, MCImageBitmap *&r_bitmap);

bool MCImageDecodeXBM(IO_handle p_stream, MCPoint &r_hotspot, char *&r_name, MCImageBitmap *&r_bitmap);
bool MCImageDecodeXPM(IO_handle p_stream, MCImageBitmap *&r_bitmap);
bool MCImageDecodeXWD(IO_handle stream, char *&r_name, MCImageBitmap *&r_bitmap);

// Legacy Functions
bool MCImageSplitPixmaps(MCImageBitmap *p_bitmap, Pixmap &r_drawdata, Pixmap &r_drawmask, MCBitmap *&r_maskimagealpha);
bool MCImageMergePixmaps(uint16_t p_pixwidth, uint16_t p_pixheight, Pixmap p_drawdata, Pixmap p_drawmask, MCBitmap *p_maskimagealpha, MCImageBitmap *&r_bitmap);
bool MCImageSplitRasters(MCImageBitmap *p_image, MCBitmap *&r_drawimage, MCBitmap *&r_maskimage, MCBitmap *&r_maskimagealpha);
bool MCImagePixmapFromBitmap(MCBitmap *p_bitmap, uindex_t p_depth, Pixmap &r_pixmap);
bool MCImageBitmapCreateWithOldBitmap(MCBitmap *p_old, MCImageBitmap *&r_new);
void MCImageBitmapSetAlpha(MCImageBitmap *p_bitmap, MCBitmap *p_maskimagealpha);
void MCImageBitmapSetAlphaFromMask(MCImageBitmap *p_bitmap, MCBitmap *p_maskimage);
void MCImageBitmapSetAlphaValue(MCImageBitmap *p_bitmap, uint8_t p_alpha);

////////////////////////////////////////////////////////////////////////////////

struct MCImageCompressedBitmap
{
	uint32_t compression;
	uint8_t *data;
	uindex_t size;

	// PICT & RLE compression schemes don't encode these
	uindex_t width, height, depth;
	MCColor *colors;
	uindex_t color_count;
	uint8_t *mask;
	uindex_t mask_size;

	uint8_t **planes;
	uindex_t *plane_sizes;
};

bool MCImageCreateCompressedBitmap(uint32_t p_compression, MCImageCompressedBitmap *&r_compressed);
bool MCImageCopyCompressedBitmap(MCImageCompressedBitmap *p_src, MCImageCompressedBitmap *&r_dst);
void MCImageFreeCompressedBitmap(MCImageCompressedBitmap *p_compressed);

bool MCImageCompressRLE(MCImageBitmap *p_bitmap, MCImageCompressedBitmap *&r_compressed);
bool MCImageCompressRLE(MCImageIndexedBitmap *p_indexed, MCImageCompressedBitmap *&r_compressed);
bool MCImageDecompressRLE(MCImageCompressedBitmap *p_compressed, MCImageBitmap *&r_bitmap);

bool MCImageCompress(MCImageBitmap *p_bitmap, bool p_dither, MCImageCompressedBitmap *&r_compressed);
bool MCImageDecompress(MCImageCompressedBitmap *p_compressed, MCImageFrame *&r_frames, uindex_t &r_frame_count);

bool MCImageGetMetafileGeometry(IO_handle p_stream, uindex_t &r_width, uindex_t &r_height);
bool MCImageImport(IO_handle p_stream, IO_handle p_mask_stream, MCPoint &r_hotspot, char *&r_name, MCImageCompressedBitmap *&r_compressed, MCImageBitmap *&r_bitmap);
bool MCImageExport(MCImageBitmap *p_bitmap, Export_format p_format, MCImagePaletteSettings *p_palette_settings, bool p_dither, IO_handle p_stream, IO_handle p_mask_stream);

bool MCImageDecode(IO_handle p_stream, MCImageFrame *&r_frames, uindex_t &r_frame_count);
bool MCImageDecode(const uint8_t *p_data, uindex_t p_size, MCImageFrame *&r_frames, uindex_t &r_frame_count);

#ifdef SHARED_STRING
bool MCImageCreateClipboardData(MCImageBitmap *p_bitmap, MCSharedString *&r_data);
#else
bool MCImageCreateClipboardData(MCImageBitmap *p_bitmap, MCStringRef &r_data);
#endif

////////////////////////////////////////////////////////////////////////////////

#if defined(TARGET_PLATFORM_WINDOWS)
bool MCImageBitmapToDIB(MCImageBitmap *p_bitmap, MCWinSysHandle &r_dib);
bool MCImageBitmapToV5DIB(MCImageBitmap *p_bitmap, MCWinSysHandle &r_dib);
bool MCImageBitmapToMetafile(MCImageBitmap *p_bitmap, MCWinSysMetafileHandle &r_metafile);

#elif defined(TARGET_PLATFORM_MACOS_X)
bool MCImageBitmapToPICT(MCImageBitmap *p_bitmap, MCMacSysPictHandle &r_pict);
#endif

////////////////////////////////////////////////////////////////////////////////

// An ImageBuffer is a RGBA premultiplied representation of image data
//
struct MCImageBuffer
{
	int4 width;
	int4 height;
	int4 stride;
	void *data;
};

#include "image_rep.h"

class MCMutableImageRep : public MCImageRep
{
public:
	MCMutableImageRep(MCImage *p_owner, MCImageBitmap *p_bitmap);
	~MCMutableImageRep();

	// Image Rep interface
	virtual MCImageRepType GetType() { return kMCImageRepMutable; }
	virtual uindex_t GetFrameCount();
	virtual bool LockImageFrame(uindex_t p_index, MCImageFrame *&r_frame);
	virtual void UnlockImageFrame(uindex_t p_index, MCImageFrame *p_frame);
	virtual bool GetGeometry(uindex_t &r_width, uindex_t &r_height);

	// implementation
	bool copy_selection(MCImageBitmap *&r_bitmap);

	Boolean image_mfocus(int2 x, int2 y);
	Boolean image_mdown(uint2 which);
	Boolean image_mup(uint2 which);
	Boolean image_doubledown(uint2 which);
	Boolean image_doubleup(uint2 which);

	bool has_selection();

	void drawsel(MCDC *dc);
	void drawselrect(MCDC *dc);

	void image_undo(Ustruct *p_undo);
	void image_freeundo(Ustruct *p_undo);

	void startdraw();
	void continuedraw();
	void enddraw();

	void canceldraw();

	void startrub();
	MCRectangle continuerub(Boolean line);
	void endrub();

	void startseldrag();
	void endsel();

	void put_brush(MCBitmap *image, int2 x, int2 y,
	               uint2 maxwidth, uint2 maxheight, MCbrushmask *which);
	void fill_line(MCBitmap *plane, int2 left, int2 right, int2 y);
	bool bucket_line(MCImageBitmap *simage, uint4 color,
	                    int2 x, int2 y, int2 &l, int2 &r);
	bool bucket_point(MCImageBitmap *simage, uint4 color, MCBitmap *dimage,
	                     MCstacktype pstack[], uint2 &pstacktop,
	                     uint2 &pstackptr, int2 xin, int2 yin, int2 direction,
	                     int2 &xleftout, int2 &xrightout, bool &collide);
	void bucket_fill(MCImageBitmap *simage, uint4 scolor, MCBitmap *dimage,
	                 int2 xleft, int2 oldy);

	MCRectangle drawbrush(Tool which);
	void drawbucket();
	MCRectangle drawline(Boolean cancenter);
	MCRectangle drawreg();
	MCRectangle drawroundrect();
	MCRectangle drawoval();
	MCRectangle drawpencil();
	MCRectangle drawrectangle();

	void pattson(MCContext *ctxt, Boolean miter, uint2 depth);
	void pattsoff(MCContext *ctxt, uint2 depth);
	void battson(MCContext *ctxt, uint2 depth);
	void battsoff(MCContext *ctxt, uint2 depth);

	void fillimage(const MCRectangle &drect);
	void eraseimage(const MCRectangle &drect);

	MCRectangle getopaqueregion(uint1 p_threshold = 0);
	void croptoopaque();

	void selimage();

	void getsel(Boolean cut);
	void cutoutsel();
	void stampsel();
	void rotatesel(int2 angle);
	void flipsel(Boolean ishorizontal);

	void pasteimage(MCImageBitmap *p_bitmap);

	static void init();
	static void shutdown();

private:
	MCImage *m_owner;
	MCImageFrame m_frame;

	MCImageBitmap *m_bitmap;
	MCImageBitmap *m_selection_image;
	MCImageBitmap *m_undo_image;
	MCImageBitmap *m_rub_image;

	/* DUPE */ MCRectangle rect;

	/* DUPE */MCRectangle selrect;

	/* DUPE */uint32_t state;

	/* DUPE */int16_t mx, my;
	/* DUPE */int16_t startx, starty;

	static Boolean erasing;

	static Pixmap editmask;
	static Pixmap editdata;
	static MCBitmap *editimage;
	static MCBitmap *editmaskimage;
	static MCBitmap *editmaskimagealpha;

	static Tool oldtool;
	static MCRectangle newrect;

	static MCPoint *points;
	static uint2 npoints;
	static uint2 polypoints;
};

class MCImageNeed
{
public:
	MCImageNeed(MCObject *p_object);
	~MCImageNeed();

	void Add(MCImageNeed *&p_list);
	void Remove(MCImageNeed *&p_list);

	MCObject *GetObject();
	MCImageNeed *GetNext();

private:
	MCObjectHandle *m_object;
	MCImageNeed *m_prev;
	MCImageNeed *m_next;
};

class MCImage : public MCControl
{
	friend class MCHcbmap;
	
	MCImageRep *m_rep;
	MCTransformedImageRep *m_transformed;
	MCImageFrame *m_locked_frame;
	uint32_t m_image_opened;

	MCImageNeed *m_needs;

	bool m_have_control_colors;
	MCColor *m_control_colors;
	char **m_control_color_names;
	uint16_t m_control_color_count;
	uint16_t m_control_color_flags;
	
	int2 xhot;
	int2 yhot;
	uint2 angle;
	int2 currentframe;
	int2 repeatcount;
	int2 irepeatcount;
	uint1 resizequality;
	MCStringRef filename;
	static int2 magmx;
	static int2 magmy;
	static MCRectangle magrect;
	static MCObject *magtoredraw;

	static Boolean filledborder;
	static MCbrushmask brush;
	static MCbrushmask spray;
	static MCbrushmask eraser;
	static MCCursorRef cursor;
	static MCCursorRef defaultcursor;
	static uint2 cmasks[8];
	
private:
	void setrep(MCImageRep *p_rep);
	
	// replace the current image data with the new bitmap
	bool setbitmap(MCImageBitmap *p_bitmap);
	bool setcompressedbitmap(MCImageCompressedBitmap *p_compressed);
	bool setfilename(MCStringRef p_filename);
	bool setdata(void *p_data, uindex_t p_size);
	
	void notifyneeds(bool p_deleting);
	

	static MCPropertyInfo kProperties[];
	static MCObjectPropertyTable kPropertyTable;
public:
	MCImage();
	MCImage(const MCImage &iref);
	// virtual functions from MCObject
	virtual ~MCImage();
	virtual Chunk_term gettype() const;
	virtual const char *gettypestring();
	virtual const MCObjectPropertyTable *getpropertytable(void) const { return &kPropertyTable; }

	virtual void open();
	virtual void close();
	virtual Boolean mfocus(int2 x, int2 y);
	virtual Boolean mdown(uint2 which);
	virtual Boolean mup(uint2 which);
	virtual Boolean doubledown(uint2 which);
	virtual Boolean doubleup(uint2 which);
	virtual void timer(MCNameRef mptr, MCParameter *params);
	virtual void setrect(const MCRectangle &nrect);
	virtual Exec_stat getprop_legacy(uint4 parid, Properties which, MCExecPoint &, Boolean effective);
	virtual Exec_stat setprop_legacy(uint4 parid, Properties which, MCExecPoint &, Boolean effective);
	virtual void select();
	virtual void deselect();
	virtual void undo(Ustruct *us);
	virtual void freeundo(Ustruct *us);
	
	// MW-2011-09-20: [[ Collision ]] Compute shape of image - will use mask directly if possible.
	virtual bool lockshape(MCObjectShape& r_shape);
	virtual void unlockshape(MCObjectShape& shape);

	// MW-2011-09-06: [[ Redraw ]] Added 'sprite' option - if true, ink and opacity are not set.
	virtual void draw(MCDC *dc, const MCRectangle &dirty, bool p_isolated, bool p_sprite);
	
	// MW-2012-03-28: [[ Bug 10130 ]] No-op for images as there is no font.
	virtual bool recomputefonts(MCFontRef parent_font);

	// virtual functions from MCControl
	IO_stat load(IO_handle stream, const char *version);
	IO_stat extendedload(MCObjectInputStream& p_stream, const char *p_version, uint4 p_length);
	IO_stat save(IO_handle stream, uint4 p_part, bool p_force_ext);
	IO_stat extendedsave(MCObjectOutputStream& p_stream, uint4 p_part);

	virtual MCControl *clone(Boolean attach, Object_pos p, bool invisible);
	virtual Boolean maskrect(const MCRectangle &srect);

	bool isediting() const;
	void startediting(uint16_t p_which);
	void finishediting();
	void sourcerectchanged(MCRectangle p_newrect);

	void invalidate_rep(MCRectangle &p_rect);

	bool convert_to_mutable();

	void resetimage();

	void apply_transform();

	uint8_t getresizequality()
	{
		return resizequality;
	}

	void setframe(int32_t p_newframe);
	void advanceframe();

	uint32_t getcompression();

	// get the current (transformed) image data
	bool lockbitmap(MCImageBitmap *&r_bitmap, bool p_update_transform = true);
	void unlockbitmap(MCImageBitmap *p_bitmap);

	void getgeometry(uint32_t &r_pixwidth, uint32_t &r_pixheight);

	void addneed(MCObject *p_object);

	void endsel();

	// in idraw.cc
	void drawme(MCDC *dc, int2 sx, int2 sy, uint2 sw, uint2 sh, int2 dx, int2 dy);
	void drawcentered(MCDC *dc, int2 x, int2 y, Boolean reverse);

	void canceldraw(void);
	void startmag(int2 x, int2 y);
	void endmag(Boolean close);
	void drawmagrect(MCDC *dc);
	void magredrawdest(const MCRectangle &brect);
	// MW-2012-01-05: [[ Bug 9895 ]] Magnifier redraw now targets a context, rather than fixed window.
	void magredrawrect(MCContext *context, const MCRectangle &dirty);
	Boolean magmfocus(int2 x, int2 y);
	Boolean magmdown(uint2 which);
	Boolean magmup(uint2 which);
	Boolean magdoubledown(uint2 which);
	Boolean magdoubleup(uint2 which);
	// in iutil.cc
	static void init();
	static void shutdown();
	
	void cutimage();
	void copyimage();
	void selimage();
	void delimage();
	void pasteimage(MCImage *clipimage);

	void rotatesel(int2 angle);
	void flipsel(Boolean ishorizontal);

	void compute_gravity(MCRectangle &trect, int2 &xorigin, int2 &yorigin);
	void compute_offset(MCRectangle &p_rect, int16_t &r_xoffset, int16_t &r_yoffset);
	void crop(MCRectangle *newrect);
	void rotate(uint32_t p_angle);
	void resize();
	void createbrush(Properties which);

	static MCbrushmask *getbrush(Tool p_which);

	MCCursorRef createcursor();
	MCCursorRef getcursor(bool p_is_default = false);
	void createpat(Pixmap &newpm, MCColor *&newcolors, uint2 &newncolors);
	// in ifile.cc
	Boolean noblack();
	void compress(MCBitmap *dataimage, bool p_take_geometry = false, bool p_includes_alpha = false);
	void recompress();
	void decompressbrush(Pixmap &m, Pixmap &d, MCBitmap *&b);
	void openimage();
	void closeimage();
	void prepareimage();
	void reopen(bool p_newfile, bool p_lock_size = false);
	// in iimport.cc
	IO_stat import(const char *newname, IO_handle stream, IO_handle mstream);

	// Return a shared string containing the image data in the format required for
	// publishing to the clipboard. For PNG, GIF, JPEG images this is the text 
	// property of the image. For RAW images, the data will be compressed and
	// returned as PNG.
#ifdef SHARED_STRING
	MCSharedString *getclipboardtext(void);
#else
	bool getclipboardtext(MCStringRef& r_data);
#endif
	// MW-2011-09-13: [[ Masks ]] Updated to return a 'MCWindowMask'
	MCWindowShape *makewindowshape(void);
	
#if defined(_MAC_DESKTOP)
	CGImageRef makeicon(uint4 p_width, uint4 p_height);
	CGImageRef converttodragimage(void);
#elif defined(_WINDOWS_DESKTOP)
	MCWinSysIconHandle makeicon(uint4 p_width, uint4 p_height);
#ifdef SHARED_STRING
	MCSharedString *converttodragimage(void);
#else
	void converttodragimage(MCStringRef& r_output);
#endif
#elif defined(_LINUX_DESKTOP)
	Pixmap makewindowregion();
#else
	MCRegionRef makewindowregion(void);
#endif

	void set_gif(uint1 *data, uint4 length);

	MCString getrawdata(void);
	
	MCImage *next()
	{
		return (MCImage *)MCDLlist::next();
	}

	MCImage *prev()
	{
		return (MCImage *)MCDLlist::prev();
	}

	void totop(MCImage *&list)
	{
		MCDLlist::totop((MCDLlist *&)list);
	}

	void insertto(MCImage *&list)
	{
		MCDLlist::insertto((MCDLlist *&)list);
	}

	void appendto(MCImage *&list)
	{
		MCDLlist::appendto((MCDLlist *&)list);
	}

	void append(MCImage *node)
	{
		MCDLlist::append((MCDLlist *)node);
	}

	void splitat(MCImage *node)
	{
		MCDLlist::splitat((MCDLlist *)node) ;
	}

	MCImage *remove(MCImage *&list)
	{
		return (MCImage *)MCDLlist::remove((MCDLlist *&)list);
	}

	////////// PROPERTY SUPPORT METHODS

	void GetTransparencyData(MCExecContext &ctxt, bool p_flatten, MCStringRef &r_data);
	void SetTransparencyData(MCExecContext &ctxt, bool p_flatten, MCStringRef p_data);
    void SetVisibility(MCExecContext& ctxt, uinteger_t part, bool setting, bool visible);
	
	////////// PROPERTY ACCESSORS

	void GetXHot(MCExecContext& ctxt, integer_t& r_x);
	void SetXHot(MCExecContext& ctxt, integer_t p_x);
	void GetYHot(MCExecContext& ctxt, integer_t& r_y);
	void SetYHot(MCExecContext& ctxt, integer_t p_y);
	void GetHotSpot(MCExecContext& ctxt, MCPoint& r_spot);
	void SetHotSpot(MCExecContext& ctxt, MCPoint p_spot);
	void GetFileName(MCExecContext& ctxt, MCStringRef& r_name);
	void SetFileName(MCExecContext& ctxt, MCStringRef p_name);
	void GetAlwaysBuffer(MCExecContext& ctxt, bool& r_setting);
	void SetAlwaysBuffer(MCExecContext& ctxt, bool setting);
	void GetImagePixmapId(MCExecContext& ctxt, uinteger_t*& r_id);
	void SetImagePixmapId(MCExecContext& ctxt, uinteger_t* p_id);
	void GetMaskPixmapId(MCExecContext& ctxt, uinteger_t*& r_id);
	void SetMaskPixmapId(MCExecContext& ctxt, uinteger_t* p_id);
	void GetDontDither(MCExecContext& ctxt, bool& r_setting);
	void SetDontDither(MCExecContext& ctxt, bool setting);
	void GetMagnify(MCExecContext& ctxt, bool& r_setting);
	void SetMagnify(MCExecContext& ctxt, bool setting);
	void GetSize(MCExecContext& ctxt, uinteger_t& r_size);
	void GetCurrentFrame(MCExecContext& ctxt, uinteger_t& r_frame);
	void SetCurrentFrame(MCExecContext& ctxt, uinteger_t p_frame);
	void GetFrameCount(MCExecContext& ctxt, integer_t& r_count);
	void GetPalindromeFrames(MCExecContext& ctxt, bool& r_setting);
	void SetPalindromeFrames(MCExecContext& ctxt, bool setting);
	void GetConstantMask(MCExecContext& ctxt, bool& r_setting);
	void SetConstantMask(MCExecContext& ctxt, bool setting);
	void GetRepeatCount(MCExecContext& ctxt, integer_t& r_count);
	void SetRepeatCount(MCExecContext& ctxt, integer_t p_count);
	void GetFormattedHeight(MCExecContext& ctxt, uinteger_t& r_height);
	void GetFormattedWidth(MCExecContext& ctxt, uinteger_t& r_width);
	void GetText(MCExecContext& ctxt, MCStringRef& r_text);
	void SetText(MCExecContext& ctxt, MCStringRef p_text);
	void GetImageData(MCExecContext& ctxt, MCStringRef& r_data);
	void SetImageData(MCExecContext& ctxt, MCStringRef p_data);
	void GetMaskData(MCExecContext& ctxt, MCStringRef& r_data);
	void SetMaskData(MCExecContext& ctxt, MCStringRef p_data);
	void GetAlphaData(MCExecContext& ctxt, MCStringRef& r_data);
	void SetAlphaData(MCExecContext& ctxt, MCStringRef p_data);
	void GetResizeQuality(MCExecContext& ctxt, intenum_t& r_quality);
	void SetResizeQuality(MCExecContext& ctxt, intenum_t p_quality);
	void GetPaintCompression(MCExecContext& ctxt, intenum_t& r_compression);
	void GetAngle(MCExecContext& ctxt, integer_t& r_angle);
	void SetAngle(MCExecContext& ctxt, integer_t p_angle);
    
    virtual void SetBlendLevel(MCExecContext& ctxt, uinteger_t level);
	virtual void SetInk(MCExecContext& ctxt, intenum_t ink);
    virtual void SetVisible(MCExecContext& ctxt, uinteger_t part, bool setting);
    virtual void SetInvisible(MCExecContext& ctxt, uinteger_t part, bool setting);
};

extern bool MCU_israwimageformat(Export_format p_format);

#endif
