#----------------------------------------------------------------------------
#       MAKEFILE
#
#	Controlling makefile for File Assistant
#
#	Created:	1st August 2005
#
#	Copyright (C) 1995-2005 T Swann
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Target to make
#	--------------

TARGET :=					FileAssistant

#----------------------------------------------------------------------------
#	Project folders
#	---------------

SOURCE_DIR :=				.
INCLUDE_DIR :=				.

PSP_MEDIA_CENTER_DIR :=		$(SOURCE_DIR)
CODEC_DIR :=				$(PSP_MEDIA_CENTER_DIR)/Codec
CODEC_INCLUDE_DIR :=		$(PSP_MEDIA_CENTER_DIR)/Codec

SC68_INCLUDE_DIR :=			$(shell psp-config --pspdev-path)/psp/include/sc68

NOFRENDO_DIR := 			$(PSP_MEDIA_CENTER_DIR)/Codec/nsf/NofrendoLib
NOFRENDO_INCLUDE_DIR := 	$(NOFRENDO_DIR)/src/ \
							$(NOFRENDO_DIR)/src/machine \
							$(NOFRENDO_DIR)/src/cpu/nes6502 \
							$(NOFRENDO_DIR)/src/sndhrdw

TINY_XML_DIR :=				$(SOURCE_DIR)/TinyXML
TINY_XML_INCLUDE_DIR :=		$(TINY_XML_DIR)

PLUGINS_DIR :=				$(SOURCE_DIR)/Plugins

GAMESWF_DIR :=				$(SOURCE_DIR)/GameSWF

FTP_DIR :=					$(SOURCE_DIR)/FTP

FFMPEG_DIR :=				../../FFMpeg
FFMPEG_CODEC_DIR :=			$(CODEC_DIR)/ffmpeg
FFMPEG_LIB_DIR :=			$(FFMPEG_DIR)/libavformat \
							$(FFMPEG_DIR)/libavcodec \
							$(FFMPEG_DIR)/libavutil \
							$(FFMPEG_CODEC_DIR)/jonny

#----------------------------------------------------------------------------
#	Source to make
#	--------------

ASM_OBJS :=					$(FTP_DIR)/sceNet.o \
							$(FTP_DIR)/sceNetAdhocctl.o \
							$(FTP_DIR)/sceNetInet.o \
							$(FTP_DIR)/sceNetAdhoc.o \
							$(FTP_DIR)/sceNetApctl.o \
							$(FTP_DIR)/sceNetResolver.o

FA_OBJS :=					$(SOURCE_DIR)/Main.o \
							$(SOURCE_DIR)/CFrameWork.o \
							$(SOURCE_DIR)/CGfx.o \
							$(SOURCE_DIR)/CVector.o \
							$(SOURCE_DIR)/CUSBManager.o \
							$(SOURCE_DIR)/CInput.o \
							$(SOURCE_DIR)/CString.o \
							$(SOURCE_DIR)/CFileSystem.o \
							$(SOURCE_DIR)/CTextureManager.o \
							$(SOURCE_DIR)/PNGReader.o \
							$(SOURCE_DIR)/JPGReader.o \
							$(SOURCE_DIR)/TGAReader.o \
							$(SOURCE_DIR)/BMPReader.o \
							$(SOURCE_DIR)/CFont.o \
							$(SOURCE_DIR)/CProcess.o \
							$(SOURCE_DIR)/CSizedItem.o \
							$(SOURCE_DIR)/CWindow.o \
							$(SOURCE_DIR)/CWindowItem.o \
							$(SOURCE_DIR)/CWindowTable.o \
							$(SOURCE_DIR)/CWindowText.o \
							$(SOURCE_DIR)/CDirectoryList.o \
							$(SOURCE_DIR)/CFileOptions.o \
							$(SOURCE_DIR)/CSkinManager.o \
							$(SOURCE_DIR)/CFileAssistant.o \
							$(SOURCE_DIR)/CMessageBox.o \
							$(SOURCE_DIR)/CBackground.o \
							$(SOURCE_DIR)/CRenderable.o \
							$(SOURCE_DIR)/CFileHandler.o \
							$(SOURCE_DIR)/CExecutableFileHandler.o \
							$(SOURCE_DIR)/CImageFileHandler.o \
							$(SOURCE_DIR)/CMusicFileHandler.o \
							$(SOURCE_DIR)/CTextBuffer.o \
							$(SOURCE_DIR)/CTextFileHandler.o \
							$(SOURCE_DIR)/CHUD.o \
							$(SOURCE_DIR)/CTextInput.o \
							$(SOURCE_DIR)/COptionsMenu.o \
							$(SOURCE_DIR)/CSkinSelectMenu.o \
							$(SOURCE_DIR)/CConfigFile.o \
							$(SOURCE_DIR)/CAssert.o \
							$(SOURCE_DIR)/CInformationDialog.o \
							$(SOURCE_DIR)/CPRXManager.o \
							$(SOURCE_DIR)/CKeyboard.o \
							$(SOURCE_DIR)/CLanguage.o \
							$(SOURCE_DIR)/CMusicPlugin.o \
							$(SOURCE_DIR)/CMatrix.o \
							$(SOURCE_DIR)/CFTPClient.o \
							$(SOURCE_DIR)/CSWFFileHandler.o \
							$(SOURCE_DIR)/CLUAFileHandler.o \
							$(SOURCE_DIR)/CMovieFileHandler.o \
							$(SOURCE_DIR)/CMath.o

