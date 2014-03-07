;NSIS ExperienceUI - Language File
;Compatible with ExperienceUI 1.09

;Language: Lithuanian (1063)
;By Viktoras Kriukovas

;--------------------------------

!insertmacro XPUI_LANGUAGEFILE_BEGIN "Lithuanian"

; Use only ASCII characters (if this is not possible, use the English name)
!insertmacro XPUI_UNSET XPUI_LANGNAME
!insertmacro XPUI_DEFAULT XPUI_LANGNAME "Lietuviu"

; BUTTONS
!insertmacro XPUI_DEFAULT XPUI_BUTTONTEXT_NEXT   Toliau
!insertmacro XPUI_DEFAULT XPUI_BUTTONTEXT_BACK   Atgal
!insertmacro XPUI_DEFAULT XPUI_BUTTONTEXT_CANCEL Atšaukti
!insertmacro XPUI_DEFAULT XPUI_BUTTONTEXT_CLOSE  Užverti

!insertmacro XPUI_DEFAULT XPUI_ABORTWARNING_TEXT "Ju-s ruošiate.s baigti diegima;.$\n$\nJei ju-s baigsite dabar, $(^Name) nebus i;diegta.$\n$\nAr norite te;sti?"

; +---------+
; | INSTALL |
; +---------+

; WELCOME PAGE
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGE_TEXT_TOP "Sveiki atvyke; i; $(^Name) i;diegimo vedli;"
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGE_TEXT "Šis vedlys pade.s jums i;diegti programa; $(^Name).\r\n\r\nPrie pradedant diegima;, rekomenduojama uždaryti visas kitas programas. Tai pade.s atlikti tinkamus pakeitimus sistemos failuose be pakartotinio kompiuterio paleidimo.\r\n\r\n"
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGE_TITLE "Sveiki"
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGE_SUBTITLE "Sveiki atvyke; i; $(^Name) diegima;."
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGE_CAPTION " "

!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGE_TEXT_TOP "Sveiki atvyke; i; $(^Name) pašalinimo vedli;"
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGE_TEXT "Šis vedlys pade.s jums pašalinti programa; $(^Name).\r\n\r\nPrie pradedant diegima;, rekomenduojama uždaryti visas kitas programas. Tai pade.s atlikti tinkamus pakeitimus sistemos failuose be pakartotinio kompiuterio paleidimo.\r\n\r\n"
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGE_TITLE "Sveiki"
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGE_SUBTITLE "Sveiki atvyke; i; $(^Name) pašalinimo vedli;."
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGE_CAPTION " "

; WELCOME PAGE STYLE 2
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGESTYLE2_TEXT_TOP "Sveiki atvyke; i; $(^Name) i;diegimo vedli;"
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGESTYLE2_TEXT "Sveiki atvyke; i; $(^Name) diegima;.  Tai i;diegs $(^Name) ju-su; kompiuteryje."
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGESTYLE2_TITLE "Sveiki"
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGESTYLE2_SUBTITLE "Sveiki atvyke; i; $(^Name) diegima;."
!insertmacro XPUI_DEFAULT XPUI_WELCOMEPAGESTYLE2_CAPTION " "

!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGESTYLE2_TEXT_TOP "Sveiki atvyke; i; $(^Name) pšalinimo vedli;."
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGESTYLE2_TEXT "Sveiki atvyke; i; $(^Name) diegima;.  Tai pašalins $(^Name) iš ju-su; kompiuterio."
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGESTYLE2_TITLE "Sveiki"
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGESTYLE2_SUBTITLE "Sveiki atvyke; i; $(^Name) pašalinima;."
!insertmacro XPUI_DEFAULT XPUI_UNWELCOMEPAGESTYLE2_CAPTION " "

