SRC_EXT    = .cpp
DEP_EXT    = .d
OBJ_EXT    = .o
SHARED_EXT = .so

INC_DIR    = include
SRC_DIR    = src
OBJ_DIR    = obj
BUILD_DIR  = build

CXX        = g++
CXXFLAGS   = -fPIC -I$(INC_DIR) -O3
LDFLAGS    = -shared

TARGET     = libkam$(SHARED_EXT)
SOURCES    = $(shell find $(SRC_DIR) -type f -name *$(SRC_EXT))

OBJS       = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SOURCES:$(SRC_EXT)=$(OBJ_EXT)))

all: prepare $(TARGET)

-include $(OBJS:$(OBJ_EXT)=$(DEP_EXT))

# make directories
prepare:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BUILD_DIR)


# link
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET) $^


# compile
$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@$(CXX) $(CXXFLAGS) -MM $(SRC_DIR)/$*$(SRC_EXT) > $(OBJ_DIR)/$*$(DEP_EXT)
	@cp -f $(OBJ_DIR)/$*$(DEP_EXT) $(OBJ_DIR)/$*$(DEP_EXT).tmp
	@sed -e 's|.*:|$(OBJ_DIR)/$*$(OBJ_EXT):|' < $(OBJ_DIR)/$*$(DEP_EXT).tmp > $(OBJ_DIR)/$*$(DEP_EXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJ_DIR)/$*$(DEP_EXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJ_DIR)/$*$(DEP_EXT)
	@rm -f $(OBJ_DIR)/$*$(DEP_EXT).tmp


# delete binaries, objects and their directories
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)

.PHONY: all prepare clean

