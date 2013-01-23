#############################################################################
# Makefile for building: UIEditor.app/Contents/MacOS/UIEditor
# Generated by qmake (2.01a) (Qt 4.8.1) on: Mon Jul 16 13:59:03 2012
# Project:  UIEditor.pro
# Template: app
# Command: ~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake -spec ~/QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/macx-xcode -o UIEditor.xcodeproj/project.pbxproj UIEditor.pro
#############################################################################

MOC       = ~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc
UIC       = ~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic
RCC =~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/rcc

LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DDAVA_DEBUG -DDDARWIN_NO_CARBON -DFT2_BUILD_LIBRARY -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I~/QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/macx-xcode -I. -I~/QtSDK/Desktop/Qt/4.8.1/gcc/lib/QtCore.framework/Versions/4/Headers -I~/QtSDK/Desktop/Qt/4.8.1/gcc/include/QtCore -I~/QtSDK/Desktop/Qt/4.8.1/gcc/lib/QtNetwork.framework/Versions/4/Headers -I~/QtSDK/Desktop/Qt/4.8.1/gcc/include/QtNetwork -I~/QtSDK/Desktop/Qt/4.8.1/gcc/lib/QtGui.framework/Versions/4/Headers -I~/QtSDK/Desktop/Qt/4.8.1/gcc/include/QtGui -I~/QtSDK/Desktop/Qt/4.8.1/gcc/lib/QtOpenGL.framework/Versions/4/Headers -I~/QtSDK/Desktop/Qt/4.8.1/gcc/include/QtOpenGL -I~/QtSDK/Desktop/Qt/4.8.1/gcc/include -I../dava.framework/Sources/Internal -I../dava.framework/Sources/External -I../dava.framework/Sources/External/Freetype -I../dava.framework/Libs/oggvorbis/include -I../dava.framework/Sources/Libs/include -IClasses -I../dava.framework/Libs/libs -I../dava.framework/Libs/freetype/include -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/AGL.framework/Headers -I. -I. -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F~/QtSDK/Desktop/Qt/4.8.1/gcc/lib
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

COMP_DIR = GeneratedFiles/
UI_DIR = UI/
SOURCE_DIR = Classes/
QRC_DIR = Data/