; LICENSE PAGE
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_CAPTION ": Licencine. sutartis"
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_RADIOBUTTONS_TEXT_ACCEPT "Aš sutinku su aukšc(iau esanc(iomis sa;lygomis"
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_RADIOBUTTONS_TEXT_DECLINE "Aš nesutinku su aukšc(iau esanc(iomis sa;lygomis"
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_TEXT_CHECKBOX "Aš sutinku su aukšc(iau esanc(iomis sa;lygomis"
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_SUBTITLE "Prašome peržiu-re.ti licencines sa;lygas prieš diegdami $(^Name)."
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_TITLE "Licencine. sutartis"
!insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_TEXT_TOP "Paspauskite mygtuka; Page Down, kad peržiu-re.tume.te likusia; sutarties dali;."
!ifndef XPUI_LICENSEPAGE_RADIOBUTTONS
  !ifndef XPUI_LICENSEPAGE_CHECKBOX
    !insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_TEXT_BOTTOM "Jei ju-s sutinkate su sutarties sa;lygomis, paspauskite Aš sutinku nore.dami te;sti. Nore.dami i;diegti $(^Name) privalote sutikti su sutartimi."
  !endif
!endif
!ifndef XPUI_LICENSEPAGE_RADIOBUTTONS
  !ifdef XPUI_LICENSEPAGE_CHECKBOX
    !insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_TEXT_BOTTOM "Jei ju-s sutinkate su sutarties sa;lygomis, pažyme.kite žemiau esanti; lauka;. Nore.dami i;diegti $(^Name) privalote sutikti su sutartimi."
  !endif
!endif
!ifdef XPUI_LICENSEPAGE_RADIOBUTTONS
  !ifndef XPUI_LICENSEPAGE_CHECKBOX
    !insertmacro XPUI_DEFAULT XPUI_LICENSEPAGE_TEXT_BOTTOM "Jei ju-s sutinkate su sutarties sa;lygomis, pasirinkite pirma; aukšc(iau esanti; varianta;. Nore.dami i;diegti $(^Name) privalote sutikti su sutartimi."
  !endif
!endif

!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_CAPTION ": Licencine. sutartis"
!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_RADIOBUTTONS_TEXT_ACCEPT "Aš sutinku su aukšc(iau esanc(iomis sa;lygomis"
!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_RADIOBUTTONS_TEXT_DECLINE "Aš nesutinku su aukšc(iau esanc(iomis sa;lygomis"
!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_TEXT_CHECKBOX "Aš sutinku su aukšc(iau esanc(iomis sa;lygomis"
!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_SUBTITLE "Prašome peržiu-re.ti licencines sa;lygas prieš pašalindami $(^Name)."
!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_TITLE "Licencine. sutartis"
!insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_TEXT_TOP "Paspauskite mygtuka; Page Down, kad peržiu-re.tume.te likusia; sutarties dali;."
!ifndef XPUI_UNLICENSEPAGE_RADIOBUTTONS
  !ifndef XPUI_UNLICENSEPAGE_CHECKBOX
    !insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_TEXT_BOTTOM "Jei ju-s sutinkate su sutarties sa;lygomis, paspauskite Aš sutinku nore.dami te;sti. Nore.dami pašalindami $(^Name) privalote sutikti su sutartimi."
  !endif
!endif
!ifndef XPUI_UNLICENSEPAGE_RADIOBUTTONS
  !ifdef XPUI_UNLICENSEPAGE_CHECKBOX
    !insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_TEXT_BOTTOM "Jei ju-s sutinkate su sutarties sa;lygomis, pažyme.kite žemiau esanti; lauka;. Nore.dami pašalinti $(^Name) privalote sutikti su sutartimi."
  !endif
!endif
!ifdef XPUI_UNLICENSEPAGE_RADIOBUTTONS
  !ifndef XPUI_UNLICENSEPAGE_CHECKBOX
    !insertmacro XPUI_DEFAULT XPUI_UNLICENSEPAGE_TEXT_BOTTOM "Jei ju-s sutinkate su sutarties sa;lygomis, pasirinkite pirma; aukšc(iau esanti; varianta;. Nore.dami pašalinti $(^Name) privalote sutikti su sutartimi."
  !endif
!endif

