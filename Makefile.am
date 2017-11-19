bin_PROGRAMS =  jape_distortion \
		jape_generator \
		jape_oscilloscope \
		#

AM_CPPFLAGS = -Isrc \
	      -Isrc/AudioCon \
	      -Isrc/GtkGui \
	      -Isrc/Pedals \
	      #

common_SOURCES = src/AudioCon/JackCon.cpp \
		 src/GtkGui/GtkBase.cpp \
		 src/GtkGui/GtkBaseSignals.cpp \
		 #

CXXFLAGS = $(CFLAGS) -std=c++11 -Wall -Wextra -Werror ${GTK_CFLAGS}
LDFLAGS = -rdynamic
LDADD = -l:libjack.so.0 ${GTK_LIBS}

# resources to generate
GENDIR = generated

# clean generated source files
CLEANFILES = $(GENDIR)/*

# jape_distortion
jape_distortion_RESOURCES_XML = resources/gresource_distortion.xml
jape_distortion_RESOURCES_CPP = generated/resources_distortion.cpp
jape_distortion_RESOURCES_PO  = $(GENDIR)/$(DEPDIR)/resources_distortion.Po

jape_distortion_SOURCES = $(common_SOURCES) \
			  src/Main/MainDistortion.cpp \
			  src/GtkGui/GtkDistortion.cpp \
			  src/GtkGui/GtkDistortionSignals.cpp \
			  src/Pedals/PedalDistortion.cpp \
			  $(jape_distortion_RESOURCES_CPP) \
			  #

# jape_distortion: generate source files
$(jape_distortion_RESOURCES_PO): $(jape_generator_RESOURCES_CPP)

$(jape_distortion_RESOURCES_CPP): $(jape_distortion_RESOURCES_XML)
	$(INSTALL) -d $(GENDIR)
	$(GLIB_COMPILE_RESOURCES) \
		--target=$(jape_distortion_RESOURCES_CPP) \
		--generate-source $(jape_distortion_RESOURCES_XML)

# jape_generator
jape_generator_RESOURCES_XML = resources/gresource_generator.xml
jape_generator_RESOURCES_CPP = generated/resources_generator.cpp
jape_generator_RESOURCES_PO  = $(GENDIR)/$(DEPDIR)/resources_generator.Po

jape_generator_SOURCES = $(common_SOURCES) \
			 src/Main/MainGenerator.cpp \
			 src/GtkGui/GtkGenerator.cpp \
			 src/Pedals/PedalGenerator.cpp \
			 $(jape_generator_RESOURCES_CPP) \
			 #

# jape_generator: generate source files
$(jape_generator_RESOURCES_PO): $(jape_generator_RESOURCES_CPP)

$(jape_generator_RESOURCES_CPP): $(jape_generator_RESOURCES_XML)
	$(INSTALL) -d $(GENDIR)
	$(GLIB_COMPILE_RESOURCES) \
		--target=$(jape_generator_RESOURCES_CPP) \
		--generate-source $(jape_generator_RESOURCES_XML)

# jape_oscilloscope
jape_oscilloscope_RESOURCES_XML = resources/gresource_oscilloscope.xml
jape_oscilloscope_RESOURCES_CPP = generated/resources_oscilloscope.cpp
jape_oscilloscope_RESOURCES_PO  = $(GENDIR)/$(DEPDIR)/resources_oscilloscope.Po

jape_oscilloscope_SOURCES = $(common_SOURCES) \
			    src/Main/MainOscilloscope.cpp \
			    src/GtkGui/GtkOscilloscope.cpp \
			    src/Pedals/PedalOscilloscope.cpp \
			    $(jape_oscilloscope_RESOURCES_CPP) \
			    #

# jape_oscilloscope: generate source files
$(jape_oscilloscope_RESOURCES_PO): $(jape_oscilloscope_RESOURCES_CPP)

$(jape_oscilloscope_RESOURCES_CPP): $(jape_oscilloscope_RESOURCES_XML)
	$(INSTALL) -d $(GENDIR)
	$(GLIB_COMPILE_RESOURCES) \
		--target=$(jape_oscilloscope_RESOURCES_CPP) \
		--generate-source $(jape_oscilloscope_RESOURCES_XML)
