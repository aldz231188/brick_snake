# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -g -fPIC -fprofile-arcs -ftest-coverage \
           -I. -I/usr/include/x86_64-linux-gnu/qt5 \
           -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
           -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
           -I/usr/include/x86_64-linux-gnu/qt5/QtCore \
           -I$(GUI_CLI_DIR)

# Библиотеки
APP_LDFLAGS = -lQt5Widgets -lQt5Gui -lQt5Core
TEST_LDFLAGS = -lgtest -lgtest_main -lpthread
CLI_LDFLAGS = -lncurses

# Папки
SRC_DIR = .
BUILD_DIR = ./build
LIB_DIR = ./lib
COMMON_DIR = ../common
TEST_DIR = ./tests
COVERAGE_DIR = ./coverage
GUI_QT_DIR = ../../gui/desktop
GUI_CLI_DIR = ../../gui/cli
QT_BUILD_DIR = $(BUILD_DIR)/qt
CLI_BUILD_DIR = $(BUILD_DIR)/cli

# Файлы
HEADERS = $(COMMON_DIR)/common.h \
           $(SRC_DIR)/controller.h \
           $(SRC_DIR)/fsm.h \
           $(SRC_DIR)/model.h

# Исходные файлы логики игры
LOGIC_SRC_CPP = $(SRC_DIR)/controller.cpp \
                $(SRC_DIR)/fsm.cpp \
                $(SRC_DIR)/model.cpp \
                $(SRC_DIR)/snake.cpp

LOGIC_SRC_C = $(COMMON_DIR)/common.c

# Объектные файлы логики игры
LOGIC_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/logic_%.o, $(LOGIC_SRC_CPP)) \
            $(patsubst $(COMMON_DIR)/%.c, $(BUILD_DIR)/logic_%.o, $(LOGIC_SRC_C))

# Исходные файлы интерфейса (Qt)
INTERFACE_SRC_QT = $(GUI_QT_DIR)/infowidget.cpp \
                   $(GUI_QT_DIR)/gamewidget.cpp \
                   $(GUI_QT_DIR)/main.cpp \
                   $(GUI_QT_DIR)/mainwindow.cpp

# Заголовочные файлы, требующие обработки moc (Qt)
MOC_HEADERS = $(GUI_QT_DIR)/infowidget.h \
              $(GUI_QT_DIR)/gamewidget.h \
              $(GUI_QT_DIR)/mainwindow.h

# Сгенерированные moc-файлы и их объектные файлы (Qt)
MOC_CPP = $(patsubst $(GUI_QT_DIR)/%.h, $(QT_BUILD_DIR)/moc_%.cpp, $(MOC_HEADERS))
MOC_OBJ = $(patsubst $(QT_BUILD_DIR)/%.cpp, $(QT_BUILD_DIR)/%.o, $(MOC_CPP))

# Объектные файлы интерфейса (Qt)
INTERFACE_OBJ_QT = $(patsubst $(GUI_QT_DIR)/%.cpp, $(QT_BUILD_DIR)/%.o, $(INTERFACE_SRC_QT)) $(MOC_OBJ)