; COMPONENTS PAGE
!insertmacro XPUI_DEFAULT XPUI_COMPONENTSPAGE_CAPTION ": Pasirinkite komponentus"
!insertmacro XPUI_DEFAULT XPUI_COMPONENTSPAGE_SUBTITLE "Pasirinkite kuriuos $(^Name) komponentus norite i;diegti."
!insertmacro XPUI_DEFAULT XPUI_COMPONENTSPAGE_TITLE "Pasirinkite komponentus"
!insertmacro XPUI_DEFAULT XPUI_COMPONENTSPAGE_TEXT_DESCRIPTION_TITLE "Aprašymas"
!insertmacro XPUI_DEFAULT XPUI_COMPONENTSPAGE_TEXT_DESCRIPTION_INFO  "Palaikykite žymekli; virš komponento, nore.dami peržiu-re.ti jo aprašyma;"

!insertmacro XPUI_DEFAULT XPUI_UNCOMPONENTSPAGE_CAPTION ": Pasirinkite komponentus"
!insertmacro XPUI_DEFAULT XPUI_UNCOMPONENTSPAGE_SUBTITLE "Pasirinkite kuriuos $(^Name) komponentus norite pašalinti."
!insertmacro XPUI_DEFAULT XPUI_UNCOMPONENTSPAGE_TITLE "Pasirinkite komponentus"
!insertmacro XPUI_DEFAULT XPUI_UNCOMPONENTSPAGE_TEXT_DESCRIPTION_TITLE "Aprašymas"
!insertmacro XPUI_DEFAULT XPUI_UNCOMPONENTSPAGE_TEXT_DESCRIPTION_INFO  "Palaikykite žymekli; virš komponento, nore.dami peržiu-re.ti jo aprašyma;"

; DIRECTORY PAGE
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_CAPTION ": Pasirinkite i;diegimo aplanka;"
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_TEXT_TOP "Diegimo programa i;diegs $(^Name) esanc(ia; sekanc(iame aplanke.$\r$\n$\r$\nNore.dami i;diegti numatytame aplanke, palikite žemiau esanti; teksta; toki;, koks yra.  Nore.dami i;diegti kitame aplanke, i;veskite pavadinima; žemiau arba paspauskite Naršyti. $_CLICK"
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_TEXT_DESTINATION "I;diegimo katalogas"
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_TEXT_BROWSE "Naršyti"
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_TEXT_BROWSEDIALOG "Prašome pasirinkti aplanka;:"
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_TITLE "Pasirinkite i;diegimo vieta;"
!insertmacro XPUI_DEFAULT XPUI_DIRECTORYPAGE_SUBTITLE "Pasirinkite aplanka;, kuriame norite i;diegti $(^Name)."

!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_CAPTION ": Pasirinkite i;diegimo aplanka;"
!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_TEXT_TOP "Diegimo programa pašalins $(^Name) esanc(ia; sekanc(iame aplanke.$\r$\n$\r$\nNore.dami pašalinti iš šio aplanko, spauskite Toliau. Nore.dami pašalinti iš kito aplanko, i;veskite pavadinima; žemiau arba paspauskite Naršyti."
!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_TEXT_DESTINATION "I;diegimo katalogas"
!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_TEXT_BROWSE "Naršyti"
!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_TEXT_BROWSEDIALOG "Prašome pasirinkti aplanka;:"
!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_TITLE "Pasirinkite i;diegimo vieta;"
!insertmacro XPUI_DEFAULT XPUI_UNDIRECTORYPAGE_SUBTITLE "Pasirinkite aplanka;, iš kurio norite pašalinti $(^Name)."

; START MENU PAGE
!insertmacro XPUI_DEFAULT XPUI_STARTMENUPAGE_CAPTION    ": Meniu Prade.ti aplankas"
!insertmacro XPUI_DEFAULT XPUI_STARTMENUPAGE_TITLE      "Pasirinkite meniu Prade.ti aplanka;"
!insertmacro XPUI_DEFAULT XPUI_STARTMENUPAGE_SUBTITLE   "Pasirinkite aplanka;, kuriame norite sukurti meniu Prade.ti nuorodas programai $(^Name):"
!insertmacro XPUI_DEFAULT XPUI_STARTMENUPAGE_TEXT       "Pasirinkite meniu Prade.ti aplanka;, kuriame norite sukurti nuorodas programai $(^Name):"
!insertmacro XPUI_DEFAULT XPUI_STARTMENUPAGE_CHECKBOX   "Nekurti meniu Prade.ti aplanko"
!insertmacro XPUI_DEFAULT XPUI_STARTMENUPAGE_FOLDER     "$(^Name)"