compilers: \
	$(COMP_DIR)moc_mainwindow.cpp \
	$(COMP_DIR)ui_mainwindow.h \
	$(COMP_DIR)moc_davaglwidget.cpp \
	$(COMP_DIR)ui_davaglwidget.h \
	$(COMP_DIR)moc_ScreenWrapper.cpp \
	$(COMP_DIR)moc_EditorFontManager.cpp \
	$(COMP_DIR)moc_BaseMetadata.cpp \
	$(COMP_DIR)moc_UIControlMetadata.cpp \
	$(COMP_DIR)moc_UIButtonMetadata.cpp \
	$(COMP_DIR)moc_UISliderMetadata.cpp \
	$(COMP_DIR)moc_UITextControlMetadata.cpp \
	$(COMP_DIR)moc_UITextFieldMetadata.cpp \
	$(COMP_DIR)moc_UIStaticTextMetadata.cpp \
	$(COMP_DIR)moc_PlatformMetadata.cpp \
	$(COMP_DIR)moc_ScreenMetadata.cpp \
	$(COMP_DIR)moc_HierarchyTreeController.cpp \
	$(COMP_DIR)moc_PropertiesGridController.cpp \
	$(COMP_DIR)moc_CommandsController.cpp \
	$(COMP_DIR)moc_hierarchytreewidget.cpp \
	$(COMP_DIR)ui_hierarchytreewidget.h \
	$(COMP_DIR)moc_propertygridcontainerwidget.cpp \
	$(COMP_DIR)ui_propertygridcontainerwidget.h \
	$(COMP_DIR)moc_basepropertygridwidget.cpp \
	$(COMP_DIR)ui_basepropertygridwidget.h \
	$(COMP_DIR)moc_rectpropertygridwidget.cpp \
	$(COMP_DIR)ui_rectpropertygridwidget.h \
	$(COMP_DIR)moc_controlpropertygridwidget.cpp \
	$(COMP_DIR)ui_controlpropertygridwidget.h \
	$(COMP_DIR)moc_flagspropertygridwidget.cpp \
	$(COMP_DIR)ui_flagspropertygridwidget.h \
	$(COMP_DIR)moc_platformpropertygridwidget.cpp \
	$(COMP_DIR)ui_platformpropertygridwidget.h \
	$(COMP_DIR)moc_screenpropertygridwidget.cpp \
	$(COMP_DIR)ui_screenpropertygridwidget.h \
	$(COMP_DIR)moc_librarywidget.cpp \
	$(COMP_DIR)ui_librarywidget.h \
	$(COMP_DIR)moc_textpropertygridwidget.cpp \
	$(COMP_DIR)ui_textpropertygridwidget.h \
	$(COMP_DIR)moc_statepropertygridwidget.cpp \
	$(COMP_DIR)ui_statepropertygridwidget.h \
	$(COMP_DIR)moc_sliderpropertygridwidget.cpp \
	$(COMP_DIR)ui_sliderpropertygridwidget.h \
	$(COMP_DIR)moc_backgroundpropertygridwidget.cpp \
	$(COMP_DIR)ui_backgroundpropertygridwidget.h \
	$(COMP_DIR)moc_uitextfieldpropertygridwidget.cpp \
	$(COMP_DIR)ui_uitextfieldpropertygridwidget.h \
	$(COMP_DIR)moc_controllist.cpp \
	$(COMP_DIR)moc_fontmanagerdialog.cpp \
	$(COMP_DIR)ui_fontmanagerdialog.h \
	$(COMP_DIR)moc_screenmanager.cpp \
	$(COMP_DIR)moc_createplatformdlg.cpp \
	$(COMP_DIR)ui_createplatformdlg.h \
	$(COMP_DIR)moc_createscreendlg.cpp \
	$(COMP_DIR)ui_createscreendlg.h \
	$(COMP_DIR)moc_hierarchytreecontrol.cpp \
	$(COMP_DIR)moc_qcolorbutton.cpp \
	$(COMP_DIR)moc_statecomboboxitemdelegate.cpp \
	$(COMP_DIR)moc_localizationeditordialog.cpp \
	$(COMP_DIR)ui_localizationeditordialog.h \
	$(COMP_DIR)qrc_icons.cpp
	
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: \
	$(COMP_DIR)moc_mainwindow.cpp \
	$(COMP_DIR)moc_davaglwidget.cpp \
	$(COMP_DIR)moc_ScreenWrapper.cpp \
	$(COMP_DIR)moc_EditorFontManager.cpp \
	$(COMP_DIR)moc_BaseMetadata.cpp \
	$(COMP_DIR)moc_UIControlMetadata.cpp \
	$(COMP_DIR)moc_UIButtonMetadata.cpp \
	$(COMP_DIR)moc_UITextControlMetadata.cpp \
	$(COMP_DIR)moc_UITextFieldMetadata.cpp \
	$(COMP_DIR)moc_UIStaticTextMetadata.cpp \
	$(COMP_DIR)moc_UISliderMetadata.cpp \
	$(COMP_DIR)moc_PlatformMetadata.cpp \
	$(COMP_DIR)moc_ScreenMetadata.cpp \
	$(COMP_DIR)moc_HierarchyTreeController.cpp \
	$(COMP_DIR)moc_PropertiesGridController.cpp \
	$(COMP_DIR)moc_CommandsController.cpp \
	$(COMP_DIR)moc_hierarchytreewidget.cpp \
	$(COMP_DIR)moc_librarywidget.cpp \
	$(COMP_DIR)moc_controllist.cpp \
	$(COMP_DIR)moc_propertygridcontainerwidget.cpp \
	$(COMP_DIR)moc_basepropertygridwidget.cpp \
	$(COMP_DIR)moc_rectpropertygridwidget.cpp \
	$(COMP_DIR)moc_controlpropertygridwidget.cpp \
	$(COMP_DIR)moc_flagspropertygridwidget.cpp \
	$(COMP_DIR)moc_librarywidget.cpp \
	$(COMP_DIR)moc_textpropertygridwidget.cpp \
	$(COMP_DIR)moc_statepropertygridwidget.cpp \
	$(COMP_DIR)moc_sliderpropertygridwidget.cpp \
	$(COMP_DIR)moc_platformpropertygridwidget.cpp \
	$(COMP_DIR)moc_screenpropertygridwidget.cpp \
	$(COMP_DIR)moc_textpropertygridwidget.cpp \
	$(COMP_DIR)moc_backgroundpropertygridwidget.cpp \
	$(COMP_DIR)moc_uitextfieldpropertygridwidget.cpp \
	$(COMP_DIR)moc_fontmanagerdialog.cpp \
	$(COMP_DIR)moc_screenmanager.cpp \
	$(COMP_DIR)moc_createplatformdlg.cpp \
	$(COMP_DIR)moc_hierarchytreecontrol.cpp \
	$(COMP_DIR)moc_qcolorbutton.cpp \
	$(COMP_DIR)moc_statecomboboxitemdelegate.cpp \
	$(COMP_DIR)moc_createscreendlg.cpp \
	$(COMP_DIR)moc_localizationeditordialog.cpp \
	$(COMP_DIR)qrc_icons.cpp