# Исходные файлы интерфейса (CLI)
INTERFACE_SRC_CLI_C = $(wildcard $(GUI_CLI_DIR)/*.c)
INTERFACE_SRC_CLI_CPP = $(wildcard $(GUI_CLI_DIR)/*.cpp)

# Объектные файлы интерфейса (CLI)
INTERFACE_OBJ_CLI = $(patsubst $(GUI_CLI_DIR)/%.c, $(CLI_BUILD_DIR)/%.o, $(INTERFACE_SRC_CLI_C))
INTERFACE_OBJ_CLI += $(patsubst $(GUI_CLI_DIR)/%.cpp, $(CLI_BUILD_DIR)/%.o, $(INTERFACE_SRC_CLI_CPP))

# Файлы тестов
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/test_%.o,$(TEST_SRC))

# Целевые файлы
LIB_TARGET = $(LIB_DIR)/libgame_logic.a
EXECUTABLE_QT = $(BUILD_DIR)/snake_game_qt
EXECUTABLE_CLI = $(BUILD_DIR)/snake_game_cli
TEST_EXECUTABLE = $(BUILD_DIR)/run_tests

# Параметры сборки документации
DOC_DIR = ./doc
DOC_SRC = $(DOC_DIR)/documentation.tex

# Параметры дистрибутива
PACKAGE_NAME = snake_game
VERSION = 1.0
DIST_DIR = ./dist
DIST_FILES = Makefile $(SRC_DIR) $(LIB_DIR) $(COMMON_DIR) $(TEST_DIR) $(GUI_QT_DIR) $(GUI_CLI_DIR) $(DOC_DIR)

# Команды
.PHONY: all install uninstall clean dvi dist tests coverage memcheck

# Сборка всего проекта
all: $(EXECUTABLE_QT) $(EXECUTABLE_CLI)

# Сборка библиотеки
$(LIB_TARGET): $(LOGIC_OBJ) | $(LIB_DIR)
	ar rcs $@ $^

# Сборка интерфейса (Qt)
$(EXECUTABLE_QT): $(INTERFACE_OBJ_QT) $(LIB_TARGET) | $(QT_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(LIB_DIR) -lgame_logic $(APP_LDFLAGS)

# Сборка интерфейса (CLI)
$(EXECUTABLE_CLI): $(INTERFACE_OBJ_CLI) $(LIB_TARGET) | $(CLI_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(LIB_DIR) -lgame_logic $(CLI_LDFLAGS)

# Генерация moc-файлов (Qt)
$(QT_BUILD_DIR)/moc_%.cpp: $(GUI_QT_DIR)/%.h | $(QT_BUILD_DIR)
	moc $< -o $@

# Компиляция moc-файлов (Qt)
$(QT_BUILD_DIR)/%.o: $(QT_BUILD_DIR)/%.cpp | $(QT_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция исходников логики игры (.cpp)
$(BUILD_DIR)/logic_%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция исходников логики игры (.c)
$(BUILD_DIR)/logic_%.o: $(COMMON_DIR)/%.c | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция исходников интерфейса (Qt)
$(QT_BUILD_DIR)/%.o: $(GUI_QT_DIR)/%.cpp | $(QT_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция исходников интерфейса (CLI) (.c)
$(CLI_BUILD_DIR)/%.o: $(GUI_CLI_DIR)/%.c | $(CLI_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция исходников интерфейса (CLI) (.cpp)
$(CLI_BUILD_DIR)/%.o: $(GUI_CLI_DIR)/%.cpp | $(CLI_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция тестов
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Создание папок
$(BUILD_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

$(QT_BUILD_DIR):
	mkdir -p $@

$(CLI_BUILD_DIR):
	mkdir -p $@

# Запуск тестов
tests: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

# Сборка тестов
$(TEST_EXECUTABLE): $(TEST_OBJ) $(LOGIC_OBJ) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(TEST_LDFLAGS)

# Проверка покрытия
coverage: $(TEST_EXECUTABLE)
	mkdir -p $(COVERAGE_DIR)
	./$(TEST_EXECUTABLE)
	# Сбор данных покрытия
	lcov --capture --directory $(BUILD_DIR) --output-file $(COVERAGE_DIR)/coverage.info
	# Удаление ненужных файлов из отчёта
	lcov --remove $(COVERAGE_DIR)/coverage.info '*/tests/*' '*/usr/*' --output-file $(COVERAGE_DIR)/coverage_filtered.info
	# Включение только файлов из текущей директории и её поддиректорий
	lcov --extract $(COVERAGE_DIR)/coverage_filtered.info "$(PWD)/*" --output-file $(COVERAGE_DIR)/coverage_filtered.info
	# Генерация HTML-отчёта
	genhtml $(COVERAGE_DIR)/coverage_filtered.info --output-directory $(COVERAGE_DIR)
	@echo "Открытие отчета покрытия:"
	xdg-open $(COVERAGE_DIR)/index.html || echo "Откройте $(COVERAGE_DIR)/index.html вручную."

# Проверка утечек памяти с помощью Valgrind
memcheck: $(TEST_EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes $(TEST_EXECUTABLE)

clang:
	cp /home/adzhanaev/s21/CPP3_BrickGame_v2.0-1/materials/linters/.clang-format .
	clang-format -n \
	$(LOGIC_SRC_CPP) \
	$(HEADERS) \
	$(INTERFACE_SRC_QT) \
	$(MOC_HEADERS) \
	$(INTERFACE_SRC_CLI_C) \
	/home/adzhanaev/s21/CPP3_BrickGame_v2.0-1/src/gui/cli/cli.h \
	tests/*.cpp
	rm -rf .clang-format

# Установка программы
install: all
	mkdir game
	cp $(EXECUTABLE_QT) game
	cp $(EXECUTABLE_CLI) game

# Удаление установленной программы
uninstall:
	rm -rf game

# Сборка документации в формате DVI
# Build documentation in PDF format using XeLaTeX
dvi:
ifdef DOC_SRC
	cd $(DOC_DIR) && xelatex $(notdir $(DOC_SRC))
else
	@echo "Нет исходных файлов для сборки документации."
endif



# Создание дистрибутива
dist:
	mkdir -p $(DIST_DIR)
	tar czf $(DIST_DIR)/$(PACKAGE_NAME)-$(VERSION).tar.gz $(DIST_FILES)

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(COVERAGE_DIR) $(DIST_DIR) *.o
	make uninstall