!insertmacro XPUI_DEFAULT XPUI_UNSTARTMENUPAGE_CAPTION  ": Meniu Prade.ti aplankas"
!insertmacro XPUI_DEFAULT XPUI_UNSTARTMENUPAGE_TITLE    "Pasirinkite meniu Prade.ti aplanka;"
!insertmacro XPUI_DEFAULT XPUI_UNSTARTMENUPAGE_SUBTITLE "Pasirinkite aplanka;, iš kurio norite pašalinti meniu Prade.ti nuorodas programai $(^Name):"
!insertmacro XPUI_DEFAULT XPUI_UNSTARTMENUPAGE_TEXT     "Pasirinkite meniu Prade.ti aplanka;, iš kurio norite pašalinti nuorodas programai $(^Name):"
!insertmacro XPUI_DEFAULT XPUI_UNSTARTMENUPAGE_CHECKBOX "Nešalinti meniu Prade.ti aplanko"
!insertmacro XPUI_DEFAULT XPUI_UNSTARTMENUPAGE_FOLDER   "$(^Name)"

; INSTALL CONFIRM PAGE
!insertmacro XPUI_DEFAULT XPUI_INSTCONFIRMPAGE_CAPTION ": Patvirtinkite i;diegima;"
!insertmacro XPUI_DEFAULT XPUI_INSTCONFIRMPAGE_SUBTITLE "I;diegimo programa baige. duomenu; surinkima; ir yra pasiruošusi i;diegti programa; $(^Name)."
!insertmacro XPUI_DEFAULT XPUI_INSTCONFIRMPAGE_TITLE "Patvirtinkite i;diegima;"
!insertmacro XPUI_DEFAULT XPUI_INSTCONFIRMPAGE_TEXT_TOP "I;diegimo programa yra pasiruošusi i;diegti $(^Name) i; ju-su; kompiuteri;."
!insertmacro XPUI_DEFAULT XPUI_INSTCONFIRMPAGE_TEXT_BOTTOM "$_CLICK"

!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_CAPTION ": Patvirtinkite i;diegima;"
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_SUBTITLE "I;diegimo programa baige. duomenu; surinkima; ir yra pasiruošusi pašalinti programa; $(^Name)."
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_TITLE "Patvirtinkite pašalinima;"
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_TEXT_TOP "I;diegimo programa yra pasiruošusi pašalinti $(^Name) iš ju-su; kompiuterio."
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_TEXT_BOTTOM "Spauskite Toliau, nore.dami te;sti."

; INSTFILES PAGE
!insertmacro XPUI_DEFAULT XPUI_INSTFILESPAGE_CAPTION ": Kopijuojami failai"
!insertmacro XPUI_DEFAULT XPUI_INSTFILESPAGE_SUBTITLE "Prašome palaukti, kol $(^Name) bus i;diegta."
!insertmacro XPUI_DEFAULT XPUI_INSTFILESPAGE_TITLE "I;diegiama"
!insertmacro XPUI_DEFAULT XPUI_INSTFILESPAGE_DONE_TITLE "I;diegimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_INSTFILESPAGE_DONE_SUBTITLE "Visi $(^Name) komponentai buvo se.kmingai nukopijuoti i; ju-su; kompiuteri;."

!insertmacro XPUI_DEFAULT XPUI_UNINSTFILESPAGE_CAPTION ": Pašalinami failai"
!insertmacro XPUI_DEFAULT XPUI_UNINSTFILESPAGE_SUBTITLE "Prašome palaukti, kol $(^Name) bus pašalinta."
!insertmacro XPUI_DEFAULT XPUI_UNINSTFILESPAGE_TITLE "Pašalinama"
!insertmacro XPUI_DEFAULT XPUI_UNINSTFILESPAGE_DONE_TITLE "Pašalinimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_UNINSTFILESPAGE_DONE_SUBTITLE "Visi $(^Name) komponentai buvo pašalinti iš ju-su; kompiuterio."