compiler_moc_header_clean:
	-$(DEL_FILE) \
		$(COMP_DIR)moc_mainwindow.cpp \
		$(COMP_DIR)moc_davaglwidget.cpp \
		$(COMP_DIR)moc_ScreenWrapper.cpp \
		$(COMP_DIR)moc_EditorFontManager.cpp \
		$(COMP_DIR)moc_BaseMetadata.cpp \
		$(COMP_DIR)moc_UIControlMetadata.cpp \
		$(COMP_DIR)moc_UIButtonMetadata.cpp \
		$(COMP_DIR)moc_UITextControlMetadata.cpp \
		$(COMP_DIR)moc_UITextFieldMetadata.cpp \
		$(COMP_DIR)moc_UIStaticTextMetadata.cpp \
		$(COMP_DIR)moc_UISliderMetadata.cpp \
		$(COMP_DIR)moc_PlatformMetadata.cpp \
		$(COMP_DIR)moc_ScreenMeadata.cpp \
		$(COMP_DIR)moc_HierarchyTreeController.cpp \
		$(COMP_DIR)moc_PropertiesGridController.cpp \
		$(COMP_DIR)moc_CommandsController.cpp \
		$(COMP_DIR)moc_hierarchytreewidget.cpp \
		$(COMP_DIR)moc_librarywidget.cpp \
		$(COMP_DIR)moc_controllist.cpp \
		$(COMP_DIR)moc_basepropertygridwidget.cpp \
		$(COMP_DIR)moc_rectpropertygridwidget.cpp \
		$(COMP_DIR)moc_controlpropertygridwidget.cpp \
		$(COMP_DIR)moc_flagspropertygridwidget.cpp \
		$(COMP_DIR)moc_librarywidget.cpp \
		$(COMP_DIR)moc_textpropertygridwidget.cpp \
		$(COMP_DIR)moc_statepropertygridwidget.cpp \
		$(COMP_DIR)moc_sliderpropertygridwidget.cpp \
		$(COMP_DIR)moc_platformpropertygridwidget.cpp \
		$(COMP_DIR)moc_screenpropertygridwidget.cpp \
		$(COMP_DIR)moc_textpropertygridwidget.cpp \
		$(COMP_DIR)moc_backgroundpropertygridwidget.cpp \
		$(COMP_DIR)moc_uitextfieldpropertygridwidget.cpp \
		$(COMP_DIR)moc_fontmanagerdialog.cpp \
		$(COMP_DIR)moc_screenmanager.cpp \
		$(COMP_DIR)moc_createplatformdlg.cpp \
		$(COMP_DIR)moc_createscreendlg.cpp \
		$(COMP_DIR)moc_hierarchytreecontrol.cpp \
		$(COMP_DIR)moc_qcolorbutton.cpp \
		$(COMP_DIR)moc_statecomboboxitemdelegate.cpp \
		$(COMP_DIR)moc_localizationeditordialog.cpp \
		$(COMP_DIR)qrc_icons.cpp

