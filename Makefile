#Variables
TARGET := main
OUTDIR := debug

SRC_DIR := ./src

DEP := -MP -MD

#Linux =========================================================================
INCLUDE := -I./include
CXX := g++
CXXFLAGS := -std=c++23 -O0 $(DEP)
LINKER :=
#================================================================================


SRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst %.cpp,$(OUTDIR)/%.o,$(patsubst %.c,$(OUTDIR)/%.o,$(SRC)))
DEPFILES := $(patsubst %.cpp,$(OUTDIR)/%.d,$(patsubst %.c,$(OUTDIR)/%.d,$(SRC)))

# Default target
all: $(OUTDIR)/$(TARGET)

# Link
$(OUTDIR)/$(TARGET): $(OBJ) | $(OUTDIR)
	$(info Linking: $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LINKER)

# Compile source files to object files
$(OUTDIR)/%.o: %.cpp | $(OUTDIR)
	@mkdir -p $(@D)
	$(info Compiling: $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OUTDIR)/%.o: %.c | $(OUTDIR)
	@mkdir -p $(@D)
	$(info Compiling: $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Ensure the output directory exists
$(OUTDIR):
	@mkdir -p $(OUTDIR)

# Run program
run: all
	./$(OUTDIR)/$(TARGET)

# Clean target
clean:
	@rm -rf $(OUTDIR)


diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

-include $(DEPFILES)

.PHONY: all clean run