; INSTALL SUCCESS PAGE
!insertmacro XPUI_DEFAULT XPUI_INSTSUCCESSPAGE_CAPTION ": I;diegimas se.kmingas"
!insertmacro XPUI_DEFAULT XPUI_INSTSUCCESSPAGE_SUBTITLE "Diegimo programa se.kmingai i;diege. $(^Name) i; ju-su; kompiuteri;."
!insertmacro XPUI_DEFAULT XPUI_INSTSUCCESSPAGE_TITLE "I;diegimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_INSTSUCCESSPAGE_TEXT_TOP    "$(^Name) buvo se.kmingai i;diegta."
!insertmacro XPUI_DEFAULT XPUI_INSTSUCCESSPAGE_TEXT_BOTTOM "Spauskite Užverti, nore.dami išeiti."

!insertmacro XPUI_DEFAULT XPUI_UNINSTSUCCESSPAGE_CAPTION ": Pašalinimas se.kmingas"
!insertmacro XPUI_DEFAULT XPUI_UNINSTSUCCESSPAGE_SUBTITLE "Diegimo programa se.kmingai pašalino $(^Name) iš ju-su; kompiuterio."
!insertmacro XPUI_DEFAULT XPUI_UNINSTSUCCESSPAGE_TITLE "Pašalinimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_UNINSTSUCCESSPAGE_TEXT_TOP    "$(^Name) buvo se.kmingai pašalinta."
!insertmacro XPUI_DEFAULT XPUI_UNINSTSUCCESSPAGE_TEXT_BOTTOM "Spauskite Užverti, nore.dami išeiti."

; FINISH PAGE

!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_TITLE "I;diegimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_SUBTITLE "Diegimo programa se.kmingai i;diege. $(^Name) i; ju-su; kompiuteri;."
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_CAPTION ": I;diegimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_TEXT_TOP "Baigiamas $(^Name) diegimo vedlys"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_TEXT_TOP_ALT "Diegimo vedlys baigtas"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_TEXT_RUN "Diegimo programa se.kmingai i;diege. $(^Name) ju-su; kompiuteryje.\r\n\r\nKokius veiksmus norite atlikti?"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_TEXT_REBOOT "Diegimo programa baige. failu; kopijavima; i; ju-su; kompiuteri;.\r\n\r\nNore.dami te;sti tiegima;, privalote paleisti kompiuteri; iš naujo. Ar norite perkrauti kompiuteri; dabar?"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_TEXT "Diegimo programa se.kmingai i;diege. $(^Name) ju-su; kompiuteryje.\r\n\r\nPrašome paspausti $(XPUI_BUTTONTEXT_CLOSE), nore.dami išeiti iš diegimo."
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_CHECKBOX_RUN "Paleisti $(^Name) dabar"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_CHECKBOX_DOCS "Rodyti $(^Name) dokumentacija;"
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_RADIOBUTTON_REBOOT "Taip, paleisti kompiuteri; iš naujo."
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_RADIOBUTTON_NOREBOOT "Ne, aš perkrausiu mano kompiuteri; ve.liau."
!insertmacro XPUI_DEFAULT XPUI_FINISHPAGE_REBOOT_MESSAGEBOX "Diegimo programa tuoj perkraus ju-su; kompiuteri;.$\r$\n$\r$\nPrašome išsaugoti ir uždaryti visus atvirus failus ir dokumentus, ir paspauskite Gerai, kad perkrautume.te kompiuteri;."

