CC = gcc
CFLAGS = -Wall -Wextra -g
SRCDIR = .
CRYPTODIR = crypto
SRC_CRYPTO = a.c b.c c.c
SRC_CRYPTO_FULL = $(addprefix $(CRYPTODIR)/, $(SRC_CRYPTO))
SRC = $(wildcard $(SRCDIR)/*.c) $(SRC_CRYPTO_FULL)
OBJ = $(SRC:.c=.o)
DEPS = $(wildcard $(SRCDIR)/*.h) $(patsubst %.c, $(CRYPTODIR)/%.h, $(SRC_CRYPTO))
EXECUTABLE = my_program

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
    $(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ) $(EXECUTABLE)
