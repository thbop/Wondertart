CFLAGS = -c

all: fk mkfs.fk copy.fk

fk: fatkatt.c
	$(CC) fatkatt.c $(CFLAGS) -o $(BUILD_DIR)/fatkatt.o

mkfs.fk: mkfs.c
	$(CC) mkfs.c $(CFLAGS) -o $(BUILD_DIR)/mkfs.fk.o
	$(CC) $(BUILD_DIR)/mkfs.fk.o $(BUILD_DIR)/fatkatt.o \
		-o $(BUILD_DIR)/mkfs.fk.exe

copy.fk: copy.c
	$(CC) copy.c $(CFLAGS) -o $(BUILD_DIR)/copy.fk.o
	$(CC) $(BUILD_DIR)/copy.fk.o $(BUILD_DIR)/fatkatt.o \
		-o $(BUILD_DIR)/copy.fk.exe