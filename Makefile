# GNU Make workspace makefile autogenerated by Premake

.NOTPARALLEL:

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  glad_config = debug
  jsonxx_config = debug
  imgui_config = debug
  Wyrd_config = debug
  TestPlayer_config = debug
  Editor_config = debug
endif
ifeq ($(config),release)
  glad_config = release
  jsonxx_config = release
  imgui_config = release
  Wyrd_config = release
  TestPlayer_config = release
  Editor_config = release
endif
ifeq ($(config),distribution)
  glad_config = distribution
  jsonxx_config = distribution
  imgui_config = distribution
  Wyrd_config = distribution
  TestPlayer_config = distribution
  Editor_config = distribution
endif

PROJECTS := glad jsonxx imgui Wyrd TestPlayer Editor

.PHONY: all clean help $(PROJECTS) Applications Third Party

all: $(PROJECTS)

Applications: Editor TestPlayer

Third Party: glad imgui jsonxx

glad:
ifneq (,$(glad_config))
	@echo "==== Building glad ($(glad_config)) ===="
	@${MAKE} --no-print-directory -C dependencies/glad/premakedir -f Makefile config=$(glad_config)
endif

jsonxx:
ifneq (,$(jsonxx_config))
	@echo "==== Building jsonxx ($(jsonxx_config)) ===="
	@${MAKE} --no-print-directory -C dependencies/jsonxx/premakedir -f Makefile config=$(jsonxx_config)
endif

imgui:
ifneq (,$(imgui_config))
	@echo "==== Building imgui ($(imgui_config)) ===="
	@${MAKE} --no-print-directory -C dependencies/imgui/premakedir -f Makefile config=$(imgui_config)
endif

Wyrd: glad jsonxx
ifneq (,$(Wyrd_config))
	@echo "==== Building Wyrd ($(Wyrd_config)) ===="
	@${MAKE} --no-print-directory -C Wyrd -f Makefile config=$(Wyrd_config)
endif

TestPlayer: Wyrd glad
ifneq (,$(TestPlayer_config))
	@echo "==== Building TestPlayer ($(TestPlayer_config)) ===="
	@${MAKE} --no-print-directory -C Apps/TestPlayer -f Makefile config=$(TestPlayer_config)
endif

Editor: Wyrd glad imgui jsonxx
ifneq (,$(Editor_config))
	@echo "==== Building Editor ($(Editor_config)) ===="
	@${MAKE} --no-print-directory -C Apps/Editor -f Makefile config=$(Editor_config)
endif

clean:
	@${MAKE} --no-print-directory -C dependencies/glad/premakedir -f Makefile clean
	@${MAKE} --no-print-directory -C dependencies/jsonxx/premakedir -f Makefile clean
	@${MAKE} --no-print-directory -C dependencies/imgui/premakedir -f Makefile clean
	@${MAKE} --no-print-directory -C Wyrd -f Makefile clean
	@${MAKE} --no-print-directory -C Apps/TestPlayer -f Makefile clean
	@${MAKE} --no-print-directory -C Apps/Editor -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo "  distribution"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   glad"
	@echo "   jsonxx"
	@echo "   imgui"
	@echo "   Wyrd"
	@echo "   TestPlayer"
	@echo "   Editor"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"