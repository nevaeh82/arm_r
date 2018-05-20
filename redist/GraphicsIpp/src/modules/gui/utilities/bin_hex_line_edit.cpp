#include "StdAfx.h"
#include "bin_hex_line_edit.h"

using namespace gui;

BinHexLineEdit::~BinHexLineEdit() {}


void gui::BinHexLineEdit::setText( QString text )
{
	line_editor_.setText(text);
}

void gui::BinHexLineEdit::setWorkMode( WorkMode workMode )
{
	if (workMode == workMode_) return;
	workMode_ = workMode;
	QString prevText = line_editor_.text();
	QString newText;
	line_editor_.setValidator(new BinHexLineEditValidator(workMode_,&line_editor_));
	if (workMode_ == mode_Hex)
	{
		miniButton_.setText("FF");
	}
	if (workMode_ == mode_Bin)
	{
		miniButton_.setText("01");
	}
	if (workMode_ == mode_Dec)
	{
		miniButton_.setText("10");
	}
	line_editor_.setText(newText);
}

void gui::BinHexLineEdit::miniButtonClicked_slot()
{
	if (workMode_ == mode_Hex) {setWorkMode(mode_Bin);return;}
	if (workMode_ == mode_Bin) {setWorkMode(mode_Dec);return;}
	if (workMode_ == mode_Dec) {setWorkMode(mode_Hex);return;}
}


QValidator::State BinHexLineEditValidator::validate( QString & input, int & pos ) const
{
	if (input.size() == 0) return QValidator::Invalid;
	input = input.toUpper();
	for (int i =0;i < input.size();i++) 
	{
		if (workMode_ == BinHexLineEdit::mode_Hex)
		{
			if (!(((input[i].toAscii() >= 0x30) && (input[i].toAscii() <= 0x39)) || ((input[i].toAscii() >= 0x41) && (input[i].toAscii() <= 0x46))))
				return QValidator::Invalid;
		}

		if (workMode_ == BinHexLineEdit::mode_Bin)
		{
			if (!((input[i].toAscii() == 0x30) || (input[i].toAscii() == 0x31)))
				return QValidator::Invalid;
		}
	}
	
	return QValidator::Acceptable;
}
