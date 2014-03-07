!macro UMUI_BG
		SetOutPath "$PLUGINSDIR"
		File "${NSISDIR}\Contrib\UltraModernUI\BGSkins\wxp\SpotlightBG.bmp"
		File "${NSISDIR}\Contrib\UltraModernUI\BGSkins\wxp\KeyBG.bmp"
		File "${NSISDIR}\Contrib\UltraModernUI\BGSkins\wxp\HeaderBG.bmp"
		File "${NSISDIR}\Contrib\UltraModernUI\BGSkins\wxp\BtmImgBG.bmp"
		BgImage::SetBg /NOUNLOAD /GRADIENT 78 111 214 78 111 214
		BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\HeaderBG.bmp" 0 0
		BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\SpotlightBG.bmp" 0 57
		BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\BtmImgBG.bmp" 0 -70
		BgImage::AddImage /NOUNLOAD "$PLUGINSDIR\KeyBG.bmp" -248 -351
		CreateFont $1 "Verdana" 30 700
		BgImage::AddText /NOUNLOAD "$(^Name)" $1 8 51 155 15 70 -1 -1
		BgImage::AddText /NOUNLOAD "$(^Name)" $1 255 255 255 10 65 -1 -1
		BgImage::Redraw /NOUNLOAD
!macroend
!macro UMUI_BG_Destroy
		BgImage::Destroy
!macroend
