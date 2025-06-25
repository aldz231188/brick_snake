# Компиляторы и флаги
CC = gcc
CXX = g++
CFLAGS = -std=gnu11 -Wall -Wextra -Werror -pedantic -g -fPIC -fprofile-arcs -ftest-coverage
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
           $(SRC_DIR)/figures.h \
           $(SRC_DIR)/fsm.h \
           $(SRC_DIR)/tetris.h \

# Исходные файлы логики игры
LOGIC_SRC_CPP = $(wildcard $(SRC_DIR)/*.cpp)
LOGIC_SRC_C = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(COMMON_DIR)/*.c)

# Исключаем файлы интерфейса из логики игры (если есть)
LOGIC_SRC_CPP := $(filter-out $(SRC_DIR)/main.cpp, $(LOGIC_SRC_CPP))
LOGIC_SRC_C := $(filter-out $(SRC_DIR)/main.c, $(LOGIC_SRC_C))

# Объектные файлы логики игры
LOGIC_OBJ_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/logic_%.o, $(LOGIC_SRC_CPP))
LOGIC_OBJ_C = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/logic_%.o, $(LOGIC_SRC_C))
LOGIC_OBJ_COMMON_C = $(patsubst $(COMMON_DIR)/%.c, $(BUILD_DIR)/logic_common_%.o, $(wildcard $(COMMON_DIR)/*.c))
LOGIC_OBJ = $(LOGIC_OBJ_CPP) $(LOGIC_OBJ_C) $(LOGIC_OBJ_COMMON_C)

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
INTERFACE_OBJ_CLI_C = $(patsubst $(GUI_CLI_DIR)/%.c, $(CLI_BUILD_DIR)/%.o, $(INTERFACE_SRC_CLI_C))
INTERFACE_OBJ_CLI_CPP = $(patsubst $(GUI_CLI_DIR)/%.cpp, $(CLI_BUILD_DIR)/%.o, $(INTERFACE_SRC_CLI_CPP))
INTERFACE_OBJ_CLI = $(INTERFACE_OBJ_CLI_C) $(INTERFACE_OBJ_CLI_CPP)

# Файлы тестов
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/test_%.o,$(TEST_SRC))

# Целевые файлы
LIB_TARGET = $(LIB_DIR)/libtetris_logic.a
EXECUTABLE_QT = $(BUILD_DIR)/tetris_qt
EXECUTABLE_CLI = $(BUILD_DIR)/tetris_cli
TEST_EXECUTABLE = $(BUILD_DIR)/run_tests

# Команды
.PHONY: all install uninstall clean

# Сборка всего проекта
all: $(EXECUTABLE_QT) $(EXECUTABLE_CLI)

# Сборка библиотеки
$(LIB_TARGET): $(LOGIC_OBJ) | $(LIB_DIR)
	ar rcs $@ $^

# Сборка интерфейса (Qt)
$(EXECUTABLE_QT): $(INTERFACE_OBJ_QT) $(LIB_TARGET) | $(QT_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(LIB_DIR) -ltetris_logic $(APP_LDFLAGS)

# Сборка интерфейса (CLI)
$(EXECUTABLE_CLI): $(INTERFACE_OBJ_CLI) $(LIB_TARGET) | $(CLI_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(LIB_DIR) -ltetris_logic $(CLI_LDFLAGS)

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
$(BUILD_DIR)/logic_%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция исходников из common (.c)
$(BUILD_DIR)/logic_common_%.o: $(COMMON_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция исходников интерфейса (Qt)
$(QT_BUILD_DIR)/%.o: $(GUI_QT_DIR)/%.cpp | $(QT_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция исходников интерфейса (CLI) (.c)
$(CLI_BUILD_DIR)/%.o: $(GUI_CLI_DIR)/%.c | $(CLI_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция исходников интерфейса (CLI) (.cpp)
$(CLI_BUILD_DIR)/%.o: $(GUI_CLI_DIR)/%.cpp | $(CLI_BUILD_DIR)
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

clang:
	cp /home/adzhanaev/s21/CPP3_BrickGame_v2.0-1/materials/linters/.clang-format .
	clang-format -n \
	$(SRC_DIR)/*.c \
	$(HEADERS)
	rm -rf .clang-format

# Установка программы
install: all
	mkdir game
	cp $(EXECUTABLE_QT) game
	cp $(EXECUTABLE_CLI) game


# Удаление установленной программы
uninstall:
	rm -rf game 

# Очистка
clean:
	make uninstall
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(COVERAGE_DIR) $(DIST_DIR) *.o
