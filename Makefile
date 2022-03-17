###
# VARIAVEIS
###

PROJECT_NAME = TESTE
PROJECT_VERS = 0.0.1
PROJECT_MODE = DEBUG

INCLUD_PATH = inc/
SOURCE_PATH = src/
BUILDS_PATH = build/

SOURCES := $(wildcard $(SOURCE_PATH)*.c)
BUILDER := $(pathsubst $(SOURCE_PATH)/%.c, $(BUILDS_PATH)/%.o, $(SOURCES))

CFLAG += -I $(INCLUD_PATH)
CFLAG += -o $(BUILDS_PATH)$(PROJECT_NAME)

###
# FUNCTIONS
###
all: header
	@ cc $(SOURCES) $(CFLAG)
	@echo " [ * ] Compilado ~> $(BUILDS_PATH)$(PROJECT_NAME)"

header:
	@echo "╔═══════════════════════════════════════════╗"
	@echo "║            C O M P I L A N D O            ║"
	@echo "╚═══════════════════════════════════════════╝"
	@echo ""
	@echo " » Projeto: 	$(PROJECT_NAME)"
	@echo " » Versão: 	$(PROJECT_VERS)"
	@echo " » Modo: 	$(PROJECT_MODE)"
	@echo ""
	@echo " ═══════════════════════════════════════════ "
	@echo ""

.PHONY: clean
clean:
	@rm -rf build/*