$(COMP_DIR)moc_mainwindow.cpp: $(SOURCE_DIR)UI/mainwindow.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/mainwindow.h -o $(COMP_DIR)moc_mainwindow.cpp

$(COMP_DIR)moc_davaglwidget.cpp: $(SOURCE_DIR)UI/davaglwidget.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/davaglwidget.h -o $(COMP_DIR)moc_davaglwidget.cpp

$(COMP_DIR)moc_ScreenWrapper.cpp: $(SOURCE_DIR)ScreenWrapper.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)ScreenWrapper.h -o $(COMP_DIR)moc_ScreenWrapper.cpp

$(COMP_DIR)moc_EditorFontManager.cpp: $(SOURCE_DIR)EditorFontManager.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)EditorFontManager.h -o $(COMP_DIR)moc_EditorFontManager.cpp

$(COMP_DIR)moc_BaseMetadata.cpp: $(SOURCE_DIR)Metadata/BaseMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/BaseMetadata.h -o $(COMP_DIR)moc_BaseMetadata.cpp

$(COMP_DIR)moc_UIControlMetadata.cpp: $(SOURCE_DIR)Metadata/UIControlMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/UIControlMetadata.h -o $(COMP_DIR)moc_UIControlMetadata.cpp

$(COMP_DIR)moc_UITextControlMetadata.cpp: $(SOURCE_DIR)Metadata/UITextControlMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/UITextControlMetadata.h -o $(COMP_DIR)moc_UITextControlMetadata.cpp

$(COMP_DIR)moc_UITextFieldMetadata.cpp: $(SOURCE_DIR)Metadata/UITextFieldMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/UITextFieldMetadata.h -o $(COMP_DIR)moc_UITextFieldMetadata.cpp

$(COMP_DIR)moc_UIButtonMetadata.cpp: $(SOURCE_DIR)Metadata/UIButtonMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/UIButtonMetadata.h -o $(COMP_DIR)moc_UIButtonMetadata.cpp

$(COMP_DIR)moc_UIStaticTextMetadata.cpp: $(SOURCE_DIR)Metadata/UIStaticTextMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/UIStaticTextMetadata.h -o $(COMP_DIR)moc_UIStaticTextMetadata.cpp

$(COMP_DIR)moc_UISliderMetadata.cpp: $(SOURCE_DIR)Metadata/UISliderMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/UISliderMetadata.h -o $(COMP_DIR)moc_UISliderMetadata.cpp

$(COMP_DIR)moc_PlatformMetadata.cpp: $(SOURCE_DIR)Metadata/PlatformMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/PlatformMetadata.h -o $(COMP_DIR)moc_PlatformMetadata.cpp

$(COMP_DIR)moc_ScreenMetadata.cpp: $(SOURCE_DIR)Metadata/ScreenMetadata.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Metadata/ScreenMetadata.h -o $(COMP_DIR)moc_ScreenMetadata.cpp

$(COMP_DIR)moc_HierarchyTreeController.cpp: $(SOURCE_DIR)Controllers/HierarchyTreeController.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Controllers/HierarchyTreeController.h -o $(COMP_DIR)moc_HierarchyTreeController.cpp

$(COMP_DIR)moc_PropertiesGridController.cpp: $(SOURCE_DIR)Controllers/PropertiesGridController.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Controllers/PropertiesGridController.h -o $(COMP_DIR)moc_PropertiesGridController.cpp

$(COMP_DIR)moc_CommandsController.cpp: $(SOURCE_DIR)Controllers/CommandsController.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)Controllers/CommandsController.h -o $(COMP_DIR)moc_CommandsController.cpp