!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_TITLE "Pašalinimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_SUBTITLE "Diegimo programa se.kmingai pašalino $(^Name) iš ju-su; kompiuterio."
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_CAPTION ": Pašalinimas baigtas"
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_TEXT_TOP "Baigiamas $(^Name) diegimo vedlys"
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_TEXT_TOP_ALT "Diegimo vedlys baigtas"
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_TEXT_REBOOT "Diegimo programa baige. failu; kopijavima; i; ju-su; kompiuteri;.\r\n\r\nNore.dami te;sti pašalinima;, privalote paleisti kompiuteri; iš naujo. Ar norite perkrauti kompiuteri; dabar?"
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_TEXT_RUN "Diegimo programa se.kmingai pašalino $(^Name) iš ju-su; kompiuterio.\r\n\r\nKokius veiksmus norite atlikti?"
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_TEXT "Diegimo programa se.kmingai pašalino $(^Name) iš ju-su; kompiuterio.\r\n\r\nPrašome paspausti $(XPUI_BUTTONTEXT_CLOSE), nore.dami išeiti iš diegimo."
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_RADIOBUTTON_REBOOT "Taip, paleisti kompiuteri; iš naujo."
!insertmacro XPUI_DEFAULT XPUI_UNFINISHPAGE_RADIOBUTTON_NOREBOOT "Ne, aš perkrausiu mano kompiuteri; ve.liau."

; ABORT PAGE
!insertmacro XPUI_DEFAULT XPUI_ABORTPAGE_TEXT_TOP "Diegimo vedlys užbaigtas"
!insertmacro XPUI_DEFAULT XPUI_ABORTPAGE_TEXT "Vedlys buvo nutrauktas prieš tai, kai $(^Name) gale.jo bu-ti se.kmingai i;diegta.\r\n\r\nJu-su; sistema nebuvo pakeista.  Nore.dami te;sti diegima; ve.liau, prašome paleisti ši; vedli; dar karta;.\r\n\r\n\r\n\r\n\r\nPrašome paspausti Užverti, nore.dami išeiti iš diegimo vedlio."
!insertmacro XPUI_DEFAULT XPUI_ABORTPAGE_TITLE "Nebaigtas i;diegimas"
!insertmacro XPUI_DEFAULT XPUI_ABORTPAGE_SUBTITLE "$(^Name) diegimas nebuvo užbaigtas se.kmingai."
!insertmacro XPUI_DEFAULT XPUI_ABORTPAGE_CAPTION ": Diegimas atšauktas"

!insertmacro XPUI_DEFAULT XPUI_UNABORTPAGE_TEXT_TOP "Pašalinimo vedlys užbaigtas"
!insertmacro XPUI_DEFAULT XPUI_UNABORTPAGE_TEXT "Vedlys buvo nutrauktas prieš tai, kai $(^Name) gale.jo bu-ti se.kmingai pašalinta.\r\n\r\nJu-su; sistema nebuvo pakeista.  Nore.dami te;sti pašalinima; ve.liau, prašome paleisti ši; vedli; dar karta;.\r\n\r\n\r\n\r\n\r\nPrašome paspausti Užverti, nore.dami išeiti iš diegimo vedlio."
!insertmacro XPUI_DEFAULT XPUI_UNABORTPAGE_TITLE "Nebaigtas pašalinimas"
!insertmacro XPUI_DEFAULT XPUI_UNABORTPAGE_SUBTITLE "$(^Name) diegimas nebuvo užbaigtas se.kmingai."
!insertmacro XPUI_DEFAULT XPUI_UNABORTPAGE_CAPTION ": Diegimas atšauktas"

; +-----------+
; | UNINSTALL |
; +-----------+

; MOST OF THE UNINSTALL PAGES ARE TAKEN CARE OF USING THE PAGE MODE SYSTEM
; THE XPUI CONFIRM, UNINSTFILES, AND SUCCESS PAGES USE THE PAGE MODE SYSTEM,
; BUT THE NSIS-STYLE UNINSTALL CONFIRM PAGE SIMPLY USES A NON-PAGE-MODE METHOD.

; UNINST CONFIRM PAGE (NSIS)
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_NSIS_CAPTION ": Patvirtinkite programos pašalinima;"
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_NSIS_SUBTITLE "Pašalinti $(^Name) iš ju-su; kompiuterio"
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_NSIS_TITLE "Pašalinti $(^Name)"
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_NSIS_TEXT_TOP "$(^Name) bus pašalinta iš sekanc(io aplanko. Paspauskite mygtuka; Pašalinti, nore.dami prade.ti pašalinima;."
!insertmacro XPUI_DEFAULT XPUI_UNINSTCONFIRMPAGE_NSIS_TEXT_FOLDER "Pašalinama iš:"

!insertmacro XPUI_LANGUAGEFILE_END
