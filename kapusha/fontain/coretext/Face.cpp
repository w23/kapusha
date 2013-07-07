#include <cstring>
#include "../String.h"
#include "Face.h"

namespace kapusha {
namespace fontain {
namespace coretext {

  Face::Face(const char *font_name, vec2i size) {
    //! \todo how to properly map dpi and stuff
    CGFloat fsize = max(size.x, size.y) * 72.f / 96.f;
    CFStringRef fname = CFStringCreateWithCString(kCFAllocatorDefault,
                                                  font_name,
                                                  kCFStringEncodingUTF8);
    font_ = CTFontCreateWithName(fname, fsize, NULL);
    CFRelease(fname);
    
    atlas_.reset(new Atlas(Surface::Meta(vec2i(256), Surface::Meta::R8)));
    
    atlasContext_ =
    CGBitmapContextCreate(const_cast<void*>(atlas_->getSurface()->pixels()),
                          atlas_->meta().size.x, atlas_->meta().size.y,
                          atlas_->meta().bytesPerPixel * 8,
                          atlas_->meta().bytesPerRow,
                          NULL,
                          kCGImageAlphaOnly);
    
    CGFontRef cgFont_ = CTFontCopyGraphicsFont(font_, NULL);
    CGContextSetFont(atlasContext_, cgFont_);
    CGContextSetFontSize(atlasContext_, CTFontGetSize(font_));
    CGContextSetRGBFillColor(atlasContext_, 1.f, 1.f, 1.f, 1.f);
    CGContextTranslateCTM(atlasContext_, 0, atlas_->meta().size.y);
    CGContextScaleCTM(atlasContext_, 1., -1.);
    
    metrics_.ascent = CTFontGetAscent(font_);
    metrics_.descent = CTFontGetDescent(font_);
    metrics_.leading = CTFontGetLeading(font_);
  }
  
  Face::~Face() {
    CFRelease(cgFont_);
    CGContextRelease(atlasContext_);
    CFRelease(font_);
  }
  
  CFAttributedStringRef Face::makeAttributedString(const char *string,
                                                   int length) const {
    if (length == -1) length = static_cast<int>(strlen(string));
    CFStringRef cfstr =
      CFStringCreateWithBytes(kCFAllocatorDefault,
                              reinterpret_cast<const UInt8*>(string), length,
                              kCFStringEncodingUTF8, false);
	const CFStringRef keys[] = { kCTFontAttributeName };
	const CFTypeRef values[] = { font_ };
	CFDictionaryRef attribs =
      CFDictionaryCreate(kCFAllocatorDefault,
                         (const void**)&keys,
                         (const void**)&values,
                         sizeof(keys)/sizeof(*keys),
                         &kCFTypeDictionaryKeyCallBacks,
                         &kCFTypeDictionaryValueCallBacks);
	CFAttributedStringRef atstring =
      CFAttributedStringCreate(kCFAllocatorDefault, cfstr, attribs);
	CFRelease(attribs);
    CFRelease(cfstr);
	return atstring;
  }

  String *Face::createString(const char *string, int length) const {
    CFAttributedStringRef atstring = makeAttributedString(string, length);
    CTLineRef line = CTLineCreateWithAttributedString(atstring);
    CFRelease(atstring);

    // calculate total glyphs count
    // here we skip all other substitute fonts
    /// \todo support several fonts per line. this will require some arch changes
    u32 total_glyphs = 0;
    CFArrayRef runs = CTLineGetGlyphRuns(line);
    CFIndex nruns = CFArrayGetCount(runs);
    for (CFIndex i = 0; i < nruns; ++i) {
      CTRunRef run = (CTRunRef)CFArrayGetValueAtIndex(runs, i);
      CFDictionaryRef attrs = CTRunGetAttributes(run);
      CTFontRef font = (CTFontRef)CFDictionaryGetValue(attrs, kCTFontAttributeName);
      if (CFEqual(font_, font)) total_glyphs += CTRunGetGlyphCount(run);
    }

    String *ret = new String(atlas_.get(), total_glyphs);
    ret->glyphs().resize(total_glyphs);
    u32 glyph_index = 0;
    for (CFIndex i = 0; i < nruns; ++i) {
      CTRunRef run = (CTRunRef)CFArrayGetValueAtIndex(runs, i);
      CFDictionaryRef attrs = CTRunGetAttributes(run);
      CTFontRef font = (CTFontRef)CFDictionaryGetValue(attrs, kCTFontAttributeName);
      if (!CFEqual(font_, font)) continue;
      
      CFIndex nglyphs = CTRunGetGlyphCount(run);
      for (CFIndex j = 0; j < nglyphs; ++j, ++glyph_index) {
        String::Glyph &g = ret->glyphs()[glyph_index];
        CFRange grange = CFRangeMake(j, 1);
        CGGlyph glyph;
        CGPoint pos;
        CFIndex cluster_index;
        CTRunGetGlyphs(run, grange, &glyph);
        CTRunGetPositions(run, grange, &pos);
        CTRunGetStringIndices(run, grange, &cluster_index);
        
        Glyph gg = getGlyph(glyph);
        g.offset = gg.origin + vec2i(pos.x, pos.y);
        g.clusterBegin = static_cast<u32>(cluster_index);
        if (glyph_index > 0)
          ret->glyphs()[glyph_index-1].clusterEnd = static_cast<u32>(cluster_index);
        g.rectInAtlas = gg.atlas;
        ret->box().extendToContain(rect2i(g.offset, g.rectInAtlas.size()+g.offset));
      }
      CFRange rrange = CTRunGetStringRange(run);
      ret->glyphs()[glyph_index-1].clusterEnd =
        static_cast<u32>(rrange.location + rrange.length);
    }
    CFRelease(line);
    return ret;
  }
  
  Face::Glyph Face::getGlyph(CGGlyph glyph) const
  {
    if (const Glyph *cached = grapheMap_.find(glyph)) return *cached;
    
    CGRect rect = CTFontGetBoundingRectsForGlyphs(font_,
                                                  kCTFontOrientationDefault,
                                                  &glyph, NULL, 1);
    vec2i size(floor(rect.size.width)+1, floor(rect.size.height)+1);
    Glyph ret;
    ret.atlas = atlas_->allocate(size, vec2i(1));
    ret.origin = vec2i(rect.origin.x, rect.origin.y);
    CGPoint pos =
      CGPointMake(ret.atlas.min.x - rect.origin.x,
                  ret.atlas.min.y - rect.origin.y);
    CGContextShowGlyphsAtPositions(atlasContext_, &glyph, &pos, 1);

    const_cast<MapOf<Glyph>*>(&grapheMap_)->set(glyph, &ret);
    return ret;
  }
} // namespace coretext
} // namespace fontain
} // namespace kapusha