$(COMP_DIR)moc_hierarchytreewidget.cpp: $(SOURCE_DIR)UI/hierarchytreewidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/hierarchytreewidget.h -o $(COMP_DIR)moc_hierarchytreewidget.cpp

$(COMP_DIR)moc_basepropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/basepropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/basepropertygridwidget.h  -o $(COMP_DIR)moc_basepropertygridwidget.cpp

$(COMP_DIR)moc_rectpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/rectpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/rectpropertygridwidget.h  -o $(COMP_DIR)moc_rectpropertygridwidget.cpp

$(COMP_DIR)moc_controlpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/controlpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/controlpropertygridwidget.h  -o $(COMP_DIR)moc_controlpropertygridwidget.cpp

$(COMP_DIR)moc_flagspropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/flagspropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/flagspropertygridwidget.h  -o $(COMP_DIR)moc_flagspropertygridwidget.cpp

$(COMP_DIR)moc_platformpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/platformpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/platformpropertygridwidget.h  -o $(COMP_DIR)moc_platformpropertygridwidget.cpp

$(COMP_DIR)moc_screenpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/screenpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/screenpropertygridwidget.h  -o $(COMP_DIR)moc_screenpropertygridwidget.cpp

$(COMP_DIR)moc_librarywidget.cpp: $(SOURCE_DIR)UI/librarywidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/librarywidget.h -o $(COMP_DIR)moc_librarywidget.cpp

$(COMP_DIR)moc_controllist.cpp: $(SOURCE_DIR)UI/controllist.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/controllist.h -o $(COMP_DIR)moc_controllist.cpp

$(COMP_DIR)moc_propertygridcontainerwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/propertygridcontainerwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/propertygridcontainerwidget.h  -o $(COMP_DIR)moc_propertygridcontainerwidget.cpp

$(COMP_DIR)moc_textpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/textpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/textpropertygridwidget.h -o $(COMP_DIR)moc_textpropertygridwidget.cpp

$(COMP_DIR)moc_statepropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/statepropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/statepropertygridwidget.h -o $(COMP_DIR)moc_statepropertygridwidget.cpp

$(COMP_DIR)moc_sliderpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/sliderpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/sliderpropertygridwidget.h -o $(COMP_DIR)moc_sliderpropertygridwidget.cpp

$(COMP_DIR)moc_backgroundpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/backgroundpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/backgroundpropertygridwidget.h -o $(COMP_DIR)moc_backgroundpropertygridwidget.cpp

$(COMP_DIR)moc_uitextfieldpropertygridwidget.cpp: $(SOURCE_DIR)UI/PropertyGridWidgets/uitextfieldpropertygridwidget.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/PropertyGridWidgets/uitextfieldpropertygridwidget.h -o $(COMP_DIR)moc_uitextfieldpropertygridwidget.cpp

$(COMP_DIR)moc_fontmanagerdialog.cpp: $(SOURCE_DIR)UI/fontmanagerdialog.h
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/fontmanagerdialog.h -o $(COMP_DIR)moc_fontmanagerdialog.cpp

$(COMP_DIR)moc_screenmanager.cpp: $(SOURCE_DIR)ScreenManager.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)ScreenManager.h -o $(COMP_DIR)moc_screenmanager.cpp

$(COMP_DIR)moc_createplatformdlg.cpp: $(SOURCE_DIR)UI/createplatformdlg.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/createplatformdlg.h -o $(COMP_DIR)moc_createplatformdlg.cpp

$(COMP_DIR)moc_createscreendlg.cpp: $(SOURCE_DIR)UI/createscreendlg.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/createscreendlg.h -o $(COMP_DIR)moc_createscreendlg.cpp

$(COMP_DIR)moc_hierarchytreecontrol.cpp: $(SOURCE_DIR)UI/hierarchytreecontrol.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/hierarchytreecontrol.h -o $(COMP_DIR)moc_hierarchytreecontrol.cpp