CODEC_OBJS :=				$(CODEC_DIR)/ahx/ahx.o \
							$(CODEC_DIR)/mod/modplayer.o \
							$(CODEC_DIR)/mp3/mp3player.o \
							$(CODEC_DIR)/ymplay/player.o \
							$(CODEC_DIR)/sid/SIDplayer.o \
							$(CODEC_DIR)/sap/SAPplayer.o \
							$(CODEC_DIR)/nsf/NSFplayer.o \
							$(CODEC_DIR)/sc68/sc68player.o \
							$(CODEC_DIR)/ogg/oggplayer.o

TINY_XML_OBJS :=			$(TINY_XML_DIR)/tinystr.o \
							$(TINY_XML_DIR)/tinyxml.o \
							$(TINY_XML_DIR)/tinyxmlerror.o \
							$(TINY_XML_DIR)/tinyxmlparser.o

GAMESWF_OBJS :=				$(GAMESWF_DIR)/gameswf_action.o \
							$(GAMESWF_DIR)/gameswf_button.o \
							$(GAMESWF_DIR)/gameswf_dlist.o \
							$(GAMESWF_DIR)/gameswf_font.o \
							$(GAMESWF_DIR)/gameswf_fontlib.o \
							$(GAMESWF_DIR)/gameswf_impl.o \
							$(GAMESWF_DIR)/gameswf_log.o \
							$(GAMESWF_DIR)/gameswf_morph.o \
							$(GAMESWF_DIR)/gameswf_render.o \
							$(GAMESWF_DIR)/gameswf_shape.o \
							$(GAMESWF_DIR)/gameswf_sound.o \
							$(GAMESWF_DIR)/gameswf_stream.o \
							$(GAMESWF_DIR)/gameswf_styles.o \
							$(GAMESWF_DIR)/gameswf_tesselate.o \
							$(GAMESWF_DIR)/gameswf_text.o \
							$(GAMESWF_DIR)/gameswf_types.o \
							$(GAMESWF_DIR)/container.o \
							$(GAMESWF_DIR)/image.o \
							$(GAMESWF_DIR)/jpeg.o \
							$(GAMESWF_DIR)/utf8.o \
							$(GAMESWF_DIR)/tu_file.o \
							$(GAMESWF_DIR)/tu_random.o \
							$(GAMESWF_DIR)/zlib_adapter.o \
							$(GAMESWF_DIR)/gameswf_sound_handler_psp.o \
							$(GAMESWF_DIR)/gameswf_render_handler_psp.o

FTP_OBJS :=					$(FTP_DIR)/nlh.o

FFPLAY_OBJECT :=			$(FFMPEG_CODEC_DIR)/ffplay.o \
							$(FFMPEG_CODEC_DIR)/jonny/mem64.o
#							$(FFMPEG_CODEC_DIR)/jonny/csc.o \
#							$(FFMPEG_CODEC_DIR)/jonny/mestub.o \
#							$(FFMPEG_CODEC_DIR)/jonny/me.o \
#							$(FFMPEG_CODEC_DIR)/jonny/me_csc.o \

