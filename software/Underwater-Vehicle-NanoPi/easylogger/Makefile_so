CROSS_COMPILE = arm-linux-gnueabihf-
AS      = $(CROSS_COMPILE)as  
LD      = $(CROSS_COMPILE)ld  
CC      = $(CROSS_COMPILE)gcc  
CPP     = $(CROSS_COMPILE)g++  
AR      = $(CROSS_COMPILE)ar  
NM      = $(CROSS_COMPILE)nm
 
SOURCE_ROOT = .
#SOURCE_ROOT = $(shell pwd`)
 
CFLAGS		:= -Wall -lpthread -O2 -fno-builtin -Xlinker –unresolved-symbols=ignore-in-shared-libs
CPPFLAGS	:= 
 
INCLUDE_DIR := -I$(SOURCE_ROOT)/ -I$(SOURCE_ROOT)/inc -I$(SOURCE_ROOT)/plugins
 
APP_NAME = libeasylogger.so

all: $(APP_NAME)
APP_OBJECTC += src/elog.c
APP_OBJECTC += src/elog_async.c
APP_OBJECTC += src/elog_buf.c
APP_OBJECTC += src/elog_utils.c
APP_OBJECTC += port/elog_port.c
APP_OBJECTC += plugins/file/elog_file.c
APP_OBJECTC += plugins/file/elog_file_port.c

STATIC_OBJ_O  = $(patsubst %.c, %.o, $(APP_OBJECTC))
STATIC_OBJ_C  = $(foreach file, $(STATIC_OBJ_O), $(file))

$(STATIC_OBJ_C) : %.o:%.c
	$(CC)  $(INCLUDE_DIR) $(CFLAGS) -c -fPIC $(APP_OBJECTC)

$(APP_NAME): $(STATIC_OBJ_C)	
	$(CC) -shared -o $(APP_NAME) ./*.o
	
clean:
	@rm -f *.o *.so
.PHONY: clean	