$(COMP_DIR)moc_qcolorbutton.cpp: $(SOURCE_DIR)UI/qcolorbutton.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/qcolorbutton.h -o $(COMP_DIR)moc_qcolorbutton.cpp	

$(COMP_DIR)moc_statecomboboxitemdelegate.cpp: $(SOURCE_DIR)UI/statecomboboxitemdelegate.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/statecomboboxitemdelegate.h -o $(COMP_DIR)moc_statecomboboxitemdelegate.cpp	
	
$(COMP_DIR)moc_localizationeditordialog.cpp: $(SOURCE_DIR)UI/Dialogs/localizationeditordialog.h 
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $(SOURCE_DIR)UI/Dialogs/localizationeditordialog.h -o $(COMP_DIR)moc_localizationeditordialog.cpp

$(COMP_DIR)qrc_icons.cpp : $(QRC_DIR)icons.qrc
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/rcc -name icons $(QRC_DIR)icons.qrc -o $(COMP_DIR)qrc_icons.cpp

compiler_rcc_make_all:  $(COMP_DIR)qrc_icons.cpp
compiler_rcc_clean:
	-$(DEL_FILE) $(COMP_DIR)qrc_icons.cpp
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all: \
	$(COMP_DIR)ui_mainwindow.h \
	$(COMP_DIR)ui_davaglwidget.h \
	$(COMP_DIR)ui_hierarchytreewidget.h \
	$(COMP_DIR)ui_basepropertygridwidget.h \
	$(COMP_DIR)ui_rectpropertygridwidget.h \
	$(COMP_DIR)ui_controlpropertygridwidget.h \
	$(COMP_DIR)ui_flagspropertygridwidget.h \
	$(COMP_DIR)ui_platformpropertygridwidget.h \
	$(COMP_DIR)ui_screenpropertygridwidget.h \
	$(COMP_DIR)ui_librarywidget.h \
	$(COMP_DIR)ui_propertygridcontainerwidget.h \
	$(COMP_DIR)ui_textpropertygridwidget.h \
	$(COMP_DIR)ui_sliderpropertygridwidget.h \
	$(COMP_DIR)ui_statepropertygridwidget.h \
	$(COMP_DIR)ui_backgroundpropertygridwidget.h \
	$(COMP_DIR)ui_uitextfieldpropertygridwidget.h \
	$(COMP_DIR)ui_fontmanagerdialog.h \
	$(COMP_DIR)ui_createplatformdlg.h \
	$(COMP_DIR)ui_createscreendlg.h \
	$(COMP_DIR)ui_localizationeditordialog.h \

compiler_uic_clean:
	-$(DEL_FILE) \
		$(COMP_DIR)ui_mainwindow.h \
		$(COMP_DIR)ui_davaglwidget.h \
		$(COMP_DIR)ui_hierarchytreewidget.h \
		$(COMP_DIR)ui_basepropertygridwidget.h \
		$(COMP_DIR)ui_rectpropertygridwidget.h \
		$(COMP_DIR)ui_controlpropertygridwidget.h \
		$(COMP_DIR)ui_flagspropertygridwidget.h \
		$(COMP_DIR)ui_platformpropertygridwidget.h \
		$(COMP_DIR)ui_screenpropertygridwidget.h \
		$(COMP_DIR)ui_librarywidget.h \
		$(COMP_DIR)ui_propertygridcontainerwidget.h \
		$(COMP_DIR)ui_textpropertygridwidget.h \
		$(COMP_DIR)ui_sliderpropertygridwidget.h \
		$(COMP_DIR)ui_statepropertygridwidget.h \
		$(COMP_DIR)ui_backgroundpropertygridwidget.h \
		$(COMP_DIR)ui_uitextfieldpropertygridwidget.h \
		$(COMP_DIR)ui_fontmanagerdialog.h \
		$(COMP_DIR)ui_createplatformdlg.h \
		$(COMP_DIR)ui_createscreendlg.h \
		$(COMP_DIR)ui_localizationeditordialog.h