#----------------------------------------------------------------------------
#	All objects to make
#	-------------------

OBJS :=						$(FA_OBJS) \
							$(ASM_OBJS) \
							$(CODEC_OBJS) \
							$(TINY_XML_OBJS) \
							$(GAMESWF_OBJS) \
							$(FTP_OBJS) \
							$(FFPLAY_OBJECT)

#----------------------------------------------------------------------------
#	Additional includes
#	-------------------

INCDIR   :=					$(INCDIR) \
							$(INCLUDE_DIR) \
							$(PSP_MEDIA_CENTER_DIR) \
							$(CODEC_INCLUDE_DIR) \
							$(SC68_INCLUDE_DIR) \
							$(TINY_XML_INCLUDE_DIR) \
							$(GAMESWF_DIR) \
							$(FTP_DIR) \
							$(NOFRENDO_INCLUDE_DIR) \
							$(FFMPEG_DIR) \
							$(FFMPEG_LIB_DIR) \
							$(FFMPEG_CODEC_DIR)

#----------------------------------------------------------------------------
#	Addditional libraries
#	---------------------

SDK_LIBS :=					-lpsprtc \
							-lpspsdk \
							-lpsputility \
							-lpspaudiolibplus \
							-lpspaudio \
							-lpsphprm \
							-lpspctrl \
							-lpspusb \
							-lpspusbstor \
							-lpsppower \
							-lpspumd \
							-lpspgu \
							-lpspwlan

EXTERN_LIBS :=				-ljpeg \
							-lpng \
							-lmad \
							-lSAP \
							-lNESNofrendo \
							-lstsound \
							-lSIDPlayer \
							-lapi68 \
							-lfile68 \
							-lunice68 \
							-lemu68 \
							-lio68 \
							-lvorbisidec \
							-lavformat \
							-lavutil \
							-lavcodec \
							-lz

LIBS :=						$(SDK_LIBS) \
							$(EXTERN_LIBS) \
							-lm \
							-lstdc++

#----------------------------------------------------------------------------
#	Preprocesser defines
#	--------------------

DEFINES :=					-DTU_CONFIG_LINK_TO_JPEGLIB=1 \
							-DTU_CONFIG_LINK_TO_ZLIB=1 \
							-D_REENTRANT \
							-DNSF_PLAYER=1 \
							-D_DEBUG \
							-DPSP

#----------------------------------------------------------------------------
#	Compiler settings
#	-----------------

CFLAGS :=					$(DEFINES) -O2 -G8 -g -Wall -DHAVE_AV_CONFIG_H
CXXFLAGS :=					$(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS :=					$(CFLAGS)

LIBDIR :=					$(NOFRENDO_DIR)/lib \
							$(FFMPEG_DIR)

LDFLAGS :=					

#----------------------------------------------------------------------------
#	PBP parameters
#	--------------

EXTRA_TARGETS :=			EBOOT.PBP
PSP_EBOOT_ICON :=			../ICON0.PNG
PSP_EBOOT_PIC1 :=			../PIC1.PNG
PSP_EBOOT_TITLE :=			$(TARGET)

#----------------------------------------------------------------------------
#	Default build settings
#	----------------------

PSPSDK :=					$(shell psp-config --pspsdk-path)

include						$(PSPSDK)/lib/build.mak

#----------------------------------------------------------------------------
#	Copy to PSP
#	-----------

ifneq ($VS_PATH),)
CC       = psp-gcc
CXX      = psp-g++
endif

kx-install: kxploit
ifeq ($(PSP_MOUNT),)
		@echo '*** Error: $$(PSP_MOUNT) undefined. Please set it to for example /cygdrive/e'
		@echo if your PSP is mounted to E: in cygwin.
else
		cp -r $(TARGET) $(PSP_MOUNT)/PSP/GAME/
		cp -r $(TARGET)% $(PSP_MOUNT)/PSP/GAME/
		cp $(TARGET).elf $(PSP_MOUNT)/PSP/GAME/$(TARGET)
		cp -r -u "../Data" $(PSP_MOUNT)/PSP/GAME/$(TARGET)
endif
