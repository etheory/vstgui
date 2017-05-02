//-----------------------------------------------------------------------------
// VST Plug-Ins SDK
// VSTGUI: Graphical User Interface Framework for VST plugins
//
// Version 4.3
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// (c) 2015, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __ctextlabel__
#define __ctextlabel__

#include "cparamdisplay.h"
#include "../cstring.h"

namespace VSTGUI {

//-----------------------------------------------------------------------------
// CLabel Declaration
//! @brief a text label
/// @ingroup controls
//-----------------------------------------------------------------------------
class CTextLabel : public CParamDisplay
{
public:
	CTextLabel (const CRect& size, UTF8StringPtr txt = nullptr, CBitmap* background = nullptr, const int32_t style = 0);
	CTextLabel (const CTextLabel& textLabel);
	
	//-----------------------------------------------------------------------------
	/// @name CTextLabel Methods
	//-----------------------------------------------------------------------------
	//@{
	virtual void setText (const UTF8String& txt);			///< set text
	virtual const UTF8String& getText () const;				///< read only access to text

	enum TextTruncateMode {
		kTruncateNone = 0,						///< no characters will be removed
		kTruncateHead,							///< characters will be removed from the beginning of the text
		kTruncateTail							///< characters will be removed from the end of the text
	};
	
	virtual void setTextTruncateMode (TextTruncateMode mode);					///< set text truncate mode
	TextTruncateMode getTextTruncateMode () const { return textTruncateMode; }	///< get text truncate mode
	const UTF8String& getTruncatedText () const { return truncatedText; }		///< get the truncated text
	//@}

	static IdStringPtr kMsgTruncatedTextChanged;								///< message which is send to dependent objects when the truncated text changes
	
	void draw (CDrawContext* pContext) override;
	bool sizeToFit () override;
	void setViewSize (const CRect& rect, bool invalid = true) override;
	void drawStyleChanged () override;
	void valueChanged () override;

	CLASS_METHODS(CTextLabel, CParamDisplay)
protected:
	~CTextLabel () noexcept override = default;
	void freeText ();
	void calculateTruncatedText ();

	bool onWheel (const CPoint& where, const float& distance, const CButtonState& buttons) override { return false; }
	bool onWheel (const CPoint& where, const CMouseWheelAxis& axis, const float& distance, const CButtonState& buttons) override { return false; }

	TextTruncateMode textTruncateMode;
	UTF8String text;
	UTF8String truncatedText;
};

//-----------------------------------------------------------------------------
/** Multi line text label
 *	@ingroup new_in_4_5
 */
class CMultiLineTextLabel : public CTextLabel
{
public:
	CMultiLineTextLabel (const CRect& size);
	CMultiLineTextLabel (const CMultiLineTextLabel&) = default;

	enum class LineLayout {
		clip, ///< clip lines overflowing the view size width
		truncate, ///< truncate lines overflowing the view size width
		wrap ///< wrap overflowing words to next line
	};
	void setLineLayout (LineLayout layout);
	LineLayout getLineLayout () const { return lineLayout; }

	/** automatically resize the view according to the contents (only the height) 
	 *	@param state on or off
	 */
	void setAutoHeight (bool state);
	/** returns true if this view resizes itself according to the contents */
	bool getAutoHeight () const { return autoHeight; }

	/** return the maximum line width of all lines */
	CCoord getMaxLineWidth ();

	void drawRect (CDrawContext* pContext, const CRect& updateRect) override;
	bool sizeToFit () override;
	void setText (const UTF8String& txt) override;
	void setViewSize (const CRect& rect, bool invalid = true) override;
	void setTextTruncateMode (TextTruncateMode mode) override;
private:
	void drawStyleChanged () override;
	void recalculateLines (CDrawContext* context);
	void recalculateHeight ();
	
	bool autoHeight {false};
	LineLayout lineLayout {LineLayout::clip};

	struct Line
	{
		CRect r;
		UTF8String str;
	};
	using Lines = std::vector<Line>;
	Lines lines;
};

} // namespace

#endif