$(COMP_DIR)ui_mainwindow.h: $(UI_DIR)mainwindow.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)mainwindow.ui -o $(COMP_DIR)ui_mainwindow.h

$(COMP_DIR)ui_davaglwidget.h: $(UI_DIR)davaglwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)davaglwidget.ui -o $(COMP_DIR)ui_davaglwidget.h

$(COMP_DIR)ui_hierarchytreewidget.h: $(UI_DIR)hierarchytreewidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)hierarchytreewidget.ui -o $(COMP_DIR)ui_hierarchytreewidget.h

$(COMP_DIR)ui_basepropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/basepropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/basepropertygridwidget.ui -o $(COMP_DIR)ui_basepropertygridwidget.h

$(COMP_DIR)ui_rectpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/rectpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/rectpropertygridwidget.ui -o $(COMP_DIR)ui_rectpropertygridwidget.h

$(COMP_DIR)ui_controlpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/controlpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/controlpropertygridwidget.ui -o $(COMP_DIR)ui_controlpropertygridwidget.h

$(COMP_DIR)ui_flagspropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/flagspropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/flagspropertygridwidget.ui -o $(COMP_DIR)ui_flagspropertygridwidget.h

$(COMP_DIR)ui_platformpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/platformpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/platformpropertygridwidget.ui -o $(COMP_DIR)ui_platformpropertygridwidget.h

$(COMP_DIR)ui_screenpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/screenpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/screenpropertygridwidget.ui -o $(COMP_DIR)ui_screenpropertygridwidget.h

$(COMP_DIR)ui_librarywidget.h: $(UI_DIR)librarywidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)librarywidget.ui -o $(COMP_DIR)ui_librarywidget.h

$(COMP_DIR)ui_propertygridcontainerwidget.h: $(UI_DIR)/PropertyGridWidgets/propertygridcontainerwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/propertygridcontainerwidget.ui -o $(COMP_DIR)ui_propertygridcontainerwidget.h

$(COMP_DIR)ui_textpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/textpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/textpropertygridwidget.ui -o $(COMP_DIR)ui_textpropertygridwidget.h

$(COMP_DIR)ui_statepropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/statepropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/statepropertygridwidget.ui -o $(COMP_DIR)ui_statepropertygridwidget.h

$(COMP_DIR)ui_sliderpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/sliderpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/sliderpropertygridwidget.ui -o $(COMP_DIR)ui_sliderpropertygridwidget.h

$(COMP_DIR)ui_backgroundpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/backgroundpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/backgroundpropertygridwidget.ui -o $(COMP_DIR)ui_backgroundpropertygridwidget.h

$(COMP_DIR)ui_uitextfieldpropertygridwidget.h: $(UI_DIR)/PropertyGridWidgets/uitextfieldpropertygridwidget.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/PropertyGridWidgets/uitextfieldpropertygridwidget.ui -o $(COMP_DIR)ui_uitextfieldpropertygridwidget.h

$(COMP_DIR)ui_fontmanagerdialog.h: $(UI_DIR)/Dialogs/fontmanagerdialog.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/Dialogs/fontmanagerdialog.ui -o $(COMP_DIR)ui_fontmanagerdialog.h

$(COMP_DIR)ui_createplatformdlg.h: $(UI_DIR)/Dialogs/createplatformdlg.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/Dialogs/createplatformdlg.ui -o $(COMP_DIR)ui_createplatformdlg.h

$(COMP_DIR)ui_createscreendlg.h: $(UI_DIR)/Dialogs/createscreendlg.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/Dialogs/createscreendlg.ui -o $(COMP_DIR)ui_createscreendlg.h

$(COMP_DIR)ui_localizationeditordialog.h: $(UI_DIR)/Dialogs/localizationeditordialog.ui
	~/QtSDK/Desktop/Qt/4.8.1/gcc/bin/uic $(UI_DIR)/Dialogs/localizationeditordialog.ui -o $(COMP_DIR)ui_localizationeditordialog.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

